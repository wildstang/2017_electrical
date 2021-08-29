#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
//#include <Adafruit_Simple_AHRS.h>
//#include "Average.h"
#include "math.h"

#define LSM9DS0_XM_CS 10
#define LSM9DS0_GYRO_CS 9

#define LSM9DS0_SCLK 13
#define LSM9DS0_MISO 12
#define LSM9DS0_MOSI 11
//(SPI)
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(LSM9DS0_SCLK, LSM9DS0_MISO, LSM9DS0_MOSI, LSM9DS0_XM_CS, LSM9DS0_GYRO_CS, 1000);

int number = 1;
int wait = 5;

const int NUM_SAMPLES = 5;

#define I2C_ADDRESS 0x20

float avgMX[NUM_SAMPLES];
float avgMY[NUM_SAMPLES];
float avgMZ[NUM_SAMPLES];

boolean Failure = false;

float mX, mY, mZ = 0;

byte compassDiv2 = 0;



/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561
*/
/**************************************************************************/
void configureSensor(void)
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);

  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
}

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Wire.begin(I2C_ADDRESS);
  Wire.onRequest(ReturnIMU);

  /* Initialise the sensor */
  if (!lsm.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    while (1);
  }

  /* Setup the sensor gain and integration time */
  configureSensor();
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t accel, mag, gyro, temp;

  lsm.getEvent(&accel, &mag, &gyro, &temp);

  int j = 0;
  for (j; j < NUM_SAMPLES; j++)
  { 
    avgMX[j] = (mag.magnetic.x);
    avgMY[j] = (mag.magnetic.y);
    avgMZ[j] = (mag.magnetic.z);
  }

  mX = mean(avgMX, NUM_SAMPLES);
  mY = mean(avgMY, NUM_SAMPLES);
  mZ = mean(avgMZ, NUM_SAMPLES);

  // Divide heading value by 2 to put in range 0-180 for a single byte
  compassDiv2 = SuperCompass(mX, mY) / 2;

  ReturnIMU();

  number ++;

}

float mean(float values[], int size)
{
  float total = 0;
  
  for (int i = 0; i < size; i++)
  {
    total += values[i];
  }
  
  return total / size;
}

double SuperCompass(float x, float y)
{
  double Direction;

  if (y > 0)
  {
    Direction = 90 - (atan(x / y)) * 180 / 3.14159;
  }

  if (y < 0)
  {
    Direction = 270 - (atan(x / y)) * 180 / 3.14159;
  }

  if (y = 0 && x < 0)
  {
    Direction = 180.0;
  }

  if (y = 0 && x > 0)
  {
    Direction = 0.0;
  }
  return Direction;
}

void ReturnIMU()
{
  //  Wire.beginTransmission(RioAddress);

  if (Wire.write(compassDiv2) == 1)//imuArray, 14) == 14)
  {
    Failure = false;
  }
  else
  {
    Wire.write(255);
//    Wire.endTransmission();
    Failure = true;
  }

}

