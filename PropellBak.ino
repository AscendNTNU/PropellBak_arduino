/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#define USE_STM32_HW_SERIAL

#include <ros.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt32.h>
#include <std_msgs/UInt32MultiArray.h>
#include <fifth_prop/TwoInts.h>

#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int32_t measured_time, measured_time_start;

uint8_t channel_select_counter;

int16_t motor_idle_speed = 1500;

uint8_t start;
int8_t i;
int8_t j;

uint32_t loop_timer;                                                         
uint16_t incoming_byte;
uint32_t esc_1;


uint32_t thrust;



//De som publiseres fra en annen node
uint32_t Channels[8];


//De variablene vi lagrer på denne noden
uint32_t Channels__[8];

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//Meldinger

ros::NodeHandle  nh;


std_msgs::String str_msg;
std_msgs::UInt32 int_msg;
std_msgs::UInt32MultiArray Arr_msg;
//auto Arr_msg = std_msgs::UInt32MultiArray();

char hello[4]  = "OK";


/////////////////////////////////////////////////////////
//Publisher

ros::Publisher pubChatter("chatter", &str_msg);

ros::Publisher pubChannel2("channel2", &int_msg);

ros::Publisher pubChannel("channel", &Arr_msg);   //denne kommer fra en annen ros-node


/////////////////////////////////////////////////////////
//Subscriber

void messageCb(const std_msgs::String &toggle_msg){
  digitalWrite(PC13, HIGH-digitalRead(PC13));   
}

ros::Subscriber<std_msgs::String> sub1("chatter", messageCb );





void channelCb(const std_msgs::UInt32MultiArray &channel_msg){
  Channels__[3] = channel_msg.data[3];
  Channels__[7] = channel_msg.data[7];
}

ros::Subscriber<std_msgs::UInt32MultiArray> subChannel("channel", channelCb );






void setup()
{
  nh.getHardware()->setBaud(57600);
  nh.initNode();              //initalisere node
  nh.advertise(pubChatter);   //Setter opp publisher
  nh.advertise(pubChannel);
  nh.advertise(pubChannel2);

  
  nh.subscribe(subChannel);   //Setter opp subscriber
  nh.subscribe(sub1);         


  //////////////////////////////////////////////////////////////////////////////////////////////
  
  //pinMode(4, INPUT_ANALOG);
  pinMode(PC13, OUTPUT);                                        //LED brukes til GPS
  digitalWrite(PC13, LOW);                                      //Motsatt funksjon (HØY == LAV)


  timer_setup();                                                //Sett opp timere og gi riktige verdier til registrene
  delay(50);                                                    //Gi timeren tid til å starte

  //Serial.begin(57600);

  start = 0;

  while (!nh.connected()){
    nh.spinOnce();                                              //Venter 
  }

  ////////////////////////////////////////////////////////////////////////////////////////
  //Setter opp UInt32MultiArray
  
  Arr_msg.layout.dim = (std_msgs::MultiArrayDimension *)
  malloc(sizeof(std_msgs::MultiArrayDimension)*2);
  Arr_msg.layout.dim[0].label = "kanaler";
  Arr_msg.layout.dim[0].size = 8;
  Arr_msg.layout.dim[0].stride = 1;
  Arr_msg.layout.data_offset = 0;
  Arr_msg.data = (uint32_t *)malloc(sizeof(uint32_t)*8);
  Arr_msg.data_length = 8;

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{  

  //endrer dataen som skal publiseres
  
  for (int j = 0; j < 8 ; j ++){
    Arr_msg.data[j] = Channels[j+1];
  }

  thrust = Channels[3];
  int_msg.data = thrust;

  str_msg.data = hello;
  
  //publiserer dataen
  //pubChannel2.publish(&int_msg);
  pubChatter.publish(&str_msg);                       //publiserer meldingen
  //pubChannel.publish(&Arr_msg);
  nh.spinOnce(); 

  nh.loginfo("Running!"); 


//////////////////////////////////////////

  if (Channels__[7] > 1600){
    start = 2;
  }
  else {
    start = 0;
  }
  
  
  if (start == 2) {                                                                //starter motoren

    esc_1 = Channels__[3];

    if (esc_1 < 1000) esc_1 = 1000;                                                //Hvis motoren er under revers, sett fart lik revers
    if (esc_1 > 2000) esc_1 = 2000;                                                 //Maks-puls lik 2000;
  }

  else esc_1 = 1000;                                                               //Hvis start != 2, fart = 0



  TIMER4_BASE->CCR1 = esc_1;                                                       //Sett lengden på pulsen lik det vi har regnet ut at den skal bli
  TIMER4_BASE->CNT = 5000;                                                         //Verdien blir nullstilt av software ikke ARR



  if (micros() - loop_timer > 4050) {
    //str_msg.data = "error";                                                                  //Hvis loopen varer i mer enn 4000 mikrosekunder = feil
  }
  while (micros() - loop_timer < 4000);                                            //Vent til 4000 mikrosekunder har gått
  loop_timer = micros();  
  
  delay(100);
}
