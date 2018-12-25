#define SZ(x) sizeof(x)

#define SEND_BUFFER_SIZE SZ(time_t) + SZ(uint16_t) + SZ(xy_t) + SZ(xy_t) + SZ(theta_t)
uint8_t send_buffer[SEND_BUFFER_SIZE] = {0};

void usb_write(const time_t stamp, const uint16_t updates, const xy_t x, const xy_t y, const theta_t theta) {
  uint8_t* idx = send_buffer;

  memcpy(idx, &stamp, SZ(stamp));     idx += SZ(stamp);
  memcpy(idx, &updates, SZ(updates)); idx += SZ(updates);
  memcpy(idx, &x, SZ(x));             idx += SZ(x);
  memcpy(idx, &y, SZ(y));             idx += SZ(y);
  memcpy(idx, &theta, SZ(theta));     idx += SZ(theta);

  Serial.write(send_buffer, SEND_BUFFER_SIZE);
  Serial.send_now();
}

#define RECIEVE_BUFFER_SIZE max(max(SZ(int8_t), SZ(uint16_t)), max(SZ(xy_t), SZ(theta_t)))
#define IO_TIMEOUT period_micros / 2
uint8_t recieve_buffer[RECIEVE_BUFFER_SIZE] = {0};

#define left_multi_c    1 // + int8_t
#define right_multi_c   2 // + int8_t
#define period_micros_c 3 // + uint16_t
#define track_length_c  4 // + uint16_t
#define x_c             5 // + xy_t
#define y_c             6 // + xy_t
#define theta_c         7 // + theta_t

boolean usb_read() {
  if (Serial.available() < 1) return false;
  const int8_t code = Serial.read();

  boolean return_value = true;
  if (
    code == left_multi_c && usb_read(SZ(left_multi))
  ) memcpy(&left_multi, recieve_buffer, SZ(left_multi));
  else if (
    code == right_multi_c && usb_read(SZ(right_multi))
  ) memcpy(&right_multi, recieve_buffer, SZ(right_multi));
  else if (
    code == period_micros_c && usb_read(SZ(period_micros))
  ) memcpy(&period_micros, recieve_buffer, SZ(period_micros));
  else if (
    code == track_length_c && usb_read(SZ(track_length))
  ) memcpy(&track_length, recieve_buffer, SZ(track_length));
  else if (
    code == x_c && usb_read(SZ(x))
  ) memcpy(&x, recieve_buffer, SZ(x));
  else if (
    code == y_c && usb_read(SZ(y))
  ) memcpy(&y, recieve_buffer, SZ(y));
  else if (
    code == theta_c && usb_read(SZ(theta))
  ) memcpy(&theta, recieve_buffer, SZ(theta));

  else return_value = false;

  return return_value;
}

boolean usb_read(const uint8_t bytes) {
  const time_t loop_start = micros();

  while (Serial.available() < bytes)
    if (micros() - loop_start > IO_TIMEOUT)
      return false;

  for (int i = 0; i < bytes; i++)
    recieve_buffer[i] = Serial.read();

  return true;
}
