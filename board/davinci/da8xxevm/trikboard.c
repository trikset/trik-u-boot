/*
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Based on da830evm.c. Original Copyrights follow:
 *
 * Copyright (C) 2009 Nick Thompson, GE Fanuc, Ltd. <nick.thompson@gefanuc.com>
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

#include <common.h>
#include <i2c.h>
#include <spi.h>
#include <spi_flash.h>
#include <asm/arch/hardware.h>
#include <asm/arch/emif_defs.h>
#include <asm/arch/pinmux_defs.h>
#include <asm/io.h>
#include <asm/arch/davinci_misc.h>
#include <asm/errno.h>
#include <hwconfig.h>
#ifdef CONFIG_MMC
#include <mmc.h>
#include <asm/arch/sdmmc_defs.h>
#endif


DECLARE_GLOBAL_DATA_PTR;

void dsp_lpsc_on(unsigned domain, unsigned int id)
{
	dv_reg_p mdstat, mdctl, ptstat, ptcmd;
	struct davinci_psc_regs *psc_regs;

	psc_regs = davinci_psc0_regs;
	mdstat = &psc_regs->psc0.mdstat[id];
	mdctl = &psc_regs->psc0.mdctl[id];
	ptstat = &psc_regs->ptstat;
	ptcmd = &psc_regs->ptcmd;

	while (*ptstat & (0x1 << domain))
		;

	if ((*mdstat & 0x1f) == 0x03)
		return;                 /* Already on and enabled */

	*mdctl |= 0x03;

	*ptcmd = 0x1 << domain;

	while (*ptstat & (0x1 << domain))
		;
	while ((*mdstat & 0x1f) != 0x03)
		;		/* Probably an overkill... */
}

static void dspwake(void)
{
	unsigned *resetvect = (unsigned *)DAVINCI_L3CBARAM_BASE;
	u32 val;

	/* if the device is ARM only, return */
	if ((readl(CHIP_REV_ID_REG) & 0x3f) == 0x10)
		return;

	if (hwconfig_subarg_cmp_f("dsp", "wake", "no", NULL))
		return;

	*resetvect++ = 0x1E000; /* DSP Idle */
	/* clear out the next 10 words as NOP */
	memset(resetvect, 0, sizeof(unsigned) * 10);

	/* setup the DSP reset vector */
	writel(DAVINCI_L3CBARAM_BASE, HOST1CFG);

	dsp_lpsc_on(1, DAVINCI_LPSC_GEM);
	val = readl(PSC0_MDCTL + (15 * 4));
	val |= 0x100;
	writel(val, (PSC0_MDCTL + (15 * 4)));
}

int misc_init_r(void)
{
	dspwake();
	return 0;
}
static const struct pinmux_config trik_gpio_keys_pins[] = {
	{ pinmux(8), 8, 3 }, /*GPIO3[4]*/ 
	{ pinmux(7), 8, 3 }, /*GPIO3[12]*/
	{ pinmux(7), 8, 2 }, /*GPIO3[13]*/
	{ pinmux(7), 8, 1 }, /*GPIO3[14]*/
	{ pinmux(7), 8, 0 }, /*GPIO3[15]*/
	{ pinmux(6), 8, 7 }, /*GPIO2[0]*/
	{ pinmux(11), 8, 7 }, /*GPIO5[8]*/
};

#ifdef CONFIG_MMC
static const struct pinmux_config trik_mmcsd0_pins[] = {
	{ pinmux(10), 8, 6 }, /* GP4[1] - insert/remove pin */
	{ pinmux(10), 2, 5 }, /* MMCSD0_DAT[3]              */
	{ pinmux(10), 2, 4 }, /* MMCSD0_DAT[2]              */
	{ pinmux(10), 2, 3 }, /* MMCSD0_DAT[1]              */
	{ pinmux(10), 2, 2 }, /* MMCSD0_DAT[0]              */
	{ pinmux(10), 2, 1 }, /* MMCSD0_CMD                 */
	{ pinmux(10), 2, 0 }, /* MMCSD0_CLK                 */
};
#endif

const struct pinmux_resource pinmuxes[] = {
#ifdef CONFIG_SPI_FLASH
	PINMUX_ITEM(spi0_pins_base),
	PINMUX_ITEM(spi0_pins_scs0),
	PINMUX_ITEM(spi0_pins_scs1),
#endif
#ifdef CONFIG_MMC
	PINMUX_ITEM(trik_mmcsd0_pins),
#endif
#ifdef CONFIG_HARD_I2C
	PINMUX_ITEM(i2c0_pins),
	PINMUX_ITEM(i2c1_pins),
#endif
	PINMUX_ITEM(uart1_pins_txrx),
#ifdef COMFIG_GPIO_BUTTON
	PINMUX_ITEM(trik_gpio_keys_pins),
#endif
};

