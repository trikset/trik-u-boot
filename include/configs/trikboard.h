/*
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Based on davinci_dvevm.h. Original Copyrights follow:
 *
 * Copyright (C) 2007 Sergey Kubushyn <ksi@koi8.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/* #define DEBUG */

/*
 * Board
 */
#define CONFIG_USE_SPIFLASH


/*
 * SoC Configuration
 */
#define CONFIG_MACH_DAVINCI_TRIK
#define CONFIG_ARM926EJS		/* arm926ejs CPU core */
#define CONFIG_SOC_DA8XX		/* TI DA8xx SoC */
#define CONFIG_SOC_DA850		/* TI DA850 SoC */
#define CONFIG_SYS_EXCEPTION_VECTORS_HIGH
#define CONFIG_SYS_CLK_FREQ		clk_get(DAVINCI_ARM_CLKID)
#define CONFIG_SYS_OSCIN_FREQ		24000000
#define CONFIG_SYS_TIMERBASE		DAVINCI_TIMER0_BASE
#define CONFIG_SYS_HZ_CLOCK		clk_get(DAVINCI_AUXCLK_CLKID)
#define CONFIG_SYS_HZ			1000
#define CONFIG_SYS_TEXT_BASE		0xc1080000
#define CONFIG_SYS_DA850_PLL_INIT
#define CONFIG_SYS_DA850_DDR_INIT

/*
 * Memory Info
 */
#define CONFIG_SYS_MALLOC_LEN	(0x10000 + 1*1024*1024) /* malloc() len */
#define PHYS_SDRAM_1		DAVINCI_DDR_EMIF_DATA_BASE /* DDR Start */
#define PHYS_SDRAM_1_SIZE	(64 << 20) /* SDRAM size 64MB */
#define CONFIG_MAX_RAM_BANK_SIZE (512 << 20) /* max size from SPRS586*/

/* memtest start addr */
#define CONFIG_SYS_MEMTEST_START	(PHYS_SDRAM_1 + 0x2000000)

/* memtest will be run on 16MB */
#define CONFIG_SYS_MEMTEST_END 	(PHYS_SDRAM_1 + 0x2000000 + 16*1024*1024)

#define CONFIG_NR_DRAM_BANKS	1 /* we have 1 bank of DRAM */
#define CONFIG_STACKSIZE	(256*1024) /* regular stack */

#define CONFIG_SYS_DA850_SYSCFG_SUSPSRC (	\
	DAVINCI_SYSCFG_SUSPSRC_TIMER0 |		\
	DAVINCI_SYSCFG_SUSPSRC_SPI0 |		\
	DAVINCI_SYSCFG_SUSPSRC_UART1 |		\
	DAVINCI_SYSCFG_SUSPSRC_I2C)


/*
 * PLL configuration (CLOCK = 24.0 MHz)
 */
#define CONFIG_SYS_DA850_PLL0_PLLM     24	/* PLL0=600MHz, PLL0=CLOCK*(PLL0_PLLM+1) */
#define CONFIG_SYS_DA850_PLL1_PLLM     21	/* PLL1=528Mhz, PLL1=CLOCK*(PLL1_PLLM+1) */

#define CONFIG_SYS_DV_CLKMODE          0
#define CONFIG_SYS_DA850_PLL0_POSTDIV  1	/* PLL0_POST=300Mhz,    PLL0_POST=PLL0/(PLL0_POSTDIV+1) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV1  0x8000	/* PLL0_SYSCLK1=300Mhz, PLL0_SYSCLK1=PLL0_POST/(PLL0_PLLDIV1+1-0x8000) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV2  0x8001	/* PLL0_SYSCLK2=150Mhz, PLL0_SYSCLK2=PLL0_POST/(PLL0_PLLDIV2+1-0x8000) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV3  0x8002	/* PLL0_SYSCLK3=100Mhz, PLL0_SYSCLK3=PLL0_POST/(PLL0_PLLDIV3+1-0x8000) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV4  0x8003	/* PLL0_SYSCLK4=75Mhz,  PLL0_SYSCLK4=PLL0_POST/(PLL0_PLLDIV4+1-0x8000) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV5  0x8002	/* PLL0_SYSCLK5=100Mhz, PLL0_SYSCLK5=PLL0_POST/(PLL0_PLLDIV5+1-0x8000) */
#define CONFIG_SYS_DA850_PLL0_PLLDIV6  CONFIG_SYS_DA850_PLL0_PLLDIV1
#define CONFIG_SYS_DA850_PLL0_PLLDIV7  0x8005	/* PLL0_SYSCLK7=50Mhz,  PLL0_SYSCLK7=PLL0_POST/(PLL0_PLLDIV7+1-0x8000) */

