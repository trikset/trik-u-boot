/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Based on davinci_dvevm.h. Original Copyrights follow:
 *
 * Copyright (C) 2007 Sergey Kubushyn <ksi@koi8.net>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * Board
 */

/*
 * SoC Configuration
 */
#define CONFIG_SYS_EXCEPTION_VECTORS_HIGH
#define CONFIG_SYS_CLK_FREQ		clk_get(DAVINCI_ARM_CLKID)
#define CONFIG_SYS_OSCIN_FREQ		24000000
#define CONFIG_SYS_TIMERBASE		DAVINCI_TIMER0_BASE
#define CONFIG_SYS_HZ_CLOCK		clk_get(DAVINCI_AUXCLK_CLKID)
#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY

#ifdef CONFIG_MTD_NOR_FLASH
#define CONFIG_SYS_DV_NOR_BOOT_CFG	(0x11)
#endif

/*
 * Memory Info
 */
#define CONFIG_SYS_MALLOC_LEN	(0x10000 + 1*1024*1024) /* malloc() len */
#define PHYS_SDRAM_1		DAVINCI_DDR_EMIF_DATA_BASE /* DDR Start */
#define PHYS_SDRAM_1_SIZE	(256 << 20) /* SDRAM size 256MB */
#define CONFIG_MAX_RAM_BANK_SIZE (512 << 20) /* max size from SPRS586*/
#define CONFIG_SPL_BSS_START_ADDR DAVINCI_DDR_EMIF_DATA_BASE
#define CONFIG_SPL_BSS_MAX_SIZE 0x1080000
/* memtest start addr */

/* memtest will be run on 16MB */

#define CONFIG_SYS_DA850_SYSCFG_SUSPSRC (	\
	DAVINCI_SYSCFG_SUSPSRC_TIMER0 |		\
	DAVINCI_SYSCFG_SUSPSRC_SPI0 |		\
	DAVINCI_SYSCFG_SUSPSRC_UART1 |		\
	DAVINCI_SYSCFG_SUSPSRC_I2C)

/*
 * PLL configuration
 */

#define CONFIG_SYS_DA850_PLL0_PLLM     24
#define CONFIG_SYS_DA850_PLL1_PLLM     21

/*
 * DDR2 memory configuration
 */
#define CONFIG_SYS_DA850_DDR2_DDRPHYCR (DV_DDR_PHY_PWRDNEN | \
					DV_DDR_PHY_EXT_STRBEN | \
					(0x4 << DV_DDR_PHY_RD_LATENCY_SHIFT))

#define CONFIG_SYS_DA850_DDR2_SDBCR (		\
	(1 << DV_DDR_SDCR_DDRDRIVE0_SHIFT) |    \
	(0 << DV_DDR_SDCR_MSDRAMEN_SHIFT) |	\
	(1 << DV_DDR_SDCR_DDR2EN_SHIFT)    |    \
	(1 << DV_DDR_SDCR_DDREN_SHIFT) |	\
	(1 << DV_DDR_SDCR_SDRAMEN_SHIFT) |	\
	(1 << DV_DDR_SDCR_BUS_WIDTH_SHIFT) |	\
	(0x3 << DV_DDR_SDCR_CL_SHIFT) |		\
	(0x3 << DV_DDR_SDCR_IBANK_SHIFT) |	\
	(0x2 << DV_DDR_SDCR_PAGESIZE_SHIFT))

/* SDBCR2 is only used if IBANK_POS bit in SDBCR is set */
#define CONFIG_SYS_DA850_DDR2_SDBCR2 0

#define DDR_CLK_MHZ             132     /* DDR_CLK_MHZ = PLL1_SYSCLK1 / 2 */
#define DDR_TIMING(ns)          (((unsigned)((ns)*DDR_CLK_MHZ + 999) / 1000) - 1)

/* 8192 refresh cycle in 64ms, so refresh_rate=64ms/8192=7812.5ns */
#define DDR_RASMAX(ns)          ((unsigned)(((ns)*10+78124) / 78125) - 1)

/* 8192 refresh cycle in 64ms */
#define DDR_REFRESH_RATE        ((DDR_CLK_MHZ*1000*64)/8192)


/* Micron mt47h128m16rt-25eit */
#define DDR_T_RFC       DDR_TIMING(195)
#define DDR_T_RP        DDR_TIMING(12.5)
#define DDR_T_RCD       DDR_TIMING(12.5)
#define DDR_T_WR        DDR_TIMING(15)
#define DDR_T_RAS       DDR_TIMING(45)
#define DDR_T_RC        DDR_TIMING(57.5)
#define DDR_T_RRD       DDR_TIMING(10)          /* we have x16 case */
#define DDR_T_WTR       (DDR_TIMING(7.5)+1)

#define DDR_T_RASMAX    DDR_RASMAX(70000)
#define DDR_T_XP        (3-1)           /* max(t_xp,t_cke)/t_ck - 1 */
#define DDR_T_ODT       0               /* not connected */
#define DDR_T_XSNR      DDR_TIMING(230) /* lower limit is t_RFC + 10  */
#define DDR_T_XSRD      (200-1)         /* t_XSRD/t_ck - 1 */
#define DDR_T_RTP       (DDR_TIMING(7.5)+1)
#define DDR_T_CKE       (3-1)           /* t_cke/t_ck-1 */

