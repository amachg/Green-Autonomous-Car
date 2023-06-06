/* Οδήγηση με 2 Οδηγούς DRV8871 και 4 κινητήρες και τηλεχειρισμό PS3
  
  Arduino Mega ADK Rev3: MAX3421E USB host controller communicate via SPI bus,
  using pins: D7 (RST), 50 (MISO), 51 (MOSI), 52 (SCK).
  Pin D7 cannot be used at all. Other pins can be used only for SPI.
*/

#include "OLED.h"
#include "PS3_controller.h"
#include "MotorLib.h"
#include "Proximity_US.h"

void setup() {
  Serial.begin(9600);
  Serial.println("\nSerial OK");

  setup_OLED();
  setup_motors();
  setup_ultrasonics();
  setup_RC();
}

void loop() {
  // Manual driving mode
  auto throttle = read_RC();
  update_motors(throttle);
}
