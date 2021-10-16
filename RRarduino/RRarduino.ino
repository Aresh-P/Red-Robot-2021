#include "controller_functions.h"

void setup() {
  Serial.begin(19200);
  init_motors();
}

void loop() {
  update_button_states();
  update_drive();

  float v1, v2;

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
    drive_from_joystick(&v1, &v2);
  }

  if (motor_forward) {
    set_sides(v1 * max_speed, v2 * max_speed);
  } else {
    set_sides(-v2 * max_speed, -v1 * max_speed);
  }
  delay(30); // IMPORTANT: Don't overload the ESP Chip with too many requests!
}
