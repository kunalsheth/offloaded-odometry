// encoder pins: 5, 6, 7, 8
#define encoder_pin_LA 5
#define encoder_pin_LB 6
#define encoder_pin_RA 7
#define encoder_pin_RB 8

void encoder_setup() {
  attachInterrupt(digitalPinToInterrupt(encoder_pin_LA), left_interrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder_pin_RA), right_interrupt, RISING);
}

volatile ticks_t left_ticks = 0;
void left_interrupt() {
  if (digitalRead(encoder_pin_LB)) left_ticks += left_multi;
  else                             left_ticks -= left_multi;
}

volatile ticks_t right_ticks = 0;
void right_interrupt() {
  if (digitalRead(encoder_pin_RB)) right_ticks += right_multi;
  else                             right_ticks -= right_multi;
}
