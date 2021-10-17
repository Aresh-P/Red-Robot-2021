#include "controller_functions.h"
#include <Servo.h>

Servo lifter;

float lifter_pos = 180;

void setup() {
  Serial.begin(19200);
  init_motors();
  lifter.attach(9);
}

void loop() {
  update_button_states();
  update_drive();

  float v1, v2;

  if (button_up_2.pressed) {
    //v1 = approach_speed;
    //v2 = approach_speed;
    set_velocity(motor3, 200);
  } else if (button_down_2.pressed) {
    //v1 = -approach_speed;
    //v2 = -approach_speed;
    set_velocity(motor3, -200);
  } else {
    set_velocity(motor3, 0);
    if (button_up_3.pressed) {
      if (lifter_pos <= 172)
        lifter_pos = lifter_pos + 8;
    } else if (button_down_3.pressed) {
      if (lifter_pos >= 8)
        lifter_pos = lifter_pos - 8;
    } else {
      drive_from_joystick(&v1, &v2);
    }
  }
  lifter.write(lifter_pos);

  if (motor_forward) {
    set_sides(v1 * max_speed, v2 * max_speed);
  } else {
    set_sides(-v2 * max_speed, -v1 * max_speed);
  }

  delay(30); // IMPORTANT: Don't overload the ESP Chip with too many requests!
}
