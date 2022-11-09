#include <HardwareSerial.h>
#include <WiFi.h>
#include "ThingSpeak.h"

HardwareSerial SerialPort(2); // use UART2


const char* ssid = "NAMAN";   // your network SSID (name) 
const char* password = "password";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1872469;
const char * myWriteAPIKey = "6NJWBT09IDE4FHLN";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;



char number  = ' ';
int empty = 0;
void setup()
{
  SerialPort.begin(15200, SERIAL_8N1, 16, 17);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}
void loop()
{
  if (SerialPort.available())
  {
    char number = SerialPort.read();
    empty = number - '0';
    Serial.println(empty);
    
  }

 if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }
    


    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 1, empty, myWriteAPIKey);
    //uncomment if you want to get temperature in Fahrenheit
    //int x = ThingSpeak.writeField(myChannelNumber, 1, temperatureF, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }

    
}
