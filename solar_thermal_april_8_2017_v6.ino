//#include <Average.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 /*-(Connect to Pin 2 )-*/

/*-----( Declare Variables )-----*/

 

//  define names for the 4 Digital pins On the Arduino   
// These data pins link to 4 Relay board pins IN1, IN2, IN3, IN4

#define RELAY1  6                        
#define RELAY2  7                        
#define RELAY3  8                        
#define RELAY4  9
#define RELAY5  10                        
#define RELAY6  11                       
#define RELAY7  12                        
#define RELAY8  13
//int led1 = 31; int led2 = 33; int led3 = 35; int led4 = 37; int led5 = 39; int led6 = 41;
int led1 = 41; int led2 = 39; int led3 = 37; int led4 = 35; int led5 = 33; int led6 = 31;
int push1 = 53; int push2 = 51; int push3 = 49; int push4 = 47; int push5 = 45; int push6 = 43; 
int kitchenled1 = 48; int kitchenled2 = 50; int kitchensw = 24;
int kitchenrequest = 0;
int val1 = 0; int val2 = 0;int val3 = 0;int val4 = 0;int val5 = 0;int val6 = 0;
//int flow1 = 22; int flow2 = 24;
int pulsepin1 = 22;
int analogPin = 0;
int flow_v = 0;
int lastflow = LOW;
int flow = LOW;
long previous =0; long startt,elapsed,fraction,sum_times; 
float pulse1,treadings; int pulsecount;
unsigned long duration1;
int toggle1 = 0;int toggle2 = 0; int sense = 0; int run_solar = 0;
float panel0,panel1,panel2, panel3,panel4,panel5,panel6,panel7, tank_top, tank_bottom, house_rtn, sol_rtn,panel_ir,panel_top,t_diff;
int tens =0;int units = 0;


OneWire ourWire(ONE_WIRE_BUS);
DallasTemperature sensors(&ourWire);


void setup()

