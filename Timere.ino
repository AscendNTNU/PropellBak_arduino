void timer_setup() {

//Input fra Servo
/////////////////////////////////////////////////////////////////////////////////////////////

  Timer3.attachCompare1Interrupt(Input_handler);
  TIMER3_BASE->CR1 = TIMER_CR1_CEN;                                                                       //timer enable
  TIMER3_BASE->CR2 = 0;
  TIMER3_BASE->SMCR = 0;
  TIMER3_BASE->DIER = TIMER_DIER_CC1IE;                                                                   //That the capture can trigger an interrupt                                                                    
  TIMER3_BASE->EGR = 0;
  TIMER3_BASE->CCMR1 = TIMER_CCMR1_CC1S_INPUT_TI1;                                                        //A6 is serial input. Detects the rising edge
  TIMER3_BASE->CCMR2 = 0;
  TIMER3_BASE->CCER = TIMER_CCER_CC1E;
  
  TIMER3_BASE->PSC = 71;                                                                                  //Defines the speed of the timer: 72MHz/72 = 1MHz
  TIMER3_BASE->ARR = 0xFFFF;                                                                              //The value of when the timer resets
  TIMER3_BASE->DCR = 0;

//Output til ESC
/////////////////////////////////////////////////////////////////////////////////////////////

  TIMER4_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
  TIMER4_BASE->CR2 = 0;
  TIMER4_BASE->SMCR = 0;
  TIMER4_BASE->DIER = 0;
  TIMER4_BASE->EGR = 0;
  TIMER4_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE | (0b110 << 12) | TIMER_CCMR1_OC2PE;
  TIMER4_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE | (0b110 << 12) | TIMER_CCMR2_OC4PE;
  TIMER4_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  
  TIMER4_BASE->PSC = 71;
  TIMER4_BASE->ARR = 5000;
  TIMER4_BASE->DCR = 0;
  
  //TIMER4_BASE->CCR1 = 1000;                                             //Output er høy til timeren kommer til <- antall mikrosekunder

                                                
  pinMode(PB6, PWM);                                                    //PB6 er output for esc
  pinMode(PB7, PWM);                                                    //PB7 er output for gimbal
  pinMode(PB8, PWM);                                                    //PB8 er output for slider
}
