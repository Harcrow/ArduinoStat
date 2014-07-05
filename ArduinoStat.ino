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
#include <DateTime.h>
#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN 2
#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);


Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();


byte light_pin = 0;
int light_value;
//declaring a pin for an LED, indicating status
byte is_on = 9;
int  set_temp = 68;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] =
 {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
 EthernetServer server(80);

int is_temp;

void setup() 
  {
	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print("server is at ");
	Serial.println(Ethernet.localIP());
          
    pinMode(is_on, OUTPUT);
    dht.begin();
	Serial.begin(9600);
	//boot screen
	lcd.begin(16,2);
	lcd.print("ArduinoStat");
	delay(2000);
	lcd.clear();
	lcd.print("By: Ty");
	delay(2000);
	lcd.clear();
  }
void loop()
{
       // lcd.clear();
        //reading temperature at start of the loop
        is_temp = dht.readTemperature(true);
        
        if(isnan(is_temp))
        {
          Serial.println("Sensor Failed.");
          lcd.clear();
          lcd.print("Sensor Failed.");
        }
        
        //looking to move set temperature
        //Button Controls for setting temperature        
        while (lcd.readButtons())
        {
          lcd.clear();
          lcd.setCursor(0,0);
          
          if (lcd.readButtons() & BUTTON_UP)
            { 
			  is_temp = dht.readTemperature(true);
              lcd.clear();
              delay(10);
              set_temp = set_temp + 1;
              lcd.print(set_temp);
              Serial.println(set_temp);
              delay(250);
            }
            if (lcd.readButtons() & BUTTON_DOWN)
            {
			  is_temp = dht.readTemperature(true);
              lcd.clear();
              delay(10);
              set_temp = set_temp - 1;
              lcd.print(set_temp);
              Serial.println(set_temp);
              delay(250);
            }
            if (lcd.readButtons() & BUTTON_LEFT)
            {
			  is_temp = dht.readTemperature(true);
              lcd.clear();
              set_temp = 58;
              lcd.print(set_temp);
              Serial.println(set_temp);
              delay(250);
            }
            if (lcd.readButtons() & BUTTON_RIGHT)
            { 
			
			  is_temp = dht.readTemperature(true);
              lcd.clear();
              set_temp = 66;
              lcd.print(set_temp);
              Serial.println(set_temp);
              delay(250);
            }
			
		if (lcd.readButtons() & BUTTON_SELECT)
		{   
			is_temp = dht.readTemperature(true);
			lcd.clear();
			lcd.print("Temperature: ");
		lcd.print(is_temp);
			lcd.println("Set Temp: ");
			lcd.print(set_temp);
			Serial.println("Temperature: ");
			Serial.print(is_temp);
			Serial.println("Set Temp: ");
			Serial.print(set_temp);
		}             
        }
        //light sensor
	//if statement to decide night time/day time
	//resort to some sort of default temperature
	light_value = analogRead(light_pin);
	//Serial.println("light sensor reads ");
        //Serial.println(light_value);
        
        if(set_temp > is_temp)
        {
          digitalWrite(is_on, HIGH);
        }
        
        else
        {
           digitalWrite(is_on, LOW);
        }
  
     
}
