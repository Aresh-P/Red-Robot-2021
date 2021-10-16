#include <AFMotor.h>
#include "controller_functions.h"

//comment
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);


#include <Servo.h>

Servo lifter1;
Servo lifter2;

float lifter_pos = 180;

float s = 0.75;
int max_speed = 255;
float approach_speed = .5;
bool motor_forward = true;

button_state button_up_1;
button_state button_down_1;
button_state button_up_2;
button_state button_down_2;
button_state button_up_3;
button_state button_down_3;

float joystick_to_expo(float joystick) {
  float joystick_cubed = joystick * joystick * joystick;
  return joystick_cubed * s + joystick * (1 - s);
}

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

  lifter1.attach(9);
  lifter2.attach(10);
}

void set_velocity(AF_DCMotor motor, float v) {
  motor.setSpeed(min(abs(v), 255));
  motor.run(v >= 0 ? FORWARD : BACKWARD);
}

void set_sides(float vl, float vr) {
  set_velocity(motor1, vl);
  set_velocity(motor4, vl);
  set_velocity(motor2, vr);
  set_velocity(motor3, vr);
}

float stickToSlider(float a, float b) {
  return a / sqrt(1 - min(b * b, 0.5));
}

void loop() {
  float joystickX, joystickY;
  float joystickX2, joystickY2;
  get_joystick_values(&joystickX, &joystickY);
  get_joystick2_values(&joystickX2, &joystickY2);
  update_button_state(&button_up_1, BTN_UP_1);
  update_button_state(&button_down_1, BTN_DOWN_1);
  update_button_state(&button_up_2, BTN_UP_2);
  update_button_state(&button_down_2, BTN_DOWN_2);
  update_button_state(&button_up_3, BTN_UP_3);
  update_button_state(&button_down_3, BTN_DOWN_3);

  lifter1.write(180-lifter_pos);
  lifter2.write(lifter_pos);
 

  float v1, v2;

  if (button_up_1.just_pressed) {
    motor_forward = !motor_forward;
  }

  if (button_up_2.pressed) {
    //v1 = approach_speed;
    //v2 = approach_speed;
    lifter_pos = lifter_pos - 8;
  } else if (button_down_2.pressed) {
    lifter_pos = lifter_pos + 8;
    //v1 = -approach_speed;
    //v2 = -approach_speed;
  } else if (button_down_1.pressed) {
    v1 = -approach_speed;
    v2 = approach_speed;
  } else if (button_down_3.pressed) {
    v1 = approach_speed;
    v2 = -approach_speed;
  } else {

    float stickY = stickToSlider(joystickY, joystickX);
    float stickX = stickToSlider(joystickX2, joystickY2);

    float expo = stickY;
    float expo2 = joystick_to_expo(stickX);

    float vp1 = expo + expo2;
    float vp2 = expo - expo2;

    float avp1 = abs(vp1);
    float avp2 = abs(vp2);

    float scaleCorrect;

    if (avp1 <= 1 && avp2 <= 1) {
      scaleCorrect = 1;
    }
    else {
      scaleCorrect = 1 / max(avp1, avp2);
    }

    v1 = vp1 * scaleCorrect;
    v2 = vp2 * scaleCorrect;
  }

  if (motor_forward) {
    set_sides(v1 * max_speed, v2 * max_speed);
  } else {
    set_sides(-v2 * max_speed, -v1 * max_speed);
  }
  delay(30); // IMPORTANT: Don't overload the ESP Chip with too many requests!
}
