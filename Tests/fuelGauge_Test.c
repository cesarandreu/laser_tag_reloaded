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
#include "initialize.h"
#include "usb.h"

// Standard libmaple init() and main.
//
// The init() part makes sure your board gets set up correctly. It's
// best to leave that alone unless you know what you're doing. main()
// is the usual "call setup(), then loop() forever", but of course can
// be whatever you want.

__attribute__((constructor)) void premain() {
    init();
}

int main(){
  usb_start();

  gauge_start(32);
  
  while(1){

    char str[10];
    gauge_voltage(str, 10);
    usb_printString(str); // Print the battery percentage
    usb_printlnString(" %");
    gauge_percent(str, 10);
    usb_printString(str);
    usb_printlnString(" V");
    usb_println();
    delay_us(1000000);
  }
  return 0;
}
