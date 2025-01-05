// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2025 Muhammad <thevancedgamer@mentallysanemainliners.org>
 * Author: Muhammad <thevancedgamer@mentallysanemainliners.org>
 *
 * Common Clock Framework support for Exynos1280 SoC.
 */

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <dt-bindings/clock/samsung,exynos1280.h>

#include "clk.h"
#include "clk-exynos-arm64.h"

#define CLKS_NR_TOP (CLK_GOUT_CMU_USB_USB20DRD + 1)
#define CLKS_NR_ALIVE (CLK_GOUT_ALIVE_GPIO_ALIVE_PCLK + 1)
#define CLKS_NR_HSI (CLK_GOUT_HSI_VGEN_LITE_HSI_CLK + 1)

/* CMU_TOP */
#define PLL_LOCKTIME_PLL_G3D 0x00
#define PLL_LOCKTIME_PLL_MMC 0x04
#define PLL_LOCKTIME_PLL_SHARED0 0x08
#define PLL_LOCKTIME_PLL_SHARED1 0x0c
#define PLL_LOCKTIME_PLL_SHARED2 0x10
#define PLL_LOCKTIME_REG_PLL_G3D 0x80
#define PLL_LOCKTIME_REG_PLL_MMC 0x84
#define PLL_LOCKTIME_REG_PLL_SHARED0 0x88
#define PLL_LOCKTIME_REG_PLL_SHARED1 0x8c
#define PLL_LOCKTIME_REG_PLL_SHARED2 0x90
#define PLL_CON3_PLL_G3D 0x10c
#define PLL_CON3_PLL_MMC 0x14c
#define PLL_CON0_PLL_SHARED0 0x180
#define PLL_CON3_PLL_SHARED0 0x18c
#define PLL_CON0_PLL_SHARED1 0x1c0
#define PLL_CON3_PLL_SHARED1 0x1cc
#define PLL_CON0_PLL_SHARED2 0x200
#define PLL_CON3_PLL_SHARED2 0x20c
#define CLK_CON_MUX_CLKCMU_G3D_BUS 0x1000
#define CLK_CON_MUX_MUX_CLKCMU_ALIVE_BUS 0x1004
#define CLK_CON_MUX_MUX_CLKCMU_AUD_BUS 0x1008
#define CLK_CON_MUX_MUX_CLKCMU_AUD_CPU 0x100c
#define CLK_CON_MUX_MUX_CLKCMU_BUSC_BUS 0x1010
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0 0x1014
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1 0x1018
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2 0x101c
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3 0x1020
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4 0x1024
#define CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5 0x1028
#define CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST 0x102c
#define CLK_CON_MUX_MUX_CLKCMU_CORE_BUS 0x1030
#define CLK_CON_MUX_MUX_CLKCMU_CORE_G3D 0x1034
#define CLK_CON_MUX_MUX_CLKCMU_CORE_SSS 0x1038
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL0_BUSP 0x103c
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL0_DBG_BUS 0x1040
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH 0x1044
#define CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH 0x1048
#define CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS 0x104c
#define CLK_CON_MUX_MUX_CLKCMU_DPU_BUS 0x1050
#define CLK_CON_MUX_MUX_CLKCMU_DPU_DSIM 0x1054
#define CLK_CON_MUX_MUX_CLKCMU_DSU_SWITCH 0x1058
#define CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH 0x105c
#define CLK_CON_MUX_MUX_CLKCMU_HSI_BUS 0x1060
#define CLK_CON_MUX_MUX_CLKCMU_HSI_UFS_EMBD 0x1064
#define CLK_CON_MUX_MUX_CLKCMU_ISP_BUS 0x1068
#define CLK_CON_MUX_MUX_CLKCMU_M2M_MSCL 0x106c
#define CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS 0x1070
#define CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC 0x1074
#define CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC 0x1078
#define CLK_CON_MUX_MUX_CLKCMU_MFC_MFC 0x1080
#define CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP 0x1084
#define CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH 0x1088
#define CLK_CON_MUX_MUX_CLKCMU_NPU0_BUS 0x108c
#define CLK_CON_MUX_MUX_CLKCMU_NPUS_BUS 0x1090
#define CLK_CON_MUX_MUX_CLKCMU_PERI_BUS 0x1094
#define CLK_CON_MUX_MUX_CLKCMU_PERI_IP 0x1098
#define CLK_CON_MUX_MUX_CLKCMU_PERI_MMC_CARD 0x109c
#define CLK_CON_MUX_MUX_CLKCMU_TAA_BUS 0x10a0
#define CLK_CON_MUX_MUX_CLKCMU_TNR_BUS 0x10a4
#define CLK_CON_MUX_MUX_CLKCMU_USB_BUS 0x10a8
#define CLK_CON_MUX_MUX_CLKCMU_USB_USB20DRD 0x10ac
#define CLK_CON_MUX_MUX_CMU_CMUREF 0x10b0
#define CLK_CON_DIV_CLKCMU_ALIVE_BUS 0x1800
#define CLK_CON_DIV_CLKCMU_AUD_BUS 0x1804
#define CLK_CON_DIV_CLKCMU_AUD_CPU 0x1808
#define CLK_CON_DIV_CLKCMU_BUSC_BUS 0x180c
#define CLK_CON_DIV_CLKCMU_CIS_CLK0 0x1810
#define CLK_CON_DIV_CLKCMU_CIS_CLK1 0x1814
#define CLK_CON_DIV_CLKCMU_CIS_CLK2 0x1818
#define CLK_CON_DIV_CLKCMU_CIS_CLK3 0x181c
#define CLK_CON_DIV_CLKCMU_CIS_CLK4 0x1820
#define CLK_CON_DIV_CLKCMU_CIS_CLK5 0x1824
#define CLK_CON_DIV_CLKCMU_CMU_BOOST 0x1828
#define CLK_CON_DIV_CLKCMU_CORE_BUS 0x182c
#define CLK_CON_DIV_CLKCMU_CORE_G3D 0x1830
#define CLK_CON_DIV_CLKCMU_CORE_SSS 0x1834
#define CLK_CON_DIV_CLKCMU_CPUCL0_BUSP 0x1838
#define CLK_CON_DIV_CLKCMU_CPUCL0_DBG_BUS 0x183c
#define CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH 0x1840
#define CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH 0x1844
#define CLK_CON_DIV_CLKCMU_CSIS_BUS 0x1848
#define CLK_CON_DIV_CLKCMU_DPU_BUS 0x184c
#define CLK_CON_DIV_CLKCMU_DPU_DSIM 0x1850
#define CLK_CON_DIV_CLKCMU_DSU_SWITCH 0x1854
#define CLK_CON_DIV_CLKCMU_G3D_SWITCH 0x1858
#define CLK_CON_DIV_CLKCMU_HSI_BUS 0x185c
#define CLK_CON_DIV_CLKCMU_HSI_UFS_EMBD 0x1860
#define CLK_CON_DIV_CLKCMU_ISP_BUS 0x1864
#define CLK_CON_DIV_CLKCMU_M2M_MSCL 0x1868
#define CLK_CON_DIV_CLKCMU_MCSC_BUS 0x186c
#define CLK_CON_DIV_CLKCMU_MCSC_GDC 0x1870
#define CLK_CON_DIV_CLKCMU_MCSC_MCSC 0x1874
#define CLK_CON_DIV_CLKCMU_MFC_MFC 0x187c
#define CLK_CON_DIV_CLKCMU_MIF_BUSP 0x1880
#define CLK_CON_DIV_CLKCMU_NPU0_BUS 0x1884
#define CLK_CON_DIV_CLKCMU_NPUS_BUS 0x1888
#define CLK_CON_DIV_CLKCMU_OTP 0x188c
#define CLK_CON_DIV_CLKCMU_PERI_BUS 0x1890
#define CLK_CON_DIV_CLKCMU_PERI_IP 0x1894
#define CLK_CON_DIV_CLKCMU_PERI_MMC_CARD 0x1898
#define CLK_CON_DIV_CLKCMU_TAA_BUS 0x189c
#define CLK_CON_DIV_CLKCMU_TNR_BUS 0x18a0
#define CLK_CON_DIV_CLKCMU_USB_BUS 0x18a4
#define CLK_CON_DIV_CLKCMU_USB_USB20DRD 0x18a8
#define CLK_CON_GAT_AP2CP_HISPEEDY_CLK 0x2000
#define CLK_CON_GAT_AP2CP_SHARED0_CLK 0x2004
#define CLK_CON_GAT_AP2CP_SHARED1_CLK 0x2008
#define CLK_CON_GAT_AP2CP_SHARED2_CLK 0x200c
#define CLK_CON_GAT_CLKCMU_MIF_SWITCH 0x2010
#define CLK_CON_GAT_GATE_CLKCMU_ALIVE_BUS 0x2014
#define CLK_CON_GAT_GATE_CLKCMU_AUD_BUS 0x2018
#define CLK_CON_GAT_GATE_CLKCMU_AUD_CPU 0x201c
#define CLK_CON_GAT_GATE_CLKCMU_BUSC_BUS 0x2020
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0 0x2024
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1 0x2028
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2 0x202c
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3 0x2030
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4 0x2034
#define CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5 0x2038
#define CLK_CON_GAT_GATE_CLKCMU_CORE_BUS 0x203c
#define CLK_CON_GAT_GATE_CLKCMU_CORE_G3D 0x2040
#define CLK_CON_GAT_GATE_CLKCMU_CORE_SSS 0x2044
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL0_BUSP 0x2048
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_BUS 0x204c
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH 0x2050
#define CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH 0x2054
#define CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS 0x2058
#define CLK_CON_GAT_GATE_CLKCMU_DPU_BUS 0x205c
#define CLK_CON_GAT_GATE_CLKCMU_DPU_DSIM 0x2060
#define CLK_CON_GAT_GATE_CLKCMU_DSU_SWITCH 0x2064
#define CLK_CON_GAT_GATE_CLKCMU_M2M_MSCL 0x2068
#define CLK_CON_GAT_GATE_CLKCMU_G3D_BUS 0x206c
#define CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH 0x2070
#define CLK_CON_GAT_GATE_CLKCMU_HSI_BUS 0x2074
#define CLK_CON_GAT_GATE_CLKCMU_HSI_UFS_EMBD 0x2078
#define CLK_CON_GAT_GATE_CLKCMU_ISP_BUS 0x207c
#define CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS 0x2080
#define CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC 0x2084
#define CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC 0x2088
#define CLK_CON_GAT_GATE_CLKCMU_MFC_MFC 0x2090
#define CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP 0x2094
#define CLK_CON_GAT_GATE_CLKCMU_NPU0_BUS 0x2098
#define CLK_CON_GAT_GATE_CLKCMU_NPUS_BUS 0x209c
#define CLK_CON_GAT_GATE_CLKCMU_PERI_BUS 0x20a0
#define CLK_CON_GAT_GATE_CLKCMU_PERI_IP 0x20a4
#define CLK_CON_GAT_GATE_CLKCMU_PERI_MMC_CARD 0x20a8
#define CLK_CON_GAT_GATE_CLKCMU_TAA_BUS 0x20ac
#define CLK_CON_GAT_GATE_CLKCMU_TNR_BUS 0x20b0
#define CLK_CON_GAT_GATE_CLKCMU_USB_BUS 0x20b4
#define CLK_CON_GAT_GATE_CLKCMU_USB_USB20DRD 0x20b8

