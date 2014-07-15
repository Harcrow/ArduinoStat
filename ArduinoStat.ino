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
#define bufferMax 24
#define light_pin 0
#define is_on 9

byte bufferSize;
char buffer[bufferMax];

char cmd[15];    // Nothing magical about 15, increase these if you need longer commands/parameters
char param1[15];
char param2[15];
String command = "";

byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x23, 0x06 };
IPAddress ip(192, 168, 1, 177);

DHT dht(DHTPIN, DHTTYPE);

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

byte set_temp = 68;
byte light_value;
byte is_temp;

EthernetServer server(80);

void setup() 
  {
    //Activating Temp sensor, webserver, LCD display, etc...
        dht.begin();
        Ethernet.begin(mac, ip);
        server.begin();
	Serial.begin(9600);
	lcd.begin(16,2);
	lcd.print("ArduinoStat");
	delay(2000);
	lcd.clear();
  }
void loop()
{
  
        //reading temperature at start of the loop
        is_temp = dht.readTemperature(true);
        light_value = analogRead(light_pin);
        Serial.println(light_value);
        if(isnan(is_temp))
        {
          Serial.println("Sensor Failed.");
          lcd.clear();
          lcd.print("Sensor Failed.");
        }
        
        //Button Controls for setting temperature
        EthernetClient client = server.available();
        if (client)
        {
        WaitForRequest(client);
        ParseReceivedRequest();
        
        client.stop();
        
        }  
        while (lcd.readButtons())
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
                lcd.clear();
		lcd.print("Day Time");
                delay(500);
		//Set Day Temperature
		//Mark 'day time' time
	}
	else
	{
		Serial.println("Probably Night Time");
                lcd.clear();
		lcd.print("Night Time");
                delay(500);
		//Set Night Temperature
		//Mark 'bed time' time
	
      
     // PrintNumber("bufferSize", bufferSize);


}
}
    void WaitForRequest(EthernetClient client) // Sets buffer[] and bufferSize
    {
      bufferSize = 0;
     
      while (client.connected()) 
        {
        if (client.available())
           {
            char c = client.read();
            if (c == '\n')
              break;
            else
              if (bufferSize < bufferMax)
                buffer[bufferSize++] = c;
              else
                break;
           }
        }
        
    }
void ParseReceivedRequest()
    {
      Serial.println("in ParseReceivedRequest");
      Serial.println(buffer);
      
      //Received buffer contains "GET /cmd/param1/param2 HTTP/1.1".  Break it up.
      char* slash1;
      char* slash2;
      char* slash3;
      char* space2;
      
      slash1 = strstr(buffer, "/") + 1; // Look for first slash
      slash2 = strstr(slash1, "/") + 1; // second slash
      slash3 = strstr(slash2, "/") + 1; // third slash
      space2 = strstr(slash2, " ") + 1; // space after second slash (in case there is no third slash)
      if (slash3 > space2) slash3=slash2;
    
      PrintString("slash1",slash1);
      PrintString("slash2",slash2);
      PrintString("slash3",slash3);
      PrintString("space2",space2);
      
      // strncpy does not automatically add terminating zero, but strncat does! So start with blank string and concatenate.
      cmd[0] = 0;
      param1[0] = 0;
      param2[0] = 0;
      strncat(cmd, slash1, slash2-slash1-1);
      strncat(param1, slash2, slash3-slash2-1);
      strncat(param2, slash3, space2-slash3-1);
      
      PrintString("cmd",cmd);
      PrintString("param1",param1);
      PrintString("param2",param2);
    }
    
    void PrintString(char* label, char* str)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(str);
}

void PrintNumber(char* label, int number)
{
  Serial.print(label);
  Serial.print("=");
  Serial.println(number, DEC);
  
}
  void PerformRequestedCommands()
{
  //Edit all of this to fit setting the temperature of things.
  if ( strcmp(cmd,"digitalWrite") == 0 )
  {
   RemoteDigitalWrite();
  }
  if ( strcmp(cmd,"analogRead") == 0 )
   {
     RemoteAnalogRead();
   }
}