#define CONFIG_SYS_DA850_PLL1_POSTDIV  1	/* PLL1_POST=264Mhz,    PLL1_POST=PLL1/(PLL1_POSTDIV+1) */
#define CONFIG_SYS_DA850_PLL1_PLLDIV1  0x8000	/* PLL1_SYSCLK1=264Mhz, PLL1_SYSCLK1=PLL1_POST/(PLL1_PLLDIV1+1-0x8000) */
#define CONFIG_SYS_DA850_PLL1_PLLDIV2  0x8001	/* PLL1_SYSCLK2=132Mhz, PLL1_SYSCLK2=PLL1_POST/(PLL1_PLLDIV2+1-0x8000) */
#define CONFIG_SYS_DA850_PLL1_PLLDIV3  0x8002	/* PLL1_SYSCLK3=88Mhz,  PLL1_SYSCLK3=PLL1_POST/(PLL1_PLLDIV3+1-0x8000) */

/*
 * DDR2 memory configuration
 */

/*
 * DDR:  SDRAMEN = 1, MSDRAMEN = 0, DDREN = 1, DDR2EN = 1
 * mDDR: SDRAMEN = 1, MSDRAMEN = 1, DDREN = 1, DDR2EN = 0
 *
 * Micron mt47h32m16hr-25e
 * --------------------------------------------------------
 *               specs           SDBCR value
 * --------------------------------------------------------
 * type          DDR             SDRAMEN = 1, MSDRAMEN = 0,
 *                               DDREN = 1, DDR2EN = 1
 * IBANKS        4               0x02
 * PAGESIZE      8192 (13 bits)  0x02
 * CAS_LATENCY   3..6            0x03..0x06
 * --------------------------------------------------------
 */
#define CONFIG_SYS_DA850_DDR2_SDBCR (		\
	(1 << DV_DDR_SDCR_DDRDRIVE0_SHIFT) |	\
	(0 << DV_DDR_SDCR_MSDRAMEN_SHIFT)  |	\
	(1 << DV_DDR_SDCR_DDR2EN_SHIFT)    |	\
	(1 << DV_DDR_SDCR_DDREN_SHIFT)     |	\
	(1 << DV_DDR_SDCR_SDRAMEN_SHIFT)   |	\
	(1 << DV_DDR_SDCR_BUS_WIDTH_SHIFT) |	\
	(0x3 << DV_DDR_SDCR_CL_SHIFT)      |	\
	(0x2 << DV_DDR_SDCR_IBANK_SHIFT)   |	\
	(0x2 << DV_DDR_SDCR_PAGESIZE_SHIFT))

/*
 * RD_LATENCY = CAS_LATENCY + (1 or 2)
 */
#define CONFIG_SYS_DA850_DDR2_DDRPHYCR (DV_DDR_PHY_PWRDNEN | \
					DV_DDR_PHY_EXT_STRBEN | \
					(0x4 << DV_DDR_PHY_RD_LATENCY_SHIFT))


/* SDBCR2 is only used if IBANK_POS bit in SDBCR is set */
#define CONFIG_SYS_DA850_DDR2_SDBCR2 0x0

#define DDR_CLK_MHZ		132	/* DDR_CLK_MHZ = PLL1_SYSCLK1 / 2 */
#define DDR_TIMING(ns)		(((unsigned)((ns)*DDR_CLK_MHZ + 999) / 1000) - 1)

/* 8192 refresh cycle in 64ms, so refresh_rate=64ms/8192=7812.5ns */
#define DDR_RASMAX(ns)		((unsigned)(((ns)*10+78124) / 78125) - 1)

/* 8192 refresh cycle in 64ms */
#define DDR_REFRESH_RATE	((DDR_CLK_MHZ*1000*64)/8192)


/* Micron mt47h32m16hr-25e */
#define DDR_T_RFC	DDR_TIMING(127.5)
#define DDR_T_RP	DDR_TIMING(12.5)
#define DDR_T_RCD	DDR_TIMING(12.5)
#define DDR_T_WR	DDR_TIMING(15)
#define DDR_T_RAS	DDR_TIMING(45)
#define DDR_T_RC	DDR_TIMING(57.5)
#define DDR_T_RRD	DDR_TIMING(10)		/* we have x16 case */
#define DDR_T_WTR	(DDR_TIMING(7.5)+1)

