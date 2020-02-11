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

int16_t motor_idle_speed = 1500;

uint32_t loop_timer;                                                         
uint16_t incoming_byte;
uint32_t esc_1;

uint8_t NewInt;
uint16_t counter;

uint16_t thrust;
uint16_t arm;

int t = 2;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Meldinger

ros::NodeHandle  nh;

std_msgs::UInt32 int_msg;


/////////////////////////////////////////////////////////
//Publisher

ros::Publisher pubChatter("chatter", &int_msg);



/////////////////////////////////////////////////////////
//Subscriber

void messageCb(const fifth_prop::BPS_thrust &toggle_msg){
  thrust = toggle_msg.thrust;
  arm = toggle_msg.arm;
  NewInt = 1;
  counter = 0;
  digitalWrite(PC13, HIGH-digitalRead(PC13));   
}

ros::Subscriber<fifth_prop::BPS_thrust> sub1("thrust", messageCb );





void setup()
{
  nh.getHardware()->setBaud(57600);
  nh.initNode();              //initalisere node

//Setter opp publisher
  nh.advertise(pubChatter);   //Setter opp publisher

//Setter opp subscriber
  nh.subscribe(sub1);         


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
  int_msg.data = esc_1;
  pubChatter.publish(&int_msg);                       //publiserer meldingen
  
  nh.spinOnce(); 

  nh.loginfo("Running!"); 


//////////////////////////////////////////
  if (NewInt == 0){
    
    counter++;
    
    if (counter >= 20){
      arm = 0;
    }
    
  }
  
  if (arm == 1) {                                                                //starter motoren

    esc_1 = thrust;

    if (esc_1 < 1000) esc_1 = 1000;                                                //Hvis motoren er under revers, sett fart lik revers
    if (esc_1 > 2000) esc_1 = 2000;                                                 //Maks-puls lik 2000;
    NewInt = 0;
  }

  else {
    esc_1 = 1000;                                                               //Hvis start != 2, fart = 0
  }

  
  

  TIMER4_BASE->CCR1 = esc_1;                                                       //Sett lengden på pulsen lik det vi har regnet ut at den skal bli
  TIMER4_BASE->CNT = 5000;                                                         //Verdien blir nullstilt av software ikke ARR


/*
  if (micros() - loop_timer > 10050) {
    //str_msg.data = "error";                                                                  //Hvis loopen varer i mer enn 4000 mikrosekunder = feil
  }
*/  
  while (micros() - loop_timer < 10000);                                            //Vent til 4000 mikrosekunder har gått
  
  loop_timer = micros();  
  
  //delay(10);
}
