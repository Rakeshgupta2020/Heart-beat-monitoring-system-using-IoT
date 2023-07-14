#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "Galaxy M3177D7";   // your network SSID (name) 
const char* password = "**********";   // your network password

WiFiClient  client;
unsigned long myChannelNumber = ******;
const char * myWriteAPIKey = "*************";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;
int sensorPin = A0;                                // A0 is the input pin for the heart rate sensor
float sensorValue = 0;                             // Variable to store the value coming from the sensor
int count = 0;
unsigned long starttime = 0;
int heartrate = 0;
boolean counted = false;
void setup() {
WiFi.mode(WIFI_STA);   
ThingSpeak.begin(client);  // Initialize ThingSpeak
Serial.begin (9600);     
}

void loop() 
{
  
starttime = millis();
while (millis()<starttime+10000)                   // Reading pulse sensor for 10 seconds
{
sensorValue = analogRead(sensorPin);
if (sensorValue > 550 && counted == false)  // Threshold value is 550 (~ 2.7V)
{
count++;
Serial.print ("count = ");
Serial.println (count);
//digitalWrite (13,HIGH);
//delay (500);
//digitalWrite (13, LOW);
counted = true;
}
else if (sensorValue < 550)
{
counted = false;
digitalWrite (13, LOW);
}
}

heartrate = count*6;                               // Multiply the count by 6 to get beats per minute
//Serial.println ();
Serial.print ("BPM = ");
Serial.println (heartrate);                        // Display BPM in the Serial Monitor
//Serial.println ();
count = 0;
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
    int x = ThingSpeak.writeField(myChannelNumber, 1, heartrate, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
