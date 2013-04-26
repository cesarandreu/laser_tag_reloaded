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

#include "usb.h"
#include <libmaple/delay.h>
#include <libmaple/nvic.h>

#define GPS_BAUD 9600
#define GPS_USART USART1
#define GPS_PORT_TX GPIOA
#define GPS_PIN_TX 9
#define GPS_PORT_RX GPIOA
#define GPS_PIN_RX 10
#define GPS_TIMER TIMER2
#define GPS_CHANNEL TIMER_CH1

//GPS VDD Information
#define GPS_VDD_PORT GPIOA
#define GPS_VDD_PIN 8

//GPS Coordinate Message Length (Including Null-Termination Character)
#define GPS_COORDINATE_LENGTH 25

#define NMEA_MAX_MESSAGE_LENGTH 82

void gps_disable(void){
    //usart_disable(GPS_USART);
    gpio_write_bit(GPS_VDD_PORT, GPS_VDD_PIN, 0);
}

void gps_enable(void){
    //usart_enable(GPS_USART);
    gpio_write_bit(GPS_VDD_PORT, GPS_VDD_PIN, 1);
}

void gps_end(void){
    timer_disable(GPS_TIMER);
    usart_disable(GPS_USART);
    gpio_write_bit(GPS_VDD_PORT, GPS_VDD_PIN, 0);
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
    while(c != '\n' && i < length-1)
    {
        c = gps_read();
        str[i++] = c;
    }
    return i;
}

//Return 0 if string is invalid size
//Return 1 if valid string is sent
//Maximum message string length is of size 83 including the null termination character
uint8 gps_readMessage(char *str, int length){
    timer_pause(GPS_TIMER);
    if(length < NMEA_MAX_MESSAGE_LENGTH+1)
    {
        timer_resume(GPS_TIMER);
        return 0;
    }
    char c = ' ';
    int i = 0;
    while(gps_read() != '$');
    while(c != '\n' && i < NMEA_MAX_MESSAGE_LENGTH-1)
    {
        c = gps_read();
        str[i++] = c;
    }
    timer_resume(GPS_TIMER);
    return 1;
}

void gps_write(unsigned char ch){
    usart_putc(GPS_USART, ch);
}

//Get GPS coordinates
//Returns 0 if location was not found or if the incoming string's length is of invalid size (Should I differentiate these?)
//Returns 1 if location was found
//The resulting string will be formatted as xxmm.dddd,<N|S>,yyymm.dddd,<E|W>
//The resulting string size will contain 24 characters.
int gps_getLocation(char *str, int length){
    //If GPS buffer starts with $, then check that it has 1+5+15+24 characters
    timer_pause(GPS_TIMER);
    gps_enable();

    //If the string's length is not 24 (+1 for string end character), return 0
    if(length != 24+1)
    {
        timer_resume(GPS_TIMER);
        return 0;
    }
    //gps_start();

    //Buffer clearing
    usart_reset_rx(GPS_USART);
    while(!gps_available());
    //gpio_write_bit(GPIOB, 1, 1);

    //Wait for the GPS to get a fix if available, if it does not after x time, return 0
    int waitCount = 0;
    while(gps_hasFix() == 0 && waitCount < 20)
    {
        waitCount++;
    }
    if(waitCount == 20)
    {
        strcpy(str, "No Coordinates Available\0");
        timer_resume(GPS_TIMER);
        gps_disable();
        return 0;
    }

    //Find the string that contains the coordinates from the GPS output
    char tmp_str[25] = "                        ";
    while(1)
    {
        if(gps_read() == '$')
        {
            gps_readString(tmp_str, 6);
            if(strcmp(tmp_str, "GPRMC                   ") == 0)
            {
                break;
            }
        }
    }

    //Scan the string until the beginning of the coordinates portion
    int i;
    for(i=0; i < 14; i++)
    {
        gps_read();
    }

    //Read the coordinates, store the in str and return 1
    gps_readString(tmp_str, 25);
    //gps_end();
    strcpy(str, tmp_str);
    timer_resume(GPS_TIMER);
    gpio_write_bit(GPIOB, 1, 1);
    gps_disable();
    return 1;
}

