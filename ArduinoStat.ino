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

#define DHTPIN 2
#define DHTTYPE DHT22
#define control_pin 9

DHT dht(DHTPIN, DHTTYPE);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte set_temp = 68;
byte is_temp;
byte refresh_check_1;
byte refresh_check_2;

//EthernetServer server(80);

void setup() 
  {
    //Activating Temp sensor, webserver, LCD display, etc...
        dht.begin();

	Serial.begin(9600);
	lcd.begin(16,2);
	lcd.print("ArduinoStat");
	delay(2000);
	lcd.clear();
              
        is_temp = dht.readTemperature(true);
        lcd.clear();
        lcd.print("Current Temp: " + String(is_temp));
        lcd.setCursor(0,1);
        lcd.print("Set Temp: " + String(set_temp));
        pinMode(control_pin, OUTPUT);
        
        setTime(13, 38, 30, 10, 15, 14);
        
        Alarm.alarmRepeat(5, 0, 0, MorningAlarm);
        Alarm.alarmRepeat(7, 0, 0, LeavingHome);
        Alarm.alarmRepeat(17, 30, 0, ArriveHome);
        Alarm.alarmRepeat(21, 30, 0, EveningAlarm);
        
        
  }
void loop()
{
        digitalClockDisplay();
        Alarm.delay(1000);
        
        //reading temperature at start of the loop
        is_temp = dht.readTemperature(true);
        
        if(set_temp <= is_temp)
          {
            digitalWrite(control_pin, HIGH);
          }
          else 
          {
            digitalWrite(control_pin, LOW);
          }
        
        
        Serial.println(is_temp);
        if(isnan(is_temp))
        {
          Serial.println("Sensor Failed.");
          lcd.clear();
          lcd.print("Sensor Failed.");
        }
        
        //Button Controls for setting temperature
//        EthernetClient client = server.available();
       
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
            if (lcd.readButtons() & BUTTON_DOWN)
            {
              lcd.clear();
              delay(100);
              set_temp = set_temp - 1;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }
            if (lcd.readButtons() & BUTTON_LEFT)
            {
              lcd.clear();
              set_temp = 58;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }
            if (lcd.readButtons() & BUTTON_RIGHT)
            {
              lcd.clear();
              set_temp = 66;
              //lcd.print(set_temp);
              Serial.println(set_temp);
            }             
        }
        
        
        if (refresh_check_1 != set_temp || refresh_check_2 != is_temp)
            {
              is_temp = dht.readTemperature(true);
              lcd.clear();
              lcd.print("Temp: " + String(is_temp));
              lcd.setCursor(0,1);
              lcd.print("Set Temp: " + String(set_temp));
              refresh_check_1 = set_temp;
              refresh_check_2 = is_temp;
           
            }  
 }
void MorningAlarm()
{
  set_temp = 68;
}

void LeavingHome()
{
   set_temp = 55;
}

void EveningAlarm()
{
  set_temp = 60;//This will set the heater to some low evening temperature.
}
void ArriveHome()
{
  set_temp = 68;
}

void digitalClockDisplay()
{
  // digital clock display of the time
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
}
