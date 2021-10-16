#include <AFMotor.h>

// Constants
const float s = 0.75;
const int max_speed = 255;
const float approach_speed = .5;

// API interaction

void get_joystick_values(float *joystick_x, float *joystick_y) {
  Serial.println('j');
  String jX = Serial.readStringUntil(',');
  String jY = Serial.readStringUntil('\n');

  *joystick_x = jX.toFloat();
  *joystick_y = jY.toFloat();
}

void get_joystick2_values(float *joystick_x, float *joystick_y) {
  Serial.println('k');
  String jX = Serial.readStringUntil(',');
  String jY = Serial.readStringUntil('\n');

  *joystick_x = jX.toFloat();
  *joystick_y = jY.toFloat();
}

bool get_button_value(char button) {
  Serial.println(button);
  String val = Serial.readStringUntil('\n');
  return val.charAt(0) == '1' ? true : false;
}

// Buttons

#define BTN_UP_1 'a'
#define BTN_DOWN_1 'b'
#define BTN_UP_2 'c'
#define BTN_DOWN_2 'd'
#define BTN_UP_3 'e'
#define BTN_DOWN_3 'f'

#define BTN_1 '1'
#define BTN_2 '2'
#define BTN_3 '3'
#define BTN_4 '4'
#define BTN_5 '5'
#define BTN_6 '6'
#define BTN_7 '7'
#define BTN_8 '8'
#define BTN_9 '9'

#define PRESSED 1
#define RELEASED 0

struct button_state {
  bool pressed = false;
  bool prev_pressed = false;
  bool just_pressed = false;
  bool just_lifted = false;
};

button_state button_up_1;
button_state button_down_1;
button_state button_up_2;
button_state button_down_2;
button_state button_up_3;
button_state button_down_3;
button_state button_1;
button_state button_2;
button_state button_3;
button_state button_4;
button_state button_5;
button_state button_6;
button_state button_7;
button_state button_8;
button_state button_9;

void update_button_state(button_state *state, char id) {
  state->pressed = get_button_value(id);
  state->just_pressed = !state->prev_pressed && state->pressed;
  state->just_lifted = state->prev_pressed && !state->pressed;
  state->prev_pressed = state->pressed;
}

void update_button_states() {
  update_button_state(&button_up_1, BTN_UP_1);
  update_button_state(&button_down_1, BTN_DOWN_1);
  update_button_state(&button_up_2, BTN_UP_2);
  update_button_state(&button_down_2, BTN_DOWN_2);
  update_button_state(&button_up_3, BTN_UP_3);
  update_button_state(&button_down_3, BTN_DOWN_3);
  update_button_state(&button_1, BTN_1);
  update_button_state(&button_2, BTN_2);
  update_button_state(&button_3, BTN_3);
  update_button_state(&button_4, BTN_4);
  update_button_state(&button_5, BTN_5);
  update_button_state(&button_6, BTN_6);
  update_button_state(&button_7, BTN_7);
  update_button_state(&button_8, BTN_8);
  update_button_state(&button_9, BTN_9);
}

// Math

float joystick_to_expo(float joystick) {
  float joystick_cubed = joystick * joystick * joystick;
  return joystick_cubed * s + joystick * (1 - s);
}

float stick_to_slider(float a, float b) {
  if (a == 0 && b == 0)
    return 0;
  float abs_a = abs(a), abs_b = abs(b);
  float mx = max(abs_a, abs_b), mn = min(abs_a, abs_b);
  float scaleCorrect = sqrt(1 + (mx * mx) / (mn * mn));
  return a * scaleCorrect;
}

// Driving
bool motor_forward = true;
bool two_joystick_mode = false;
float joystickX, joystickY;
float joystickX2, joystickY2;

void update_drive() {
  get_joystick_values(&joystickX, &joystickY);
  get_joystick2_values(&joystickX2, &joystickY2);
  if (button_up_1.just_pressed) {
    motor_forward = !motor_forward;
  }
  if (
    button_1.pressed || button_2.pressed || button_3.pressed ||
    button_4.pressed || button_5.pressed || button_6.pressed ||
    button_7.pressed || button_8.pressed || button_9.pressed) {
    two_joystick_mode = false;
  }
  if (joystickX2 != 0 || joystickY2 != 0)
    two_joystick_mode = true;
}

void drive_from_joystick(float *v1, float *v2) {
  float stickY = stick_to_slider(joystickY, joystickX);
  float stickX;
  if (two_joystick_mode) {
    stickX = stick_to_slider(joystickX2, joystickY2);
  } else {
    stickX = stick_to_slider(joystickX, joystickY);
  }

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

  *v1 = vp1 * scaleCorrect;
  *v2 = vp2 * scaleCorrect;
}

// Motors

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void init_motors() {
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