uint8 gps_hasFix(void){
    //Buffer clearing
    usart_reset_rx(GPS_USART);
    while(!gps_available());
    
    char str[6] = "     ";
    int waitCount = 0;
    while(waitCount < 20)
    {
        if(gps_read() == '$')
        {
            gps_readString(str, 6);
            if(strcmp(str, "GPRMC") == 0)
            {
                break;
            }
        }
        waitCount++;
    }

    if(waitCount == 20)
        return 0;

    //Check for new GPS line, then check for GPRMC, then check for valid fix flag, if valid true, if invalid false
    int i;
    for(i=0; i < 12; i++)
    {
        gps_read();
    }
    char flag = gps_read();
    if(flag == 'V')
    {
        return 0;
    }
    else
    {
        return 1;
    }
    gpio_write_bit(GPIOB, 1, 1);
}


void gps_warmRefresh(void){
    //int waitCount = 0;
    //Check for fix 20 times or until fix is found, whichever comes first
    /*while((gps_hasFix() == 0 || gps_hasFix() == 2) && waitCount < 20)
    {
        waitCount++;
    }*/
    //gps_disable();
}

//int value = 0; //FOR TESTING

void gps_coldRefresh(void){

    timer_pause(GPS_TIMER);
    //gpio_write_bit(GPIOB, 1, 1);
    //int waitCount = 0;
    //Check for fix 20 times or until fix is found, whichever comes first
    /*while((gps_hasFix() == 0 || gps_hasFix() == 2) && waitCount < 20)
    {
        waitCount++;
    }*/

    /*if(value == 0)
        value = 1;
    else
        value = 0;
    gpio_write_bit(GPIOB, 1, value);*/
    //delay_us(100000);

    /*if(gps_hasFix() == 1)
    {
        timer_set_compare(GPS_TIMER, GPS_CHANNEL, 58800);
        timer_set_reload(GPS_TIMER, 58800); //Set to 100ms
        timer_set_count(GPS_TIMER, 0);
        timer_detach_interrupt(GPS_TIMER, GPS_CHANNEL);
        timer_attach_interrupt(GPS_TIMER, GPS_CHANNEL, gps_warmRefresh);
        timer_generate_update(GPS_TIMER);
        gps_disable();
    }*/
    //timer_set_count(GPS_TIMER, 0);
    timer_resume(GPS_TIMER);
}

void gps_start(void){
    //TEST
    //timer_set_mode(TIMER1, TIMER_CH2, TIMER_DISABLED);
    //timer_set_mode(TIMER1, TIMER_CH3, TIMER_DISABLED);

    usart_config_gpios_async(GPS_USART, GPS_PORT_RX, GPS_PIN_RX, GPS_PORT_TX, GPS_PIN_TX, 0);
    usart_init(GPS_USART);
    //nvic_irq_set_priority(GPS_USART->irq_num, 0);
    usart_set_baud_rate(GPS_USART, USART_USE_PCLK, GPS_BAUD);

    //Enable BJT to turn on GPS
    //TODO: Confirm this isn't affected by being on UART CLK Pin (Currently unused)
    gpio_set_mode(GPS_VDD_PORT, GPS_VDD_PIN, GPIO_OUTPUT_PP);
    gpio_write_bit(GPS_VDD_PORT, GPS_VDD_PIN, 1);
    
    //Enable GPS USART Port
    usart_enable(GPS_USART);
    usart_putstr(GPS_USART, "$PMTK313,1*2E\r\n");
    usart_putstr(GPS_USART, "$PMTK301,2*2E\r\n");

    //Test LED
    gpio_set_mode(GPIOB, 1, GPIO_OUTPUT_PP);
    gpio_write_bit(GPIOB, 1, 0);

    //Enable timer and begin 30 second wait for fix
    /*timer_init(GPS_TIMER);
    timer_pause(GPS_TIMER);
    timer_set_reload(GPS_TIMER, 58800);
    //timer_set_reload(GPS_TIMER, 5880); //Set to 1ms
    timer_set_prescaler(GPS_TIMER, 120); //Setting prescaler to 120 (600KHz)
    //timer_set_compare(GPS_TIMER, GPS_CHANNEL, 588);
    timer_set_compare(GPS_TIMER, GPS_CHANNEL, 58800);
    timer_set_mode(GPS_TIMER, GPS_CHANNEL, TIMER_OUTPUT_COMPARE);
    timer_attach_interrupt(GPS_TIMER, GPS_CHANNEL, gps_coldRefresh);
    timer_generate_update(GPS_TIMER);
    timer_resume(GPS_TIMER);*/
}