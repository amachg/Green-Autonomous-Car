/*
  Distance-metering Ultrasonic Sensors,
  need digital pins, or analog pins (as digital) using the aliases Ax.
*/

#define left_trigger_pin  17 // Ultrasonic Sensor HY-SRF05
#define left_echo_pin     18

#define right_trigger_pin 14 // Ultrasonic Sensor HC-SR04
#define right_echo_pin    15
  
void setup_ultrasonics() {
  pinMode(left_trigger_pin, OUTPUT);
  pinMode(left_echo_pin, INPUT);
  pinMode(right_trigger_pin, OUTPUT);
  pinMode(right_echo_pin, INPUT);
  println("Sonics OK");
}

#include <limits.h>

int get_distance_cm(const byte trigger_pin, const byte echo_pin) {
  // Send a pulse
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, LOW);
  
  // Time to wait for the pulse to complete, defaults to 1.000.000us (1 sec)
  // Minimum is 30000us, which can measure distances up to 3 meters.
  static constexpr unsigned long timeout_us = 30000;
  const unsigned long pulse_duration_us = pulseIn(echo_pin, HIGH, timeout_us);
  
  // If echo pulse is undetected or timed-out, pulseIn returns 0, meaning very long distance.
  // Return this as the largest possible integer number.
  if(pulse_duration_us == 0) 
    return INT_MAX;
  
  const auto on_way_time_us = pulse_duration_us / 2;
  // The speed of sound: 343,21 m/s = 343210 mm/s = 0,34321 mm/Î¼s
  const auto distance_mm = on_way_time_us * 0.34321;
  return distance_mm / 10;
}

unsigned long last_moment = millis();

int get_obstacle_delta(const int safety_limit_cm, const unsigned period_ms) {
  // Check every period_ms. Hardware minimum is 29 ms.
  if (const auto now = millis(); now - last_moment >= period_ms ) {
    last_moment = now;
    
    // Fire the two sensors > 65ms apart to avoid interference 
    const int left_dist_cm = get_distance_cm(left_trigger_pin, left_echo_pin);
    delay(70);
    const int right_dist_cm = get_distance_cm(right_trigger_pin, right_echo_pin);

    const int least_dist = min(left_dist_cm, right_dist_cm);
    const int delta = left_dist_cm - right_dist_cm;
  
    if (least_dist < safety_limit_cm) {
      println((String)"LR" + left_dist_cm  + "-" + right_dist_cm + "= " + delta);
      return delta;
    }
  }
  return false; // that is 0
}

constexpr float inter_sensor_dist = 15.5; // in cm 
  
int calc_attack_angle(const int delta_dist) {
  // Depending on delta, calc car angle of approach to the (wall) obstacle
  const int angle = atan(inter_sensor_dist / delta_dist) * 180 / PI;
  return angle;
}
