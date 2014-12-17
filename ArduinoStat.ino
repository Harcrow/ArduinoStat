/* This is the begining work on a digital thermostat.  This is to replace the round hockey
puck style thermostat the currently resides in my house.  I'm
going to train this device to react to night/day temperatures.  It will also
indicate when the thermostat is sending a signal to begin heating the building.
My hope is that this will simplify consuming less natrual gas.
*/
//define analog inputs to which we have connected our sensors
//These libraries will change.  I'd like to use as simple equipment and
//code as possible, just to decrease over powerconsumption.
//Ideally I'd like a couple 9V batteries to power this for the season.

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#include "DHT.h"
#include <Time.h>
#include <TimeAlarms.h>
#include "Average.h"

#define DHTPIN 2
#define DHTTYPE DHT22
#define control_pin 8

DHT dht(DHTPIN, DHTTYPE);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte set_temp = 68;
byte real_temp;
float is_temp;
byte refresh_check_1;
byte refresh_check_2;
//float temperatures[100];
//float sum;
//int j, i = 0;
time_t turned_on, turned_off;

Average<float> ave(100);


void setup() 
  {
    //Activating Temp sensor, LCD display, etc...
        dht.begin();

	Serial.begin(9600);
	lcd.begin(16,2);
	lcd.print("ArduinoStat");
	delay(2000);
	lcd.clear();
              
       
        lcd.clear();
       
        pinMode(control_pin, OUTPUT);
        digitalWrite(control_pin, LOW);
        
        setTime(22, 00, 00, 12, 9, 14);
        
        Alarm.alarmRepeat(4, 0, 0, MorningAlarm);
        Alarm.alarmRepeat(7, 30, 0, LeavingHome);
        Alarm.alarmRepeat(16, 30, 0, ArriveHome);
        Alarm.alarmRepeat(22, 30, 0, EveningAlarm);
        
     
        
  }
void loop()
{
        
        Alarm.delay(2);
        
        //reading temperature at start of the loop
        
       /* temperatures[i] = dht.readTemperature(true);
          Serial.print("Temps: ");
          Serial.println(temperatures[i]);
          Serial.print("Is: ");
          Serial.println(is_temp, i);
          is_temp += temperatures[i];
          i++;
          
        if(i > 99)
          {
            i = 0;
          }
          
        you're stupid ->>is_temp = is_temp / i;
        */        
        ave.push(dht.readTemperature(true));
        is_temp = ave.mean();
        Serial.print("Mean: ");
        Serial.println(is_temp);
        
        if(set_temp > is_temp && (now() - turned_off) > 900)
          {  
            turned_on = now();
            //Relay is active low...only took  me 6 months to figure out
            digitalWrite(control_pin, LOW);
           
          }
         else if(set_temp < is_temp && (now() - turned_on) > 900 )
          {
            turned_off = now();
            digitalWrite(control_pin, HIGH);
           
          }
        
        
   
        
        if(isnan(is_temp))
        {
          Serial.println("Sensor Failed.");
          lcd.clear();
          lcd.print("Sensor Failed.");
        }
        
        //Button Controls for setting temperature

       
        while (lcd.readButtons())
        {
          lcd.clear();
          lcd.setCursor(0,0);
          
          if (lcd.readButtons() & BUTTON_UP)
            { 
              lcd.clear();
              delay(100);
              set_temp = set_temp + 1;
              //lcd.print(set_temp);
              Serial.println(set_temp);
              
            }
            else if (lcd.readButtons() & BUTTON_DOWN)
            {
              lcd.clear();
              delay(100);
              set_temp = set_temp - 1;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }
            else if (lcd.readButtons() & BUTTON_LEFT)
            {
              lcd.clear();
              set_temp = 58;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }
            else if (lcd.readButtons() & BUTTON_RIGHT)
            {
              lcd.clear();
              set_temp = 66;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }
            else;  
            
            if(set_temp > is_temp)
                {
                  turned_on = now();
                  //Relay is active low...only took  me 6 months to figure out
                  digitalWrite(control_pin, LOW);
                  Serial.println("ON");
                }
             else if(set_temp < is_temp)
                {
                   turned_off = now();
                   digitalWrite(control_pin, HIGH);
                   Serial.println("OFF");
                }
              
        }
        real_temp = dht.readTemperature(true);
        
        //I think I can remove the refresh variables to save memory
        if (refresh_check_1 != set_temp || refresh_check_2 != real_temp)
            {
              //real_temp = dht.readTemperature(true);
              lcd.clear();
              lcd.print("Temp: " + String(real_temp));
              lcd.setCursor(0,1);
              lcd.print("Set Temp: " + String(set_temp));
              refresh_check_1 = set_temp;
              refresh_check_2 = real_temp;
           
            }  
 }
void MorningAlarm()
{
  set_temp = 72;
}

void LeavingHome()
{
   set_temp = 55;
}

void EveningAlarm()
{
  set_temp = 66;//This will set the heater to some low evening temperature.
}
void ArriveHome()
{
  set_temp = 72;
}

/*void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.println("test");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println(); 
}
void printDigits(int digits)
{
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}*/
