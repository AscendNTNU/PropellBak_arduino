
void Input_handler() {

  if (rising_edge_set) {                                    //If the receiver channel 1 input pulse on A0 is high.
    measured_time_start = TIMER3_BASE->CCR1;
    
    TIMER3_BASE->CCER |= TIMER_CCER_CC1P;                   //Change the input capture mode to the falling edge of the pulse.
    rising_edge_set = 0;
  } 

  
  else {

//If the timer is reset (after 65536 cycles it will go back to 0). Usually measured_time will be e.g. 5000 - 3500 = 1500, but in the edge
//case where the timer i reset, the value will be e.g. 1000 - 65035 = -64035 instead of 1500. So then we just add 65535 to that value and we get 1500

    measured_time = TIMER3_BASE->CCR1 - measured_time_start;  //Calculates the time the signal was high. (The pwm signal)
    if (measured_time < 0)measured_time += 0xFFFF;   

    TIMER3_BASE->CCER &= ~TIMER_CCER_CC1P;                  //Change the input capture mode to the rising edge of the pulse.
    rising_edge_set = 1;
  }
}
