ticks_t last_left_ticks = 0, last_right_ticks = 0;

void update_position() {
  const ticks_t left_ticks_copy = left_ticks;
  const ticks_t right_ticks_copy = right_ticks;

  const delta_ticks_t dsl = left_ticks_copy - last_left_ticks;
  const delta_ticks_t dsr = right_ticks_copy - last_right_ticks;

  const xy_t s = (xy_t)(dsl + dsr) / 2;
  x += s * lookup_cos(theta);
  y += s * lookup_sin(theta);

  theta += (theta_t)(dsl - dsr) / track_length;
  while(theta > TWO_PI) theta -= TWO_PI;
  while(theta < 0)      theta += TWO_PI;

  last_left_ticks = left_ticks_copy;
  last_right_ticks = right_ticks_copy;
}
