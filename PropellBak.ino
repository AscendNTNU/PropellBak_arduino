
/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

//Hvordan man setter opp 32UIntArray
 /*
  Arr_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension)*2);
  Arr_msg.layout.dim[0].label = "kanaler";
  Arr_msg.layout.dim[0].size = 8;
  Arr_msg.layout.dim[0].stride = 1;
  Arr_msg.layout.data_offset = 0;
  Arr_msg.data = (uint32_t *)malloc(sizeof(uint32_t)*8);
  Arr_msg.data_length = 8;
*/


//viktig!!

#define USE_STM32_HW_SERIAL

#include <ros.h>
#include <std_msgs/UInt32.h>
#include <fifth_prop/BPS_thrust.h>

#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t loop_timer;                                                         
uint16_t incoming_byte;
uint32_t esc_1;
uint32_t gimbal_angle;


uint32_t target_angle;              // Global shared variables
uint8_t gimbal_Kp = 50;
uint8_t slider_Kp = 1;    

uint8_t NewInt;
uint8_t gimbal_int;
uint16_t counter;
uint8_t slider_counter;


//Timere
uint8_t rising_edge_set;
uint32_t measured_time;
float feedback;
float prev_feedback;
uint32_t measured_time_start;

uint16_t thrust;
uint16_t arm;
uint16_t gimbal_thrust;
uint16_t slider;
uint16_t gimbal_arm;
uint16_t slider_arm;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Meldinger

ros::NodeHandle  nh;

std_msgs::UInt32 int_msg;


/////////////////////////////////////////////////////////
//Publisher

ros::Publisher pubChatter("chatter", &int_msg);


/////////////////////////////////////////////////////////
//Subscriber

void BPSCb(const fifth_prop::BPS_thrust &thrust_msg){
  thrust = thrust_msg.thrust;
  arm = thrust_msg.arm;
  NewInt = 1;
  counter = 0;
  digitalWrite(PC13, HIGH-digitalRead(PC13));   
}

void Gimbal_angleCb(const fifth_prop::BPS_thrust &gimbal_msg){
  gimbal_angle = gimbal_msg.thrust;
  gimbal_arm = gimbal_msg.arm;   
}



ros::Subscriber<fifth_prop::BPS_thrust> sub_BPS("thrust", BPSCb);

ros::Subscriber<fifth_prop::BPS_thrust> sub_gimbal("gimbal", Gimbal_angleCb);


void setup()
{
  nh.getHardware()->setBaud(57600);
  nh.initNode();              //initalisere node

//Setter opp publisher
  nh.advertise(pubChatter);   //Setter opp publisher

//Setter opp subscriber
  nh.subscribe(sub_BPS); 
  nh.subscribe(sub_gimbal);         


  //////////////////////////////////////////////////////////////////////////////////////////////
  
  pinMode(PC13, OUTPUT);                                        //LED brukes til GPS
  digitalWrite(PC13, LOW);                                      //Motsatt funksjon (HØY == LAV)


  timer_setup();                                                //Sett opp timere og gi riktige verdier til registrene
  delay(50);                                                    //Gi timeren tid til å starte

  arm = 0;

  while (!nh.connected()){
    nh.spinOnce();                                              //Venter 
  }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{  
  
  //sender thrusten tilbake
  int_msg.data = thrust;
  
  //int_msg.data = esc_1;
  pubChatter.publish(&int_msg);                       //publiserer meldingen
  
  nh.spinOnce(); 

  nh.loginfo("Running!"); 


//esc_1
///////////////////////////////////////////

  if (NewInt == 0){
    
    counter++;
    
    if (counter >= 20){
      arm = 0;
    }
    
  }
  
  if (arm == 1) {                                                                //starter motoren

    esc_1 = thrust;

    if (esc_1 < 1000) esc_1 = 1000;                                                //Hvis motoren er under revers, sett fart lik revers
    if (esc_1 > 2000) esc_1 = 2000;                                                //Maks-puls lik 2000;
    NewInt = 0;
  }

  else {
    esc_1 = 1000;                                                                  //Hvis start != 2, fart = 0
  }


//////////////////////////////////////////

  if (gimbal_int){

//gimbal_angle

    if (gimbal_arm == 1) {

      gimbal_control();

 
      //NewInt = 0;
    }

    else {
      gimbal_thrust = 1500;                                                                  
    }



//slider


    if (slider_arm == 1) {

      feedback360();

 
      //NewInt = 0;
    }

    else {
      slider = 1500;                                                                  
    }


    gimbal_int--;
  }
  else {
    gimbal_int++; 
  }

///////////////////////////////////////
  
  

  TIMER4_BASE->CCR1 = esc_1;                                                       //Sett lengden på pulsen lik det vi har regnet ut at den skal bli
  TIMER4_BASE->CCR2 = gimbal_thrust;
  TIMER4_BASE->CCR3 = slider;
  TIMER4_BASE->CNT = 5000;                                                         //Verdien blir nullstilt av software ikke ARR

 
  while (micros() - loop_timer < 10000);                                            //Vent til 10000 mikrosekunder har gått, 100 Hz
  loop_timer = micros();  
}
