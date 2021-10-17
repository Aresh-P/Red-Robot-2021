#include "controller_functions.h"

#include <Servo.h>

Servo lifter1;
Servo lifter2;

float lifter_pos = 180;

void setup() {
  Serial.begin(19200);
  init_motors();
  lifter1.attach(9);
  lifter2.attach(10);
}

void loop() {
  update_button_states();
  update_drive();

  float v1, v2;

  if (button_up_2.pressed) {
    //v1 = approach_speed;
    //v2 = approach_speed;
    if (lifter_pos >= 8)
      lifter_pos = lifter_pos - 8;
  } else if (button_down_2.pressed) {
    //v1 = -approach_speed;
    //v2 = -approach_speed;
    if (lifter_pos <= 172)
      lifter_pos = lifter_pos + 8;
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

  lifter1.write(180 - lifter_pos);
  lifter2.write(lifter_pos);

  delay(30); // IMPORTANT: Don't overload the ESP Chip with too many requests!
}
