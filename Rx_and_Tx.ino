
void handler_channel_1() {
  measured_time = TIMER2_BASE->CCR1 - measured_time_start;
  if (measured_time < 0) {
    measured_time += 0xFFFF;                                      //Brukes hvis det er negativ tid som kan skyldes at registeret har nådd enden
  }
  measured_time_start = TIMER2_BASE->CCR1;
  if (measured_time > 3000) {
    channel_select_counter = 0;
  }
  else channel_select_counter++;



  if (channel_select_counter == 10){
    channel_select_counter = 0;
  }

  Channels[channel_select_counter] = measured_time;
  
}
