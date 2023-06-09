#include <LSM6.h>
//#include <LIS3MDL.h>

LSM6 gyro_acc;

void I2C_Init()
{
  Wire.begin();
}

void Accel_Init()
{
  gyro_acc.init();
  gyro_acc.enableDefault();
  gyro_acc.writeReg(LSM6::CTRL1_XL, 0x3C); // 52 Hz, 8 g full scale
}

// Reads x,y and z accelerometer registers
void Read_Accel()
{
  gyro_acc.readAcc();

  AN[3] = gyro_acc.a.x >> 4; // shift right 4 bits to use 12-bit representation (1 g = 256)
  AN[4] = gyro_acc.a.y >> 4;
  AN[5] = gyro_acc.a.z >> 4;

  accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
  accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
  accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
}