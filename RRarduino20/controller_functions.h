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

struct button_state {
  bool pressed = false;
  bool prev_pressed = false;
  bool just_pressed = false;
  bool just_lifted = false;
};

button_state states[NUM_BTNS];

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

void update_button_state(button_state *state, char id) {
  state->pressed = get_button_value(id);
  state->just_pressed = !state->prev_pressed && state->pressed;
  state->just_lifted = state->prev_pressed && !state->pressed;
  state->prev_pressed = state->pressed;
}
