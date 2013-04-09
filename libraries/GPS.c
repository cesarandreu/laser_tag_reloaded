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


#include "GPS.h"

#define GPS_BAUD 9600
#define GPS_USART USART1
#define GPS_PORT_TX GPIOA
#define GPS_PORT_RX GPIOA
#define GPS_PIN_TX 9
#define GPS_PIN_RX 10

#define GPS_VDD 8

#define NMEA_MAX_MESSAGE_LENGTH 82

void gps_start(void){
    usart_config_gpios_async(GPS_USART, GPS_PORT_RX, GPS_PIN_RX, GPS_PORT_TX, GPS_PIN_TX, 0);
    usart_init(GPS_USART);
    usart_set_baud_rate(GPS_USART, USART_USE_PCLK, GPS_BAUD);

    //Enable BJT to turn on GPS
    //TODO: Confirm this isn't affected by being on UART CLK Pin (Currently unused)
    //gpio_set_mode(GPIOA, 1, GPIO_OUTPUT_PP);
    //gpio_write_bit(GPIOA, 8, 1);
    
    //Enable GPS USART Port
    usart_enable(GPS_USART);
}

void gps_end(void){
    usart_disable(GPS_USART);
    //gpio_write_bit(GPIOA, 8, 0);
}

uint32 gps_available(void){
    return usart_data_available(GPS_USART);
}

uint8 gps_read(void){
    while(!gps_available());
    return usart_getc(GPS_USART);
}

//Read GPS string up to the specified length or the end of that string, whichever comes first.
//Returns the amount of characters read from the GPS String
int gps_readString(char *str, int length){
    char c = ' ';
    int i = 0;
    while(c != '\n' || i < length)
    {
        c = gps_read();
        str[i++] = c;
    }
    return i;
}

void gps_readMessage(char *str){
    char c = ' ';
    int i = 0;
    while(gps_read() != '$');
    while(c != '\n' || i < NMEA_MAX_MESSAGE_LENGTH)
    {
        c = gps_read();
        str[i++] = c;
    }
}

void gps_write(unsigned char ch){
    usart_putc(GPS_USART, ch);
}

uint8 gps_hasFix(void){
    char str[5];
    while(gps_read() != '$' && gps_readString(str, 5) && strcmp(str, "GPRMC") == 0);
    //Check for new GPS line, then check for GPRMC, then check for valid fix flag, if valid true, if invalid false
    int i;
    for(i=0; i < 11; i++){
        gps_read();
    }
    char flag = gps_read();
    if(flag == 'V')
        return 0;
    else
        return 1;
    //TODO: Do I want to read the rest of the message to clear it from buffer?
}




