/*
  My Motor Library
  Drivers: x2 DRV8871
  Motors: x4 XD-37GB520 (DFRobot FIT0492-B), Metal Geared, 12VDC/5W, 100RPM, 12kg.cm
*/

#define pin_left_in1 5    // left side motors
#define pin_left_in2 6
#define pin_right_in1 10  // right side motors
#define pin_right_in2 11 

void step_on_breaks();

void setup_motors() {
  // Turns pins to OUTPUT mode (after boot they are set in INPUT)
  pinMode(pin_left_in1, OUTPUT);
  pinMode(pin_left_in2, OUTPUT);
  pinMode(pin_right_in1, OUTPUT);
  pinMode(pin_right_in2, OUTPUT);
  step_on_breaks();
  println("Motors OK");
}

enum MoveDirection  { backward=0, forward=1 };
enum AccelerateSign { negative=0, positive=1 };

void accelerate(const MoveDirection dir, const AccelerateSign acc_sign) {  
  digitalWrite(dir ? pin_left_in2 : pin_left_in1, LOW);
  digitalWrite(dir ? pin_right_in1 : pin_right_in2, LOW);

  // Οι κινητήρες, αφόρτωτοι, ξεκινάνε με τιμή PWM ~95/255 και σταματάνε ~35/255
  constexpr byte low_start_PWM = 105,
                 high_PWM = 255,
                 low_stop_PWM = 0,
                 step_PWM = 5;
  const byte pin_left_PWM = dir ? pin_left_in1 : pin_left_in2;
  const byte pin_right_PWM = dir ? pin_right_in2 : pin_right_in1;
  const auto wait_step = 20;
  
  for (int i = acc_sign ? low_start_PWM : high_PWM; 
                acc_sign ? i <= high_PWM : i >= low_stop_PWM;
                acc_sign ? i += step_PWM : i -= step_PWM) {
    analogWrite(pin_left_PWM, i);
    analogWrite(pin_right_PWM, i);
		delay(wait_step);     // (255-105) / 5 x 20ms = 30 x 20ms = 600ms
  }
}

void turn(const int angle) { // αριστερόστροφη ή δεξιόστροφη στροφή
  const auto abs_angle = abs(angle);
  println(angle > 0 ? (String)"Turn.L: " : (String)"Turn.R: " + abs_angle);
  
  // Positive angle is a Left car spins, all motors CW
  // Negative angle is a Right car spin, all motors CCW
  digitalWrite(pin_left_in1, angle > 0 ? LOW : HIGH);
  digitalWrite(pin_left_in2, angle > 0 ? HIGH : LOW);
  digitalWrite(pin_right_in1, angle > 0 ? LOW : HIGH);
  digitalWrite(pin_right_in2, angle > 0 ? HIGH : LOW);
  
  const auto wait_step = 10 * abs_angle;  // experimental multiplier
	delay(wait_step);                       // e.g. 90°<->900ms
}

void coast() {
  // println("Coast");  // τσούλησε
  digitalWrite(pin_left_in1, LOW);
  digitalWrite(pin_left_in2, LOW);
  digitalWrite(pin_right_in1, LOW);
  digitalWrite(pin_right_in2, LOW);
  delay(500);
}
void step_on_breaks() {
  // println("Break");  // φρέναρε
  digitalWrite(pin_left_in1, HIGH);
  digitalWrite(pin_left_in2, HIGH);
  digitalWrite(pin_right_in1, HIGH);
  digitalWrite(pin_right_in2, HIGH);
}

// void full_speed_forward() {
//   println("forward full speed");
//   digitalWrite(pin_left_in1, HIGH); // CCW wheels turn
//   digitalWrite(pin_left_in2, LOW);
  
//   digitalWrite(pin_right_in1, LOW); // CW wheels turn
//   digitalWrite(pin_right_in2, HIGH);
// }
// void full_speed_backward() {
//   println("backward full speed");
//   digitalWrite(pin_left_in1, LOW); // CW wheels turn
//   digitalWrite(pin_left_in2, HIGH);
  
//   digitalWrite(pin_right_in1, HIGH); // CCW wheels turn
//   digitalWrite(pin_right_in2, LOW);
// }

void motor_tests() {
	println("Starting motor tests");
  static constexpr auto wait_between = 3000;

  println("Forward Acc-Dec ");
	accelerate(forward, positive); delay(wait_between);
	accelerate(forward, negative); delay(wait_between);
  
  println("Backward Acc-Dec ");
	accelerate(backward, positive); delay(wait_between);
	accelerate(backward, negative); delay(wait_between);
	
	turn(90); coast(); delay(wait_between);
	turn(-90); coast(); delay(wait_between);
	turn(45); coast(); delay(wait_between);
	turn(-45); coast(); delay(wait_between);

  println("finished tests\n");
}

// void update_motors(const Throttle throttle) {  
//   // left side motors
//   // on forwarding turn CCW
//   if (throttle.left >= dzl) {
//     analogWrite(pin_left_in1, throttle.left);
//     digitalWrite(pin_left_in2, LOW);
//   }
//   // on backwarding turn CW
//   else if (throttle.left <= -dzl) { 
//     digitalWrite(pin_left_in1, LOW);
//     analogWrite(pin_left_in2, -throttle.left);
//   }
//   else { // coast this side
//     digitalWrite(pin_left_in1, LOW);
//     digitalWrite(pin_left_in2, LOW);
//   }
  
//   // right side motors
//   // on forwarding turn CW
//   if (throttle.right >= dzl) {
//     digitalWrite(pin_right_in1, LOW);
//     analogWrite(pin_right_in2, throttle.right);
//   }
//   // on backwarding turn CCW
//   else if (throttle.right <= -dzl) { 
//     analogWrite(pin_right_in1, -throttle.right);
//     digitalWrite(pin_right_in2, LOW);
//   }
//   else { // coast this side
//     digitalWrite(pin_right_in1, LOW);
//     digitalWrite(pin_right_in2, LOW);
//   }
// }
