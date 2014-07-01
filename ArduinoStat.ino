/*Beginning work on a digital thermostat.  This is to replace the round hockey
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
#include <DateTime.h>
#include "DHT.h"

#define DHTPIN 2

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte light_value;
//declaring a pin for an LED, indicating status
byte is_on;
int  set_temp;

void setup() 
  {
	//open serial port
	Serial.begin(9600);
	//boot screen
	lcd.begin(16,2);
	pinMode(moisture_LED, OUTPUT);
	lcd.print("ArduinoStat");
	delay(2000);
	lcd.clear();
	lcd.print("By: Ty");
	delay(2000);
	lcd.clear();
  }
void loop()
{
        //looking to move set temperature
        
        while (lcd.readButtons();)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          
          if (lcd.readButtons() & BUTTON_UP)
            { 
              lcd.clear();
              delay(10);
              set_temp = set_temp + 1;
              lcd.print(set_temp);
              Serial.println(set_temp);
            }
            if (lcd.readButtons() & BUTTON_DOWN)
            {
              lcd.clear();
              delay(10);
              set_temp = set_temp - 1;
              lcd.print(set_temp);
              Serial.println(set_temp);
            }
            if (lcd.readButtons() & BUTTON_LEFT)
            {
              lcd.clear();
              set_temp = 58;
              lcd.print(set_temp);
              Serial.println(set_temp);
            }
            if (lcd.readButtons() & BUTTON_RIGHT)
            {
              lcd.clear();
              set_temp = 66;
              lcd.print(set_temp);
              Serial.println(set_temp);
            }             
            
        //light sensor
	//if statement to decide night time/day time
	//resort to some sort of default temperature
	light_value = analogRead(light_value);
	Serial.print("light sensor reads ");
	Serial.println(light_value);
        //Check photoresistor values
        //Look into using Date/Time as a while, to avoid so many ifs
	if (light_value > 300)
	{        
                Serial.println("Probably Day Time");
		lcd.print("Day Time");
		//Set Day Temperature
		//Mark 'day time' time
	}
	else
	{
		Serial.println("Probably Night Time")
		lcd.print("Night Time")
		//Set Night Temperature
		//Mark 'bed time' time
	}

	//Button Controls for setting temperature
	
	//Feedback from buttons, indicating temperature change
	//Should I develope some sort of averaging ('learning') algorithim?
}
