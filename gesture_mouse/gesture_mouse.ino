/**
 * This Mouse.h and Keyboard.h libraries don't run on ATmega328P(Arduino Uno) instead it requires ATmega32u4(Arduino leonardo) type microcontroller
 * Go to Tools->Board and Select board as Arduino Leonardo
 */
#include<Mouse.h>
#include<Wire.h>

int gyro_x;
int gyro_y;
int gyro_z; 

int sens=20;          //Sensitivity of the mouse
void setup() {
  Wire.begin();
  Serial.begin(9600);
  setupmpu();
  Serial.println("MPU6050 ready");
}

void loop() {
                      // Manual I2C protocol. You can also use other MPU6050 libraries to get the gyroscope data
  Wire.beginTransmission(0b1101000);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);    //request 6 bits from Gyroscope sensor
  
  while(Wire.available()<6);
  gyro_x=Wire.read()<<8|Wire.read();
  gyro_y=Wire.read()<<8|Wire.read();
  gyro_z=Wire.read()<<8|Wire.read();

        //Computing the Gyroscope data
  gyro_x/=131;
  gyro_y/=131;
  gyro_z/=131;
  
  gyro_x=gyro_x+3;      // correction of the reading depending on the sensor
        
                        // For detecting motion
  if(gyro_x<=100 && gyro_x>=-100)
    gyro_x=0;
  if(gyro_y<=100 && gyro_y>=-100)
    gyro_y=0;

                        //Depending on the screen resolution
  gyro_x=constrain(gyro_x,-1366,1366);
  gyro_y=constrain(gyro_y,-768,768);

  gyro_x=map(gyro_x,-1366,1366,-sens,sens);
  gyro_y=map(gyro_y,-768,768,-sens,sens);

  Mouse.move(gyro_x,gyro_y);

  Serial.println(gyro_x);
  Serial.println(gyro_y);
}

void setupmpu()
{
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00001000);         //Disable temperature sensor
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B);
  Wire.write(0b00000000);         //Setting the full scale range to 250degrees/s
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0b00000000);        //Setting the full scale range to 2g
  Wire.endTransmission();
}
