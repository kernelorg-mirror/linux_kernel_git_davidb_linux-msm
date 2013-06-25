/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/of_platform.h>
#include <asm/mach/arch.h>

static const char * const msm8974_dt_match[] __initconst = {
	"qcom,msm8974",
	NULL
};

DT_MACHINE_START(MSM8974_DT, "Qualcomm MSM (Flattened Device Tree)")
	.dt_compat = msm8974_dt_match,
MACHINE_END
