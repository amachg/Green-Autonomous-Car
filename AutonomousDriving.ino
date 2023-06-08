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
  
  // Self driving mode
  accelerate(forward, positive);
}

void loop() {
  // Obstacle avoidance maneuver at 40cm, checking every .5 sec.
  if (const auto delta_dist = get_obstacle_delta(40, 500)) {
    coast();
    accelerate(backward, positive);   // backup
    coast();
    // For positive angle turn left, for negative angle turn right.
    const auto angle = calc_attack_angle(delta_dist); // PROBLEM if angle is ZERO
    turn(angle);
    coast();
    accelerate(forward, positive);
  }
  
}
