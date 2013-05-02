/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2011 LeafLabs, LLC.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

//@TODO: Organize this file more. Remove redundant or useless data.

#include "initialize.h"
#include <libmaple/libmaple_types.h>
#include <libmaple/gpio.h>
#include <libmaple/timer.h>
#include <libmaple/flash.h>
#include <libmaple/nvic.h>
#include <libmaple/systick.h>
#include <libmaple/rcc.h>
#include <libmaple/usb_cdcacm.h>
#include <libmaple/usb.h>


//TODO: Add remaining functions
static void setup_flash(void);
static void setup_clocks(void);
static void setup_nvic(void);
static void setup_timers(void);
static void board_setup_usb(void);
static void series_init(void);
static void disableDebugPorts(void);



//***************************
// MAPLE MINI SPECIFIC VALUES
//***************************

// Allow boards to provide a PLL multiplier. This is useful for
// e.g. STM32F100 value line MCUs, which use slower multipliers.
// (We're leaving the default to RCC_PLLMUL_9 for now, since that
// works for F103 performance line MCUs, which is all that LeafLabs
// currently officially supports).
#ifndef BOARD_RCC_PLLMUL
#define BOARD_RCC_PLLMUL RCC_PLLMUL_9
#endif

/*
 * Chip-specific initialization data
 */

static stm32f1_rcc_pll_data pll_data = {BOARD_RCC_PLLMUL};
static rcc_pll_cfg w_board_pll_cfg = {RCC_PLLSRC_HSE, &pll_data};

#define PMAP_ROW(gpio_dev, gpio_bit, timer_dev, timer_ch, adc_dev, adc_ch) \
    { gpio_dev, timer_dev, adc_dev, gpio_bit, timer_ch, adc_ch }

//*******************************
// MAPLE MINI SPECIFIC VALUES END
//*******************************





static void setup_flash(void) {
    // Turn on as many Flash "go faster" features as
    // possible. flash_enable_features() just ignores any flags it
    // can't support.
    flash_enable_features(FLASH_PREFETCH | FLASH_ICACHE | FLASH_DCACHE);
    // Configure the wait states, assuming we're operating at "close
    // enough" to 3.3V.
    flash_set_latency(FLASH_SAFE_WAIT_STATES);
}

static void setup_clocks(void) {
    // Turn on HSI. We'll switch to and run off of this while we're
    // setting up the main PLL.
    rcc_turn_on_clk(RCC_CLK_HSI);

    // Turn off and reset the clock subsystems we'll be using, as well
    // as the clock security subsystem (CSS). Note that resetting CFGR
    // to its default value of 0 implies a switch to HSI for SYSCLK.
    RCC_BASE->CFGR = 0x00000000;
    rcc_disable_css();
    rcc_turn_off_clk(RCC_CLK_PLL);
    rcc_turn_off_clk(RCC_CLK_HSE);
    
    //Removed board_reset_pll(void) because it's not implemented

    // Clear clock readiness interrupt flags and turn off clock
    // readiness interrupts.
    RCC_BASE->CIR = 0x00000000;

    // Enable HSE, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_HSE);
    while (!rcc_is_clk_ready(RCC_CLK_HSE))
        ;

    // Configure AHBx, APBx, etc. prescalers and the main PLL.
    //wirish::priv::board_setup_clock_prescalers();
        rcc_set_prescaler(RCC_PRESCALER_AHB, RCC_AHB_SYSCLK_DIV_1);
        rcc_set_prescaler(RCC_PRESCALER_APB1, RCC_APB1_HCLK_DIV_2);
        rcc_set_prescaler(RCC_PRESCALER_APB2, RCC_APB2_HCLK_DIV_1);

    rcc_configure_pll(&w_board_pll_cfg);

    // Enable the PLL, and wait until it's ready.
    rcc_turn_on_clk(RCC_CLK_PLL);
    while(!rcc_is_clk_ready(RCC_CLK_PLL))
        ;

    // Finally, switch to the now-ready PLL as the main clock source.
    rcc_switch_sysclk(RCC_CLKSRC_PLL);
}

/*
 * These addresses are where usercode starts when a bootloader is
 * present. If no bootloader is present, the user NVIC usually starts
 * at the Flash base address, 0x08000000.
 *
 * FIXME Let the build specify the vector table address numerically to
 * avoid having these magic values -- some people have been fixing up
 * the bootloader so it uses less space.
 */
#define USER_ADDR_ROM 0x08005000
#define USER_ADDR_RAM 0x20000C00

static void setup_nvic(void) {
#ifdef VECT_TAB_FLASH
    nvic_init(USER_ADDR_ROM, 0);
#elif defined VECT_TAB_RAM
    nvic_init(USER_ADDR_RAM, 0);
#elif defined VECT_TAB_BASE
    nvic_init((uint32)0x08000000, 0);
#else
#error "You must select a base address for the vector table."
#endif
}

static void board_setup_gpio(void) {
            gpio_init_all();
}

static void timer_default_config(timer_dev *dev) {
    /*
    timer_adv_reg_map *regs = (dev->regs).adv;
    const uint16 full_overflow = 0xFFFF;
    const uint16 half_duty = 0x8FFF;

    timer_init(dev);
    timer_pause(dev);

    regs->CR1 = TIMER_CR1_ARPE;
    regs->PSC = 1;
    regs->SR = 0;
    regs->DIER = 0;
    regs->EGR = TIMER_EGR_UG;
    switch (dev->type) {
    case TIMER_ADVANCED:
        regs->BDTR = TIMER_BDTR_MOE | TIMER_BDTR_LOCK_OFF;
        // fall-through
    case TIMER_GENERAL:
        timer_set_reload(dev, full_overflow);
        uint8 channel = 1;
        for (channel=1 ; channel <= 4; channel++) {
            if (timer_has_cc_channel(dev, channel)) {
                timer_set_compare(dev, channel, half_duty);
                timer_oc_set_mode(dev, channel, TIMER_OC_MODE_PWM_1,
                                  TIMER_OC_PE);
            }
        }
        // fall-through
    case TIMER_BASIC:
        break;
    }

    timer_generate_update(dev);
    timer_resume(dev);
    */
}

static void setup_timers(void) {
    //timer_foreach(timer_default_config);
    timer_init_all();
}

static void board_setup_usb(void) {
    usb_cdcacm_enable(GPIOB, 9);

}

static void series_init(void) {
    // Initialize AFIO here, too, so peripheral remaps and external
    // interrupts work out of the box.
    afio_init();
}

static void disableDebugPorts(void) {
    afio_cfg_debug_ports(AFIO_DEBUG_NONE);
}



//Initialize functions for the board
//Starts up 
void init(void) {
    setup_flash();
    setup_clocks();
    setup_nvic();
    systick_init(SYSTICK_RELOAD_VAL);
    board_setup_gpio();
    setup_timers();
    board_setup_usb();
    series_init();
    disableDebugPorts();

}
