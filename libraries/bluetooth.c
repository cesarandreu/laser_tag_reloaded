/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 * Copyright (c) 2011, 2012 LeafLabs, LLC.
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


#include "bluetooth.h"
#include <libmaple/libmaple.h>
#include <libmaple/gpio.h>
#include <libmaple/timer.h>
#include <libmaple/usart.h>

#define BLUETOOTH_BAUD 115200
#define BLUETOOTH_USART USART2
#define BLUETOOTH_PORT_TX GPIOA
#define BLUETOOTH_PORT_RX GPIOA
#define BLUETOOTH_PIN_TX 2
#define BLUETOOTH_PIN_RX 3

void bluetooth_start(void){
    usart_config_gpios_async(BLUETOOTH_USART, BLUETOOTH_PORT_RX, BLUETOOTH_PIN_RX, BLUETOOTH_PORT_TX, BLUETOOTH_PIN_TX, 0);
    usart_init(BLUETOOTH_USART);
    usart_set_baud_rate(BLUETOOTH_USART, USART_USE_PCLK, BLUETOOTH_BAUD);
    usart_enable(BLUETOOTH_USART);
}

void bluetooth_end(void){
    usart_disable(BLUETOOTH_USART);
}

uint32 bluetooth_available(void){
    return usart_data_available(BLUETOOTH_USART);
}

uint8 bluetooth_read(void){
    while(!bluetooth_available());
    return usart_getc(BLUETOOTH_USART);
}

void bluetooth_write(unsigned char ch){
    usart_putc(BLUETOOTH_USART, ch);
}

void bluetooth_flush(void){
    usart_reset_rx(BLUETOOTH_USART);
}