static const unsigned long top_clk_regs[] __initconst = {
	PLL_LOCKTIME_PLL_G3D,
	PLL_LOCKTIME_PLL_MMC,
	PLL_LOCKTIME_PLL_SHARED0,
	PLL_LOCKTIME_PLL_SHARED1,
	PLL_LOCKTIME_PLL_SHARED2,
	PLL_LOCKTIME_REG_PLL_G3D,
	PLL_LOCKTIME_REG_PLL_MMC,
	PLL_LOCKTIME_REG_PLL_SHARED0,
	PLL_LOCKTIME_REG_PLL_SHARED1,
	PLL_LOCKTIME_REG_PLL_SHARED2,
	PLL_CON3_PLL_G3D,
	PLL_CON3_PLL_MMC,
	PLL_CON0_PLL_SHARED0,
	PLL_CON3_PLL_SHARED0,
	PLL_CON0_PLL_SHARED1,
	PLL_CON3_PLL_SHARED1,
	PLL_CON0_PLL_SHARED2,
	PLL_CON3_PLL_SHARED2,
	CLK_CON_MUX_CLKCMU_G3D_BUS,
	CLK_CON_MUX_MUX_CLKCMU_ALIVE_BUS,
	CLK_CON_MUX_MUX_CLKCMU_AUD_BUS,
	CLK_CON_MUX_MUX_CLKCMU_AUD_CPU,
	CLK_CON_MUX_MUX_CLKCMU_BUSC_BUS,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4,
	CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5,
	CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST,
	CLK_CON_MUX_MUX_CLKCMU_CORE_BUS,
	CLK_CON_MUX_MUX_CLKCMU_CORE_G3D,
	CLK_CON_MUX_MUX_CLKCMU_CORE_SSS,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL0_BUSP,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS,
	CLK_CON_MUX_MUX_CLKCMU_DPU_BUS,
	CLK_CON_MUX_MUX_CLKCMU_DPU_DSIM,
	CLK_CON_MUX_MUX_CLKCMU_DSU_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_HSI_BUS,
	CLK_CON_MUX_MUX_CLKCMU_HSI_UFS_EMBD,
	CLK_CON_MUX_MUX_CLKCMU_ISP_BUS,
	CLK_CON_MUX_MUX_CLKCMU_M2M_MSCL,
	CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS,
	CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC,
	CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC,
	CLK_CON_MUX_MUX_CLKCMU_MFC_MFC,
	CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP,
	CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH,
	CLK_CON_MUX_MUX_CLKCMU_NPU0_BUS,
	CLK_CON_MUX_MUX_CLKCMU_NPUS_BUS,
	CLK_CON_MUX_MUX_CLKCMU_PERI_BUS,
	CLK_CON_MUX_MUX_CLKCMU_PERI_IP,
	CLK_CON_MUX_MUX_CLKCMU_PERI_MMC_CARD,
	CLK_CON_MUX_MUX_CLKCMU_TAA_BUS,
	CLK_CON_MUX_MUX_CLKCMU_TNR_BUS,
	CLK_CON_MUX_MUX_CLKCMU_USB_BUS,
	CLK_CON_MUX_MUX_CLKCMU_USB_USB20DRD,
	CLK_CON_MUX_MUX_CMU_CMUREF,
	CLK_CON_DIV_CLKCMU_ALIVE_BUS,
	CLK_CON_DIV_CLKCMU_AUD_BUS,
	CLK_CON_DIV_CLKCMU_AUD_CPU,
	CLK_CON_DIV_CLKCMU_BUSC_BUS,
	CLK_CON_DIV_CLKCMU_CIS_CLK0,
	CLK_CON_DIV_CLKCMU_CIS_CLK1,
	CLK_CON_DIV_CLKCMU_CIS_CLK2,
	CLK_CON_DIV_CLKCMU_CIS_CLK3,
	CLK_CON_DIV_CLKCMU_CIS_CLK4,
	CLK_CON_DIV_CLKCMU_CIS_CLK5,
	CLK_CON_DIV_CLKCMU_CMU_BOOST,
	CLK_CON_DIV_CLKCMU_CORE_BUS,
	CLK_CON_DIV_CLKCMU_CORE_G3D,
	CLK_CON_DIV_CLKCMU_CORE_SSS,
	CLK_CON_DIV_CLKCMU_CPUCL0_BUSP,
	CLK_CON_DIV_CLKCMU_CPUCL0_DBG_BUS,
	CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_DIV_CLKCMU_CSIS_BUS,
	CLK_CON_DIV_CLKCMU_DPU_BUS,
	CLK_CON_DIV_CLKCMU_DPU_DSIM,
	CLK_CON_DIV_CLKCMU_DSU_SWITCH,
	CLK_CON_DIV_CLKCMU_G3D_SWITCH,
	CLK_CON_DIV_CLKCMU_HSI_BUS,
	CLK_CON_DIV_CLKCMU_HSI_UFS_EMBD,
	CLK_CON_DIV_CLKCMU_ISP_BUS,
	CLK_CON_DIV_CLKCMU_M2M_MSCL,
	CLK_CON_DIV_CLKCMU_MCSC_BUS,
	CLK_CON_DIV_CLKCMU_MCSC_GDC,
	CLK_CON_DIV_CLKCMU_MCSC_MCSC,
	CLK_CON_DIV_CLKCMU_MFC_MFC,
	CLK_CON_DIV_CLKCMU_MIF_BUSP,
	CLK_CON_DIV_CLKCMU_NPU0_BUS,
	CLK_CON_DIV_CLKCMU_NPUS_BUS,
	CLK_CON_DIV_CLKCMU_OTP,
	CLK_CON_DIV_CLKCMU_PERI_BUS,
	CLK_CON_DIV_CLKCMU_PERI_IP,
	CLK_CON_DIV_CLKCMU_PERI_MMC_CARD,
	CLK_CON_DIV_CLKCMU_TAA_BUS,
	CLK_CON_DIV_CLKCMU_TNR_BUS,
	CLK_CON_DIV_CLKCMU_USB_BUS,
	CLK_CON_DIV_CLKCMU_USB_USB20DRD,
	CLK_CON_GAT_AP2CP_HISPEEDY_CLK,
	CLK_CON_GAT_AP2CP_SHARED0_CLK,
	CLK_CON_GAT_AP2CP_SHARED1_CLK,
	CLK_CON_GAT_AP2CP_SHARED2_CLK,
	CLK_CON_GAT_CLKCMU_MIF_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_ALIVE_BUS,
	CLK_CON_GAT_GATE_CLKCMU_AUD_BUS,
	CLK_CON_GAT_GATE_CLKCMU_AUD_CPU,
	CLK_CON_GAT_GATE_CLKCMU_BUSC_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4,
	CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5,
	CLK_CON_GAT_GATE_CLKCMU_CORE_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CORE_G3D,
	CLK_CON_GAT_GATE_CLKCMU_CORE_SSS,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL0_BUSP,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL0_DBG_BUS,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS,
	CLK_CON_GAT_GATE_CLKCMU_DPU_BUS,
	CLK_CON_GAT_GATE_CLKCMU_DPU_DSIM,
	CLK_CON_GAT_GATE_CLKCMU_DSU_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_M2M_MSCL,
	CLK_CON_GAT_GATE_CLKCMU_G3D_BUS,
	CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH,
	CLK_CON_GAT_GATE_CLKCMU_HSI_BUS,
	CLK_CON_GAT_GATE_CLKCMU_HSI_UFS_EMBD,
	CLK_CON_GAT_GATE_CLKCMU_ISP_BUS,
	CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS,
	CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC,
	CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC,
	CLK_CON_GAT_GATE_CLKCMU_MFC_MFC,
	CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP,
	CLK_CON_GAT_GATE_CLKCMU_NPU0_BUS,
	CLK_CON_GAT_GATE_CLKCMU_NPUS_BUS,
	CLK_CON_GAT_GATE_CLKCMU_PERI_BUS,
	CLK_CON_GAT_GATE_CLKCMU_PERI_IP,
	CLK_CON_GAT_GATE_CLKCMU_PERI_MMC_CARD,
	CLK_CON_GAT_GATE_CLKCMU_TAA_BUS,
	CLK_CON_GAT_GATE_CLKCMU_TNR_BUS,
	CLK_CON_GAT_GATE_CLKCMU_USB_BUS,
	CLK_CON_GAT_GATE_CLKCMU_USB_USB20DRD,
};

static const struct samsung_pll_rate_table pll_shared0_rate_tables[] __initconst = {
    PLL_35XX_RATE(26 * MHZ, 1599000000, 123, 2, 0),
    PLL_35XX_RATE(26 * MHZ, 1599000000, 246, 4, 0),
};

static const struct samsung_pll_rate_table pll_shared1_rate_tables[] __initconst = {
    PLL_35XX_RATE(26 * MHZ, 1332500000, 205, 4, 0),
};

static const struct samsung_pll_rate_table pll_shared2_rate_tables[] __initconst = {
    PLL_35XX_RATE(26 * MHZ, 936000000, 72, 2, 0),
};

static const struct samsung_pll_clock top_pll_clks[] __initconst = {
	PLL(pll_531x, CLK_FOUT_SHARED0_PLL, "fout_shared0_pll", "oscclk",
        	PLL_LOCKTIME_PLL_SHARED0, PLL_CON3_PLL_SHARED0,
		pll_shared0_rate_tables),
	PLL(pll_531x, CLK_FOUT_SHARED1_PLL, "fout_shared1_pll", "oscclk",
        	PLL_LOCKTIME_PLL_SHARED1, PLL_CON3_PLL_SHARED1,
	        pll_shared1_rate_tables),
	PLL(pll_531x, CLK_FOUT_SHARED2_PLL, "fout_shared2_pll", "oscclk",
        	PLL_LOCKTIME_PLL_SHARED2, PLL_CON3_PLL_SHARED2,
        	pll_shared2_rate_tables),
	PLL(pll_531x, CLK_FOUT_G3D_PLL, "fout_g3d_pll", "oscclk",
		PLL_LOCKTIME_PLL_G3D, PLL_CON3_PLL_G3D, NULL),
	PLL(pll_531x, CLK_FOUT_MMC_PLL, "fout_mmc_pll", "oscclk",
		PLL_LOCKTIME_PLL_MMC, PLL_CON3_PLL_MMC, NULL),
};

