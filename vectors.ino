#include "fxpt.h"
ticks_t last_left_ticks = 0, last_right_ticks = 0;

void update_position() {
  const ticks_t left_ticks_copy = left_ticks;
  const ticks_t right_ticks_copy = right_ticks;

  const delta_ticks_t dsl = left_ticks_copy - last_left_ticks;
  const delta_ticks_t dsr = right_ticks_copy - last_right_ticks;

  const xy_t s = i_to_fx(dsl + dsr) / 2;
  x += fx_to_i(s * lookup_cos(theta));
  y += fx_to_i(s * lookup_sin(theta));

  theta += i_to_fx(dsl - dsr) / track_length;
  while(theta > fxpt_two_pi)  theta -= fxpt_two_pi;
  while(theta < fxpt_zero)    theta += fxpt_two_pi;

  last_left_ticks = left_ticks_copy;
  last_right_ticks = right_ticks_copy;
}