{    
  /*-(start serial port to see results )-*/
  delay(1000);
  Serial.begin(9600);
  delay(1000);
  /*-( Start up the DallasTemperature library )-*/
  sensors.begin();
  Serial.begin(9600);      // open the serial port at 9600 bps: 
  pinMode(RELAY1, OUTPUT); pinMode(RELAY2, OUTPUT); pinMode(RELAY3, OUTPUT); pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT); pinMode(RELAY6, OUTPUT); pinMode(RELAY7, OUTPUT); pinMode(RELAY8, OUTPUT);
  pinMode(led1, OUTPUT); pinMode(led2, OUTPUT);pinMode(led3, OUTPUT);pinMode(led4, OUTPUT);pinMode(led5, OUTPUT);pinMode(led6, OUTPUT); // declare LED as output
  pinMode(push1, INPUT); pinMode(push2, INPUT);pinMode(push3, INPUT); pinMode(push4, INPUT); pinMode(push5, INPUT); pinMode(push6, INPUT);   // declare pushbutton as input
  //pinMode(flow1, INPUT); pinMode(flow2,INPUT);
  pinMode(kitchenled1, OUTPUT);pinMode(kitchenled2, OUTPUT);pinMode(kitchensw, INPUT);
  pinMode(pulsepin1, INPUT);
  reset();
  Serial.println("***** S T A R T I N G *********");

}
 void loop()
{
  digitalWrite(led1, HIGH);delay(5);digitalWrite(led1, LOW);
duration1 = pulseIn(val1,HIGH,5000000);
pulse1 =0;pulsecount = 0;
val1 = digitalRead(push1);val2 = digitalRead(push2);val3 = digitalRead(push3);val4 = digitalRead(push4);val5 = digitalRead(push5);val6 = digitalRead(push6);
//Serial.print(val1);Serial.print(val2);Serial.print(val3);Serial.print(val4);Serial.print(val5);Serial.print(val6);



 startt = millis();
for(int x = 0; x<1000;x++)
{
  flow_v = analogRead(analogPin);
  if (flow_v >800)
  { flow = HIGH; }
  else if (flow_v<400)
  {flow = LOW; }
  if (flow == HIGH && lastflow == LOW)
  {digitalWrite(led2, HIGH);
   }
  else if (flow == LOW && lastflow == HIGH)
  {digitalWrite(led2, LOW);
  pulsecount++;
   }
    lastflow = flow;
  delay(2);
}
elapsed = millis() -startt;

//digitalWrite(led2, LOW);

//pulse1 = pulsecount/(t_readings/100)   
 
 
 digitalWrite(led2, HIGH);

sensors.requestTemperatures();
panel_ir = panel0 = sensors.getTempCByIndex(0); digitalWrite(led1, HIGH); delay(3); digitalWrite(led1, LOW);
tank_bottom =  panel1 = sensors.getTempCByIndex(1); digitalWrite(led2, HIGH); delay(3); digitalWrite(led2, LOW);
sol_rtn = panel3 = sensors.getTempCByIndex(2); digitalWrite(led3, HIGH); delay(3); digitalWrite(led3, LOW);  
tank_top = panel4 = sensors.getTempCByIndex(3); digitalWrite(led4, HIGH); delay(3); digitalWrite(led4, LOW);  




 /* if (val1 == LOW)  //Relay 1 = power to timer, toggle 1
    { 
      if (toggle1 == 0 )
      {toggle1 = 1; 
      digitalWrite(RELAY1,HIGH);
      }  
      else if (toggle1 == 1)
      {toggle1 = 0;
      digitalWrite(RELAY1,LOW);
      }     
      else {}
    }  */

  if (val6 == LOW)  // test everything, toggle 6
    {
  resetandtest();
    }
    else {}

  if (tank_top > 30)            //test tank temp - increase to >50 and <49 
  {digitalWrite(RELAY1,LOW);
  Serial.print(" ,HouseON,1 ");
  }
  else if (tank_top <25)
  {digitalWrite(RELAY1,HIGH);
  Serial.print(" ,HouseOFF,0 ");
  }


 t_diff = panel_ir - tank_bottom;

 if (t_diff > 0 && panel_ir > 27)
  {digitalWrite(RELAY4,LOW);
  Serial.print(" ,SolON,1, ");
  }
  else if (panel_ir >32 && t_diff >-2.1)
  {
  digitalWrite(RELAY4,LOW);
  Serial.print(" ,SolON,1, ");
  }

  else if (t_diff < -2 || panel_ir <27 )
  {digitalWrite(RELAY4,HIGH);
  Serial.print(" ,SolOFF,0, ");
  }
  
     digitalWrite(led6, LOW); 
   digitalWrite(led5, LOW); 


//
 
 
 kitchenrequest = digitalRead(kitchensw);
 
if (kitchenrequest == 0)
digitalWrite(RELAY2,LOW); //bypass
else
digitalWrite(RELAY2,HIGH); //timer


tens = round(tank_top/10);
units = round(tank_top);
for(int x = 0; x<tens;x++)
{
  digitalWrite(kitchenled1, HIGH);delay(50);digitalWrite(kitchenled1, LOW);
}



delay(1000)



Serial.print(", tens = ,");Serial.print(tens);
Serial.print(", panel= ,");Serial.print(panel_ir);Serial.print(", tankB= ,");Serial.print(tank_bottom);
Serial.print(", solRTN= ,");Serial.print(sol_rtn);Serial.print(", tankT= ,");Serial.print(tank_top);
Serial.print(", t_diff= ,");Serial.print(t_diff);
Serial.print(", Pulses= ,");Serial.print(pulsecount),Serial.print(",over ,");Serial.println(elapsed);




}

 int reset(){
     digitalWrite(RELAY1,HIGH);          // Turns Relay Off
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
  digitalWrite(RELAY4,HIGH);          // Turns Relay Off        
   digitalWrite(RELAY5,HIGH);          // Turns Relay Off
   digitalWrite(RELAY6,HIGH);          // Turns Relay Off
   digitalWrite(RELAY7,HIGH);          // Turns Relay Off
   digitalWrite(RELAY8,HIGH);          // Turns Relay Off 
   digitalWrite(led1, HIGH); 
   digitalWrite(led2, HIGH); 
   digitalWrite(led3, HIGH); 
   digitalWrite(led4, HIGH); 
   digitalWrite(led5, HIGH); 
   digitalWrite(led6, HIGH); 
   
 }

 

 int resetandtest (){
   digitalWrite(RELAY1,LOW);               // Turns ON Relays 1
   delay(100);                          // Wait 2 seconds
   digitalWrite(RELAY1,HIGH);          // Turns Relay Off
   digitalWrite(RELAY2,LOW);           // Turns ON Relays 2
   delay(100);                                      // Wait 2 seconds
   digitalWrite(RELAY2,HIGH);          // Turns Relay Off
   digitalWrite(RELAY3,LOW);           // Turns ON Relays 3
   delay(800);                                      // Wait 2 seconds
   digitalWrite(RELAY3,HIGH);          // Turns Relay Off
   digitalWrite(RELAY4,LOW);           // Turns ON Relays 4
   delay(100);                                      // Wait 2 seconds
  digitalWrite(RELAY4,HIGH);          // Turns Relay Off        
   digitalWrite(RELAY5,LOW);           // Turns ON Relays 1
   delay(100);                                      // Wait 2 seconds
   digitalWrite(RELAY5,HIGH);          // Turns Relay Off
   digitalWrite(RELAY6,LOW);           // Turns ON Relays 2
   delay(100);                                      // Wait 2 seconds
   digitalWrite(RELAY6,HIGH);          // Turns Relay Off
   digitalWrite(RELAY7,LOW);           // Turns ON Relays 3
   delay(100);                                      // Wait 2 seconds
   digitalWrite(RELAY7,HIGH);          // Turns Relay Off
   digitalWrite(RELAY8,LOW);           // Turns ON Relays 4
   delay(100);                                      // Wait 2 seconds
   digitalWrite(RELAY8,HIGH);          // Turns Relay Off 
   digitalWrite(led1, HIGH); 
   delay(100);
   digitalWrite(led1, LOW);
   digitalWrite(led2, HIGH); 
   delay(100);
   digitalWrite(led2, LOW);
   digitalWrite(led3, HIGH); 
   delay(100);
   digitalWrite(led3, LOW);
   digitalWrite(led4, HIGH); 
   delay(100);
   digitalWrite(led4, LOW);
   digitalWrite(led5, HIGH); 
   delay(100);
   digitalWrite(led5, LOW);
   digitalWrite(led6, HIGH); 
   delay(100);
   digitalWrite(led6, LOW);
 }