PNAME(mout_pll_shared0_p)		= { "oscclk", "fout_shared0_pll" };
PNAME(mout_pll_shared1_p)		= { "oscclk", "fout_shared1_pll" };
PNAME(mout_pll_shared2_p)		= { "oscclk", "fout_shared2_pll" };
PNAME(mout_cmu_g3d_switch_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_g3d_bus_p)		= { "fout_g3d_pll" };
PNAME(mout_cmu_alive_bus_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_aud_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div3", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_aud_cpu_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_busc_bus_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_cis_clk0_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cis_clk1_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cis_clk2_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cis_clk3_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cis_clk4_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cis_clk5_p)		= { "oscclk", "dout_cmu_shared0_div4" };
PNAME(mout_cmu_cmu_boost_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_core_bus_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_core_g3d_p)		= { "fout_g3d_pll", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_core_sss_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_cpucl0_busp_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div3", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_cpucl0_switch_p) 	= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_cpucl1_switch_p) 	= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_csis_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_dpu_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_dpu_dsim_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_dsu_switch_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_hsi_bus_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4",
					"dout_cmu_mmc_div2", "oscclk" };
PNAME(mout_cmu_hsi_ufs_embd_p)		= { "oscclk", "dout_cmu_shared0_div2",
					"dout_cmu_shared0_div4", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_isp_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_m2m_mscl_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared0_div4", "dout_cmu_shared1_div2" };
PNAME(mout_cmu_mcsc_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_mcsc_gdc_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_mcsc_mcsc_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_mfc_mfc_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_mif_busp_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_mif_switch_p)		= { "fout_shared0_pll", "dout_cmu_shared0_div2",
					"fout_shared1_pll", "fout_shared2_pll" };
PNAME(mout_cmu_npu0_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared1_div2",
					"dout_cmu_shared1_div3", "fout_shared2_pll" };
PNAME(mout_cmu_npus_bus_p)		= { "dout_cmu_shared0_div2", "dout_cmu_shared0_div3",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_peri_bus_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4",
					"dout_cmu_mmc_div2", "oscclk" };
PNAME(mout_cmu_peri_ip_p)		= { "dout_cmu_shared0_div4", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_peri_mmc_card_p)		= { "oscclk", "dout_cmu_shared0_div2", "fout_mmc_pll",
					"dout_cmu_shared1_div2" };
PNAME(mout_cmu_taa_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_tnr_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div2", "dout_cmu_shared1_div3" };
PNAME(mout_cmu_usb_bus_p)		= { "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div3", "dout_cmu_shared1_div4" };
PNAME(mout_cmu_usb_usb20drd_p)		= { "oscclk", "dout_cmu_shared0_div3", "dout_cmu_shared0_div4",
					"dout_cmu_shared1_div4" };
PNAME(mout_cmu_cmuref_p)		= { "oscclk", "mout_cmu_cmu_boost" };

/*
 * Register name to clock name mangling strategy used in this file
 *
 * Replace PLL_CON0/3_PLL		with CLK_MOUT_PLL and mout_pll
 * Replace CLK_CON_MUX_MUX_CLKCMU	with CLK_MOUT_CMU and mout_cmu
 * Replace CLK_CON_DIV_CLKCMU      	with CLK_DOUT_CMU and dout_cmu
 * Replace CLK_CON_DIV_DIV_CLKCMU  	with CLK_DOUT_CMU and dout_cmu
 * Replace CLK_CON_GAT_CLKCMU      	with CLK_GOUT_CMU and gout_cmu
 * Replace CLK_CON_GAT_GATE_CLKCMU 	with CLK_GOUT_CMU and gout_cmu
 *
 * For gates remove _UID, _BLK, _IPCLKPORT, _RSTNSYNC, _APBIF, _RCO and _I
 */

static const struct samsung_mux_clock top_mux_clks[] __initconst = {
	MUX(CLK_MOUT_PLL_SHARED0, "mout_pll_shared0", mout_pll_shared0_p,
	    PLL_CON0_PLL_SHARED0, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED1, "mout_pll_shared1", mout_pll_shared1_p,
	    PLL_CON0_PLL_SHARED1, 4, 1),
	MUX(CLK_MOUT_PLL_SHARED2, "mout_pll_shared2", mout_pll_shared2_p,
	    PLL_CON0_PLL_SHARED2, 4, 1),
	MUX(CLK_MOUT_CMU_G3D_SWITCH, "mout_cmu_g3d_switch", mout_cmu_g3d_switch_p,
		CLK_CON_MUX_MUX_CLKCMU_G3D_SWITCH, 0, 2),
	MUX(CLK_MOUT_CMU_G3D_BUS, "mout_cmu_g3d_bus", mout_cmu_g3d_bus_p,
		CLK_CON_MUX_CLKCMU_G3D_BUS, 0, 1),
	MUX(CLK_MOUT_CMU_ALIVE_BUS, "mout_cmu_alive_bus", mout_cmu_alive_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_ALIVE_BUS, 0, 1),
	MUX(CLK_MOUT_CMU_AUD_BUS, "mout_cmu_aud_bus", mout_cmu_aud_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_AUD_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_AUD_CPU, "mout_cmu_aud_cpu", mout_cmu_aud_cpu_p,
		CLK_CON_MUX_MUX_CLKCMU_AUD_CPU, 0, 2),
	MUX(CLK_MOUT_CMU_BUSC_BUS, "mout_cmu_busc_bus", mout_cmu_busc_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_BUSC_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_CIS_CLK0, "mout_cmu_cis_clk0", mout_cmu_cis_clk0_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK0, 0, 1),
	MUX(CLK_MOUT_CMU_CIS_CLK1, "mout_cmu_cis_clk1", mout_cmu_cis_clk1_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK1, 0, 1),
	MUX(CLK_MOUT_CMU_CIS_CLK2, "mout_cmu_cis_clk2", mout_cmu_cis_clk2_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK2, 0, 1),
	MUX(CLK_MOUT_CMU_CIS_CLK3, "mout_cmu_cis_clk3", mout_cmu_cis_clk3_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK3, 0, 1),
	MUX(CLK_MOUT_CMU_CIS_CLK4, "mout_cmu_cis_clk4", mout_cmu_cis_clk4_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK4, 0, 1),
	MUX(CLK_MOUT_CMU_CIS_CLK5, "mout_cmu_cis_clk5", mout_cmu_cis_clk5_p,
		CLK_CON_MUX_MUX_CLKCMU_CIS_CLK5, 0, 1),
	MUX(CLK_MOUT_CMU_CMU_BOOST, "mout_cmu_cmu_boost", mout_cmu_cmu_boost_p,
		CLK_CON_MUX_MUX_CLKCMU_CMU_BOOST, 0, 1),
	MUX(CLK_MOUT_CMU_CORE_BUS, "mout_cmu_core_bus", mout_cmu_core_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_CORE_BUS, 0, 1),
	MUX(CLK_MOUT_CMU_CORE_G3D, "mout_cmu_core_g3d", mout_cmu_core_g3d_p,
		CLK_CON_MUX_MUX_CLKCMU_CORE_G3D, 0, 2),
	MUX(CLK_MOUT_CMU_CORE_SSS, "mout_cmu_core_sss", mout_cmu_core_sss_p,
		CLK_CON_MUX_MUX_CLKCMU_CORE_SSS, 0, 1),
	MUX(CLK_MOUT_CMU_CPUCL0_BUSP, "mout_cmu_cpucl0_busp", mout_cmu_cpucl0_busp_p,
		CLK_CON_MUX_MUX_CLKCMU_CPUCL0_BUSP, 0, 2),
	MUX(CLK_MOUT_CMU_CPUCL0_SWITCH, "mout_cmu_cpucl0_switch", mout_cmu_cpucl0_switch_p,
		CLK_CON_MUX_MUX_CLKCMU_CPUCL0_SWITCH, 0, 2),
	MUX(CLK_MOUT_CMU_CPUCL1_SWITCH, "mout_cmu_cpucl1_switch", mout_cmu_cpucl1_switch_p,
		CLK_CON_MUX_MUX_CLKCMU_CPUCL1_SWITCH, 0, 2),
	MUX(CLK_MOUT_CMU_CSIS_BUS, "mout_cmu_csis_bus", mout_cmu_csis_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_CSIS_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_DPU_BUS, "mout_cmu_dpu_bus", mout_cmu_dpu_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_DPU_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_DPU_DSIM, "mout_cmu_dpu_dsim", mout_cmu_dpu_dsim_p,
		CLK_CON_MUX_MUX_CLKCMU_DPU_DSIM, 0, 1),
	MUX(CLK_MOUT_CMU_DSU_SWITCH, "mout_cmu_dsu_switch", mout_cmu_dsu_switch_p,
		CLK_CON_MUX_MUX_CLKCMU_DSU_SWITCH, 0, 2),
	MUX(CLK_MOUT_CMU_HSI_BUS, "mout_cmu_hsi_bus", mout_cmu_hsi_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_HSI_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_HSI_UFS_EMBD, "mout_cmu_hsi_ufs_embd", mout_cmu_hsi_ufs_embd_p,
		CLK_CON_MUX_MUX_CLKCMU_HSI_UFS_EMBD, 0, 2),
	MUX(CLK_MOUT_CMU_ISP_BUS, "mout_cmu_isp_bus", mout_cmu_isp_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_ISP_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_M2M_MSCL, "mout_cmu_m2m_mscl", mout_cmu_m2m_mscl_p,
		CLK_CON_MUX_MUX_CLKCMU_M2M_MSCL, 0, 2),
	MUX(CLK_MOUT_CMU_MCSC_BUS, "mout_cmu_mcsc_bus", mout_cmu_mcsc_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_MCSC_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_MCSC_GDC, "mout_cmu_mcsc_gdc", mout_cmu_mcsc_gdc_p,
		CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC, 0, 2),
	MUX(CLK_MOUT_CMU_MCSC_MCSC, "mout_cmu_mcsc_mcsc", mout_cmu_mcsc_mcsc_p,
		CLK_CON_MUX_MUX_CLKCMU_MCSC_MCSC, 0, 2),
	MUX(CLK_MOUT_CMU_MFC_MFC, "mout_cmu_mfc_mfc", mout_cmu_mfc_mfc_p,
		CLK_CON_MUX_MUX_CLKCMU_MFC_MFC, 0, 2),
	MUX(CLK_MOUT_CMU_MIF_BUSP, "mout_cmu_mif_busp", mout_cmu_mif_busp_p,
		CLK_CON_MUX_MUX_CLKCMU_MIF_BUSP, 0, 1),
	MUX(CLK_MOUT_CMU_MIF_SWITCH, "mout_cmu_mif_switch", mout_cmu_mif_switch_p,
		CLK_CON_MUX_MUX_CLKCMU_MIF_SWITCH, 0, 2),
	MUX(CLK_MOUT_CMU_NPU0_BUS, "mout_cmu_npu0_bus", mout_cmu_npu0_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_NPU0_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_NPUS_BUS, "mout_cmu_npus_bus", mout_cmu_npus_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_NPUS_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_PERI_BUS, "mout_cmu_peri_bus", mout_cmu_peri_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_PERI_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_PERI_IP, "mout_cmu_peri_ip", mout_cmu_peri_ip_p,
		CLK_CON_MUX_MUX_CLKCMU_PERI_IP, 0, 1),
	MUX(CLK_MOUT_CMU_PERI_MMC_CARD, "mout_cmu_peri_mmc_card", mout_cmu_peri_mmc_card_p,
		CLK_CON_MUX_MUX_CLKCMU_PERI_MMC_CARD, 0, 2),
	MUX(CLK_MOUT_CMU_TAA_BUS, "mout_cmu_taa_bus", mout_cmu_taa_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_TAA_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_TNR_BUS, "mout_cmu_tnr_bus", mout_cmu_tnr_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_TNR_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_USB_BUS, "mout_cmu_usb_bus", mout_cmu_usb_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_USB_BUS, 0, 1),
	MUX(CLK_MOUT_CMU_USB_USB20DRD, "mout_cmu_usb_usb20drd", mout_cmu_usb_usb20drd_p,
		CLK_CON_MUX_MUX_CLKCMU_MCSC_GDC, 0, 1),
	MUX(CLK_MOUT_CMU_CMUREF, "mout_cmu_cmuref", mout_cmu_cmuref_p,
		CLK_CON_MUX_MUX_CMU_CMUREF, 0, 1),
};

static const struct samsung_div_clock top_div_clks[] __initconst = {
	DIV(CLK_DOUT_CMU_ALIVE_BUS, "dout_cmu_alive_bus", "gout_cmu_alive_bus",
		CLK_CON_DIV_CLKCMU_ALIVE_BUS, 0, 3),
	DIV(CLK_DOUT_CMU_AUD_BUS, "dout_cmu_aud_bus", "gout_cmu_aud_bus",
		CLK_CON_DIV_CLKCMU_AUD_BUS, 0, 3),
	DIV(CLK_DOUT_CMU_AUD_CPU, "dout_cmu_aud_cpu", "gout_cmu_aud_cpu",
		CLK_CON_DIV_CLKCMU_AUD_CPU, 0, 4),
	DIV(CLK_DOUT_CMU_BUSC_BUS, "dout_cmu_busc_bus", "gout_cmu_busc_bus",
		CLK_CON_DIV_CLKCMU_BUSC_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_CIS_CLK0, "dout_cmu_cis_clk0", "gout_cmu_cis_clk0",
		CLK_CON_DIV_CLKCMU_CIS_CLK0, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK1, "dout_cmu_cis_clk1", "gout_cmu_cis_clk1",
		CLK_CON_DIV_CLKCMU_CIS_CLK1, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK2, "dout_cmu_cis_clk2", "gout_cmu_cis_clk2",
		CLK_CON_DIV_CLKCMU_CIS_CLK2, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK3, "dout_cmu_cis_clk3", "gout_cmu_cis_clk3",
		CLK_CON_DIV_CLKCMU_CIS_CLK3, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK4, "dout_cmu_cis_clk4", "gout_cmu_cis_clk4",
		CLK_CON_DIV_CLKCMU_CIS_CLK4, 0, 5),
	DIV(CLK_DOUT_CMU_CIS_CLK5, "dout_cmu_cis_clk5", "gout_cmu_cis_clk5",
		CLK_CON_DIV_CLKCMU_CIS_CLK5, 0, 5),
	DIV(CLK_DOUT_CMU_BOOST, "dout_cmu_boost", "mout_cmu_boost",
		CLK_CON_DIV_CLKCMU_CMU_BOOST, 0, 3),
	DIV(CLK_DOUT_CMU_CORE_BUS, "dout_cmu_core_bus", "gout_cmu_core_bus",
		CLK_CON_DIV_CLKCMU_CORE_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_CORE_G3D, "dout_cmu_core_g3d", "gout_cmu_core_g3d",
		CLK_CON_DIV_CLKCMU_CORE_G3D, 0, 4),
	DIV(CLK_DOUT_CMU_CORE_SSS, "dout_cmu_core_sss", "gout_cmu_core_sss",
		CLK_CON_DIV_CLKCMU_CORE_SSS, 0, 4),
	DIV(CLK_DOUT_CMU_CPUCL0_BUSP, "dout_cmu_cpucl0_busp", "gout_cmu_cpucl0_busp",
		CLK_CON_DIV_CLKCMU_CPUCL0_BUSP, 0, 4),
	DIV(CLK_DOUT_CMU_CPUCL0_SWITCH, "dout_cmu_cpucl0_switch", "gout_cmu_cpucl0_switch",
		CLK_CON_DIV_CLKCMU_CPUCL0_SWITCH, 0, 3),
	DIV(CLK_DOUT_CMU_CPUCL1_SWITCH, "dout_cmu_cpucl1_switch", "gout_cmu_cpucl1_switch",
		CLK_CON_DIV_CLKCMU_CPUCL1_SWITCH, 0, 3),
	DIV(CLK_DOUT_CMU_CSIS_BUS, "dout_cmu_csis_bus", "gout_cmu_csis_bus",
		CLK_CON_DIV_CLKCMU_CSIS_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_DPU_BUS, "dout_cmu_dpu_bus", "gout_cmu_dpu_bus",
		CLK_CON_DIV_CLKCMU_DPU_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_DPU_DSIM, "dout_cmu_dpu_dsim", "gout_cmu_dpu_dsim",
		CLK_CON_DIV_CLKCMU_DPU_DSIM, 0, 4),
	DIV(CLK_DOUT_CMU_DSU_SWITCH, "dout_cmu_dsu_switch", "gout_cmu_dsu_switch",
		CLK_CON_DIV_CLKCMU_DSU_SWITCH, 0, 3),
	DIV(CLK_DOUT_CMU_G3D_SWITCH, "dout_cmu_g3d_switch", "gout_cmu_g3d_switch",
		CLK_CON_DIV_CLKCMU_DSU_SWITCH, 0, 3),
	DIV(CLK_DOUT_CMU_HSI_BUS, "dout_cmu_hsi_bus", "gout_cmu_hsi_bus",
		CLK_CON_DIV_CLKCMU_HSI_BUS, 0, 3),
	DIV(CLK_DOUT_CMU_HSI_UFS_EMBD, "dout_cmu_hsi_ufs_embd", "gout_cmu_hsi_ufs_embd",
		CLK_CON_DIV_CLKCMU_HSI_UFS_EMBD, 0, 3),
	DIV(CLK_DOUT_CMU_ISP_BUS, "dout_cmu_isp_bus", "gout_cmu_isp_bus",
		CLK_CON_DIV_CLKCMU_ISP_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_M2M_MSCL, "dout_cmu_m2m_mscl", "gout_cmu_m2m_mscl",
		CLK_CON_DIV_CLKCMU_M2M_MSCL, 0, 4),
	DIV(CLK_DOUT_CMU_MCSC_BUS, "dout_cmu_mcsc_bus", "gout_cmu_mcsc_bus",
		CLK_CON_DIV_CLKCMU_MCSC_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_MCSC_GDC, "dout_cmu_mcsc_gdc", "gout_cmu_mcsc_gdc",
		CLK_CON_DIV_CLKCMU_MCSC_GDC, 0, 4),
	DIV(CLK_DOUT_CMU_MCSC_MCSC, "dout_cmu_mcsc_mcsc", "gout_cmu_mcsc_mcsc",
		CLK_CON_DIV_CLKCMU_MCSC_MCSC, 0, 4),
	DIV(CLK_DOUT_CMU_MFC_MFC, "dout_cmu_mfc_mfc", "gout_cmu_mfc_mfc",
		CLK_CON_DIV_CLKCMU_MFC_MFC, 0, 4),
	DIV(CLK_DOUT_CMU_MIF_BUSP, "dout_cmu_mif_busp", "gout_cmu_mif_busp",
		CLK_CON_DIV_CLKCMU_MIF_BUSP, 0, 4),
	DIV(CLK_DOUT_CMU_NPU0_BUS, "dout_cmu_npu0_bus", "gout_cmu_npu0_bus",
		CLK_CON_DIV_CLKCMU_NPU0_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_NPUS_BUS, "dout_cmu_npus_bus", "gout_cmu_npus_bus",
		CLK_CON_DIV_CLKCMU_NPUS_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_PERI_BUS, "dout_cmu_peri_bus", "gout_cmu_peri_bus",
		CLK_CON_DIV_CLKCMU_PERI_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_PERI_IP, "dout_cmu_peri_ip", "gout_cmu_peri_ip",
		CLK_CON_DIV_CLKCMU_PERI_IP, 0, 4),
	DIV(CLK_DOUT_CMU_PERI_MMC_CARD, "dout_cmu_peri_mmc_card", "gout_cmu_peri_mmc_card",
		CLK_CON_DIV_CLKCMU_PERI_MMC_CARD, 0, 9),
	DIV(CLK_DOUT_CMU_TAA_BUS, "dout_cmu_taa_bus", "gout_cmu_taa_bus",
		CLK_CON_DIV_CLKCMU_TAA_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_TNR_BUS, "dout_cmu_tnr_bus", "gout_cmu_tnr_bus",
		CLK_CON_DIV_CLKCMU_TNR_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_USB_BUS, "dout_cmu_usb_bus", "gout_cmu_usb_bus",
		CLK_CON_DIV_CLKCMU_USB_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_USB_USB20DRD, "dout_cmu_usb_usb20drd", "gout_cmu_usb_usb20drd",
		CLK_CON_DIV_CLKCMU_USB_USB20DRD, 0, 5),
};

static const struct samsung_fixed_factor_clock top_fixed_factor_clks[] __initconst = {
	FFACTOR(CLK_DOUT_CMU_SHARED0_DIV2, "dout_cmu_shared0_div2",
		"mout_pll_shared0", 1, 2, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED0_DIV3, "dout_cmu_shared0_div3",
		"mout_pll_shared0", 1, 3, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED0_DIV4, "dout_cmu_shared0_div4",
		"mout_pll_shared0", 1, 4, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED1_DIV2, "dout_cmu_shared1_div2",
		"mout_pll_shared1", 1, 2, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED1_DIV3, "dout_cmu_shared1_div3",
		"mout_pll_shared1", 1, 3, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED1_DIV4, "dout_cmu_shared1_div4",
		"mout_pll_shared1", 1, 4, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED2_DIV2, "dout_cmu_shared2_div2",
		"mout_pll_shared2", 1, 2, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED2_DIV3, "dout_cmu_shared2_div3",
		"mout_pll_shared2", 1, 3, 0),
	FFACTOR(CLK_DOUT_CMU_SHARED2_DIV4, "dout_cmu_shared2_div4",
		"mout_pll_shared2", 1, 4, 0),
	FFACTOR(CLK_DOUT_CMU_MMC_DIV2, "dout_cmu_mmc_div2",
		"fout_mmc_pll", 1, 2, 0),
};

static const struct samsung_gate_clock top_gate_clks[] __initconst = {
	GATE(CLK_GOUT_HISPEEDY_CLK, "gout_hispeedy_clk", "fout_shared0_pll",
		CLK_CON_GAT_AP2CP_HISPEEDY_CLK, 21, 0, 0),
	GATE(CLK_GOUT_SHARED0_CLK, "gout_shared0_clk", "fout_shared1_pll",
		CLK_CON_GAT_AP2CP_SHARED0_CLK, 21, 0, 0),
	GATE(CLK_GOUT_SHARED1_CLK, "gout_shared1_clk", "fout_shared0_pll",
		CLK_CON_GAT_AP2CP_SHARED1_CLK, 21, 0, 0),
	GATE(CLK_GOUT_SHARED2_CLK, "gout_shared2_clk", "fout_shared0_pll",
		CLK_CON_GAT_AP2CP_SHARED2_CLK, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MIF_SWITCH, "gout_cmu_mif_switch", "mout_cmu_mif_switch",
		CLK_CON_GAT_CLKCMU_MIF_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_ALIVE_BUS, "gout_cmu_alive_bus", "mout_cmu_alive_bus",
		CLK_CON_GAT_GATE_CLKCMU_ALIVE_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_AUD_BUS, "gout_cmu_aud_bus", "mout_cmu_aud_bus",
		CLK_CON_GAT_GATE_CLKCMU_AUD_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_AUD_CPU, "gout_cmu_aud_cpu", "mout_cmu_aud_cpu",
		CLK_CON_GAT_GATE_CLKCMU_AUD_CPU, 21, 0, 0),
	GATE(CLK_GOUT_CMU_BUSC_BUS, "gout_cmu_busc_bus", "mout_cmu_busc_bus",
		CLK_CON_GAT_GATE_CLKCMU_BUSC_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK0, "gout_cmu_cis_clk0", "mout_cmu_cis_clk0",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK0, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK1, "gout_cmu_cis_clk1", "mout_cmu_cis_clk1",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK1, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK2, "gout_cmu_cis_clk2", "mout_cmu_cis_clk2",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK2, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK3, "gout_cmu_cis_clk3", "mout_cmu_cis_clk3",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK3, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK4, "gout_cmu_cis_clk4", "mout_cmu_cis_clk4",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK4, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CIS_CLK5, "gout_cmu_cis_clk5", "mout_cmu_cis_clk5",
		CLK_CON_GAT_GATE_CLKCMU_CIS_CLK5, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CORE_BUS, "gout_cmu_core_bus", "mout_cmu_core_bus",
		CLK_CON_GAT_GATE_CLKCMU_CORE_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CORE_G3D, "gout_cmu_core_g3d", "mout_cmu_core_g3d",
		CLK_CON_GAT_GATE_CLKCMU_CORE_G3D, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CORE_SSS, "gout_cmu_core_sss", "mout_cmu_core_sss",
		CLK_CON_GAT_GATE_CLKCMU_CORE_SSS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL0_BUSP, "gout_cmu_cpucl0_busp", "mout_cmu_cpucl0_busp",
		CLK_CON_GAT_GATE_CLKCMU_CPUCL0_BUSP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL0_SWITCH, "gout_cmu_cpucl0_switch", "mout_cmu_cpucl0_switch",
		CLK_CON_GAT_GATE_CLKCMU_CPUCL0_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CPUCL1_SWITCH, "gout_cmu_cpucl1_switch", "mout_cmu_cpucl1_switch",
		CLK_CON_GAT_GATE_CLKCMU_CPUCL1_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CSIS_BUS, "gout_cmu_csis_bus", "mout_cmu_csis_bus",
		CLK_CON_GAT_GATE_CLKCMU_CSIS_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DPU_BUS, "gout_cmu_dpu_bus", "mout_cmu_dpu_bus",
		CLK_CON_GAT_GATE_CLKCMU_DPU_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DPU_DSIM, "gout_cmu_dpu_dsim", "mout_cmu_dpu_dsim",
		CLK_CON_GAT_GATE_CLKCMU_DPU_DSIM, 21, 0, 0),
	GATE(CLK_GOUT_CMU_DSU_SWITCH, "gout_cmu_dsu_switch", "mout_cmu_dsu_switch",
		CLK_CON_GAT_GATE_CLKCMU_DSU_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3D_BUS, "gout_cmu_g3d_bus", "fout_g3d_pll",
		CLK_CON_GAT_GATE_CLKCMU_G3D_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_G3D_SWITCH, "gout_cmu_g3d_switch", "mout_cmu_g3d_switch",
		CLK_CON_GAT_GATE_CLKCMU_G3D_SWITCH, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI_BUS, "gout_cmu_hsi_bus", "mout_cmu_hsi_bus",
		CLK_CON_GAT_GATE_CLKCMU_HSI_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_HSI_UFS_EMBD, "gout_cmu_hsi_ufs_embd", "mout_cmu_hsi_ufs_embd",
		CLK_CON_GAT_GATE_CLKCMU_HSI_UFS_EMBD, 21, 0, 0),
	GATE(CLK_GOUT_CMU_ISP_BUS, "gout_cmu_isp_bus", "mout_cmu_isp_bus",
		CLK_CON_GAT_GATE_CLKCMU_ISP_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_M2M_MSCL, "gout_cmu_m2m_mscl", "mout_cmu_m2m_mscl",
		CLK_CON_GAT_GATE_CLKCMU_M2M_MSCL, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MCSC_BUS, "gout_cmu_mcsc_bus", "mout_cmu_mcsc_bus",
		CLK_CON_GAT_GATE_CLKCMU_MCSC_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MCSC_GDC, "gout_cmu_mcsc_gdc", "mout_cmu_mcsc_gdc",
		CLK_CON_GAT_GATE_CLKCMU_MCSC_GDC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MCSC_MCSC, "gout_cmu_mcsc_mcsc", "mout_cmu_mcsc_mcsc",
		CLK_CON_GAT_GATE_CLKCMU_MCSC_MCSC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MFC_MFC, "gout_cmu_mfc_mfc", "mout_cmu_mfc_mfc",
		CLK_CON_GAT_GATE_CLKCMU_MFC_MFC, 21, 0, 0),
	GATE(CLK_GOUT_CMU_MIF_BUSP, "gout_cmu_mif_busp", "mout_cmu_mif_busp",
		CLK_CON_GAT_GATE_CLKCMU_MIF_BUSP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_NPU0_BUS, "gout_cmu_npu0_bus", "mout_cmu_npu0_bus",
		CLK_CON_GAT_GATE_CLKCMU_NPU0_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_NPUS_BUS, "gout_cmu_npus_bus", "mout_cmu_npus_bus",
		CLK_CON_GAT_GATE_CLKCMU_NPUS_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERI_BUS, "gout_cmu_peri_bus", "mout_cmu_peri_bus",
		CLK_CON_GAT_GATE_CLKCMU_PERI_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERI_IP, "gout_cmu_peri_ip", "mout_cmu_peri_ip",
		CLK_CON_GAT_GATE_CLKCMU_PERI_IP, 21, 0, 0),
	GATE(CLK_GOUT_CMU_PERI_MMC_CARD, "gout_cmu_peri_mmc_card", "mout_cmu_peri_mmc_card",
		CLK_CON_GAT_GATE_CLKCMU_PERI_MMC_CARD, 21, 0, 0),
	GATE(CLK_GOUT_CMU_TAA_BUS, "gout_cmu_taa_bus", "mout_cmu_taa_bus",
		CLK_CON_GAT_GATE_CLKCMU_TAA_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_TNR_BUS, "gout_cmu_tnr_bus", "mout_cmu_tnr_bus",
		CLK_CON_GAT_GATE_CLKCMU_TNR_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_USB_BUS, "gout_cmu_usb_bus", "mout_cmu_usb_bus",
		CLK_CON_GAT_GATE_CLKCMU_USB_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_USB_USB20DRD, "gout_cmu_usb_usb20drd", "mout_cmu_usb_usb20drd",
		CLK_CON_GAT_GATE_CLKCMU_USB_USB20DRD, 21, 0, 0),
};

static const struct samsung_cmu_info top_cmu_info __initconst = {
	.pll_clks		= top_pll_clks,
	.nr_pll_clks		= ARRAY_SIZE(top_pll_clks),
	.mux_clks		= top_mux_clks,
	.nr_mux_clks		= ARRAY_SIZE(top_mux_clks),
	.div_clks 		= top_div_clks,
	.nr_div_clks	= ARRAY_SIZE(top_div_clks),
	.fixed_factor_clks	= top_fixed_factor_clks,
	.nr_fixed_factor_clks	= ARRAY_SIZE(top_fixed_factor_clks),
	.gate_clks		= top_gate_clks,
	.nr_gate_clks	= ARRAY_SIZE(top_gate_clks),
	.nr_clk_ids		= CLKS_NR_TOP,
	.clk_regs		= top_clk_regs,
	.nr_clk_regs		= ARRAY_SIZE(top_clk_regs),
};

static void __init exynos1280_cmu_top_init(struct device_node *np)
{
	exynos_arm64_register_cmu(NULL, np, &top_cmu_info);
}

/* Register CMU_TOP early, as it's a dependency for other early domains */
CLK_OF_DECLARE(exynos1280_cmu_top, "samsung,exynos1280-cmu-top",
            exynos1280_cmu_top_init);

/* CMU_ALIVE */
#define PLL_CON0_MUX_CLKCMU_ALIVE_BUS_USER 0x600
#define PLL_CON1_MUX_CLKCMU_ALIVE_BUS_USER 0x604
#define PLL_CON0_MUX_CLKMUX_ALIVE_RCO_I3C_PMIC_USER 0x610
#define PLL_CON1_MUX_CLKMUX_ALIVE_RCO_I3C_PMIC_USER 0x614
#define PLL_CON0_MUX_CLK_ALIVE_TIMER 0x620
#define PLL_CON1_MUX_CLK_ALIVE_TIMER 0x624
#define PLL_CON0_MUX_CLK_RCO_ALIVE_USER 0x630
#define PLL_CON1_MUX_CLK_RCO_ALIVE_USER 0x634
#define CLK_CON_MUX_MUX_CLKCMU_AP2GNSS 0x1000
#define CLK_CON_MUX_MUX_CLKCMU_CHUBVTS_BUS 0x1004
#define CLK_CON_MUX_MUX_CLKCMU_CHUB_PERI 0x1008
#define CLK_CON_MUX_MUX_CLKCMU_CMGP_BUS 0x1010
#define CLK_CON_MUX_MUX_CLKCMU_CMGP_PERI 0x1014
#define CLK_CON_MUX_MUX_CLK_ALIVE_BUS 0x101c
#define CLK_CON_MUX_MUX_CLK_ALIVE_DBGCORE_UART 0x1020
#define CLK_CON_MUX_MUX_CLK_ALIVE_I2C 0x1024
#define CLK_CON_MUX_MUX_CLK_ALIVE_I3C_PMIC 0x1028
#define CLK_CON_MUX_MUX_CLK_ALIVE_USI0 0x102c
#define CLK_CON_DIV_CLKCMU_CHUBVTS_BUS 0x1800
#define CLK_CON_DIV_CLKCMU_CHUB_PERI 0x1804
#define CLK_CON_DIV_CLKCMU_CMGP_BUS 0x180c
#define CLK_CON_DIV_CLKCMU_CMGP_PERI 0x1810
#define CLK_CON_DIV_DIV_CLK_ALIVE_BUS 0x1818
#define CLK_CON_DIV_DIV_CLK_ALIVE_DBGCORE_UART 0x181c
#define CLK_CON_DIV_DIV_CLK_ALIVE_I2C 0x1820
#define CLK_CON_DIV_DIV_CLK_ALIVE_I3C_PMIC 0x1824
#define CLK_CON_DIV_DIV_CLK_ALIVE_USI0 0x1828
#define CLK_CON_GAT_AP2GNSS_CLK 0x2000
#define CLK_CON_GAT_CLKCMU_CHUBVTS_RCO 0x2004
#define CLK_CON_GAT_CLKCMU_CHUB_RCO 0x2008
#define CLK_CON_GAT_CLKCMU_CMGP_RCO 0x200c
#define CLK_CON_GAT_CLKCMU_VTS_RCO 0x2010
#define CLK_CON_GAT_CLK_BLK_ALIVE_UID_ALIVE_CMU_ALIVE_IPCLKPORT_PCLK 0x2014
#define CLK_CON_GAT_CLK_BLK_ALIVE_UID_DBGCORE_UART_IPCLKPORT_IPCLK 0x2018
#define CLK_CON_GAT_CLK_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_OSCCLK_RCO_IPCLKPORT_CLK 0x201c
#define CLK_CON_GAT_GATE_CLKCMU_CHUB_BUS 0x2020
#define CLK_CON_GAT_GATE_CLKCMU_CHUB_PERI 0x2024
#define CLK_CON_GAT_GATE_CLKCMU_CMGP_BUS 0x2028
#define CLK_CON_GAT_GATE_CLKCMU_CMGP_PERI 0x202c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_CHUB_RTC_IPCLKPORT_PCLK 0x2030
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_GPIO_ALIVE_IPCLKPORT_PCLK 0x2034
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_PMU_ALIVE_IPCLKPORT_PCLK 0x2038
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_RTC_IPCLKPORT_PCLK 0x203c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2APM_IPCLKPORT_PCLK 0x2040
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2AP_IPCLKPORT_PCLK 0x2044
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2PMU_IPCLKPORT_PCLK 0x2048
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_TOP_RTC_IPCLKPORT_PCLK 0x204c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_DBGCORE_UART_IPCLKPORT_PCLK 0x2050
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_D_TZPC_ALIVE_IPCLKPORT_PCLK 0x2054
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_GREBEINTEGRATION_IPCLKPORT_HCLK 0x2058
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_HW_SCANDUMP_CLKSTOP_CTRL_IPCLKPORT_ACLK 0x205c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I2C_ALIVE0_IPCLKPORT_IPCLK 0x2060
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I2C_ALIVE0_IPCLKPORT_PCLK 0x2064
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I3C_APM_PMIC_IPCLKPORT_I_PCLK 0x2080
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I3C_APM_PMIC_IPCLKPORT_I_SCLK 0x2084
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_INTMEM_IPCLKPORT_ACLK 0x208c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_INTMEM_IPCLKPORT_PCLK 0x2090
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_AP_IPCLKPORT_PCLK 0x2094
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_CHUB_IPCLKPORT_PCLK 0x2098
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_CP_IPCLKPORT_PCLK 0x209c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_GNSS_IPCLKPORT_PCLK 0x20a0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_VTS_IPCLKPORT_PCLK 0x20a4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_WLBT_IPCLKPORT_PCLK 0x20a8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CHUB_IPCLKPORT_PCLK 0x20ac
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CP_IPCLKPORT_PCLK 0x20b0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CP_S_IPCLKPORT_PCLK 0x20b4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_DBGCORE_IPCLKPORT_PCLK 0x20b8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_GNSS_IPCLKPORT_PCLK 0x20bc
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_WLBT_BT_IPCLKPORT_PCLK 0x20c0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_WLBT_WL_IPCLKPORT_PCLK 0x20c4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_CHUB_IPCLKPORT_PCLK 0x20c8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_GNSS_IPCLKPORT_PCLK 0x20cc
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_WLBT_BT_IPCLKPORT_PCLK 0x20d0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_WLBT_WL_IPCLKPORT_PCLK 0x20d4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_GNSS_CHUB_IPCLKPORT_PCLK 0x20d8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_GNSS_WLBT_IPCLKPORT_PCLK 0x20dc
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_SHARED_SRAM_IPCLKPORT_PCLK 0x20e0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_VTS_CHUB_IPCLKPORT_PCLK 0x20e4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_WLBT_ABOX_IPCLKPORT_PCLK 0x20e8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_WLBT_CHUB_IPCLKPORT_PCLK 0x20ec
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_PMU_INTR_GEN_IPCLKPORT_PCLK 0x20f0
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_ROM_CRC32_HOST_IPCLKPORT_ACLK 0x20f4
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_ROM_CRC32_HOST_IPCLKPORT_PCLK 0x20f8
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_BUS_IPCLKPORT_CLK 0x20fc
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_DBGCORE_UART_IPCLKPORT_CLK 0x2100
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_GREBE_DBGCORE_IPCLKPORT_CLK 0x2104
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_GREBE_IPCLKPORT_CLK 0x2108
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_I2C_IPCLKPORT_CLK 0x210c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_I3C_PMIC_IPCLKPORT_CLK 0x2110
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_USI0_IPCLKPORT_CLK 0x2114
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_CHUBVTS_IPCLKPORT_I_CLK 0x2124
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_GNSS_IPCLKPORT_I_CLK 0x2128
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_MODEM_IPCLKPORT_I_CLK 0x212c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_WLBT_IPCLKPORT_I_CLK 0x2130
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_P_APM_IPCLKPORT_I_CLK 0x2134
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_C_CMGP_IPCLKPORT_I_CLK 0x2138
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_D_APM_IPCLKPORT_I_CLK 0x213c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_G_DBGCORE_IPCLKPORT_I_CLK 0x2140
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_G_SCAN2DRAM_IPCLKPORT_I_CLK 0x2144
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_LP_CHUBVTS_IPCLKPORT_I_CLK 0x2148
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SS_DBGCORE_IPCLKPORT_SS_DBGCORE_IPCLKPORT_HCLK 0x214c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SWEEPER_P_ALIVE_IPCLKPORT_ACLK 0x2150
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SYSREG_ALIVE_IPCLKPORT_PCLK 0x2154
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_USI_ALIVE0_IPCLKPORT_IPCLK 0x2158
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_USI_ALIVE0_IPCLKPORT_PCLK 0x215c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_VGEN_LITE_ALIVE_IPCLKPORT_CLK 0x2178
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_WDT_ALIVE_IPCLKPORT_PCLK 0x217c
#define CLK_CON_GAT_GOUT_BLK_ALIVE_UID_XIU_DP_ALIVE_IPCLKPORT_ACLK 0x2180

static const unsigned long alive_clk_regs[] __initconst = {
        PLL_CON0_MUX_CLKCMU_ALIVE_BUS_USER,
        PLL_CON1_MUX_CLKCMU_ALIVE_BUS_USER,
        PLL_CON0_MUX_CLKMUX_ALIVE_RCO_I3C_PMIC_USER,
        PLL_CON1_MUX_CLKMUX_ALIVE_RCO_I3C_PMIC_USER,
        PLL_CON0_MUX_CLK_ALIVE_TIMER,
        PLL_CON1_MUX_CLK_ALIVE_TIMER,
        PLL_CON0_MUX_CLK_RCO_ALIVE_USER,
        PLL_CON1_MUX_CLK_RCO_ALIVE_USER,
        CLK_CON_MUX_MUX_CLKCMU_AP2GNSS,
        CLK_CON_MUX_MUX_CLKCMU_CHUBVTS_BUS,
        CLK_CON_MUX_MUX_CLKCMU_CHUB_PERI,
        CLK_CON_MUX_MUX_CLKCMU_CMGP_BUS,
        CLK_CON_MUX_MUX_CLKCMU_CMGP_PERI,
        CLK_CON_MUX_MUX_CLK_ALIVE_BUS,
        CLK_CON_MUX_MUX_CLK_ALIVE_DBGCORE_UART,
        CLK_CON_MUX_MUX_CLK_ALIVE_I2C,
        CLK_CON_MUX_MUX_CLK_ALIVE_I3C_PMIC,
        CLK_CON_MUX_MUX_CLK_ALIVE_USI0,
        CLK_CON_DIV_CLKCMU_CHUBVTS_BUS,
        CLK_CON_DIV_CLKCMU_CHUB_PERI,
        CLK_CON_DIV_CLKCMU_CMGP_BUS,
        CLK_CON_DIV_CLKCMU_CMGP_PERI,
        CLK_CON_DIV_DIV_CLK_ALIVE_BUS,
        CLK_CON_DIV_DIV_CLK_ALIVE_DBGCORE_UART,
        CLK_CON_DIV_DIV_CLK_ALIVE_I2C,
        CLK_CON_DIV_DIV_CLK_ALIVE_I3C_PMIC,
        CLK_CON_DIV_DIV_CLK_ALIVE_USI0,
        CLK_CON_GAT_AP2GNSS_CLK,
        CLK_CON_GAT_CLKCMU_CHUBVTS_RCO,
        CLK_CON_GAT_CLKCMU_CHUB_RCO,
        CLK_CON_GAT_CLKCMU_CMGP_RCO,
        CLK_CON_GAT_CLKCMU_VTS_RCO,
        CLK_CON_GAT_CLK_BLK_ALIVE_UID_ALIVE_CMU_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_CLK_BLK_ALIVE_UID_DBGCORE_UART_IPCLKPORT_IPCLK,
        CLK_CON_GAT_CLK_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_OSCCLK_RCO_IPCLKPORT_CLK,
        CLK_CON_GAT_GATE_CLKCMU_CHUB_BUS,
        CLK_CON_GAT_GATE_CLKCMU_CHUB_PERI,
        CLK_CON_GAT_GATE_CLKCMU_CMGP_BUS,
        CLK_CON_GAT_GATE_CLKCMU_CMGP_PERI,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_CHUB_RTC_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_GPIO_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_PMU_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_RTC_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2APM_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2AP_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_SYSREG_VGPIO2PMU_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_TOP_RTC_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_DBGCORE_UART_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_D_TZPC_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_GREBEINTEGRATION_IPCLKPORT_HCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_HW_SCANDUMP_CLKSTOP_CTRL_IPCLKPORT_ACLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I2C_ALIVE0_IPCLKPORT_IPCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I2C_ALIVE0_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I3C_APM_PMIC_IPCLKPORT_I_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_I3C_APM_PMIC_IPCLKPORT_I_SCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_INTMEM_IPCLKPORT_ACLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_INTMEM_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_AP_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_CP_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_GNSS_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_VTS_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_APM_WLBT_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CP_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_CP_S_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_DBGCORE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_GNSS_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_WLBT_BT_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_AP_WLBT_WL_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_GNSS_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_WLBT_BT_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_CP_WLBT_WL_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_GNSS_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_GNSS_WLBT_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_SHARED_SRAM_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_VTS_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_WLBT_ABOX_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_MAILBOX_WLBT_CHUB_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_PMU_INTR_GEN_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_ROM_CRC32_HOST_IPCLKPORT_ACLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_ROM_CRC32_HOST_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_BUS_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_DBGCORE_UART_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_GREBE_DBGCORE_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_GREBE_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_I2C_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_I3C_PMIC_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_USI0_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_CHUBVTS_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_GNSS_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_MODEM_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_C_WLBT_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_MI_P_APM_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_C_CMGP_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_D_APM_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_G_DBGCORE_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_G_SCAN2DRAM_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SLH_AXI_SI_LP_CHUBVTS_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SS_DBGCORE_IPCLKPORT_SS_DBGCORE_IPCLKPORT_HCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SWEEPER_P_ALIVE_IPCLKPORT_ACLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_SYSREG_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_USI_ALIVE0_IPCLKPORT_IPCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_USI_ALIVE0_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_VGEN_LITE_ALIVE_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_WDT_ALIVE_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_ALIVE_UID_XIU_DP_ALIVE_IPCLKPORT_ACLK,
};

PNAME(mout_alive_bus_user_p)		= { "oscclk_rco_alive", "mout_cmu_alive_bus" };
PNAME(mout_alive_rco_i3c_pmic_user_p)	= { "oscclk_rco_alive", "clk_rco_i3c_pmic" };
PNAME(mout_alive_timer_p)		= { "oscclk_rco_alive", "oscclk" };
PNAME(mout_rco_alive_user_p)		= { "oscclk_rco_alive", "clk_rco_alive" };
PNAME(mout_cmu_ap2gnss_p)		= { "mout_alive_bus_user", "rco_400" };
PNAME(mout_cmu_chubvts_bus_p)		= { "mout_rco_alive_user", "rco_400",
					"mout_alive_bus_user", "oscclk_rco_alive" };
PNAME(mout_cmu_chub_peri_p)		= { "mout_rco_alive_user", "rco_400",
					"mout_alive_bus_user", "oscclk_rco_alive" };
PNAME(mout_cmu_cmgp_bus_p)		= { "mout_rco_alive_user", "rco_400",
					"mout_alive_bus_user", "oscclk_rco_alive" };
PNAME(mout_cmu_cmgp_peri_p)		= { "mout_rco_alive_user", "rco_400",
					"mout_alive_bus_user", "oscclk_rco_alive" };
PNAME(mout_alive_bus_p)			= { "mout_rco_alive_user", "rco_400",
					"mout_alive_bus_user", "oscclk_rco_alive" };
PNAME(mout_alive_dbgcore_p)		= { "oscclk_rco_alive", "mout_alive_bus" };
PNAME(mout_alive_i2c_p)			= { "oscclk_rco_alive", "mout_alive_bus" };
PNAME(mout_alive_i3c_pmic_p)		= { "dout_alive_i3c_pmic", "mout_alive_rco_i3c_pmic_user" };
PNAME(mout_alive_usi0_p)		= { "oscclk_rco_alive", "mout_alive_bus" };

static const struct samsung_fixed_rate_clock alive_fixed_clks[] __initconst = {
	FRATE(OSCCLK_RCO_ALIVE, "oscclk_rco_alive", NULL, 0, 24576000),
	FRATE(CLK_RCO_ALIVE, "clk_rco_alive", NULL, 0, 60000000),
	FRATE(CLK_RCO_I3C_PMIC, "clk_rco_i3c_pmic", NULL, 0, 100000000),
	FRATE(RCO_400, "rco_400", NULL, 0, 393216000),
};

static const struct samsung_mux_clock alive_mux_clks[] __initconst = {
	MUX(CLK_MOUT_ALIVE_BUS_USER, "mout_alive_bus_user", mout_alive_bus_user_p,
		PLL_CON0_MUX_CLKCMU_ALIVE_BUS_USER, 4, 1),
	MUX(CLK_MOUT_ALIVE_RCO_I3C_PMIC_USER, "mout_alive_rco_i3c_pmic_user",
		mout_alive_rco_i3c_pmic_user_p, PLL_CON0_MUX_CLKMUX_ALIVE_RCO_I3C_PMIC_USER,
		4, 1),
	MUX(CLK_MOUT_ALIVE_TIMER, "mout_alive_timer", mout_alive_timer_p,
		PLL_CON0_MUX_CLK_ALIVE_TIMER, 4, 1),
	MUX(CLK_MOUT_RCO_ALIVE_USER, "mout_rco_alive_user", mout_rco_alive_user_p,
		PLL_CON0_MUX_CLK_RCO_ALIVE_USER, 4, 1),
	MUX(CLK_MOUT_CMU_AP2GNSS, "mout_cmu_ap2gnss", mout_cmu_ap2gnss_p,
		CLK_CON_MUX_MUX_CLKCMU_AP2GNSS, 0, 1),
	MUX(CLK_MOUT_CMU_CHUBVTS_BUS, "mout_cmu_chubvts_bus", mout_cmu_chubvts_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_CHUBVTS_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_CHUB_PERI, "mout_cmu_chub_peri", mout_cmu_chub_peri_p,
		CLK_CON_MUX_MUX_CLKCMU_CHUB_PERI, 0, 2),
	MUX(CLK_MOUT_CMU_CMGP_BUS, "mout_cmu_cmgp_bus", mout_cmu_cmgp_bus_p,
		CLK_CON_MUX_MUX_CLKCMU_CMGP_BUS, 0, 2),
	MUX(CLK_MOUT_CMU_CMGP_PERI, "mout_cmu_cmgp_peri", mout_cmu_cmgp_peri_p,
		CLK_CON_MUX_MUX_CLKCMU_CMGP_PERI, 0, 2),
	MUX(CLK_MOUT_ALIVE_BUS, "mout_alive_bus", mout_alive_bus_p,
		CLK_CON_MUX_MUX_CLK_ALIVE_BUS, 0, 2),
	MUX(CLK_MOUT_ALIVE_DBGCORE_UART, "mout_alive_dbgcore_uart", mout_alive_dbgcore_p,
		CLK_CON_MUX_MUX_CLK_ALIVE_DBGCORE_UART, 0, 1),
	MUX(CLK_MOUT_ALIVE_I2C, "mout_alive_i2c", mout_alive_i2c_p,
		CLK_CON_MUX_MUX_CLK_ALIVE_I2C, 0, 1),
	MUX(CLK_MOUT_ALIVE_I3C_PMIC, "mout_alive_i3c_pmic", mout_alive_i3c_pmic_p,
		CLK_CON_MUX_MUX_CLK_ALIVE_I3C_PMIC, 0, 1),
	MUX(CLK_MOUT_ALIVE_USI0, "mout_alive_usi0", mout_alive_usi0_p,
		CLK_CON_MUX_MUX_CLK_ALIVE_USI0, 0, 1),
};

static const struct samsung_div_clock alive_div_clks[] __initconst = {
	DIV(CLK_DOUT_CMU_CHUBVTS_BUS, "dout_cmu_chubvts_bus", "gout_cmu_chub_bus",
		CLK_CON_DIV_CLKCMU_CHUBVTS_BUS, 0, 4),
	DIV(CLK_DOUT_CMU_CHUB_PERI, "dout_cmu_chub_peri", "gout_cmu_chub_peri",
		CLK_CON_DIV_CLKCMU_CHUB_PERI, 0, 4),
	DIV(CLK_DOUT_CMU_CMGP_BUS, "dout_cmu_cmgp_bus", "gout_cmu_cmgp_bus",
		CLK_CON_DIV_CLKCMU_CMGP_BUS, 0, 3),
	DIV(CLK_DOUT_CMU_CMGP_PERI, "dout_cmu_cmgp_peri", "gout_cmu_cmgp_peri",
		CLK_CON_DIV_CLKCMU_CMGP_PERI, 0, 3),
	DIV(CLK_DOUT_ALIVE_BUS, "dout_alive_bus", "mout_alive_bus",
		CLK_CON_DIV_DIV_CLK_ALIVE_BUS, 0, 3),
	DIV(CLK_DOUT_ALIVE_DBGCORE_UART, "dout_alive_dbgcore_uart", "mout_alive_dbgcore_uart",
		CLK_CON_DIV_DIV_CLK_ALIVE_DBGCORE_UART, 0, 4),
	DIV(CLK_DOUT_ALIVE_I2C, "dout_alive_i2c", "mout_alive_i2c",
		CLK_CON_DIV_DIV_CLK_ALIVE_I2C, 0, 4),
	DIV(CLK_DOUT_ALIVE_I3C_PMIC, "dout_alive_i3c_pmic", "mout_alive_bus",
		CLK_CON_DIV_DIV_CLK_ALIVE_I3C_PMIC, 0, 3),
	DIV(CLK_DOUT_ALIVE_USI0, "dout_alive_usi0", "mout_alive_usi0",
		CLK_CON_DIV_DIV_CLK_ALIVE_USI0, 0, 4),
};

static const struct samsung_gate_clock alive_gate_clks[] __initconst = {
	GATE(CLK_GOUT_AP2GNSS_CLK, "gout_ap2gnss_clk", "mout_cmu_ap2gnss",
		CLK_CON_GAT_AP2GNSS_CLK, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CHUBVTS, "gout_cmu_chubvts", "mout_rco_alive_user",
		CLK_CON_GAT_CLKCMU_CHUBVTS_RCO, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CHUB, "gout_cmu_chub", "mout_rco_alive_user",
		CLK_CON_GAT_CLKCMU_CHUB_RCO, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CMGP, "gout_cmu_cmgp", "mout_rco_alive_user",
		CLK_CON_GAT_CLKCMU_CMGP_RCO, 21, 0, 0),
	GATE(CLK_GOUT_CMU_VTS, "gout_cmu_vts", "mout_rco_alive_user",
		CLK_CON_GAT_CLKCMU_VTS_RCO, 21, 0, 0),
	GATE(CLK_GOUT_ALIVE_ALIVE_CMU_ALIVE_PCLK, "gout_alive_alive_cmu_alive_pclk", "dout_alive_bus",
		CLK_CON_GAT_CLK_BLK_ALIVE_UID_ALIVE_CMU_ALIVE_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_ALIVE_DBGCORE_UART_IPCLK, "gout_alive_dbgcore_uart_ipclk", "dout_alive_dbgcore_uart",
		CLK_CON_GAT_CLK_BLK_ALIVE_UID_DBGCORE_UART_IPCLKPORT_IPCLK, 21, 0, 0),
	GATE(CLK_GOUT_ALIVE_CLK_ALIVE_OSCCLK_CLK, "gout_alive_clk_alive_oscclk_clk", "oscclk_rco_alive",
		CLK_CON_GAT_CLK_BLK_ALIVE_UID_RSTNSYNC_CLK_ALIVE_OSCCLK_RCO_IPCLKPORT_CLK, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CHUB_BUS, "gout_cmu_chub_bus", "mout_cmu_chubvts_bus",
		CLK_CON_GAT_GATE_CLKCMU_CHUB_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CHUB_PERI, "gout_cmu_chub_peri", "mout_cmu_chub_peri",
		CLK_CON_GAT_GATE_CLKCMU_CHUB_PERI, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CMGP_BUS, "gout_cmu_cmgp_bus", "mout_cmu_cmgp_bus",
		CLK_CON_GAT_GATE_CLKCMU_CMGP_BUS, 21, 0, 0),
	GATE(CLK_GOUT_CMU_CMGP_PERI, "gout_cmu_cmgp_peri", "mout_cmu_cmgp_peri",
		CLK_CON_GAT_GATE_CLKCMU_CMGP_PERI, 21, 0, 0),
	GATE(CLK_GOUT_ALIVE_CHUB_RTC_PCLK, "gout_alive_chub_rtc_pclk", "dout_alive_bus",
		CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_CHUB_RTC_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_ALIVE_GPIO_ALIVE_PCLK, "gout_alive_gpio_alive_pclk", "dout_alive_bus",
		CLK_CON_GAT_GOUT_BLK_ALIVE_UID_APBIF_GPIO_ALIVE_IPCLKPORT_PCLK,
		21, 0, 0),
};

static const struct samsung_cmu_info alive_cmu_info __initconst = {
	.mux_clks = alive_mux_clks,
	.nr_mux_clks = ARRAY_SIZE(alive_mux_clks),
	.fixed_clks = alive_fixed_clks,
	.nr_fixed_clks = ARRAY_SIZE(alive_fixed_clks),
	.div_clks 	= alive_div_clks,
	.nr_div_clks = ARRAY_SIZE(alive_div_clks),
	.gate_clks	= alive_gate_clks,
	.nr_gate_clks	= ARRAY_SIZE(alive_gate_clks),
	.clk_regs = alive_clk_regs,
	.nr_clk_regs = ARRAY_SIZE(alive_clk_regs),
	.nr_clk_ids = CLKS_NR_ALIVE,

	.clk_name = "bus",
};

/* CMU_HSI */
#define PLL_CON0_MUX_CLKCMU_HSI_BUS_USER 0x600
#define PLL_CON1_MUX_CLKCMU_HSI_BUS_USER 0x604
#define PLL_CON0_MUX_CLKCMU_HSI_UFS_EMBD_USER 0x630
#define PLL_CON1_MUX_CLKCMU_HSI_UFS_EMBD_USER 0x634
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_D_TZPC_HSI_IPCLKPORT_PCLK 0x2000
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_IPCLKPORT_PCLK 0x2004
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_UFS_IPCLKPORT_PCLK 0x2008
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_HSI_CMU_HSI_IPCLKPORT_PCLK 0x200c
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_ACLK 0x2010
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_PCLK 0x2014
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_BUS_IPCLKPORT_CLK 0x2018
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_OSCCLK_IPCLKPORT_CLK 0x201c
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_SR_CLK_HSI_BUS_IPCLKPORT_CLK 0x2020
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_S2MPU_D_HSI_IPCLKPORT_CLK_S2 0x2024
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_MI_P_HSI_IPCLKPORT_I_CLK 0x2028
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_SI_D_HSI_IPCLKPORT_I_CLK 0x202c
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_SYSREG_HSI_IPCLKPORT_PCLK 0x2030
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_ACLK 0x2034
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO 0x2038
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK 0x203c
#define CLK_CON_GAT_GOUT_BLK_HSI_UID_VGEN_LITE_HSI_IPCLKPORT_CLK 0x2040

static const unsigned long hsi_clk_regs[] __initconst = {
        PLL_CON0_MUX_CLKCMU_HSI_BUS_USER,
        PLL_CON1_MUX_CLKCMU_HSI_BUS_USER,
        PLL_CON0_MUX_CLKCMU_HSI_UFS_EMBD_USER,
        PLL_CON1_MUX_CLKCMU_HSI_UFS_EMBD_USER,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_D_TZPC_HSI_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_UFS_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_HSI_CMU_HSI_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_ACLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_BUS_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_OSCCLK_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_SR_CLK_HSI_BUS_IPCLKPORT_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_S2MPU_D_HSI_IPCLKPORT_CLK_S2,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_MI_P_HSI_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_SI_D_HSI_IPCLKPORT_I_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_SYSREG_HSI_IPCLKPORT_PCLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_ACLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK,
        CLK_CON_GAT_GOUT_BLK_HSI_UID_VGEN_LITE_HSI_IPCLKPORT_CLK,
};

PNAME(mout_cmu_hsi_bus_user_p)		= { "oscclk", "dout_cmu_hsi_bus" };
PNAME(mout_cmu_hsi_ufs_embd_user_p)	= { "oscclk", "dout_cmu_hsi_ufs_embd" };

static const struct samsung_mux_clock hsi_mux_clks[] __initconst = {
	MUX(CLK_MOUT_CMU_HSI_BUS_USER, "mout_cmu_hsi_bus_user", mout_cmu_hsi_bus_user_p,
		PLL_CON0_MUX_CLKCMU_HSI_BUS_USER, 4, 1),
	MUX(CLK_MOUT_CMU_HSI_UFS_EMBD_USER, "mout_cmu_hsi_ufs_embd_user",
		mout_cmu_hsi_ufs_embd_user_p, PLL_CON0_MUX_CLKCMU_HSI_UFS_EMBD_USER,
		4, 1),
};

static const struct samsung_gate_clock hsi_gate_clks[] __initconst = {
	GATE(CLK_GOUT_HSI_D_TZPC_HSI_PCLK, "gout_hsi_d_tzpc_hsi_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_D_TZPC_HSI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_GPIO_HSI_PCLK, "gout_hsi_gpio_hsi_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_GPIO_HSI_UFS_PCLK, "gout_hsi_gpio_hsi_ufs_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_GPIO_HSI_UFS_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_HSI_CMU_HSI_PCLK, "gout_hsi_hsi_cmu_hsi_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_HSI_CMU_HSI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_PPMU_HSI_ACLK, "gout_hsi_ppmu_hsi_aclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_PPMU_HSI_PCLK, "gout_hsi_ppmu_hsi_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_PPMU_HSI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_CLK_HSI_BUS_CLK, "gout_hsi_clk_hsi_bus_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_BUS_IPCLKPORT_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_CLK_HSI_OSCCLK_CLK, "gout_hsi_clk_hsi_oscclk_clk", "oscclk",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_CLK_HSI_OSCCLK_IPCLKPORT_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_SR_CLK_HSI_BUS_CLK, "gout_hsi_sr_clk_hsi_bus_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_RSTNSYNC_SR_CLK_HSI_BUS_IPCLKPORT_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_S2MPU_D_HSI_CLK_S2, "gout_hsi_s2mpu_d_hsi_clk_s2", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_S2MPU_D_HSI_IPCLKPORT_CLK_S2, 21, 0, 0),
	GATE(CLK_GOUT_HSI_SLH_AXI_MI_P_HSI_CLK, "gout_hsi_slh_axi_mi_p_hsi_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_MI_P_HSI_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_SLH_AXI_SI_D_HSI_CLK, "gout_hsi_slh_axi_si_d_hsi_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_SLH_AXI_SI_D_HSI_IPCLKPORT_I_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_SYSREG_HSI_PCLK, "gout_hsi_sysreg_hsi_pclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_SYSREG_HSI_IPCLKPORT_PCLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_UFS_EMBD_ACLK, "gout_hsi_ufs_embd_aclk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_ACLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_UFS_EMBD_CLK_UNIPRO, "gout_hsi_ufs_embd_clk_unipro", "mout_cmu_hsi_ufs_embd_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_CLK_UNIPRO, 21, 0, 0),
	GATE(CLK_GOUT_HSI_UFS_EMBD_FMP_CLK, "gout_hsi_ufs_embd_fmp_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_UFS_EMBD_IPCLKPORT_I_FMP_CLK, 21, 0, 0),
	GATE(CLK_GOUT_HSI_VGEN_LITE_HSI_CLK, "gout_hsi_vgen_lite_hsi_clk", "mout_cmu_hsi_bus_user",
		CLK_CON_GAT_GOUT_BLK_HSI_UID_VGEN_LITE_HSI_IPCLKPORT_CLK, 21, 0, 0),
};

static const struct samsung_cmu_info hsi_cmu_info __initconst = {
	.mux_clks	= hsi_mux_clks,
	.nr_mux_clks 	= ARRAY_SIZE(hsi_mux_clks),
	.gate_clks	= hsi_gate_clks,
	.nr_gate_clks	= ARRAY_SIZE(hsi_gate_clks),
	.nr_clk_ids	= CLKS_NR_HSI,
	.clk_regs	= hsi_clk_regs,
	.nr_clk_regs	= ARRAY_SIZE(hsi_clk_regs),
	.clk_name	= "bus",
};

static int __init exynos1280_cmu_probe(struct platform_device *pdev)
{
	const struct samsung_cmu_info *info;
	struct device *dev = &pdev->dev;

	info = of_device_get_match_data(dev);

	exynos_arm64_register_cmu(dev, dev->of_node, info);

	return 0;
}

static const struct of_device_id exynos1280_cmu_of_match[] = {
	{
		.compatible = "samsung,exynos1280-cmu-alive",
		.data = &alive_cmu_info,
	},
	{
		.compatible = "samsung,exynos1280-cmu-hsi",
		.data = &hsi_cmu_info,
	},
};

static struct platform_driver exynos1280_cmu_driver __refdata = {
	.driver	= {
		.name = "exynos1280-cmu",
		.of_match_table = exynos1280_cmu_of_match,
		.suppress_bind_attrs = true,
	},
	.probe = exynos1280_cmu_probe,
};

static int __init exynos1280_cmu_init(void)
{
	return platform_driver_register(&exynos1280_cmu_driver);
}
core_initcall(exynos1280_cmu_init);