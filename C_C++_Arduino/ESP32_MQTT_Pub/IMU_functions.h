/*
 * IMU functions to compute spatial position and heading
 */

// =======================  Print Gyroscopic data   ====================
void printGyro() {
  imu.readGyro();         // (imu.gx, ...gy, ...gz) [deg/s]
  //Serial.print("GYR,");

  format( imu.calcGyro(imu.gx), imu.calcGyro(imu.gy), imu.calcGyro(imu.gz) );

}


// ======================= Print Acceleration  Data ====================
void printAccel() {
  // Reading raw data from accelerometer
  imu.readAccel(); // (imu.ax, ...ay, ...az) [g]
  //Serial.print("ACC,");

  format( imu.calcAccel(imu.ax), imu.calcAccel(imu.ay), imu.calcAccel(imu.az) );

}


// =======================  Print Magnetometer data ====================
void printMag() {
  // Reading raw data from magnetometer
  imu.readMag(); // (imu.mx, ...my, ...mz) [gauss]
  //Serial.print("MAG,");

  format( imu.calcMag(imu.mx), imu.calcMag(imu.my), imu.calcMag(imu.mz) );

}


// ===================  Calculate Pitch, Roll, Heading  ================
/*
 * Pitch/roll calculations take from this app note:
 * http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
 * Heading calculations taken from this app note:
 * http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
 */
void printAttitude(float ax, float ay, float az, float mx, float my, float mz) {
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));

  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);

  heading -= DECLINATION * PI / 180;

  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;

  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;

  //Serial.print("PIT,");
  format( pitch, roll, heading );

}
