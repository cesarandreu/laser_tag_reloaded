/* LiPower Shield Example Code
  by: Jim Lindblom
  SparkFun Electronics
  date: 1/3/12
  
  license: Creative Commons Attribution-ShareAlike 3.0 (CC BY-SA 3.0)
  Do whatever you'd like with this code, use it for any purpose.
  Please attribute and keep this license. And let me know if you
  make any major, awesome changes!
  
  This is example code for the MAX17043G chip on the LiPower Shield.
  The MAX17043G+U is a compact, low-cost 1S LiPo fuel gauge.
  The MapleMini talks with the MAX17043 over an I2C (two-wire) interface,
  so we'll use the HardWireL.h library to talk with it.
  
  It's a silly example. It reads the battery voltage, and its percentage
  full and prints it out over serial. You probably wouldn't care about
  the battery voltage if you had the MapleMini connected via USB. But this
  code does show you how to configure the MAX17043G, and how to read and
  manipulate the voltage values.
*/

#ifndef _FUELGAUGE_H_
#define _FUELGAUGE_H_

/* Conditional extern "C" so we're safe to call from C++ files */
#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/gpio.h>
#include <libmaple/delay.h>
#include <libmaple/libmaple.h>
#include <stdio.h>

#include "HardWireL.h"
#include "WireBaseL.h"

#define MAX17043_ADDRESS 0x36  // R/W =~ 0x6D/0x6C

void gauge_start(uint8 percent);
void gauge_alert(void);
void gauge_voltage(char *str, int length);
void gauge_percent(char *str, int length);

#ifdef __cplusplus
}
#endif

#endif 
