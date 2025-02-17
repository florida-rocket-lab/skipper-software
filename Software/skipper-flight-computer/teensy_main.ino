#include "teensy.h"
#include "../skipper-lib/_wrapper.h"

IMUData get_imu_data()
{
  // TODO: retrieve IMU Data from a library/Drehmflight?
  return {};
}

ControlSignal run_GNC(const SkipperState& state, const SkipperFlag flags=SkipperFlag::Idle)
{
  // TODO: take the state struct and run GNC's control model to produce the control signals
  switch (flags)
  {
    case SkipperFlag::Idle:
      // run this GNC section whenever Skipper is doing absolutely nothing
      return {}; // this return might be fine just as it is
    case SkipperFlag::Armed:
      // run this GNC section whenever Skipper is on the ground getting ready to fly
      return {}; // this return definitely must change
    case SkipperFlag::Takeoff:
      // run this GNC section whenever Skipper is actively lifting up off the ground
      return {}; // this return definitely must change
    case SkipperFlag::Hover:
      // run this GNC section whenever Skipper is actively hovering
      return {}; // this return definitely must change
  }
}

void update_controls(const ControlSignal& control_signal)
{
  // TODO: take the floats and convert them to the actual signal that needs to get sent to the physical components
  // TODO: send the actual data to the components
}

Ground2Teensy read_message_from_nano()
{
  // TODO: set the connecting pin from the Teensy 4.0 to the Nano to HI, letting the Nano know that we're good to read.
  // TODO: read the streaming UART data from Nano
  // TODO: set the connecting pin from the Teensy 4.0 to the Nano to LOW, letting the Nano know to get ready for our message.
  return {};
}

void update_skipper_flags(const Ground2Teensy &command, enum class SkipperFlag &skipper_flag)
{
  // TODO: implement the command logic with skipper flag
  // if (command == "goto IDLE") skipper_flag = SkipperFlag::Idle;
  // if (command == "goto ARMED") skipper_flag = SkipperFlag::Armed;
  // if (command == "goto TAKEOFF") skipper_flag = SkipperFlag::Takeoff;
  // if (STOP_TAKEOFF?) skipper_flag = SkipperFlag::Hover;
  // if (command == "goto HOVER") skipper_flag = SkipperFlag::Hover;
}

void send_message_to_nano(const IMUData &id, const SkipperState &ss, const ControlSignal &cs)
{
  Teensy2Ground t2g {id, ss, cs};
  // TODO: serialize the data? or do something to prepare the data to be sent to the Nano.
  // TODO: send the data to the Nano via UART
}

