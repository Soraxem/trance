/*

Hardware definition for Trance firmware

*/

#define NETWORK "WIFI"

#define INTERFACE_CHANNELS 3

void interface_setup() {
  ledcAttachChannel(1, 5000, 8, 1);
  ledcAttachChannel(3, 5000, 8, 2);
  ledcAttachChannel(4, 5000, 8, 3);

  delay(100);
  ledcWrite(3,  0);
  ledcWrite(4,  0);
  ledcWrite(1,  0);
}

void interface_handle() {
  // there is nothing to handle
}

void interface_call(int channels[INTERFACE_CHANNELS]) {
  ledcWrite(3,  channels[0]);
  ledcWrite(4,  channels[1]);
  ledcWrite(1,  channels[2]);
}
