// Inlcude libraries
#include <SparkFunLSM9DS1.h>                      // IMU  Library
#include <Adafruit_VL6180X.h>                     // ToF  Library
#include <WiFi.h>                                 // WiFi Library
#include <PubSubClient.h>                         // MQTT Library

// Define important constants
#define LSM9DS1_M             0x1E                // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG            0x6B                // Would be 0x6A if SDO_AG is LOW
#define DECLINATION           6.29                // Accounting for the Earth's magnetic field
#define BAUDRATE              115200              // Serial communication baudrate
#define VL6180X_ADDRESS       0x29                // I2C Address    

/************************* WiFi Access Point *********************************/
#define WLAN_SSID             "pd3d_panels"
#define WLAN_PASS             "pd3d@ist"

/************************* MQTT Server Setup *********************************/
#define MQTT_SERVER           "192.168.42.1"
#define MQTT_PORT             1883                // use 8883 for SSL

// Use WiFiFlientSecure for SSL
WiFiClient espClient;                             // WiFiClient object to connect to MQTT
PubSubClient client(espClient);                   // MQTT publisher/subscriber

Adafruit_VL6180X ToF = Adafruit_VL6180X();        // Instantiate ToF
LSM9DS1 imu;                                      // Instantiate IMU sensor

// Include in-house libraries
#include "functions.h"

void setup() {
  Serial.begin( BAUDRATE );                       // Start serial monitor

//  setupIMU();                                     // Setup IMU sensor
//  if ( !imu.begin() )                             // Start IMU sensor
//  {
//    Serial.print( F("[INFO] IMU Error.\n") );
//    bool imu_not_ready = !imu.begin();                  
//    while( imu_not_ready )
//    {
//      imu_not_ready = !imu.begin();
//      Serial.print( "IMU Status: " );
//      Serial.println( imu_not_ready );
//    }
//  }

  if ( !ToF.begin() )                             // Start IMU sensor
  {
    Serial.println( F("[INFO] ToF Error.") );
    while (1);
  }

  setup_WiFi();                                   // Connect to WiFi
  client.setServer( MQTT_SERVER, MQTT_PORT );     // Point to the server
  client.setCallback( MQTT_onRecieve );           // Attach callback function
}

void loop() {

  if( !client.connected() )                       // This ensures we are always
  {                                               // connected to MQTT server
    MQTT_reconnect();                             // ...
  }                                               // ...
  if( !client.loop() )
  {
    client.connect( "ESP32" );
  }
  float lux = ToF.readLux(VL6180X_ALS_GAIN_5);
  uint8_t range = ToF.readRange();

  Serial.print( "====================\n" );
//  Serial.print( "Gyros: "   ); printGyro();         client.publish( "panel2/vo/imu/gyro", buff );
//  Serial.print( "\nAccel: " ); printAccel();        client.publish( "panel2/vo/imu/accel", buff );
//  Serial.print( "\nMagnt: " ); printMag();          client.publish( "panel2/vo/imu/magn", buff );
//  Serial.print( "\nAttde: " );
//  printAttitude(imu.ax, imu.ay, imu.az, -imu.my, -imu.mx, imu.mz);
//  client.publish( "panel2/vo/tof/att", buff );
  dtostrf( lux, 7, 2, buff);
  Serial.print( "\n\nLux  : " ); Serial.print(lux); client.publish( "panel2/vo/tof/lux", buff );
  if ( ToF.readRangeStatus() == VL6180X_ERROR_NONE )
  {
    dtostrf( range, 7, 2, buff);
    Serial.print("\nRange: "); Serial.println(range); client.publish( "panel2/vo/tof/dist", buff );
  } else {
    Serial.println( " " );
  }
  Serial.println( "====================\n" );
  delay( 1000 );

}
