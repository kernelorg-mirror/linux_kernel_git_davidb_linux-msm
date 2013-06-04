/*
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
 *  Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/jiffies.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/smp.h>
#include <linux/io.h>

#include <asm/cacheflush.h>
#include <asm/cputype.h>
#include <asm/mach-types.h>
#include <asm/smp_plat.h>

#include "scm-boot.h"
#include "common.h"

#define VDD_SC1_ARRAY_CLAMP_GFS_CTL 0x15A0
#define SCSS_CPU1CORE_RESET 0xD80
#define SCSS_DBG_STATUS_CORE_PWRDUP 0xE64

extern void msm_secondary_startup(void);

static DEFINE_SPINLOCK(boot_lock);

static void __cpuinit write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));
}

static inline int get_core_count(void)
{
	/* 1 + the PART[1:0] field of MIDR */
	return ((read_cpuid_id() >> 4) & 3) + 1;
}

static void __cpuinit msm_secondary_init(unsigned int cpu)
{
	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	write_pen_release(-1);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

static int __cpuinit scorpion_release_secondary(void)
{
	void __iomem *sc1_base_ptr;
	struct device_node *dn = NULL;

	dn = of_find_compatible_node(dn, NULL, "qcom,scss");
	if (!dn) {
		pr_err("%s: Missing scss node in device tree\n", __func__);
		return -ENXIO;
	}

	sc1_base_ptr = of_iomap(dn, 0);
	if (sc1_base_ptr) {
		writel_relaxed(0, sc1_base_ptr + VDD_SC1_ARRAY_CLAMP_GFS_CTL);
		writel_relaxed(0, sc1_base_ptr + SCSS_CPU1CORE_RESET);
		writel_relaxed(3, sc1_base_ptr + SCSS_DBG_STATUS_CORE_PWRDUP);
		mb();
		iounmap(sc1_base_ptr);
	} else {
		return -ENOMEM;
	}

	return 0;
}

static DEFINE_PER_CPU(int, cold_boot_done);

static __cpuinit void boot_cold_cpu(unsigned int cpu)
{
	const char *enable_method;
	struct device_node *dn = NULL;

	dn = of_find_node_by_name(dn, "cpus");
	if (!dn) {
		pr_err("%s: Missing node cpus in device tree\n", __func__);
		return;
	}

	enable_method = of_get_property(dn, "enable-method", NULL);
	if (!enable_method) {
			pr_err("%s: cpus node is missing enable-method property\n",
					__func__);
	} else if (!strcmp(enable_method, "qcom,scss")) {
		if (per_cpu(cold_boot_done, cpu) == false) {
			scorpion_release_secondary();
			per_cpu(cold_boot_done, cpu) = true;
		}
	} else {
		pr_err("%s: Invalid enable-method property: %s\n",
				__func__, enable_method);
	}
}

static int __cpuinit msm_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;

	boot_cold_cpu(cpu);

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 *
	 * Note that "pen_release" is the hardware CPU ID, whereas
	 * "cpu" is Linux's internal ID.
	 */
	write_pen_release(cpu_logical_map(cpu));

	/*
	 * Send the secondary CPU a soft interrupt, thereby causing
	 * the boot monitor to read the system wide flags register,
	 * and branch to the address found there.
	 */
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		smp_rmb();
		if (pen_release == -1)
			break;

		udelay(10);
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system. The msm8x60
 * does not support the ARM SCU, so just set the possible cpu mask to
 * NR_CPUS.
 */
static void __init msm_smp_init_cpus(void)
{
	unsigned int i, ncores = get_core_count();

	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
}

static const int cold_boot_flags[] __initconst = {
	0,
	SCM_FLAG_COLDBOOT_CPU1,
};

static void __init msm_smp_prepare_cpus(unsigned int max_cpus)
{
	int cpu, map;
	unsigned int flags = 0;

	for_each_present_cpu(cpu) {
		map = cpu_logical_map(cpu);
		if (map > ARRAY_SIZE(cold_boot_flags)) {
			set_cpu_present(cpu, false);
			__WARN();
			continue;
		}
		flags |= cold_boot_flags[map];
	}

	if (scm_set_boot_addr(virt_to_phys(msm_secondary_startup), flags))
		pr_warn("Failed to set CPU boot address\n");
}

struct smp_operations msm_smp_ops __initdata = {
	.smp_init_cpus		= msm_smp_init_cpus,
	.smp_prepare_cpus	= msm_smp_prepare_cpus,
	.smp_secondary_init	= msm_secondary_init,
	.smp_boot_secondary	= msm_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= msm_cpu_die,
#endif
};
