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

#ifndef _INITIALIZE_H_
#define _INITIALIZE_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif


#include <libmaple/libmaple_types.h>
#include <libmaple/gpio.h>
#include <libmaple/timer.h>
#include <libmaple/flash.h>
#include <libmaple/nvic.h>
#include <libmaple/systick.h>
#include <libmaple/rcc.h>

void init(void);


#define CYCLES_PER_MICROSECOND    72
#define SYSTICK_RELOAD_VAL     71999 /* takes a cycle to reload */

#define BOARD_BUTTON_PIN          32
#define BOARD_LED_PIN             33

#define BOARD_NR_USARTS           3
#define BOARD_USART1_TX_PIN       26
#define BOARD_USART1_RX_PIN       25
#define BOARD_USART2_TX_PIN       9
#define BOARD_USART2_RX_PIN       8
#define BOARD_USART3_TX_PIN       1
#define BOARD_USART3_RX_PIN       0

#define BOARD_NR_SPI              2
#define BOARD_SPI1_NSS_PIN        7
#define BOARD_SPI1_MOSI_PIN       4
#define BOARD_SPI1_MISO_PIN       5
#define BOARD_SPI1_SCK_PIN        6
#define BOARD_SPI2_NSS_PIN        31
#define BOARD_SPI2_MOSI_PIN       28
#define BOARD_SPI2_MISO_PIN       29
#define BOARD_SPI2_SCK_PIN        30

#define BOARD_NR_GPIO_PINS        34
#define BOARD_NR_PWM_PINS         12
#define BOARD_NR_ADC_PINS          9
#define BOARD_NR_USED_PINS         4

#define BOARD_JTMS_SWDIO_PIN      22
#define BOARD_JTCK_SWCLK_PIN      21
#define BOARD_JTDI_PIN            20
#define BOARD_JTDO_PIN            19
#define BOARD_NJTRST_PIN          18

#define BOARD_USB_DISC_DEV        GPIOB
#define BOARD_USB_DISC_BIT        9



/*
 * Derived and default board definitions
 */

#define CLOCK_SPEED_MHZ                 CYCLES_PER_MICROSECOND
#define CLOCK_SPEED_HZ                  (CLOCK_SPEED_MHZ * 1000000UL)

#ifndef SYSTICK_RELOAD_VAL
#define SYSTICK_RELOAD_VAL              (1000 * CYCLES_PER_MICROSECOND - 1)
#endif

#ifndef BOARD_BUTTON_PRESSED_LEVEL
#define BOARD_BUTTON_PRESSED_LEVEL      HIGH
#endif

/**
 * @brief Does the board break out a USART/UART's RX and TX pins?
 *
 * BOARD_HAVE_USART(n) is nonzero iff USARTn is available (n must be
 * an integer literal, 1 through 6). Also see BOARD_HAVE_USART1, ...,
 * BOARD_HAVE_UART4 (sic), etc.
 */
#define BOARD_HAVE_USART(n) (defined(BOARD_USART##n##_TX_PIN) && \
                             defined(BOARD_USART##n##_RX_PIN))
/** Feature test: nonzero iff the board has USART1. */
#define BOARD_HAVE_USART1               BOARD_HAVE_USART(1)
/** Feature test: nonzero iff the board has USART2, 0 otherwise. */
#define BOARD_HAVE_USART2               BOARD_HAVE_USART(2)
/** Feature test: nonzero iff the board has USART3, 0 otherwise. */
#define BOARD_HAVE_USART3               BOARD_HAVE_USART(3)
/** Feature test: nonzero iff the board has UART4, 0 otherwise. */
#define BOARD_HAVE_UART4                BOARD_HAVE_USART(4)
/** Feature test: nonzero iff the board has UART5, 0 otherwise. */
#define BOARD_HAVE_UART5                BOARD_HAVE_USART(5)
/** Feature test: nonzero iff the board has USART6, 0 otherwise. */
#define BOARD_HAVE_USART6               BOARD_HAVE_USART(6)

/**
 * @brief Does the board break out a SPI peripheral's pins?
 *
 * BOARD_HAVE_SPI(n) is nonzero iff SPIn is available (n must be an
 * integer literal: 1, 2, or 3). Also see BOARD_HAVE_SPI1,
 * BOARD_HAVE_SPI2, BOARD_HAVE_SPI3. */
#define BOARD_HAVE_SPI(n) (defined(BOARD_SPI##n##_NSS_PIN)  &&          \
                           defined(BOARD_SPI##n##_SCK_PIN)  &&          \
                           defined(BOARD_SPI##n##_MISO_PIN) &&          \
                           defined(BOARD_SPI##n##_MOSI_PIN))
/** Feature test: nonzero iff the board has SPI1. */
#define BOARD_HAVE_SPI1 BOARD_HAVE_SPI(1)
/** Feature test: nonzero iff the board has SPI2. */
#define BOARD_HAVE_SPI2 BOARD_HAVE_SPI(2)
/** Feature test: nonzero iff the board has SPI3. */
#define BOARD_HAVE_SPI3 BOARD_HAVE_SPI(3)

/**
 * @brief Feature test: nonzero iff the board has SerialUSB.
 */
#define BOARD_HAVE_SERIALUSB (defined(BOARD_USB_DISC_DEV) && \
                              defined(BOARD_USB_DISC_BIT))


#ifdef __cplusplus
}
#endif

#endif 
