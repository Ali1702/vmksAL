#include <SoftwareSerial.h>
#include <Wire.h>

const byte rxpin = 9;
const byte txpin = 8;
SoftwareSerial hc06(rxpin, txpin);

   // X axis pointing forward
   // Y axis pointing to the left
   // and Z axis pointing up.
int SENSOR_SIGN[9] = {1,-1,-1,-1,1,1,1,-1,-1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
// accelerometer: 8 g sensitivity
// 3.9 mg/digit; 1 g = 256
#define GRAVITY 256  //this is equivalent to 1G in the raw data coming from the accelerometer

long timer = 0;  
long timer_old = 0;
long diff_time = 0;
int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors
float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
int accel_x;
int accel_y;
int accel_z;


void setup()
{
  pinMode(rxpin, INPUT);
  pinMode(txpin, OUTPUT);
  Serial.begin(115200);
  hc06.begin(9600);

  I2C_Init();

  Serial.println("Initializing...");
  delay(1500);

  Accel_Init();
  delay(20);

  for(int i=0;i<32;i++)    // We take some readings...
    {
      Read_Accel();

      for(int y=0; y<6; y++)   // Cumulate values
        AN_OFFSET[y] += AN[y];
      delay(10);
    }

  for(int y=0; y<6; y++)
    AN_OFFSET[y] = AN_OFFSET[y]/32;

  AN_OFFSET[5] -= GRAVITY * SENSOR_SIGN[5];

  delay(2000);
  timer_old = 0;
  timer = 0;
  delay(20);
}
  float total_accel;
  float diff_velocity = 0;

void loop()
{
  Read_Accel();
  
  Accel_Vector[0]=accel_x;
  Accel_Vector[1]=accel_y;
  Accel_Vector[2]=accel_z;

  total_accel = sqrt(Accel_Vector[0]*Accel_Vector[0] + Accel_Vector[1]*Accel_Vector[1] + Accel_Vector[2]*Accel_Vector[2]);
  total_accel = total_accel / GRAVITY; // Scale to gravity.
  total_accel -= 1;

  if(total_accel < 0)
    total_accel = 0;
  total_accel *= 9.80665;

  timer_old = timer;
  timer = millis();
  diff_time = timer - timer_old;
  diff_velocity = total_accel * (diff_time);
  diff_velocity /= 1000;


  if(diff_velocity != 0.00 || total_accel != 0.00) {
    hc06.print("A:");
    hc06.print(diff_velocity);
    hc06.print(",");
    hc06.print(total_accel);
    hc06.println(); 

    Serial.print("Velocity:");
    Serial.print( diff_velocity);
    Serial.print("m/s");
    Serial.print("  Acceleration:");
    Serial.print( total_accel);
    Serial.println("m/s^2");
  }

  delay(10);
}