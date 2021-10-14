#include <AFMotor.h>
#include "controller_functions.h"

//comment
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

float s = 0.5;

void setup() {
  Serial.begin(19200);
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor3.setSpeed(0);
  motor4.setSpeed(0);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void joystickToExpo(float joystick){
  float joystickCubed = joystick*joystick*joystick;
  return joystickCubed*s+joystick*(1-s);
}

void setVelocity(AF_DCMotor motor, float v){
  motor.setSpeed(abs(v));
  motor.run(v >= 0 ? FORWARD : BACKWARD);
}

void loop() {
  float joystickX, joystickY;
  float joystickX2, joystickY2;
  get_joystick_values(&joystickX, &joystickY);
  get_joystick2_values(&joystickX2, &joystickY2);

  if (get_button_value(BTN_UP_2) == PRESSED) {
    joystickY = 1;
    joystickY2 = 1;
  } else if (get_button_value(BTN_DOWN_2) == PRESSED) {
    joystickY = -1;
    joystickY2 = -1;
  }

  float expo = joystickToExpo(joystickY);
  float expo2 = joystickToExpo(joystickY2);

  setVelocity(motor1, expo * 255);
  setVelocity(motor4, expo * 255);
  setVelocity(motor2, expo2 * 255);
  setVelocity(motor3, expo2 * 255);

  // motor1.setSpeed(abs(joystickY * 255));
  // motor4.setSpeed(abs(joystickY * 255));
  // motor2.setSpeed(abs(joystickY2 * 255));
  // motor3.setSpeed(abs(joystickY2 * 255));

  // motor1.run(joystickY >= 0 ? FORWARD : BACKWARD);
  // motor4.run(joystickY >= 0 ? FORWARD : BACKWARD);
  // motor2.run(joystickY2 >= 0 ? FORWARD : BACKWARD);
  // motor3.run(joystickY2 >= 0 ? FORWARD : BACKWARD);

  //  int speed = int(joystickY * 127);
  //  int bias = int(joystickX * 127);
  //  motor1.setSpeed(abs(speed + bias));
  //  motor4.setSpeed(abs(speed + bias));
  //  motor2.setSpeed(abs(speed - bias));
  //  motor3.setSpeed(abs(speed - bias));
  //
  //  motor1.run(speed + bias >= 0 ? FORWARD : BACKWARD);
  //  motor4.run(speed + bias >= 0 ? FORWARD : BACKWARD);
  //  motor2.run(speed - bias >= 0 ? FORWARD : BACKWARD);
  //  motor3.run(speed - bias >= 0 ? FORWARD : BACKWARD);

  delay(100); // IMPORTANT: Don't overload the ESP Chip with too many requests!
}