const int pinmuxes_size = ARRAY_SIZE(pinmuxes);

const struct lpsc_resource lpsc[] = {
#ifdef CONFIG_SPI_FLASH
	{ DAVINCI_LPSC_SPI0   },	/* Serial Flash */
#endif
#ifdef CONFIG_MMC
	{ DAVINCI_LPSC_MMC_SD },
#endif
#ifdef CONFIG_HARD_I2C
	{ DAVINCI_LPSC_I2C1   },
#endif
	{ DAVINCI_LPSC_UART1  },	/* console */
	{ DAVINCI_LPSC_GPIO   },
};

const int lpsc_size = ARRAY_SIZE(lpsc);

#ifndef CONFIG_DA850_EVM_MAX_CPU_CLK
#define CONFIG_DA850_EVM_MAX_CPU_CLK	300000000
#endif

#define REV_AM18X_EVM		0x100

/*
 * get_board_rev() - setup to pass kernel board revision information
 * Returns:
 * bit[0-3]	Maximum cpu clock rate supported by onboard SoC
 *		0000b - 300 MHz
 *		0001b - 372 MHz
 *		0010b - 408 MHz
 *		0011b - 456 MHz
 */
u32 get_board_rev(void)
{
	char *s;
	u32 maxcpuclk = CONFIG_DA850_EVM_MAX_CPU_CLK;
	u32 rev = 0;

	s = getenv("maxcpuclk");
	if (s)
		maxcpuclk = simple_strtoul(s, NULL, 10);

	if (maxcpuclk >= 456000000)
		rev = 3;
	else if (maxcpuclk >= 408000000)
		rev = 2;
	else if (maxcpuclk >= 372000000)
		rev = 1;
#ifdef CONFIG_DA850_AM18X_EVM
	rev |= REV_AM18X_EVM;
#endif
	return rev;
}

int board_early_init_f(void)
{
	/*
	 * Power on required peripherals
	 * ARM does not have access by default to PSC0 and PSC1
	 * assuming here that the DSP bootloader has set the IOPU
	 * such that PSC access is available to ARM
	 */
	if (da8xx_configure_lpsc_items(lpsc, ARRAY_SIZE(lpsc)))
		return 1;

	return 0;
}

int board_init(void)
{
#ifndef CONFIG_USE_IRQ
	irq_init();
#endif

	/* arch number of the board */
	gd->bd->bi_arch_number = MACH_TYPE_DAVINCI_TRIK;

	/* address of boot parameters */
	gd->bd->bi_boot_params = LINUX_BOOT_PARAM_ADDR;

	/* setup the SUSPSRC for ARM to control emulation suspend */
	writel(readl(&davinci_syscfg_regs->suspsrc) &
	       ~(DAVINCI_SYSCFG_SUSPSRC_I2C | DAVINCI_SYSCFG_SUSPSRC_SPI0 |
		 DAVINCI_SYSCFG_SUSPSRC_TIMER0 | DAVINCI_SYSCFG_SUSPSRC_UART1),
	       &davinci_syscfg_regs->suspsrc);

	/* configure pinmux settings */
	if (davinci_configure_pin_mux_items(pinmuxes, ARRAY_SIZE(pinmuxes)))
		return 1;

	/* enable the console UART */
	writel((DAVINCI_UART_PWREMU_MGMT_FREE | DAVINCI_UART_PWREMU_MGMT_URRST |
		DAVINCI_UART_PWREMU_MGMT_UTRST),
	       &davinci_uart1_ctrl_regs->pwremu_mgmt);

	return 0;
}

#ifdef CONFIG_DAVINCI_MMC
static struct davinci_mmc mmc_sd0 = {
	.reg_base	= (struct davinci_mmc_regs *)DAVINCI_MMC_SD0_BASE,
	.input_clk	= 228000000,
	.host_caps	= MMC_MODE_4BIT,
	.voltages	= MMC_VDD_32_33 | MMC_VDD_33_34,
	.version	= MMC_CTLR_VERSION_2,
};

int board_mmc_init(bd_t *bis)
{
	mmc_sd0.input_clk = clk_get(DAVINCI_MMC_CLKID);
	/* Add slot-0 to mmc subsystem */
	return davinci_mmc_init(bis, &mmc_sd0);
}
#endif
