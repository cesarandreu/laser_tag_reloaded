/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
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

#include "usb.h"
#include <string.h>
#include <stdint.h>
#include <libmaple/libmaple.h>
#include <libmaple/systick.h>
#include <libmaple/nvic.h>
#include <libmaple/usb_cdcacm.h>
#include <libmaple/usb.h>


#define USB_TIMEOUT 50


void usb_start(void){
    usb_cdcacm_enable(GPIOB, 9);
}

void usb_end(void){
    usb_cdcacm_disable(GPIOB, 9);
}

uint8 usb_isConnected(void){
    return (uint8)(usb_is_connected(USBLIB) && usb_is_configured(USBLIB));
}

void usb_write(const void *buf, uint32 len){
    if(!usb_isConnected() || !buf){
        return;
    }

    uint32 txed = 0;
    uint32 old_txed = 0;
    uint32 start = systick_uptime();

    uint32 sent = 0;

    while (txed < len && (systick_uptime() - start < USB_TIMEOUT)) {
        sent = usb_cdcacm_tx((const uint8*)buf + txed, len - txed);
        txed += sent;
        if (old_txed != txed) {
            start = systick_uptime();
        }
        old_txed = txed;
    }


    if (sent == USB_CDCACM_TX_EPSIZE) {
        while (usb_cdcacm_is_transmitting() != 0) {
        }
        /* flush out to avoid having the pc wait for more data */
        usb_cdcacm_tx(NULL, 0);
    }

}

void usb_writeChar(uint8 ch){
    usb_write(&ch, 1);
}

void usb_writeString(const char *str){
    usb_write(str, strlen(str));
}

uint32 usb_available(void){
    return usb_cdcacm_data_available();
}

uint32 usb_readString(void *buf, uint32 len){
    if (!buf) {
        return 0;
    }

    uint32 rxed = 0;
    while (rxed < len) {
        rxed += usb_cdcacm_rx((uint8*)buf + rxed, len - rxed);
    }

    return rxed;
}

uint32 usb_read(void){
    uint8 b;
    usb_readString(&b, 1);
    return b;
}

uint8 usb_pending(void){
    return usb_cdcacm_get_pending();
}

void usb_println(void){
    usb_writeChar('\r');
    usb_writeChar('\n');
}

void usb_printString(const char str[]){
    usb_writeString(str);
}

void usb_printlnString(const char str[]){
    usb_writeString(str);
    usb_println();
}

void usb_printChar(char c){
    usb_writeChar(c);
}

void usb_printlnChar(char c){
    usb_writeChar(c);
    usb_println();
}
