#define led_pin 11
void enter_error_state() {
  digitalWrite(led_pin, HIGH);
}
void exit_error_state() {
  digitalWrite(led_pin, LOW);
}

#include "fxpt.h"
#define theta_t fxpt_t
#define xy_t fxpt_t

#define ticks_t int64_t
#define delta_ticks_t int16_t
#define time_t uint64_t

uint16_t period_micros = 10 * 1000;

int8_t left_multi = 1, right_multi = 1;
uint16_t track_length = 1000;

xy_t x = 0, y = 0;
theta_t theta = 0;

void setup() { // called by arduino main
  pinMode(led_pin, OUTPUT);
  enter_error_state();
  Serial.begin(115200);
  while (!usb_read());
  encoder_setup();
  exit_error_state();
}

uint16_t updates = 0;
void loop() { // called by arduino main
  const time_t loop_start = micros();

  usb_read();
  usb_write(loop_start, updates, x, y, theta);

  updates = 0;
  while (micros() - loop_start < period_micros) {
    updates++;
    update_position();
  }
}
