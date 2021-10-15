#define NUM_BTNS 15

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

struct buttonState {
  bool pressed = false;
  bool prevPressed = false;
  bool justPressed = false;
  bool justLifted = false;
};

buttonState states[NUM_BTNS];

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

void update_button_state(buttonState *state, char id) {
  state->pressed = get_button_value(id);
  state->justPressed = !state->prevPressed && state->pressed;
  state->justLifted = state->prevPressed && !state->pressed;
  state->prevPressed = state->pressed;
}
