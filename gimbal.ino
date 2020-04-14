

void feedback360()                           
{

  if (measured_time > 1160 || measured_time < 1040) return;
  
  feedback = measured_time/1100.0;                       //Control signal with duty cycle between 0.029(0 deg) - 0.971(359 deg) or 2.9 - 97.1%
  feedback = (feedback - 0.029)* 359/0.942;              //if feedback = 0.971, feedback - 0.029 = 0.942, which divided by 0.942 times 359 = 359 
                                                         //This formula transforms from duty_cycle to degrees between(0 - 359)
  
  uint8_t offset = 0;
  float error;

  error = feedback - target_angle;                       //Positive thrust is CCW
  
  if(target_angle < 0)  offset = 30;
  else if(target_angle > 0) offset = -30;
  
  if (slider_counter){
    if (slider_counter < 0){
      slider = 1700;
    }
    else {
      slider = 1300;  
    }
  }
    else{
    slider = 1500 + (error * slider_Kp) + offset;
  }

  if (prev_feedback - feedback > 350){
    slider_counter -= 1;
  }

  else if (prev_feedback - feedback < 350){
    slider_counter += 1;
  }
  
  prev_feedback = feedback;
}    

                                          
void gimbal_control(){

  //endre gimbal_Kp = (halve intervallet av pwm input)/(halve total vinkel den kan bevege seg)

  gimbal_thrust = (1500 + (gimbal_angle * gimbal_Kp));

  if (gimbal_thrust < 1280) gimbal_thrust = 1280;    //endre ut ifra motor
  if (gimbal_thrust > 1720) gimbal_thrust = 1720;

}  
