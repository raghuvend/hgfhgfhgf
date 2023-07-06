#include <SoftwareSerial.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#define MQ3 D4
#include <TM1637Display.h>
#include <TinyGPS++.h>
float lattitude,longitude;
SoftwareSerial gpsSerial(4,5);//rx,tx
TinyGPSPlus gps;// create gps object


#define CLK D3
#define DIO D0

#define TEST_DELAY   2000


TM1637Display display(CLK, DIO);


const uint8_t SEG_DONE[] = {
  SEG_A | SEG_F | SEG_E| SEG_G,           // F
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // U
  SEG_F | SEG_E | SEG_D,                           // L
  SEG_F | SEG_E | SEG_D            // L
  };




char auth[] = "U_oLaPukkdhJdjTOa78b8yDBntBUp_3K";//Enter your Auth token
char ssid[] = "Nothing Phone_1";//Enter your WIFI name
char pass[] = "sam12345";//Enter your WIFI password

BlynkTimer timer;

const int trigPin = D5; 
const int echoPin = D6; 

long duration;
int distance;
int a;
void dustbin_space() {

 

digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(5);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

distance= duration*0.034/2;
Serial.print("Distance: ");
Serial.println(distance);

if(distance>=30)
{a=100;}

if(distance<30 && distance>27 )
{a=90;}

if(distance<27 && distance>24 )
{a=80;}

if(distance<24 && distance>21 )
{a=70;}

if(distance<21 && distance>18 )
{a=60;}

if(distance<18 && distance>15 )
{a=50;}

if(distance<15 && distance>12 )
{a=40;}

if(distance<12 && distance>9 )
{a=30;}

if(distance<9 && distance>6 )
{a=20;}

if(distance<6 && distance>3 )
{a=10;}

if(distance<3 )
{a=0;
Blynk.logEvent("dustbin_alert" , "Dustbin has been full please replace with vacant one");
}

  

  uint8_t data[] = { 0x0, 0x0, 0x0, 0x0 };
  display.setSegments(data);


  display.setSegments(data);
display.showNumberDec(a);
if(a<5)
{
  display.setSegments(SEG_DONE);
  
  }

  distance = map(distance, 0, 1023, 0, 100);
  Blynk.virtualWrite(V0, a);
  Serial.println(a);
  delay(1000);
 
  



  
  
  

}

/*void Mq3sensor() {
 bool value = digitalRead(MQ3);
 if (value == 0) {
    WidgetLED LED(V4);
    LED.on();
    

  Blynk.logEvent("stench_alert" , "Too much Smell");
  
  
   

  } else {
    WidgetLED LED(V4);
    LED.off();


  }

}*/

void setup() {
  display.setBrightness(0x0f);
  Serial.begin(115200);
  gpsSerial.begin(9600); // connect gps sensor
  Wire.begin(D2, D1);
 // pinMode(MQ3, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
 Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  timer.setInterval(100L, dustbin_space);
 // timer.setInterval(100L, Mq3sensor);
  timer.setInterval(100L, SendMessage);

}
void loop() {


  
  Blynk.run();
  timer.run();
}

void SendMessage()
{
      

       boolean newData = false;
  
  for (unsigned long start = millis(); millis() - start < 2000;)

       {
         while (gpsSerial.available() > 0)
         {
                 if (gps.encode(gpsSerial.read()))
                        {newData = true;}
         }
       }           


   if(newData)
  {
     Serial.print("Latitude= "); 
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude= "); 
    Serial.println(gps.location.lng(), 6);
    newData = false;

   lattitude=gps.location.lat();
   longitude=gps.location.lng();
    Blynk.virtualWrite(V6, lattitude);
    Blynk.virtualWrite(V5,longitude );
   
}}