#define DDR_T_RASMAX	DDR_RASMAX(70000)
#define DDR_T_XP	(3-1)		/* max(t_xp,t_cke)/t_ck - 1 */
#define DDR_T_ODT	0		/* not connected */
#define DDR_T_XSNR	DDR_TIMING(160)	/* t_RFC + 10 */
#define DDR_T_XSRD	(200-1)		/* t_XSRD/t_ck - 1 */
#define DDR_T_RTP	(DDR_TIMING(7.5)+1)
#define DDR_T_CKE	(3-1)		/* t_cke/t_ck-1 */

#define CONFIG_SYS_DA850_DDR2_SDTIMR (		  \
	(DDR_T_RFC << DV_DDR_SDTMR1_RFC_SHIFT)	| \
	(DDR_T_RP  << DV_DDR_SDTMR1_RP_SHIFT)	| \
	(DDR_T_RCD << DV_DDR_SDTMR1_RCD_SHIFT)	| \
	(DDR_T_WR  << DV_DDR_SDTMR1_WR_SHIFT)	| \
	(DDR_T_RAS << DV_DDR_SDTMR1_RAS_SHIFT)	| \
	(DDR_T_RC  << DV_DDR_SDTMR1_RC_SHIFT)	| \
	(DDR_T_RRD << DV_DDR_SDTMR1_RRD_SHIFT)	| \
	(DDR_T_WTR << DV_DDR_SDTMR1_WTR_SHIFT))

#define CONFIG_SYS_DA850_DDR2_SDTIMR2 (		       \
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
#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	-4	/* NS16550 register size */
#define CONFIG_SYS_NS16550_COM1	DAVINCI_UART1_BASE /* Base address of UART1 */
#define CONFIG_SYS_NS16550_CLK	clk_get(DAVINCI_UART1_CLKID)
#define CONFIG_CONS_INDEX	1		/* use UART1 for console */
#define CONFIG_BAUDRATE		115200		/* Default baud rate */
#define CONFIG_SYS_BAUDRATE_TABLE	{ 9600, 19200, 38400, 57600, 115200 }

#define CONFIG_SPI
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_DAVINCI_SPI
#define CONFIG_SYS_SPI_BASE		DAVINCI_SPI0_BASE
#define CONFIG_SYS_SPI_CLK		clk_get(DAVINCI_SPI0_CLKID)
#define CONFIG_SF_DEFAULT_SPEED		30000000
#define CONFIG_ENV_SPI_MAX_HZ	CONFIG_SF_DEFAULT_SPEED

/* SD/MMC */
#define CONFIG_MMC
#define CONFIG_GENERIC_MMC
#define CONFIG_DAVINCI_MMC
#define CONFIG_MMC_MBLOCK
#define CONFIG_DOS_PARTITION
#define CONFIG_CMD_I2C
#define CONFIG_CMD_FAT
#define CONFIG_CMD_EXT2
#define CONFIG_CMD_MMC

/*
 * USB configuration
 */
#define CONFIG_USB_DA8XX	/* Platform hookup to MUSB controller */
#define CONFIG_MUSB_HCD		/* include support for usb host */
#define CONFIG_CMD_USB		/* include support for usb cmd */

#define CONFIG_USB_STORAGE	/* MSC class support */
#define CONFIG_CMD_STORAGE	/* inclue support for usb-storage cmd */
#define CONFIG_CMD_FAT		/* inclue support for FAT/storage */
#define CONFIG_DOS_PARTITION	/* inclue support for FAT/storage */

#ifdef CONFIG_USB_KEYBOARD	/* HID class support */
#define CONFIG_SYS_USB_EVENT_POLL
#define CONFIG_PREBOOT "usb start"
#endif /* CONFIG_USB_KEYBOARD */

/*
 * I2C Configuration
 */
#define CONFIG_HARD_I2C
#define CONFIG_DRIVER_DAVINCI_I2C
#define CONFIG_SYS_I2C_SPEED		25000
#define CONFIG_SYS_I2C_SLAVE		10 /* Bogus, master-only in U-Boot */
#define CONFIG_SYS_I2C_EXPANDER_ADDR	0x20

#ifdef CONFIG_USE_SPIFLASH
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SECT_SIZE		(256 << 10)
#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#define CONFIG_ENV_OFFSET		(256 << 10)
#define CONFIG_SYS_NO_FLASH
#endif

/*
 * U-Boot general configuration
 */
