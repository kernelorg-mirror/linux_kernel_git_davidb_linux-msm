/*
 * Copyright (C) 2008 Google, Inc.
 * Copyright (c) 2008-2011, Code Aurora Forum. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/clkdev.h>
#include <linux/dma-mapping.h>

#include <mach/irqs.h>
#include <mach/msm_iomap.h>
#include <mach/dma.h>
#include <mach/board.h>

#include "devices.h"

#include <asm/mach/flash.h>

#include <linux/platform_data/mmc-msm_sdcc.h>
#include "clock-pcom.h"
#include "clock-voter.h"

static struct resource resources_uart3[] = {
	{
		.start	= INT_UART3,
		.end	= INT_UART3,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART3_PHYS,
		.end	= MSM_UART3_PHYS + MSM_UART3_SIZE - 1,
		.flags	= IORESOURCE_MEM,
		.name  = "uart_resource"
	},
};

struct platform_device msm_device_uart3 = {
	.name	= "msm_serial",
	.id	= 2,
	.num_resources	= ARRAY_SIZE(resources_uart3),
	.resource	= resources_uart3,
};

struct platform_device msm_device_smd = {
	.name   = "msm_smd",
	.id     = -1,
};

static struct resource resources_otg[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_otg = {
	.name		= "msm_otg",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_otg),
	.resource	= resources_otg,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct resource resources_hsusb[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_hsusb = {
	.name		= "msm_hsusb",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb),
	.resource	= resources_hsusb,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static u64 dma_mask = 0xffffffffULL;
static struct resource resources_hsusb_host[] = {
	{
		.start	= MSM_HSUSB_PHYS,
		.end	= MSM_HSUSB_PHYS + MSM_HSUSB_SIZE,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_USB_HS,
		.end	= INT_USB_HS,
		.flags	= IORESOURCE_IRQ,
	},
};

struct platform_device msm_device_hsusb_host = {
	.name		= "msm_hsusb_host",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(resources_hsusb_host),
	.resource	= resources_hsusb_host,
	.dev		= {
		.dma_mask               = &dma_mask,
		.coherent_dma_mask      = 0xffffffffULL,
	},
};

static struct resource resources_sdc1[] = {
	{
		.start	= MSM_SDC1_PHYS,
		.end	= MSM_SDC1_PHYS + MSM_SDC1_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC1_0,
		.end	= INT_SDC1_0,
		.flags	= IORESOURCE_IRQ,
		.name	= "cmd_irq",
	},
	{
		.flags	= IORESOURCE_IRQ | IORESOURCE_DISABLED,
		.name	= "status_irq"
	},
	{
		.start	= 8,
		.end	= 8,
		.flags	= IORESOURCE_DMA,
	},
};

static struct resource resources_sdc2[] = {
	{
		.start	= MSM_SDC2_PHYS,
		.end	= MSM_SDC2_PHYS + MSM_SDC2_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC2_0,
		.end	= INT_SDC2_0,
		.flags	= IORESOURCE_IRQ,
		.name	= "cmd_irq",
	},
	{
		.flags	= IORESOURCE_IRQ | IORESOURCE_DISABLED,
		.name	= "status_irq"
	},
	{
		.start	= 8,
		.end	= 8,
		.flags	= IORESOURCE_DMA,
	},
};

static struct resource resources_sdc3[] = {
	{
		.start	= MSM_SDC3_PHYS,
		.end	= MSM_SDC3_PHYS + MSM_SDC3_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC3_0,
		.end	= INT_SDC3_0,
		.flags	= IORESOURCE_IRQ,
		.name	= "cmd_irq",
	},
	{
		.flags	= IORESOURCE_IRQ | IORESOURCE_DISABLED,
		.name	= "status_irq"
	},
	{
		.start	= 8,
		.end	= 8,
		.flags	= IORESOURCE_DMA,
	},
};

static struct resource resources_sdc4[] = {
	{
		.start	= MSM_SDC4_PHYS,
		.end	= MSM_SDC4_PHYS + MSM_SDC4_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
	{
		.start	= INT_SDC4_0,
		.end	= INT_SDC4_0,
		.flags	= IORESOURCE_IRQ,
		.name	= "cmd_irq",
	},
	{
		.flags	= IORESOURCE_IRQ | IORESOURCE_DISABLED,
		.name	= "status_irq"
	},
	{
		.start	= 8,
		.end	= 8,
		.flags	= IORESOURCE_DMA,
	},
};

struct platform_device msm_device_sdc1 = {
	.name		= "msm_sdcc",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(resources_sdc1),
	.resource	= resources_sdc1,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc2 = {
	.name		= "msm_sdcc",
	.id		= 2,
	.num_resources	= ARRAY_SIZE(resources_sdc2),
	.resource	= resources_sdc2,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc3 = {
	.name		= "msm_sdcc",
	.id		= 3,
	.num_resources	= ARRAY_SIZE(resources_sdc3),
	.resource	= resources_sdc3,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

struct platform_device msm_device_sdc4 = {
	.name		= "msm_sdcc",
	.id		= 4,
	.num_resources	= ARRAY_SIZE(resources_sdc4),
	.resource	= resources_sdc4,
	.dev		= {
		.coherent_dma_mask	= 0xffffffff,
	},
};

static struct platform_device *msm_sdcc_devices[] __initdata = {
	&msm_device_sdc1,
	&msm_device_sdc2,
	&msm_device_sdc3,
	&msm_device_sdc4,
};

int __init msm_add_sdcc(unsigned int controller,
			struct msm_mmc_platform_data *plat,
			unsigned int stat_irq, unsigned long stat_irq_flags)
{
	struct platform_device	*pdev;
	struct resource *res;

	if (controller < 1 || controller > 4)
		return -EINVAL;

	pdev = msm_sdcc_devices[controller-1];
	pdev->dev.platform_data = plat;

	res = platform_get_resource_byname(pdev, IORESOURCE_IRQ, "status_irq");
	if (!res)
		return -EINVAL;
	else if (stat_irq) {
		res->start = res->end = stat_irq;
		res->flags &= ~IORESOURCE_DISABLED;
		res->flags |= stat_irq_flags;
	}

	return platform_device_register(pdev);
}

static DEFINE_CLK_PCOM(adm_clk,		ADM_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(ce_clk,		CE_CLK,		CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(ebi1_clk,	EBI1_CLK,	CLKFLAG_MIN);
static DEFINE_CLK_PCOM(ebi2_clk,	EBI2_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(ecodec_clk,	ECODEC_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(emdh_clk,	EMDH_CLK,	CLKFLAG_MIN |
							CLKFLAG_MAX);
static DEFINE_CLK_PCOM(gp_clk,		GP_CLK,		CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(grp_3d_clk,	GRP_3D_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(i2c_clk,		I2C_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(icodec_rx_clk,	ICODEC_RX_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(icodec_tx_clk,	ICODEC_TX_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(imem_clk,	IMEM_CLK,	0);
static DEFINE_CLK_PCOM(mdc_clk,		MDC_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(pmdh_clk,	PMDH_CLK,	CLKFLAG_MIN |
							CLKFLAG_MAX);
static DEFINE_CLK_PCOM(mdp_clk,		MDP_CLK,	0);
static DEFINE_CLK_PCOM(mdp_lcdc_pclk_clk,	MDP_LCDC_PCLK_CLK,
		CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(mdp_lcdc_pad_pclk_clk,	MDP_LCDC_PAD_PCLK_CLK,
		CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(mdp_vsync_clk,	MDP_VSYNC_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(pbus_clk,	PBUS_CLK,	CLKFLAG_MIN);
static DEFINE_CLK_PCOM(pcm_clk,		PCM_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(sdac_clk,	SDAC_CLK,	0);
static DEFINE_CLK_PCOM(sdc1_clk,	SDC1_CLK,	0);
static DEFINE_CLK_PCOM(sdc1_p_clk,	SDC1_P_CLK,	0);
static DEFINE_CLK_PCOM(sdc2_clk,	SDC2_CLK,	0);
static DEFINE_CLK_PCOM(sdc2_p_clk,	SDC2_P_CLK,	0);
static DEFINE_CLK_PCOM(sdc3_clk,	SDC3_CLK,	0);
static DEFINE_CLK_PCOM(sdc3_p_clk,	SDC3_P_CLK,	0);
static DEFINE_CLK_PCOM(sdc4_clk,	SDC4_CLK,	0);
static DEFINE_CLK_PCOM(sdc4_p_clk,	SDC4_P_CLK,	0);
static DEFINE_CLK_PCOM(spi_clk,		SPI_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(tsif_clk,	TSIF_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(tsif_ref_clk,	TSIF_REF_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(tv_dac_clk,	TV_DAC_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(tv_enc_clk,	TV_ENC_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(uart1_clk,	UART1_CLK,	0);
static DEFINE_CLK_PCOM(uart2_clk,	UART2_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(uart3_clk,	UART3_CLK,	0);
static DEFINE_CLK_PCOM(uart1dm_clk,	UART1DM_CLK,	0);
static DEFINE_CLK_PCOM(uart2dm_clk,	UART2DM_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(usb_hs_clk,	USB_HS_CLK,	0);
static DEFINE_CLK_PCOM(usb_hs_p_clk,	USB_HS_P_CLK,	0);
static DEFINE_CLK_PCOM(usb_otg_clk,	USB_OTG_CLK,	CLKFLAG_SKIP_AUTO_OFF);
static DEFINE_CLK_PCOM(vdc_clk,		VDC_CLK,	CLKFLAG_MIN);
static DEFINE_CLK_PCOM(vfe_clk,		VFE_CLK,	0);
static DEFINE_CLK_PCOM(vfe_mdc_clk,	VFE_MDC_CLK,	0);
static DEFINE_CLK_PCOM(vfe_axi_clk,	VFE_AXI_CLK,	0);
static DEFINE_CLK_PCOM(usb_hs2_clk,	USB_HS2_CLK,	0);
static DEFINE_CLK_PCOM(usb_hs2_p_clk,	USB_HS2_P_CLK,	0);
static DEFINE_CLK_PCOM(usb_hs3_clk,	USB_HS3_CLK,	0);
static DEFINE_CLK_PCOM(usb_hs3_p_clk,	USB_HS3_P_CLK,	0);
static DEFINE_CLK_PCOM(usb_phy_clk,	USB_PHY_CLK,	CLKFLAG_SKIP_AUTO_OFF);

static DEFINE_CLK_VOTER(ebi1_acpu_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_kgsl_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_lcdc_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_mddi_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_tv_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_usb_clk,	&ebi1_clk.c);
static DEFINE_CLK_VOTER(ebi1_vfe_clk,	&ebi1_clk.c);

struct clk_lookup msm_clocks_8x50[] = {
	CLK_LOOKUP("adm_clk",		adm_clk.c,	"msm_dmov"),
	CLK_LOOKUP("ce_clk",		ce_clk.c,		NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_clk.c,	NULL),
	CLK_LOOKUP("ebi2_clk",		ebi2_clk.c,	NULL),
	CLK_LOOKUP("ecodec_clk",	ecodec_clk.c,	NULL),
	CLK_LOOKUP("emdh_clk",		emdh_clk.c,	NULL),
	CLK_LOOKUP("gp_clk",		gp_clk.c,		NULL),
	CLK_LOOKUP("grp_clk",		grp_3d_clk.c,	NULL),
	CLK_LOOKUP("i2c_clk",		i2c_clk.c,	NULL),
	CLK_LOOKUP("icodec_rx_clk",	icodec_rx_clk.c,	NULL),
	CLK_LOOKUP("icodec_tx_clk",	icodec_tx_clk.c,	NULL),
	CLK_LOOKUP("imem_clk",		imem_clk.c,	NULL),
	CLK_LOOKUP("mdc_clk",		mdc_clk.c,	NULL),
	CLK_LOOKUP("mddi_clk",		pmdh_clk.c,	NULL),
	CLK_LOOKUP("mdp_clk",		mdp_clk.c,	NULL),
	CLK_LOOKUP("mdp_lcdc_pclk_clk",	mdp_lcdc_pclk_clk.c, NULL),
	CLK_LOOKUP("mdp_lcdc_pad_pclk_clk", mdp_lcdc_pad_pclk_clk.c, NULL),
	CLK_LOOKUP("mdp_vsync_clk",	mdp_vsync_clk.c,	NULL),
	CLK_LOOKUP("pbus_clk",		pbus_clk.c,	NULL),
	CLK_LOOKUP("pcm_clk",		pcm_clk.c,	NULL),
	CLK_LOOKUP("sdac_clk",		sdac_clk.c,	NULL),
	CLK_LOOKUP("sdc_clk",		sdc1_clk.c,	"msm_sdcc.1"),
	CLK_LOOKUP("sdc_pclk",		sdc1_p_clk.c,	"msm_sdcc.1"),
	CLK_LOOKUP("sdc_clk",		sdc2_clk.c,	"msm_sdcc.2"),
	CLK_LOOKUP("sdc_pclk",		sdc2_p_clk.c,	"msm_sdcc.2"),
	CLK_LOOKUP("sdc_clk",		sdc3_clk.c,	"msm_sdcc.3"),
	CLK_LOOKUP("sdc_pclk",		sdc3_p_clk.c,	"msm_sdcc.3"),
	CLK_LOOKUP("sdc_clk",		sdc4_clk.c,	"msm_sdcc.4"),
	CLK_LOOKUP("sdc_pclk",		sdc4_p_clk.c,	"msm_sdcc.4"),
	CLK_LOOKUP("spi_clk",		spi_clk.c,	NULL),
	CLK_LOOKUP("tsif_clk",		tsif_clk.c,	NULL),
	CLK_LOOKUP("tsif_ref_clk",	tsif_ref_clk.c,	NULL),
	CLK_LOOKUP("tv_dac_clk",	tv_dac_clk.c,	NULL),
	CLK_LOOKUP("tv_enc_clk",	tv_enc_clk.c,	NULL),
	CLK_LOOKUP("uart_clk",		uart1_clk.c,	NULL),
	CLK_LOOKUP("uart_clk",		uart2_clk.c,	NULL),
	CLK_LOOKUP("uart_clk",		uart3_clk.c,	"msm_serial.2"),
	CLK_LOOKUP("uartdm_clk",	uart1dm_clk.c,	NULL),
	CLK_LOOKUP("uartdm_clk",	uart2dm_clk.c,	NULL),
	CLK_LOOKUP("usb_hs_clk",	usb_hs_clk.c,	NULL),
	CLK_LOOKUP("usb_hs_pclk",	usb_hs_p_clk.c,	NULL),
	CLK_LOOKUP("usb_otg_clk",	usb_otg_clk.c,	NULL),
	CLK_LOOKUP("vdc_clk",		vdc_clk.c,	NULL),
	CLK_LOOKUP("vfe_clk",		vfe_clk.c,	NULL),
	CLK_LOOKUP("vfe_mdc_clk",	vfe_mdc_clk.c,	NULL),
	CLK_LOOKUP("vfe_axi_clk",	vfe_axi_clk.c,	NULL),
	CLK_LOOKUP("usb_hs2_clk",	usb_hs2_clk.c,	NULL),
	CLK_LOOKUP("usb_hs2_pclk",	usb_hs2_p_clk.c,	NULL),
	CLK_LOOKUP("usb_hs3_clk",	usb_hs3_clk.c,	NULL),
	CLK_LOOKUP("usb_hs3_pclk",	usb_hs3_p_clk.c,	NULL),
	CLK_LOOKUP("usb_phy_clk",	usb_phy_clk.c,	NULL),

	CLK_LOOKUP("ebi1_clk",		ebi1_acpu_clk.c,	NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_kgsl_clk.c,	NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_lcdc_clk.c,	NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_mddi_clk.c,	NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_tv_clk.c,		NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_usb_clk.c,		NULL),
	CLK_LOOKUP("ebi1_clk",		ebi1_vfe_clk.c,		NULL),
};

unsigned msm_num_clocks_8x50 = ARRAY_SIZE(msm_clocks_8x50);

