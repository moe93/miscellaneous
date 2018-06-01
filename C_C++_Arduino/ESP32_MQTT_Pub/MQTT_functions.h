/*
   MQTT Functions such as: Connect to WiFi, subscribe, publish, etc...
*/

String WiFiErrorCode( int );                      // Function prototype

// =========================     Setup WiFi     ========================
void setup_WiFi( ) {

  int     cnt       =  0;                         // Counter for reset

  delay( 10 );                                    // Delay for stability
  Serial.print( "\nConnecting to " );             // [INFO] ...
  Serial.println( WLAN_SSID );                    // [INFO] ...
  WiFi.disconnect();                              // NOTE: For some reason we need this
  WiFi.begin( WLAN_SSID, WLAN_PASS );             // Connect to WiFi

  while ( WiFi.status() != WL_CONNECTED )         // Wait while we connect
  {
    delay( 500 );
    Serial.print( "." );
    if ( ++cnt % 20 == 0 ) Serial.println();      // Print new-line every 10sec

    if ( cnt == 40 )                              // cnt==40 for 20 seconds
    {
      int s = WiFi.status();                      // Get error message
      Serial.print( "\nTimed out. Resetting!\n" );// [INFO] ...
      Serial.print( "WiFi status = " );           // [INFO] ...
      Serial.println( WiFiErrorCode( s ) );       // [INFO] ...
      WiFi.begin( WLAN_SSID, WLAN_PASS );         // Re-issue connect to WiFi command
      cnt = 0;                                    // Reset counter
    }

  }
  Serial.print( "\nWiFi connected - ESP IP address: " );
  Serial.println( WiFi.localIP() );
}

// ===================   WiFi Error Codes Translator  ==================
String WiFiErrorCode( int n )
{
  String s = "";
  switch ( n )
  {
    case WL_IDLE_STATUS     : s = "WL_IDLE_STATUS";     //   0
      break;
    case WL_NO_SSID_AVAIL   : s = "WL_NO_SSID_AVAIL";   //   1
      break;
    case WL_SCAN_COMPLETED  : s = "WL_SCAN_COMPLETED";  //   2
      break;
    case WL_CONNECTED       : s = "WL_CONNECTED";       //   3
      break;
    case WL_CONNECT_FAILED  : s = "WL_CONNECT_FAILED";  //   4
      break;
    case WL_CONNECTION_LOST : s = "WL_CONNECTION_LOST"; //   5
      break;
    case WL_DISCONNECTED    : s = "WL_DISCONNECTED";    //   6
      break;
    case WL_NO_SHIELD       : s = "WL_NO_SHIELD";       // 255
      break;
  }
  return s;
}

// ====================   Reconnect to MQTT Server  ====================
void MQTT_reconnect() {
  // Loop until we're reconnected
  while ( !client.connected() )
  {
    Serial.print( "Attempting MQTT connection..." );
    if ( client.connect( "ESP32" ) )
    {
      Serial.println( "Connected" );
      client.subscribe( "test/test" );          // Subscribe or resubscribe to a topic
//      client.subscribe( "Topic/blah2" );          // Subscribe or resubscribe to a topic
      delay( 250 );                               // Delay for stabilidad!
    }
    else
    {
      Serial.print( "Failed, rc=" ); Serial.print( client.state() );
      Serial.println( " Retry: in 5 seconds" );
      // Wait 5 seconds before retrying
      delay( 5000 );
    }
  }
}

// =====================   Publish to MQTT Server  =====================
void MQTT_publish( const char* topic, char data ) {
  
  //client.publish(topic, data);

}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
// =============   Callback When MQTT Message is Recieved  =============
void MQTT_onRecieve(char* topic, byte* message, unsigned int length) {
  Serial.print( "Message arrived on topic: ");
  Serial.println( topic );
  String messageTemp;
  
  for( int i = 0; i < length; i++ )
  {
    messageTemp += (char)message[i];
  }
  
  Serial.println();

  // Check Topics
  if( strcmp(topic, "test/test")==0 )
  {
      Serial.print( "Recieved: " );
      Serial.print( messageTemp );
      Serial.print( "\n" );
  }
  
//  if( strcmp(topic, "topic/blah")==0 )
//  {
//      Serial.print( "Topic says: " );
//      Serial.print( messageTemp );
//      Serial.print( " the end!" );
//  }

  Serial.println();
}

