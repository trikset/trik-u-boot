/*
 * Copyright (C) 2011
 * Heiko Schocher, DENX Software Engineering, hs@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <config.h>
#include <spl.h>
#include <asm/u-boot.h>
#include <asm/utils.h>
#include <nand.h>
#include <asm/arch/dm365_lowlevel.h>
#include <asm/arch/davinci_boot.h>
#include <ns16550.h>
#include <malloc.h>
#include <spi_flash.h>
#include <mmc.h>
#include <linux/compiler.h>

DECLARE_GLOBAL_DATA_PTR;

#ifndef CONFIG_SPL_LIBCOMMON_SUPPORT
void puts(const char *str)
{
	while (*str)
		putc(*str++);
}

void putc(char c)
{
	if (c == '\n')
		NS16550_putc((NS16550_t)(CONFIG_SYS_NS16550_COM1), '\r');

	NS16550_putc((NS16550_t)(CONFIG_SYS_NS16550_COM1), c);
}
#endif /* CONFIG_SPL_LIBCOMMON_SUPPORT */

void board_init_f(ulong dummy)
{
	/* First, setup our stack pointer. */
	asm volatile("mov sp, %0\n" : : "r"(CONFIG_SPL_STACK));

	/* Second, perform our low-level init. */
#ifdef CONFIG_SOC_DM365
	dm36x_lowlevel_init(0);
#endif
#ifdef CONFIG_SOC_DA8XX
	arch_cpu_init();
#endif

	/* Third, we clear the BSS. */
	memset(__bss_start, 0, __bss_end__ - __bss_start);

	/* Finally, setup gd and move to the next step. */
	gd = &gdata;
	board_init_r(NULL, 0);
}

void spl_board_init(void)
{
	preloader_console_init();
}

u32 spl_boot_mode(void)
{
	return MMCSD_MODE_RAW;
}

u32 spl_boot_device(void)
{
#if defined(CONFIG_SOC_DA8XX) && defined(CONFIG_SPL_BOOT_DEVICE_AUTODETECT)
	u32 bootmode = readl(BOOTCFG_REG) & BOOTCFG_REG_DEVICE_MASK;
	switch (bootmode) {
#ifdef CONFIG_SPL_NAND_SIMPLE
	case BOOTCFG_DEVICE_NAND8:
	case BOOTCFG_DEVICE_NAND16:
		return BOOT_DEVICE_NAND;
#endif
#ifdef CONFIG_SPL_SPI_LOAD
	case BOOTCFG_DEVICE_SPI0_FLASH:
	case BOOTCFG_DEVICE_SPI1_FLASH:
		return BOOT_DEVICE_SPI;
#endif
#ifdef CONFIG_SPL_YMODEM_LOAD
	case BOOTCFG_DEVICE_UART0:
	case BOOTCFG_DEVICE_UART1:
	case BOOTCFG_DEVICE_UART2:
		return BOOT_DEVICE_UART;
#endif
#ifdef CONFIG_SPL_MMC_LOAD
	case BOOTCFG_DEVICE_MMC_OR_SD0:
		return BOOT_DEVICE_MMC1;
#endif
	default:
		puts("Unknown boot device\n");
		hang();
	}
#elif defined(CONFIG_SPL_NAND_SIMPLE)
	return BOOT_DEVICE_NAND;
#elif defined(CONFIG_SPL_SPI_LOAD)
	return BOOT_DEVICE_SPI;
#elif defined(CONFIG_SPL_MMC_LOAD)
	return BOOT_DEVICE_MMC1;
#elif defined(CONFIG_SPL_YMODEM_LOAD)
	return BOOT_DEVICE_UART;
#else
	puts("Unknown boot device\n");
	hang();
#endif
}
