/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ================ LSM9DS1 IMU Setup ================ //
/*                 *******************                 //
int ImuSetup()
{
  imu.settings.device.commInterface = IMU_MODE_I2C; 
  imu.settings.device.mAddress      = LSM9DS1_M;
  imu.settings.device.agAddress     = LSM9DS1_AG;

  if ( !imu.begin() )
  {
    Serial.println( "\nFailed to communicate with LSM9DS1; check wiring.\nMoving on..." );
//    while ( true );
  }
  else
  {
    Serial.println( "\nIMU initialized." );
  }
  return 0;
} //*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ================ VL6180x ToF Setup ================ //
/*                 *******************                 /
int TofSetup()
{
  sensor.getIdentification( &identification );
  if( sensor.VL6180xInit() != 0 )
  {
    Serial.println( "\nFailed to initialize proximity sensor; check wiring.\nMoving on..." );
//    while ( true );
  }
  else
  {
    sensor.VL6180xDefautSettings();
    Serial.println( "\nProximity sensor initialized." );
  }
  return 0;
} //*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// =============== SSD1306--OLED Setup =============== //
/*                *********************                */
int OledSetup( String title, String headline, int pauseTime )
{
  _title    = title;
  _headline = headline;
  display.init();
  display.flipScreenVertically();
  splashScreen();
  delay( pauseTime );
  headLine();
//idLine();       // has to be called after WiFi init
  display.display();
  return 0;
} //*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ==================== WiFi Setup =================== //
/*                     ************                    /
boolean WifiSetup()
{
  boolean wifiFail  = false;
  int     cnt       =  0;

  wifiState         = DISCONNECTED;   // work with this a little later to remove wifiFail
  
  WiFi.mode( WIFI_STA );
  WiFi.begin( ssid, password );
  Serial.println( "\nConnecting to WiFi..." );
  while ( ( WiFi.status() != WL_CONNECTED ) && ( !wifiFail ) )
  {
    Serial.print( "." );
    delay( 500 );
    if ( ++cnt % 20 == 0 ) Serial.println();
    if ( cnt == 10 )  // 40 for 20 seconds
    {
      int s = WiFi.status();
      Serial.println( "\nTimed out connecting to WiFi.  Correct and reset." );
      Serial.print( "WiFi status = " ); Serial.println( WiFiErrorCode( s ) );
      lastLine( "No WiFi connection. Reset.", ArialMT_Plain_10 );
      wifiFail = true;
      delay( 1000 );
      ESP.restart();
    }
  }
  if ( wifiFail )
  {
    wifiState = DISCONNECTED;
  }
  else
  {
    Serial.println( "\nConnected to WiFi." );
    Serial.print( "IP: " );
    Serial.println( WiFi.localIP() );

    uint8_t* MAC  = WiFi.macAddress( MAC_init );
    for ( int i = 2; i < 15; i += 3 ) MAC_char[i] = ':';
    for ( int i = 0, j = 0; i < 17; i += 3, j++ )
    {
      MAC_char[i]   = (MAC[j] & 0xF0) >> 4;
      MAC_char[i+1] = MAC[j] & 0x0F;
      if ( MAC_char[i] > 0x09 ) MAC_char[i] += 0x37; else MAC_char[i] += 0x30;
      if ( MAC_char[i+1] > 0x09 ) MAC_char[i+1] += 0x37; else MAC_char[i+1] += 0x30;
    }
    MAC_char[17] = '\0';
    Serial.printf( "MAC: %s\n", MAC_char );   
    idLine( lookupID( MAC_char ) );
    display.display();
    
    wifiState = CONNECTED;
  }
  return !wifiFail;
} //*/


// ==================== OTA Setup ==================== //
/*                     ***********                     /
boolean OTASetup()
{
  boolean result  = true;

// Port defaults to 3232
// ArduinoOTA.setPort( 3232 );

// Hostname defaults to esp3232-[MAC]
  int str_len = mqttID.length() + 1;
  char mqttID_charArray[str_len];
  mqttID.toCharArray( mqttID_charArray, str_len );
  ArduinoOTA.setHostname( mqttID_charArray );

// No authentication by default
// ArduinoOTA.setPassword( "admin" );

// Password can be set with it's md5 value as well
// MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
// ArduinoOTA.setPasswordHash( "21232f297a57a5a743894a0e4a801fc3" );

  ArduinoOTA.onStart( []() 
  {
    String type;
    if ( ArduinoOTA.getCommand() == U_FLASH )
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println( "Start updating " + type );
  } );

  ArduinoOTA.onEnd( []()
  {
    Serial.println( "\nEnd" );
  } );

  ArduinoOTA.onProgress( []( unsigned int progress, unsigned int total )
  {
    Serial.printf( "Progress: %u%%\r", (progress / (total / 100)) );
  });

  ArduinoOTA.onError( [&]( ota_error_t error )
    {
      result = false;
//      Serial.printf( "Error[%u]: ", OTAErrorCode( error ) );
      Serial.print( "Error[" ); Serial.print( OTAErrorCode( error ) );
      Serial.println( "]:" );
      delay( 3000 );
    } );

  ArduinoOTA.begin();

  return result;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ==================== MQTT Class =================== //
/*                     ************                    /
class System: public MqttClient::System                 // Object class to provide system functions
{
  public:
  unsigned long millis() const
    {
      return ::millis();
    }

  void yield( void )
  {
    ::yield();
  }
}; //*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ==================== MQTT Setup =================== //
/*                     ************                    /
boolean MqttSetup()                                     // Configure client-object options
{
  MqttClient::System *mqttSystem                    = new System;

  MqttClient::Logger *mqttLogger                    = new MqttClient::LoggerImpl<HardwareSerial>( Serial );

  MqttClient::Network *mqttNetwork                  = new MqttClient::NetworkClientImpl<WiFiClient>( network, *mqttSystem );

  MqttClient::Buffer *mqttSendBuffer                = new MqttClient::ArrayBuffer<128>();

  MqttClient::Buffer *mqttRecvBuffer                = new MqttClient::ArrayBuffer<128>();

  MqttClient::MessageHandlers *mqttMessageHandlers  = new MqttClient::MessageHandlersImpl<2>();
                                                        // Allow up to 2 simultaneous subscriptions

  MqttClient::Options mqttOptions;                      // Configure client options                      

  mqttOptions.commandTimeoutMs = 10000;                 // Set command timeout to 10 seconds

  mqtt = new MqttClient(  mqttOptions,                  // Create client object
                          *mqttLogger,
                          *mqttSystem,
                          *mqttNetwork,
                          *mqttSendBuffer,
                          *mqttRecvBuffer,
                          *mqttMessageHandlers
  );
  return ( mqtt != NULL );
} //*/

