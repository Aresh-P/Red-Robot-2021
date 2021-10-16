#include <AFMotor.h>
#include "controller_functions.h"

//comment
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

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

float stick_to_slider(float a, float b) {
  if (a == 0 && b == 0)
    return 0;
  float abs_a = abs(a), abs_b = abs(b);
  float mx = max(abs_a, abs_b), mn = min(abs_a, abs_b);
  float scaleCorrect = sqrt(1 + (mx * mx) / (mn * mn));
  return a * scaleCorrect;
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

  float v1, v2;

  if (button_up_1.just_pressed) {
    motor_forward = !motor_forward;
  }

  if (button_up_2.pressed) {
    v1 = approach_speed;
    v2 = approach_speed;
  } else if (button_down_2.pressed) {
    v1 = -approach_speed;
    v2 = -approach_speed;
  } else if (button_down_1.pressed) {
    v1 = -approach_speed;
    v2 = approach_speed;
  } else if (button_down_3.pressed) {
    v1 = approach_speed;
    v2 = -approach_speed;
  } else {

    float stickY = stick_to_slider(joystickY, joystickX);
    float stickX = stick_to_slider(joystickX2, joystickY2);

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