void calibrateESC(){
  // calibrates both ESCs (should hear a beep after)
    esc1.attach(#PIN); // Connect ESC signal wire to pin 9
    esc2.attach(#PIN); // Connect ESC signal wire to pin 9
    esc1.writeMicroseconds(1000); // Start at minimum throttle
    esc2.writeMicroseconds(1000);
    delay(5000); // Wait 5 seconds before arming


}
IMUData imu_data {};
SkipperState skipper_state {};
EKF ekf {};
ControlSignal control_signal {};
Ground2Teensy command {};
SkipperFlag skipper_flag = SkipperFlag::Idle;

void setup() 
{
  // I don't know what needs to be initialized. Is there an initial UART handshake?
}

void loop() 
{
  imu_data = get_imu_data();
  skipper_state = ekf.filter_data(imu_data);
  control_signal = run_GNC(skipper_state, skipper_flag);
  update_controls(control_signal);
  command = read_message_from_nano();
  update_skipper_flags(command, skipper_flag);
  send_message_to_nano(imu_data, skipper_state, control_signal);
}


// ----------------------- Start of dhremFlight IMU code ----------------------------
void IMUinit() {
  //DESCRIPTION: Initialize IMU
  /*
   * Don't worry about how this works.
   */
  #if defined USE_MPU6050_I2C
    Wire.begin();
    Wire.setClock(1000000); //Note this is 2.5 times the spec sheet 400 kHz max...
    
    mpu6050.initialize();
    
    if (mpu6050.testConnection() == false) {
      Serial.println("MPU6050 initialization unsuccessful");
      Serial.println("Check MPU6050 wiring or try cycling power");
      while(1) {}
    }

    //From the reset state all registers should be 0x00, so we should be at
    //max sample rate with digital low pass filter(s) off.  All we need to
    //do is set the desired fullscale ranges
    mpu6050.setFullScaleGyroRange(GYRO_SCALE);
    mpu6050.setFullScaleAccelRange(ACCEL_SCALE);
    
  #elif defined USE_MPU9250_SPI
    int status = mpu9250.begin();    

    if (status < 0) {
      Serial.println("MPU9250 initialization unsuccessful");
      Serial.println("Check MPU9250 wiring or try cycling power");
      Serial.print("Status: ");
      Serial.println(status);
      while(1) {}
    }

    //From the reset state all registers should be 0x00, so we should be at
    //max sample rate with digital low pass filter(s) off.  All we need to
    //do is set the desired fullscale ranges
    mpu9250.setGyroRange(GYRO_SCALE);
    mpu9250.setAccelRange(ACCEL_SCALE);
    mpu9250.setMagCalX(MagErrorX, MagScaleX);
    mpu9250.setMagCalY(MagErrorY, MagScaleY);
    mpu9250.setMagCalZ(MagErrorZ, MagScaleZ);
    mpu9250.setSrd(0); //sets gyro and accel read to 1khz, magnetometer read to 100hz
  #endif
}

void getIMUdata() {
  //DESCRIPTION: Request full dataset from IMU and LP filter gyro, accelerometer, and magnetometer data
  /*
   * Reads accelerometer, gyro, and magnetometer data from IMU as AccX, AccY, AccZ, GyroX, GyroY, GyroZ, MagX, MagY, MagZ. 
   * These values are scaled according to the IMU datasheet to put them into correct units of g's, deg/sec, and uT. A simple first-order
   * low-pass filter is used to get rid of high frequency noise in these raw signals. Generally you want to cut
   * off everything past 80Hz, but if your loop rate is not fast enough, the low pass filter will cause a lag in
   * the readings. The filter parameters B_gyro and B_accel are set to be good for a 2kHz loop rate. Finally,
   * the constant errors found in calculate_IMU_error() on startup are subtracted from the accelerometer and gyro readings.
   */
  int16_t AcX,AcY,AcZ,GyX,GyY,GyZ; //MgX,MgY,MgZ;  // commeted since not being used


  #if defined USE_MPU6050_I2C
    mpu6050.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);
  #elif defined USE_MPU9250_SPI
    mpu9250.getMotion9(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ, &MgX, &MgY, &MgZ);
  #endif

 //Accelerometer
  AccX = AcX / ACCEL_SCALE_FACTOR; //G's
  AccY = AcY / ACCEL_SCALE_FACTOR;
  AccZ = AcZ / ACCEL_SCALE_FACTOR;
  //Correct the outputs with the calculated error values
  AccX = AccX - AccErrorX;
  AccY = AccY - AccErrorY;
  AccZ = AccZ - AccErrorZ;
  //LP filter accelerometer data
  AccX = (1.0 - B_accel)*AccX_prev + B_accel*AccX;
  AccY = (1.0 - B_accel)*AccY_prev + B_accel*AccY;
  AccZ = (1.0 - B_accel)*AccZ_prev + B_accel*AccZ;
  AccX_prev = AccX;
  AccY_prev = AccY;
  AccZ_prev = AccZ;

  //Gyro
  GyroX = GyX / GYRO_SCALE_FACTOR; //deg/sec
  GyroY = GyY / GYRO_SCALE_FACTOR;
  GyroZ = GyZ / GYRO_SCALE_FACTOR;
  //Correct the outputs with the calculated error values
  GyroX = GyroX - GyroErrorX;
  GyroY = GyroY - GyroErrorY;
  GyroZ = GyroZ - GyroErrorZ;
  //LP filter gyro data
  GyroX = (1.0 - B_gyro)*GyroX_prev + B_gyro*GyroX;
  GyroY = (1.0 - B_gyro)*GyroY_prev + B_gyro*GyroY;
  GyroZ = (1.0 - B_gyro)*GyroZ_prev + B_gyro*GyroZ;
  GyroX_prev = GyroX;
  GyroY_prev = GyroY;
  GyroZ_prev = GyroZ;

  //Magnetometer 
  // commeted since not being used
  // MagX = MgX/6.0; //uT
  // MagY = MgY/6.0;
  // MagZ = MgZ/6.0;
  //Correct the outputs with the calculated error values
  MagX = (MagX - MagErrorX)*MagScaleX;
  MagY = (MagY - MagErrorY)*MagScaleY;
  MagZ = (MagZ - MagErrorZ)*MagScaleZ;
  //LP filter magnetometer data
  MagX = (1.0 - B_mag)*MagX_prev + B_mag*MagX;
  MagY = (1.0 - B_mag)*MagY_prev + B_mag*MagY;
  MagZ = (1.0 - B_mag)*MagZ_prev + B_mag*MagZ;
  MagX_prev = MagX;
  MagY_prev = MagY;
  MagZ_prev = MagZ;
}

oid calculate_IMU_error() {
  //DESCRIPTION: Computes IMU accelerometer and gyro error on startup. Note: vehicle should be powered up on flat surface
  /*
   * Don't worry too much about what this is doing. The error values it computes are applied to the raw gyro and 
   * accelerometer values AccX, AccY, AccZ, GyroX, GyroY, GyroZ in getIMUdata(). This eliminates drift in the
   * measurement. 
   */
  int16_t AcX,AcY,AcZ,GyX,GyY,GyZ; //MgX,MgY,MgZ;
  AccErrorX = 0.0;
  AccErrorY = 0.0;
  AccErrorZ = 0.0;
  GyroErrorX = 0.0;
  GyroErrorY= 0.0;
  GyroErrorZ = 0.0;
  
  //Read IMU values 12000 times
  int c = 0;
  while (c < 12000) {
  
    mpu6050.getMotion6(&AcX, &AcY, &AcZ, &GyX, &GyY, &GyZ);
   
    
    AccX  = AcX / ACCEL_SCALE_FACTOR;
    AccY  = AcY / ACCEL_SCALE_FACTOR;
    AccZ  = AcZ / ACCEL_SCALE_FACTOR;
    GyroX = GyX / GYRO_SCALE_FACTOR;
    GyroY = GyY / GYRO_SCALE_FACTOR;
    GyroZ = GyZ / GYRO_SCALE_FACTOR;
    
    //Sum all readings
    AccErrorX  = AccErrorX + AccX;
    AccErrorY  = AccErrorY + AccY;
    AccErrorZ  = AccErrorZ + AccZ;
    GyroErrorX = GyroErrorX + GyroX;
    GyroErrorY = GyroErrorY + GyroY;
    GyroErrorZ = GyroErrorZ + GyroZ;
    c++;
  }
  //Divide the sum by 12000 to get the error value
  AccErrorX  = AccErrorX / c;
  AccErrorY  = AccErrorY / c;
  AccErrorZ  = AccErrorZ / c - 1.0;
  GyroErrorX = GyroErrorX / c;
  GyroErrorY = GyroErrorY / c;
  GyroErrorZ = GyroErrorZ / c;

  Serial.print("float AccErrorX = ");
  Serial.print(AccErrorX);
  Serial.println(";");
  Serial.print("float AccErrorY = ");
  Serial.print(AccErrorY);
  Serial.println(";");
  Serial.print("float AccErrorZ = ");
  Serial.print(AccErrorZ);
  Serial.println(";");
  
  Serial.print("float GyroErrorX = ");
  Serial.print(GyroErrorX);
  Serial.println(";");
  Serial.print("float GyroErrorY = ");
  Serial.print(GyroErrorY);
  Serial.println(";");
  Serial.print("float GyroErrorZ = ");
  Serial.print(GyroErrorZ);
  Serial.println(";");

  Serial.println("Paste these values in user specified variables section and comment out calculate_IMU_error() in void setup.");
}

// ----------------------- End of dhremFlight IMU code ----------------------------
