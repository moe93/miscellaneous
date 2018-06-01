#define               SCL_PIN              4
#define               SDA_PIN              5
#define               I2C_ADDR          0x3C

SSD1306               display( I2C_ADDR, SDA_PIN, SCL_PIN );

String                ID                = "ID - ";
String                _title            = "";
String                _headline         = "";
String                _lastline         = "";
String                _datalineL        = "";
String                _datalineR        = "";

String                dispLine[4]   = {
                                            "          ",
                                            "          ",
                                            "          ",
                                            "          "
                                          };


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ============== Splash-screen on OLED ============== //
/*               ***********************               */
void splashScreen()
{
  display.clear();
  display.setColor( WHITE );
  display.setFont( ArialMT_Plain_16 );
  display.setTextAlignment( TEXT_ALIGN_CENTER );
  display.drawString( 64, 16, _title   );
  display.display();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// =============== Device name on OLED =============== //
/*                *********************                */
void headLine()
{
  //display.clear();
  display.setFont( ArialMT_Plain_16 );
  display.setTextAlignment( TEXT_ALIGN_CENTER );
  display.drawString( 64, 0, _headline );
  display.display();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// =============== ID centered on OLED =============== //
/*                *********************                */
void idLine( String id )
{
//  mqttID.concat( id );
  ID.concat( id );
  display.setFont( ArialMT_Plain_10 );
  display.setTextAlignment( TEXT_ALIGN_CENTER );
  display.drawString( 63, 24, id );
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ========== Message on last line of OLED =========== //
///*           ******************************            */
//void lastLine( String s, const char* fSize )
//{
//    int vPos = 52;
//    if ( strcmp( fSize, ArialMT_Plain_10 ) ) vPos = 46;
//
//    display.clear();
//
//    display.setTextAlignment( TEXT_ALIGN_CENTER );			// reprint the head line
//    display.setFont( ArialMT_Plain_16 );
//    display.drawString( 64, 0, _headline );
//
//    display.setTextAlignment( TEXT_ALIGN_LEFT );			  // reprint the left-side data line
//    display.drawString( 0, 36, _datalineL );
//    
//    display.setTextAlignment( TEXT_ALIGN_RIGHT );       // reprint the right-side data line
//    display.drawString( 127, 36, _datalineR );
//    
//    display.setTextAlignment( TEXT_ALIGN_CENTER );			// reprint the device ID line
//    display.setFont( ArialMT_Plain_10 );
//    display.drawString( 63, 24, ID );
//    
//    display.setFont( fSize );
//    display.drawString( 63, vPos, s );						      // add the scenario (small font)
//    display.display();
//    _lastline = s;
//}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ========== Message on data line of OLED =========== //
/*           ******************************            */
void dataLine( String sL, String sR )
{
    display.clear();

    display.setTextAlignment( TEXT_ALIGN_CENTER );			// reprint the head line
    display.setFont( ArialMT_Plain_16 );
    display.drawString( 64, 0, _headline );

    display.setTextAlignment( TEXT_ALIGN_LEFT );			  // add the left-side data line
    display.drawString( 0, 36, sL );

    display.setTextAlignment( TEXT_ALIGN_RIGHT );       // add the right-side data line
    display.drawString( 127, 36, sR );
    
    display.setTextAlignment( TEXT_ALIGN_CENTER );			// reprint the device ID line
    display.setFont( ArialMT_Plain_10 );
    display.drawString( 63, 24, ID );
    
    display.drawString( 63, 52, _lastline );				    // add the scenario (small font)
    display.display();
    _datalineL = sL;
    _datalineR = sR;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ============= Individual line on OLED ============= //
/*               ***********************               */
void parseLine( uint8_t line, uint8_t posL, uint8_t posC, uint8_t posR, String s, char c )
{
  String Lstring = s.substring( 0, s.indexOf( c ) );
  String Rstring = s.substring( s.indexOf( c ) + 1 );

  display.setTextAlignment( TEXT_ALIGN_LEFT );
  display.drawString( posL, line, Lstring );

  display.drawString( posC, line, String( c ) );

  display.setTextAlignment( TEXT_ALIGN_RIGHT );
  display.drawString( posR, line, Rstring );
}

void outputLine( uint8_t lineNum, String lineVal )
{
  //dispLine[lineNum] = lineVal;
  display.clear();
  display.setColor( WHITE );
  display.setFont( ArialMT_Plain_16 );
  display.setTextAlignment( TEXT_ALIGN_LEFT );
  for ( int i = 0; i < 4; i++ )
  {
    //parseLine( (16 * i), 10, 75, 120, dispLine[i], ':' );
    parseLine( (16 * i), 10, 60, 120, dispLine[i], ':' );
  }
  display.display();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * */
// =============== SSD1306--OLED Setup =============== //
/*                *********************                */
int OledSetup( String title, String headline, uint8_t pauseTime )
{
  _title    = title;
  _headline = headline;
  display.init();
  display.flipScreenVertically();
  splashScreen();
  delay( pauseTime * 10 );
  headLine();
//idLine();       // has to be called after WiFi init
  display.display();
  delay( pauseTime * 10 );
  return 0;
} //*/


/*
String lookupID( String mac )
{
  String s = "";
  if (      mac == "30:AE:A4:0E:8E:C0" ) s = "01";
  else if ( mac == "30:AE:A4:21:6D:20" ) s = "02";
  else if ( mac == "30:AE:A4:20:D4:F8" ) s = "03";
  else Serial.println( "This MAC isn't listed." );
  return s;
} //*/

