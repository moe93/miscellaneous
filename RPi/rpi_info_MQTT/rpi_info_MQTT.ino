/*****
 
 All the resources for this project:
 https://randomnerdtutorials.com/
 
*****/

// Loading the ESP8266WiFi library and the PubSubClient library
#include <WiFi.h>
#include <PubSubClient.h>

#include <SSD1306.h>
#include "OledFunctions.h"

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "MechEng 43va!";
const char* password = "ENGINEERING101";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "192.168.0.31";

// Initializes the espClient
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  
  Serial.begin(115200);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  OledSetup( "Hail Moe", "RPi Monitor", 300 );
}

// For this project, you don't need to change anything in the loop function. 
// Basically it ensures that you ESP is connected to your broker
void loop() {
  if( !client.connected() )
  {
    reconnect();
  }
  
  if( !client.loop() )
  {
    client.connect( "ESP8266Client" );
  }
}


// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print( "Connecting to " );
  Serial.println( ssid );
  WiFi.begin( ssid, password );
  
  while( WiFi.status() != WL_CONNECTED )
  {
    delay( 500 );
    Serial.print( "." );
  }
  
  Serial.println( "" );
  Serial.print( "WiFi connected - ESP IP address: " );
  Serial.println( WiFi.localIP() );
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print( "Message arrived on topic: ");
  Serial.println( topic );
  String messageTemp;
  
  for( int i = 0; i < length; i++ )
  {
    messageTemp += (char)message[i];
  }
  
  Serial.println();

  // Check Topics
  if( strcmp(topic, "RPi/armFreq")==0 )
  {
      Serial.print( "Frequency: " );
      Serial.print( messageTemp );
      Serial.print( "GHz\n" );
      dispLine[0] = "Freq :" + String( messageTemp ) + "GHz";
  }
  
  if( strcmp(topic, "RPi/temperature")==0 )
  {
      Serial.print( "Temperature: " );
      Serial.print( messageTemp );
      Serial.print( " C\n" );
      dispLine[1] = "Temp :" + String( messageTemp ) + " C";
  }
  
  if( strcmp(topic, "RPi/fanSpeed")==0 )
  {
      Serial.print( "Fan Speed: " );
      Serial.println( messageTemp );
      dispLine[2] = "Speed :" + String( messageTemp );
  }
  
  if( strcmp(topic, "RPi/turboMode")==0 )
  {
      Serial.print( "Turbo Mode is: " );
      Serial.println( messageTemp );
      dispLine[3] = "Turbo: " + String( messageTemp );
  }

  outputLine( 4, "blah" );        // Empty placeholder to display thangs!S
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while( !client.connected() )
  {
    Serial.print( "Attempting MQTT connection..." );
    // Attempt to connect
     /*
     YOU  NEED TO CHANGE THIS NEXT LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a unique name to the ESP8266.
     Here's how it looks like now:
       if (client.connect("ESP8266Client")) {
     If you want more devices connected to the MQTT broker, you can do it like this:
       if (client.connect("ESPOffice")) {
     Then, for the other ESP:
       if (client.connect("ESPGarage")) {
      That should solve your MQTT multiple connections problem

     THE SECTION IN loop() function should match your device name
    */
    if( client.connect("ESP8266Client") )
    {
      Serial.println( "connected" );
      display.setFont( ArialMT_Plain_10 );
      display.setTextAlignment( TEXT_ALIGN_CENTER );
      display.drawString( 64, 32, "Connected!" );
      display.display();
      // Subscribe or resubscribe to a topic
      client.subscribe( "RPi/armFreq" );
      client.subscribe( "RPi/temperature" );
      client.subscribe( "RPi/fanSpeed" );
      client.subscribe( "RPi/turboMode" );
      delay( 1000 );      // Delay for stabilidad!
    } else {
      Serial.print( "failed, rc=" );
      Serial.print( client.state() );
      Serial.println( " try again in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}
