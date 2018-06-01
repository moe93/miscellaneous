/*
 * Configure all aspects of the IMU sensor
 */

// =========================    Setup IMU       ========================
void setupIMU() {
  
  imu.settings.device.commInterface = IMU_MODE_I2C;     //
  imu.settings.device.mAddress      = LSM9DS1_M;        // Load IMU settings
  imu.settings.device.agAddress     = LSM9DS1_AG;       //

  imu.settings.gyro.enabled         = true;             // Enable gyro
  imu.settings.accel.enabled        = true;             // Enable accelerometer
  imu.settings.mag.enabled          = true;             // Enable magnetometer
  imu.settings.temp.enabled         = true;             // Enable temperature sensor

  imu.settings.mag.scale            = 16;               // Set mag scale to +/-12 Gs
  imu.settings.mag.sampleRate       = 7;                // Set sampling rate to 80Hz
  imu.settings.mag.tempCompensationEnable = true;       // Enable temperature compensation (good stuff!)

  imu.settings.mag.XYPerformance    = 3;                // 0 = Low || 1 = Medium || 2 = High || 3 = Ultra-high
  imu.settings.mag.ZPerformance     = 3;                // Ultra-high performance

  imu.settings.mag.lowPowerEnable   = false;            // Disable low power mode
  imu.settings.mag.operatingMode    = 0;                // 0 = Continuous || 1 = Single || 2 = OFF

}
