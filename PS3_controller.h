/*
  Connect a game console controller to usb host of Arduino ADK
  using USB_Host_Shield_2.0 library (included in Library Manager)
*/

#include <PS3BT.h>
#include <usbhub.h>
//#include <SPI.h> //already included in OLED.h

USB Usb;
BTD Btd(&Usb); 
PS3BT PS3(&Btd); 

void setup_RC() {
  if (Usb.Init() == -1) {
    println(F("USB host did not start"));
    while (1); // Halt
  }
  println(F("PS3RC OK\n"));
}

struct Throttle { 
  int left, right;
};

constexpr int dzl = 100; // dead zone limit

Throttle read_RC() { // both joysticks only Y, in the range of 0(up) to 255(doown).
  Usb.Task();
  
  if (PS3.PS3Connected) {
    Throttle throttle;
    
    // read the analog joysticks values and covert them to the range 255(up) to -255(down)
    const auto left = PS3.getAnalogHat(LeftHatY);       
    throttle.left = map(left, 0, 255, 255, -255);
    
    const auto right = PS3.getAnalogHat(RightHatY); 
    throttle.right = map(right, 0, 255, 255, -255);
    
    return throttle;
  }
  
  return {0, 0};
}