#define CONFIG_SYS_DA850_DDR2_SDTIMR (            \
        (DDR_T_RFC << DV_DDR_SDTMR1_RFC_SHIFT)  | \
        (DDR_T_RP  << DV_DDR_SDTMR1_RP_SHIFT)   | \
        (DDR_T_RCD << DV_DDR_SDTMR1_RCD_SHIFT)  | \
        (DDR_T_WR  << DV_DDR_SDTMR1_WR_SHIFT)   | \
        (DDR_T_RAS << DV_DDR_SDTMR1_RAS_SHIFT)  | \
        (DDR_T_RC  << DV_DDR_SDTMR1_RC_SHIFT)   | \
        (DDR_T_RRD << DV_DDR_SDTMR1_RRD_SHIFT)  | \
        (DDR_T_WTR << DV_DDR_SDTMR1_WTR_SHIFT))

#define CONFIG_SYS_DA850_DDR2_SDTIMR2 (                \
        (DDR_T_RASMAX << DV_DDR_SDTMR2_RASMAX_SHIFT) | \
        (DDR_T_XP     << DV_DDR_SDTMR2_XP_SHIFT)     | \
        (DDR_T_ODT    << DV_DDR_SDTMR2_ODT_SHIFT)    | \
        (DDR_T_XSNR   << DV_DDR_SDTMR2_XSNR_SHIFT)   | \
        (DDR_T_XSRD   << DV_DDR_SDTMR2_XSRD_SHIFT)   | \
        (DDR_T_RTP    << DV_DDR_SDTMR2_RTP_SHIFT)    | \
        (DDR_T_CKE    << DV_DDR_SDTMR2_CKE_SHIFT))

#define CONFIG_SYS_DA850_DDR2_SDRCR    DDR_REFRESH_RATE
#define CONFIG_SYS_DA850_DDR2_PBBPR    0x30

/*
 * Serial Driver info
 */
#define CONFIG_SYS_NS16550_CLK	clk_get(DAVINCI_UART2_CLKID)

#define CONFIG_SYS_SPI_CLK		clk_get(DAVINCI_SPI0_CLKID)

/*
 * I2C Configuration
 */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SYS_I2C_EXPANDER_ADDR   0x20
#endif


/*
 * U-Boot general configuration
 */
#define CONFIG_BOOTFILE		"uImage" /* Boot file name */
#define CONFIG_SYS_CBSIZE	1024 /* Console I/O Buffer Size	*/
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE /* Boot Args Buffer Size */
#define CONFIG_SYS_LOAD_ADDR	(PHYS_SDRAM_1 + 0x700000)

/*
 * Linux Information
 */
#define LINUX_BOOT_PARAM_ADDR	(PHYS_SDRAM_1 + 0x100)
#define CONFIG_HWCONFIG		/* enable hwconfig */
#define CONFIG_CMDLINE_TAG
#define CONFIG_SETUP_MEMORY_TAGS

#define CONFIG_BOOTCOMMAND \
		"run mmcboot; "

#define DEFAULT_LINUX_BOOT_ENV \
	"loadaddr=0xc0700000\0" \
	"fdtaddr=0xc0600000\0" \
	"scriptaddr=0xc0600000\0"

#include <environment/ti/mmc.h>

#define CONFIG_EXTRA_ENV_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	DEFAULT_MMC_TI_ARGS \
	"boot=mmc\0" \
	"bootpart=0:1\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"fdtfile=trikboard.dtb\0" \
	"boot_fdt=try\0" \
	"boot_fit=0\0" \
	"console=ttyS1,115200n8\0" \
	"hwconfig=dsp:wake=yes\0" \
	"rootdev=/dev/mmcblk0p1\0" \
	"optargs=mem=128M@0xC0000000 mem=64M@0xCC000000 noinitrd vt.global_cursor_default=0 consoleblank=0\0" \
	"trik_uboot_version=20180422"

#ifdef CONFIG_CMD_BDI
#define CONFIG_CLOCKS
#endif

#ifndef CONFIG_MTD_NOR_FLASH
#define CONFIG_SPL_PAD_TO	32768
#endif

#ifdef CONFIG_SPL_BUILD
/* defines for SPL */
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SYS_TEXT_BASE - \
						CONFIG_SYS_MALLOC_LEN)
#define CONFIG_SYS_SPL_MALLOC_SIZE	CONFIG_SYS_MALLOC_LEN
#define CONFIG_SPL_STACK	0x8001ff00
#define CONFIG_SPL_MAX_FOOTPRINT	32768

#endif

/* Load U-Boot Image From MMC */

/* additions for new relocation code, must added to all boards */
#define CONFIG_SYS_SDRAM_BASE		0xc0000000

#ifdef CONFIG_MTD_NOR_FLASH
#define CONFIG_SYS_INIT_SP_ADDR		0x8001ff00
#else
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x1000 - /* Fix this */ \
					GENERATED_GBL_DATA_SIZE)
#endif /* CONFIG_MTD_NOR_FLASH */

#define CONFIG_SYS_MONITOR_LEN		(512 << 10)

#include <asm/arch/hardware.h>

#endif /* __CONFIG_H */