#define CONFIG_MISC_INIT_R
#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOOTFILE		"uImage" /* Boot file name */
#define CONFIG_SYS_PROMPT	"U-Boot > " /* Command Prompt */
#define CONFIG_SYS_CBSIZE	1024 /* Console I/O Buffer Size	*/
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE+sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS	16 /* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE /* Boot Args Buffer Size */
#define CONFIG_SYS_LOAD_ADDR	(PHYS_SDRAM_1 + 0x700000)
#define CONFIG_VERSION_VARIABLE
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_HUSH_PARSER
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP
#define CONFIG_CRC32_VERIFY
#define CONFIG_MX_CYCLIC

/*
 * Linux Information
 */
#define LINUX_BOOT_PARAM_ADDR	(PHYS_SDRAM_1 + 0x100)
#define CONFIG_HWCONFIG		/* enable hwconfig */
#define CONFIG_CMDLINE_TAG
#define CONFIG_REVISION_TAG
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_BOOTARGS		\
	"mem=64M console=ttyS1,115200n8 rw noinitrd rootwait root=/dev/mmcblk0p2"
#define CONFIG_BOOTDELAY	3
#define CONFIG_EXTRA_ENV_SETTINGS	"hwconfig=dsp:wake=yes"

/*
 * U-Boot commands
 */
#include <config_cmd_default.h>
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS

#define CONFIG_CMD_ENV
#define CONFIG_CMD_ASKENV
#define CONFIG_CMD_SAVES
#define CONFIG_CMD_MEMORY

#ifdef CONFIG_USE_SPIFLASH
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_FLASH
#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF
#define CONFIG_CMD_SAVEENV
#endif

/* defines for SPL */
#define CONFIG_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT

#define CONFIG_SPL_MAX_SIZE	49152	/* 48 Kb for spl */
#define CONFIG_SPL_STACK	0x8001ff00
#define CONFIG_SPL_TEXT_BASE	0x80000000
#define CONFIG_SPL_LDSCRIPT	"board/$(BOARDDIR)/u-boot-spl-trik.lds"

#define CONFIG_SPL_SERIAL_SUPPORT
#define CONFIG_SPL_LIBCOMMON_SUPPORT
#define CONFIG_SPL_LIBGENERIC_SUPPORT
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SYS_TEXT_BASE - \
						CONFIG_SYS_MALLOC_LEN)
#define CONFIG_SYS_SPL_MALLOC_SIZE	CONFIG_SYS_MALLOC_LEN

#define CONFIG_SPL_BOOT_DEVICE_AUTODETECT
#define CONFIG_SPL_SPI_LOAD
#define CONFIG_SPL_YMODEM_LOAD
#define CONFIG_SPL_MMC_LOAD
#define CONFIG_SPL_GUNZIP_SUPPORT

#ifdef CONFIG_SPL_SPI_LOAD
#define CONFIG_SPL_SPI_SUPPORT
#define CONFIG_SPL_SPI_FLASH_SUPPORT
#define CONFIG_SPL_SPI_BUS 0
#define CONFIG_SPL_SPI_CS 0
#define CONFIG_SYS_SPI_U_BOOT_OFFS	CONFIG_SPL_MAX_SIZE
#endif /* CONFIG_SPL_SPI_LOAD */

#ifdef CONFIG_SPL_YMODEM_LOAD
#define CONFIG_SPL_YMODEM_SUPPORT
#endif /* CONFIG_SPL_YMODEM_LOAD */

#ifdef CONFIG_SPL_MMC_LOAD
#define CONFIG_SPL_MMC_SUPPORT
#define CONFIG_SPL_LIBDISK_SUPPORT
#define CONFIG_SYS_MMCSD_RAW_MODE_U_BOOT_SECTOR	(((1 << 20) + CONFIG_SPL_MAX_SIZE) / 512)	/* Assume:                                               */
												/*  1) standard MS-DOS partition layout                  */
												/*  2) partitions aligned at 1Mb boundary (2048 sectors) */
												/*  3) u-boot.ais partition start at 1Mb offset from     */
												/*     SD/MMC card begin and at least of 256Kb long      */
#endif /* CONFIG_SPL_MMC_LOAD */

#ifdef CONFIG_SPL_GUNZIP_SUPPORT
#define CONFIG_SPL_GUNZIP_MAX_SIZE	(100 << 10)	/* size of gzipped U-Boot: 100 Kb */
#define CONFIG_SPL_GUNZIP_LOAD_ADDR	0xc0000000	/* shouldn't overlap with CONFIG_SYS_TEXT_BASE */
#endif /* CONFIG_SPL_GUNZIP_SUPPORT */

/* additions for new relocation code, must added to all boards */
#define CONFIG_SYS_SDRAM_BASE		0xc0000000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x1000 - /* Fix this */ \
					GENERATED_GBL_DATA_SIZE)
#endif /* __CONFIG_H */
