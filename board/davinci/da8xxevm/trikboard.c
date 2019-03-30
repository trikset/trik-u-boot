/*
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Based on da830evm.c. Original Copyrights follow:
 *
 * Copyright (C) 2009 Nick Thompson, GE Fanuc, Ltd. <nick.thompson@gefanuc.com>
 * Copyright (C) 2007 Sergey Kubushyn <ksi@koi8.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <i2c.h>
#include <spi.h>
#include <spi_flash.h>
#include <asm/arch/hardware.h>
#include <asm/ti-common/davinci_nand.h>
#include <asm/arch/pinmux_defs.h>
#include <asm/io.h>
#include <asm/arch/davinci_misc.h>
#include <asm/arch/gpio.h>
#include <asm/gpio.h>
#include <linux/errno.h>
#include <hwconfig.h>
#include <asm/mach-types.h>
#include <mmc.h>
#include <asm/arch/sdmmc_defs.h>
#include <environment.h>

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
	memset(resetvect, 0, sizeof(unsigned) *10);

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

static struct davinci_mmc mmc_sd0 = {
	.reg_base = (struct davinci_mmc_regs *)DAVINCI_MMC_SD0_BASE,
	.host_caps = MMC_MODE_4BIT,     /* DA850 supports only 4-bit SD/MMC */
	.voltages = MMC_VDD_32_33 | MMC_VDD_33_34,
	.version = MMC_CTLR_VERSION_2,
};

int board_mmc_init(bd_t *bis)
{
	mmc_sd0.input_clk = clk_get(DAVINCI_MMCSD_CLKID);

	/* Add slot-0 to mmc subsystem */
	return davinci_mmc_init(bis, &mmc_sd0);
}

#define LED_GREEN_GPIO  88
#define LED_RED_GPIO    87
static const struct pinmux_config trik_gpio_leds_pins[] = {
	{ pinmux(11), 8, 7 }, /*GPIO5[8] green */
	{ pinmux(12), 8, 0 }, /*GPIO5[7] red */
};

/* This pins cannot be properly configured in linux */
#define WIFI_LVL_GPIO    104
#define POWER_CONNECTORS 94
#define POWER_E1_EN      35
#define POWER_C_EN       36
#define POWER_E0_EN      37
static const struct pinmux_config trik_clock_and_power_pins[] = {
	{ pinmux(13), 1, 1 }, /* OBSCLK0 */
	{ pinmux(13), 8, 7 }, /* GPIO6[8] */
	{ pinmux(11), 8, 1 }, /* GPIO5[14] */
	{ pinmux(6),  8, 4 }, /* GPIO2[3] */
	{ pinmux(6),  8, 3 }, /* GPIO2[4] */
	{ pinmux(6),  8, 2 }, /* GPIO2[5] */
};

/* This pins cannot be properly configured in linux */
#define MSP_BSL_GPIO     80
#define MSP_TEST_GPIO    85
#define MSP_RESET_GPIO   93
static const struct pinmux_config trik_msp430_pins[] = {
	{ pinmux(12), 8, 7}, /* GP5[0] */
	{ pinmux(12), 8, 2}, /* GP5[5] */
	{ pinmux(11), 8, 2}, /* GP5[13] */
};

static const struct pinmux_config trik_mmcsd0_pins[] = {
	{ pinmux(10), 8, 6 }, /* GP4[1] - insert/remove pin */
	{ pinmux(10), 2, 5 }, /* MMCSD0_DAT[3]              */
	{ pinmux(10), 2, 4 }, /* MMCSD0_DAT[2]              */
	{ pinmux(10), 2, 3 }, /* MMCSD0_DAT[1]              */
	{ pinmux(10), 2, 2 }, /* MMCSD0_DAT[0]              */
	{ pinmux(10), 2, 1 }, /* MMCSD0_CMD                 */
	{ pinmux(10), 2, 0 }, /* MMCSD0_CLK                 */
};

const struct pinmux_resource pinmuxes[] = {
#ifdef CONFIG_SPI_FLASH
	PINMUX_ITEM(spi0_pins_base),
	PINMUX_ITEM(spi0_pins_scs0),
	PINMUX_ITEM(spi0_pins_scs1),
#endif
	PINMUX_ITEM(uart1_pins_txrx),
	PINMUX_ITEM(trik_mmcsd0_pins),
#ifdef CONFIG_HARD_I2C
	PINMUX_ITEM(i2c0_pins),
	PINMUX_ITEM(i2c1_pins),
#endif
	PINMUX_ITEM(trik_gpio_leds_pins),
	PINMUX_ITEM(trik_clock_and_power_pins),
	PINMUX_ITEM(trik_msp430_pins),
};

const int pinmuxes_size = ARRAY_SIZE(pinmuxes);

const struct lpsc_resource lpsc[] = {
//	{ DAVINCI_LPSC_AEMIF },	/* NAND, NOR */
	{ DAVINCI_LPSC_SPI0 },	/* Serial Flash */
	{ DAVINCI_LPSC_UART1 },	/* console */
	{ DAVINCI_LPSC_GPIO },
	{ DAVINCI_LPSC_MMC_SD },
};

const int lpsc_size = ARRAY_SIZE(lpsc);

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
	irq_init();

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

	/* Turn on main LED */
	gpio_direction_output(LED_GREEN_GPIO, 0);
	gpio_direction_output(LED_RED_GPIO, 1);

	/* Enable WIFI level shifter */
	gpio_direction_output(WIFI_LVL_GPIO, 1);

	/* Enable 5V circuits */
	gpio_direction_output(POWER_CONNECTORS, 1);
	gpio_direction_output(POWER_E0_EN, 1);
	gpio_direction_output(POWER_E1_EN, 1);
	gpio_direction_output(POWER_C_EN, 1);

	/* Start MSP in application mode */
	gpio_direction_output(MSP_BSL_GPIO, 0);
	gpio_direction_output(MSP_TEST_GPIO, 0);
	gpio_direction_output(MSP_RESET_GPIO, 0);
	udelay(50000);
	gpio_direction_output(MSP_RESET_GPIO, 1);

	return 0;
}

#ifndef CONFIG_SPL_BUILD
enum env_location env_get_location(enum env_operation op, int prio)
{
	/* At most one env location */
	if (prio)
		return ENVL_UNKNOWN;

#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
	if (davinci_syscfg_regs->bootcfg == DAVINCI_SPI0_FLASH_BOOT)
		return ENVL_SPI_FLASH;
#endif
	/* MMC assumed */
	return ENVL_NOWHERE;
}
#endif

#if (!defined CONFIG_ENV_IS_NOWHERE) && (!defined CONFIG_SPL_BUILD)
static int env_nowhere_init(void)
{
	gd->env_addr	= (ulong)&default_environment[0];
	gd->env_valid	= ENV_INVALID;

	return 0;
}

U_BOOT_ENV_LOCATION(nowhere) = {
	.location	= ENVL_NOWHERE,
	.init		= env_nowhere_init,
	ENV_NAME("nowhere")
};
#endif
