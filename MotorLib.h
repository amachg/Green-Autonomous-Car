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

void step_on_breaks() { // φρέναρε
  digitalWrite(pin_left_in1, HIGH);
  digitalWrite(pin_left_in2, HIGH);
  digitalWrite(pin_right_in1, HIGH);
  digitalWrite(pin_right_in2, HIGH);
}

void update_motors(const Throttle throttle) {  
  // left side motors
	
  // on forwarding turn CCW
  if (throttle.left >= dzl) {
    analogWrite(pin_left_in1, throttle.left);
    digitalWrite(pin_left_in2, LOW);
  }
  // on backwarding turn CW
  else if (throttle.left <= -dzl) { 
    digitalWrite(pin_left_in1, LOW);
    analogWrite(pin_left_in2, -throttle.left);
  }
  else { // coast this side
    digitalWrite(pin_left_in1, LOW);
    digitalWrite(pin_left_in2, LOW);
  }
  
  // right side motors
	
  // on forwarding turn CW
  if (throttle.right >= dzl) {
    digitalWrite(pin_right_in1, LOW);
    analogWrite(pin_right_in2, throttle.right);
  }
  // on backwarding turn CCW
  else if (throttle.right <= -dzl) { 
    analogWrite(pin_right_in1, -throttle.right);
    digitalWrite(pin_right_in2, LOW);
  }
  else { // coast this side
    digitalWrite(pin_right_in1, LOW);
    digitalWrite(pin_right_in2, LOW);
  }
}
