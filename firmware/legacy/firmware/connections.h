/*

Connection management module for trance firmware

*/

void connection_setup() {
      // Choose one to begin listening for E1.31 data
    //if (e131.begin(E131_UNICAST))                               // Listen via Unicast
    if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))   // Listen via Multicast
        DEBUG_PRINTLN(F("Listening for data..."));
    else 
        DEBUG_PRINTLN(F("*** e131.begin failed ***"));
}

void connection_handle() {
  if (!e131.isEmpty()) {
    e131_packet_t packet;
    e131.pull(&packet);     // Pull packet from ring buffer

    int values[INTERFACE_CHANNELS];
    for (int i = 0; i < INTERFACE_CHANNELS; i++) {
      values[i] = packet.property_values[ADDRESS + i];
    }
    interface_call(values);

    DEBUG_PRINTLN(packet.property_values[ADDRESS]);
  }
}