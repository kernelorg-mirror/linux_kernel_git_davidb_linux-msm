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

#include <linux/dma-mapping.h>
#include <linux/clkdev.h>
#include <mach/irqs.h>
#include <mach/msm_iomap.h>
#include <mach/dma.h>
#include <mach/board.h>

#include "devices.h"
#include "smd_private.h"

#include <asm/mach/flash.h>

#include "clock-pcom.h"
#include "clock-7x30.h"

#include <mach/mmc.h>

static struct resource resources_uart2[] = {
	{
		.start	= INT_UART2,
		.end	= INT_UART2,
		.flags	= IORESOURCE_IRQ,
	},
	{
		.start	= MSM_UART2_PHYS,
		.end	= MSM_UART2_PHYS + MSM_UART2_SIZE - 1,
		.flags	= IORESOURCE_MEM,
		.name  = "uart_resource"
	},
};

struct platform_device msm_device_uart2 = {
	.name	= "msm_serial",
	.id	= 1,
	.num_resources	= ARRAY_SIZE(resources_uart2),
	.resource	= resources_uart2,
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

static DEFINE_CLK_PCOM(adm_clk,		ADM_CLK,		0);
static DEFINE_CLK_PCOM(adsp_clk,	ADSP_CLK,		0);
static DEFINE_CLK_PCOM(cam_m_clk,	CAM_M_CLK,		0);
static DEFINE_CLK_PCOM(camif_pad_p_clk,	CAMIF_PAD_P_CLK,	OFF);
static DEFINE_CLK_PCOM(ce_clk,		CE_CLK,			0);
static DEFINE_CLK_PCOM(codec_ssbi_clk,	CODEC_SSBI_CLK,		0);
static DEFINE_CLK_PCOM(ebi1_clk,	EBI1_CLK,		CLK_MIN);
static DEFINE_CLK_PCOM(ecodec_clk,	ECODEC_CLK,		0);
static DEFINE_CLK_PCOM(emdh_clk,	EMDH_CLK,	OFF | CLK_MINMAX);
static DEFINE_CLK_PCOM(emdh_p_clk,	EMDH_P_CLK,		OFF);
static DEFINE_CLK_PCOM(gp_clk,		GP_CLK,			0);
static DEFINE_CLK_PCOM(grp_2d_clk,	GRP_2D_CLK,		0);
static DEFINE_CLK_PCOM(grp_2d_p_clk,	GRP_2D_P_CLK,		0);
static DEFINE_CLK_PCOM(grp_3d_clk,	GRP_3D_CLK,		0);
static DEFINE_CLK_PCOM(grp_3d_p_clk,	GRP_3D_P_CLK,		0);
static DEFINE_CLK_PCOM(hdmi_clk,	HDMI_CLK,		0);
static DEFINE_CLK_PCOM(imem_clk,	IMEM_CLK,		OFF);
static DEFINE_CLK_PCOM(jpeg_clk,	JPEG_CLK,		OFF);
static DEFINE_CLK_PCOM(jpeg_p_clk,	JPEG_P_CLK,		OFF);
static DEFINE_CLK_PCOM(lpa_codec_clk,	LPA_CODEC_CLK,		0);
static DEFINE_CLK_PCOM(lpa_core_clk,	LPA_CORE_CLK,		0);
static DEFINE_CLK_PCOM(lpa_p_clk,	LPA_P_CLK,		0);
static DEFINE_CLK_PCOM(mdc_clk,		MDC_CLK,		0);
static DEFINE_CLK_PCOM(pmdh_clk,	PMDH_CLK,	OFF | CLK_MINMAX);
static DEFINE_CLK_PCOM(pmdh_p_clk,	PMDH_P_CLK,		0);
static DEFINE_CLK_PCOM(mdp_clk,		MDP_CLK,		OFF);
static DEFINE_CLK_PCOM(mdp_p_clk,	MDP_P_CLK,		0);
static DEFINE_CLK_PCOM(mdp_lcdc_pclk_clk, MDP_LCDC_PCLK_CLK,	0);
static DEFINE_CLK_PCOM(mdp_lcdc_pad_pclk_clk, MDP_LCDC_PAD_PCLK_CLK, 0);
static DEFINE_CLK_PCOM(mdp_vsync_clk,	MDP_VSYNC_CLK,		0);
static DEFINE_CLK_PCOM(mfc_clk,		MFC_CLK,		0);
static DEFINE_CLK_PCOM(mfc_div2_clk,	MFC_DIV2_CLK,		0);
static DEFINE_CLK_PCOM(mfc_p_clk,	MFC_P_CLK,		0);
static DEFINE_CLK_PCOM(mi2s_m_clk,	MI2S_M_CLK,  		0);
static DEFINE_CLK_PCOM(mi2s_s_clk,	MI2S_S_CLK,  		0);
static DEFINE_CLK_PCOM(mi2s_codec_rx_m_clk,	MI2S_CODEC_RX_M_CLK,  0);
static DEFINE_CLK_PCOM(mi2s_codec_rx_s_clk,	MI2S_CODEC_RX_S_CLK,  0);
static DEFINE_CLK_PCOM(mi2s_codec_tx_m_clk,	MI2S_CODEC_TX_M_CLK,  0);
static DEFINE_CLK_PCOM(mi2s_codec_tx_s_clk,	MI2S_CODEC_TX_S_CLK,  0);
static DEFINE_CLK_PCOM(pbus_clk,	PBUS_CLK,		CLK_MIN);
static DEFINE_CLK_PCOM(pcm_clk,		PCM_CLK,		0);
static DEFINE_CLK_PCOM(axi_rotator_clk,	AXI_ROTATOR_CLK,	0);
static DEFINE_CLK_PCOM(rotator_imem_clk, ROTATOR_IMEM_CLK,	OFF);
static DEFINE_CLK_PCOM(rotator_p_clk,	ROTATOR_P_CLK,		OFF);
static DEFINE_CLK_PCOM(sdac_clk,	SDAC_CLK,		OFF);
static DEFINE_CLK_PCOM(spi_clk,		SPI_CLK,		0);
static DEFINE_CLK_PCOM(spi_p_clk,	SPI_P_CLK,		0);
static DEFINE_CLK_PCOM(tv_dac_clk,	TV_DAC_CLK,		0);
static DEFINE_CLK_PCOM(tv_enc_clk,	TV_ENC_CLK,		0);
static DEFINE_CLK_PCOM(uart2_clk,	UART2_CLK,		0);
static DEFINE_CLK_PCOM(usb_phy_clk,	USB_PHY_CLK,		0);
static DEFINE_CLK_PCOM(usb_hs_clk,	USB_HS_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs_p_clk,	USB_HS_P_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs_core_clk,	USB_HS_CORE_CLK,	OFF);
static DEFINE_CLK_PCOM(usb_hs2_clk,	USB_HS2_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs2_p_clk,	USB_HS2_P_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs2_core_clk,USB_HS2_CORE_CLK,	OFF);
static DEFINE_CLK_PCOM(usb_hs3_clk,	USB_HS3_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs3_p_clk,	USB_HS3_P_CLK,		OFF);
static DEFINE_CLK_PCOM(usb_hs3_core_clk,USB_HS3_CORE_CLK,	OFF);
static DEFINE_CLK_PCOM(vdc_clk,		VDC_CLK,		OFF | CLK_MIN);
static DEFINE_CLK_PCOM(vfe_camif_clk,	VFE_CAMIF_CLK, 		0);
static DEFINE_CLK_PCOM(vfe_clk,		VFE_CLK,		0);
static DEFINE_CLK_PCOM(vfe_mdc_clk,	VFE_MDC_CLK,		0);
static DEFINE_CLK_PCOM(vfe_p_clk,	VFE_P_CLK,		OFF);
static DEFINE_CLK_PCOM(vpe_clk,		VPE_CLK,		0);

static DEFINE_CLK_PCOM(csi0_clk,	CSI0_CLK,		0);
static DEFINE_CLK_PCOM(csi0_p_clk,	CSI0_P_CLK,		0);
static DEFINE_CLK_PCOM(csi0_vfe_clk,	CSI0_VFE_CLK,		0);

struct clk_lookup msm_clocks_7x30[] = {
	CLK_LOOKUP("adm_clk",			adm_clk.c,	"msm_dmov"),
	CLK_LOOKUP("adsp_clk",			adsp_clk.c,	NULL),
	CLK_LOOKUP("cam_m_clk",			cam_m_clk.c,	NULL),
	CLK_LOOKUP("camif_pad_pclk",		camif_pad_p_clk.c,	NULL),
	CLK_LOOKUP("ce_clk",			ce_clk.c,	NULL),
	CLK_LOOKUP("codec_ssbi_clk",		codec_ssbi_clk.c,	NULL),
	CLK_LOOKUP("ebi1_clk",			ebi1_clk.c,	NULL),
	CLK_LOOKUP("ecodec_clk",		ecodec_clk.c,	NULL),
	CLK_LOOKUP("emdh_clk",			emdh_clk.c,	NULL),
	CLK_LOOKUP("emdh_pclk",			emdh_p_clk.c,	NULL),
	CLK_LOOKUP("gp_clk",			gp_clk.c,	NULL),
	CLK_LOOKUP("grp_2d_clk",		grp_2d_clk.c,	NULL),
	CLK_LOOKUP("grp_2d_pclk",		grp_2d_p_clk.c,	NULL),
	CLK_LOOKUP("grp_clk",			grp_3d_clk.c,	NULL),
	CLK_LOOKUP("grp_pclk",			grp_3d_p_clk.c,	NULL),
	CLK_LOOKUP("grp_src_clk",		grp_3d_clk.c,	NULL),
	CLK_LOOKUP("hdmi_clk",			hdmi_clk.c,	NULL),
	CLK_LOOKUP("imem_clk",			imem_clk.c,	NULL),
	CLK_LOOKUP("jpeg_clk",			jpeg_clk.c,	NULL),
	CLK_LOOKUP("jpeg_pclk",			jpeg_p_clk.c,	NULL),
	CLK_LOOKUP("lpa_codec_clk",		lpa_codec_clk.c,	NULL),
	CLK_LOOKUP("lpa_core_clk",		lpa_core_clk.c,	NULL),
	CLK_LOOKUP("lpa_pclk",			lpa_p_clk.c,	NULL),
	CLK_LOOKUP("mdc_clk",			mdc_clk.c,	NULL),
	CLK_LOOKUP("mddi_clk",			pmdh_clk.c,	NULL),
	CLK_LOOKUP("mddi_pclk",			pmdh_p_clk.c,	NULL),
	CLK_LOOKUP("mdp_clk",			mdp_clk.c,	NULL),
	CLK_LOOKUP("mdp_pclk",			mdp_p_clk.c,	NULL),
	CLK_LOOKUP("mdp_lcdc_pclk_clk",		mdp_lcdc_pclk_clk.c,	NULL),
	CLK_LOOKUP("mdp_lcdc_pad_pclk_clk",	mdp_lcdc_pad_pclk_clk.c, NULL),
	CLK_LOOKUP("mdp_vsync_clk",		mdp_vsync_clk.c,	NULL),
	CLK_LOOKUP("mfc_clk",			mfc_clk.c,		NULL),
	CLK_LOOKUP("mfc_div2_clk",		mfc_div2_clk.c,		NULL),
	CLK_LOOKUP("mfc_pclk",			mfc_p_clk.c,		NULL),
	CLK_LOOKUP("mi2s_m_clk",		mi2s_m_clk.c,		NULL),
	CLK_LOOKUP("mi2s_s_clk",		mi2s_s_clk.c,		NULL),
	CLK_LOOKUP("mi2s_codec_rx_m_clk",	mi2s_codec_rx_m_clk.c,	NULL),
	CLK_LOOKUP("mi2s_codec_rx_s_clk",	mi2s_codec_rx_s_clk.c,	NULL),
	CLK_LOOKUP("mi2s_codec_tx_m_clk",	mi2s_codec_tx_m_clk.c,	NULL),
	CLK_LOOKUP("mi2s_codec_tx_s_clk",	mi2s_codec_tx_s_clk.c,	NULL),
	CLK_LOOKUP("pbus_clk",			pbus_clk.c,		NULL),
	CLK_LOOKUP("pcm_clk",			pcm_clk.c,		NULL),
	CLK_LOOKUP("rotator_clk",		axi_rotator_clk.c,	NULL),
	CLK_LOOKUP("rotator_imem_clk",		rotator_imem_clk.c,	NULL),
	CLK_LOOKUP("rotator_pclk",		rotator_p_clk.c,	NULL),
	CLK_LOOKUP("sdac_clk",			sdac_clk.c,	NULL),
	CLK_LOOKUP("spi_clk",			spi_clk.c,	NULL),
	CLK_LOOKUP("spi_pclk",			spi_p_clk.c,	NULL),
	CLK_LOOKUP("tv_src_clk",		tv_enc_clk.c,	NULL),
	CLK_LOOKUP("tv_dac_clk",		tv_dac_clk.c,	NULL),
	CLK_LOOKUP("tv_enc_clk",		tv_enc_clk.c,	NULL),
	CLK_LOOKUP("uart_clk",			uart2_clk.c,	"msm_serial.1"),
	CLK_LOOKUP("usb_phy_clk",		usb_phy_clk.c,	NULL),
	CLK_LOOKUP("usb_hs_clk",		usb_hs_clk.c,		NULL),
	CLK_LOOKUP("usb_hs_pclk",		usb_hs_p_clk.c,		NULL),
	CLK_LOOKUP("usb_hs_core_clk",		usb_hs_core_clk.c,	NULL),
	CLK_LOOKUP("usb_hs2_clk",		usb_hs2_clk.c,		NULL),
	CLK_LOOKUP("usb_hs2_pclk",		usb_hs2_p_clk.c,	NULL),
	CLK_LOOKUP("usb_hs2_core_clk",		usb_hs2_core_clk.c,	NULL),
	CLK_LOOKUP("usb_hs3_clk",		usb_hs3_clk.c,		NULL),
	CLK_LOOKUP("usb_hs3_pclk",		usb_hs3_p_clk.c,	NULL),
	CLK_LOOKUP("usb_hs3_core_clk",		usb_hs3_core_clk.c,	NULL),
	CLK_LOOKUP("vdc_clk",			vdc_clk.c,		NULL),
	CLK_LOOKUP("vfe_camif_clk",		vfe_camif_clk.c,	NULL),
	CLK_LOOKUP("vfe_clk",			vfe_clk.c,	NULL),
	CLK_LOOKUP("vfe_mdc_clk",		vfe_mdc_clk.c,	NULL),
	CLK_LOOKUP("vfe_pclk",			vfe_p_clk.c,	NULL),
	CLK_LOOKUP("vpe_clk",			vpe_clk.c,	NULL),

	/* 7x30 v2 hardware only. */
	CLK_LOOKUP("csi_clk",			csi0_clk.c,	NULL),
	CLK_LOOKUP("csi_pclk",			csi0_p_clk.c,	NULL),
	CLK_LOOKUP("csi_vfe_clk",		csi0_vfe_clk.c,	NULL),
};

unsigned msm_num_clocks_7x30 = ARRAY_SIZE(msm_clocks_7x30);

