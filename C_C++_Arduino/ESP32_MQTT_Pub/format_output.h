/*
   Format the data output
*/

char    buff[21]             = {0};                    // String buffer

// =======================  Format 1-Valued Output  ====================
void format_old(float reading) {
  // dtostrf(floatVar, StringLength, decimalPts, buff_arr)

  //strcpy( buff, " " );                            // Appends the converted float-string
  //dtostrf( reading, 7, 2, &buff[strlen(buff)]);   // to the end of the "buff" array!
  dtostrf( reading, 7, 2, buff);
  Serial.print( buff );
}

// =======================  Format 3-Valued Output  ====================
String format( float val_1, float val_2, float val_3 ) {
  // Format a floating variable into a string
  //    dtostrf(floatVar, StringLength, decimalPts, buff_arr)

  // Concatenate two string
  //    strcat( dst, src )

  // Copy a string into another, overwriting content
  //    strcpy( dst, src );

  // Append float-string to end of another string
  //    dtostrf( reading, 7, 2, &buff[strlen(buff)]);

  //strcpy( buff, " " );                            // Appends the converted float-string
  //dtostrf( reading, 7, 2, &buff[strlen(buff)]);   // to the end of the "buff" array!
  dtostrf( val_1, 7, 2, buff);
  strcat( buff, "," );

  dtostrf( val_2, 7, 2, &buff[strlen(buff)]);
  strcat( buff, "," );

  dtostrf( val_3, 7, 2, &buff[strlen(buff)]);
  //strcat( buff, "," );

  Serial.print( buff );
}
