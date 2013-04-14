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

#include "fuelGauge.h"

// Pin definitions
//TODO: Determine which pin will be used to for battery alert interrupt
//int alertPin = 2;  // This is the alert interrupt pin, connected to pin 2 on the LiPower Shield

// Global Variables
float batVoltage;
float batPercentage;
int alertStatus;

void gauge_start(uint8 percent)
{
  i2c_HardWireLEnable(1, 0);
  i2c_begin();  // Start I2C
  delay_us(100000);
  configMAX17043(percent);  // Configure the MAX17043's alert percentage
  qsMAX17043();  // restart fuel-gauge calculations

  //TODO: Set interrupt here and determine pin to connect gauge alert
  //FIXFIXFIXgpio_set_mode(GPIOA, 1, GPIO_INPUT_PP);
}

void gauge_alert(void)
{
  //TODO: Determine what to do when battery is low.
  //Turn off system by SW?
  //Attempt to send current data via BT and then turn off?
  //Implement some kind of temporary storage we can pull from?
}

/*
i2cWrite16(unsigned int data, unsigned char address) writes 16 bits
of data beginning at an 8-bit address, and continuing to the next.
*/
//TODO: Move this to HardWire or WireBase?
void i2cWrite16(unsigned int data, unsigned char address)
{
  i2c_beginTransmission((uint8)MAX17043_ADDRESS);
  i2c_send(address);
  i2c_send((byte)((data >> 8) & 0x00FF));
  i2c_send((byte)(data & 0x00FF));
  i2c_endTransmission();
}

/* 
i2cRead16(unsigned char address) reads a 16-bit value beginning
at the 8-bit address, and continuing to the next address. A 16-bit
value is returned.
*/
//TODO: Move this to HardWire or WireBase?
unsigned int i2cRead16(unsigned char address)
{
  int data = 0;
  
  i2c_beginTransmission(MAX17043_ADDRESS);
  i2c_send(address);
  i2c_endTransmission();
  
  i2c_requestFrom(MAX17043_ADDRESS, 2);
  while (i2c_available() < 2)
    ;
  data = ((int) i2c_receive()) << 8;
  data |= i2c_receive();
  
  return data;
}

/*
gauge_voltage() returns a 12-bit ADC reading of the battery voltage,
as reported by the MAX17043's VCELL register.
This does not return a voltage value. To convert this to a voltage,
multiply by 5 and divide by 4096.
*/
//TODO: Determine output string length to perform input length verification
void gauge_voltage(char *str, int length)
{
  unsigned int vcell;
  
  vcell = i2cRead16(0x02);
  vcell = vcell >> 4;  // last 4 bits of vcell are nothing
  batVoltage = (float) vcellMAX17043() * 1/800;

  sprintf(str, "%02f", (double)batVoltage);
  //return vcell;
}

/*
gauge_percent() returns a float value of the battery percentage
reported from the SOC register of the MAX17043.
*/
//TODO: Determine output string length to perform input length verification
void gauge_percent(char *str, int length)
{
  unsigned int soc;
  float percent;
  
  soc = i2cRead16(0x04);  // Read SOC register of MAX17043
  percent = (byte) (soc >> 8);  // High byte of SOC is percentage
  percent += ((float)((byte)soc))/256;  // Low byte is 1/256%
  
  sprintf(str, "%02f", (double)batPercentage);
  //return percent;
}

/* 
configMAX17043(byte percent) configures the config register of
the MAX170143, specifically the alert threshold therein. Pass a 
value between 1 and 32 to set the alert threshold to a value between
1 and 32%. Any other values will set the threshold to 32%.
*/
void configMAX17043(byte percent)
{
  if ((percent >= 32)||(percent == 0))  // Anything 32 or greater will set to 32%
    i2cWrite16(0x9700, 0x0C);
  else
  {
    byte percentBits = 32 - percent;
    i2cWrite16((0x9700 | percentBits), 0x0C);
  }
}

/* 
qsMAX17043() issues a quick-start command to the MAX17043.
A quick start allows the MAX17043 to restart fuel-gauge calculations
in the same manner as initial power-up of the IC. If an application's
power-up sequence is very noisy, such that excess error is introduced
into the IC's first guess of SOC, the MapleMini can issue a quick-start
to reduce the error.
*/
void qsMAX17043()
{
  i2cWrite16(0x4000, 0x06);  // Write a 0x4000 to the MODE register
}