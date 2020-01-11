/*
 * rosserial Publisher Example
 * Prints "hello world!"
 */

#define USE_STM32_HW_SERIAL

#include <ros.h>
#include <std_msgs/String.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////

int32_t measured_time, measured_time_start;

uint8_t channel_select_counter;

int16_t motor_idle_speed = 1500;

uint8_t start;
int8_t i;

uint32_t loop_timer;                                                         
uint16_t incoming_byte;
int16_t esc_1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////

ros::NodeHandle  nh;

std_msgs::String str_msg;

//Publisher

ros::Publisher pubChatter("chatter", &str_msg);


//Subscriber

void messageCb(const std_msgs::String &toggle_msg){
  digitalWrite(PC13, HIGH-digitalRead(PC13));   
  
  
  //Serial.print(toggle_msg.data);
}

ros::Subscriber<std_msgs::String> sub1("chatter", messageCb );



char hello[4]  = "hei, baby <3";

void setup()
{
  nh.initNode();              //initalisere node
  nh.advertise(pubChatter);   //Setter opp publisher

  nh.subscribe(sub1);         //Setter opp subscriber


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
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop()
{  
  str_msg.data = hello;
  pubChatter.publish( &str_msg );                       //publiserer meldingen
  nh.spinOnce();
  
  nh.loginfo("melding: "); 

  
  if (start == 2) {                                                                //starter motoren

    esc_1 = 1500;

    if (esc_1 < 1000) esc_1 = 1000;                                                //Hvis motoren er under revers, sett fart lik revers
    if (esc_1 > 2000) esc_1 = 2000;                                                 //Maks-puls lik 2000;
    
  }

  else esc_1 = 1500;                                                               //Hvis start != 2, fart = 0



  TIMER4_BASE->CCR1 = esc_1;                                                       //Sett lengden på pulsen lik det vi har regnet ut at den skal bli
  TIMER4_BASE->CNT = 5000;                                                         //Verdien blir nullstilt av software ikke ARR



  if (micros() - loop_timer > 4050) {
    str_msg.data = "error";                                                                  //Hvis loopen varer i mer enn 4000 mikrosekunder = feil
  }
  while (micros() - loop_timer < 4000);                                            //Vent til 4000 mikrosekunder har gått
  loop_timer = micros();  
  
  delay(1000);
}
