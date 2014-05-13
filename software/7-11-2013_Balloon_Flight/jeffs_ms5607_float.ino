#include <Wire.h>
#define ADDRESS 0x76 //0x77

float D1 = 0;
float D2 = 0;
float dT = 0;
float TEMP = 0;
float OFF = 0; 
float SENS = 0; 
float P = 0;
float P2 = 0;
uint16_t C[7];

//D1 = getVal(ADDRESS, 0x48); // Pressure raw
//D2 = getVal(ADDRESS, 0x58);// Temperature raw

void setup() {
  
// Disable internal pullups, 10Kohms are on the breakout
 PORTC |= (1 << 4);
 PORTC |= (1 << 5);

Wire.begin();
  Serial.begin(9600); //9600 changed 'cos of timing?
  delay(100);
  
}  


// D1 = getVal(ADDRESS, 0x48); // Pressure raw
// D2 = getVal(ADDRESS, 0x58);// Temperature raw

void loop()
{
//Get raw Pressure
Wire.beginTransmission(ADDRESS);
Wire.write(0x48);
Wire.endTransmission();
delay(10);
  // start read sequence
Wire.beginTransmission(ADDRESS);
Wire.write((byte) 0x00);
Wire.endTransmission();
Wire.beginTransmission(ADDRESS);
Wire.requestFrom(ADDRESS, (int)3);

if (Wire.available() >= 3)
  {
    D1 = Wire.read() * (unsigned long)65536 + Wire.read() * (unsigned long)256 + Wire.read();
  }
  else {
    D1 = -1;
  }




//Get raw Temp
Wire.beginTransmission(ADDRESS);
Wire.write(0x58);
Wire.endTransmission();
delay(10);
  // start read sequence
Wire.beginTransmission(ADDRESS);
Wire.write((byte) 0x00);
Wire.endTransmission();
Wire.beginTransmission(ADDRESS);
Wire.requestFrom(ADDRESS, (int)3);

if (Wire.available() >= 3)
  {
    D2 = Wire.read() * (unsigned long)65536 + Wire.read() * (unsigned long)256 + Wire.read();
  }
  else {
    D2 = -1;
  }
  
    Serial.println("******************************************************");
    
  Serial.print("RAW Temp D2= ");
  Serial.print(D2);
  Serial.print(" RAW Pressure D1= ");
  Serial.println(D1);
  Serial.println();
  Serial.println("PROM COEFFICIENTS ivan");
  
  
   //get the calibration coeffiecnts 

  Wire.beginTransmission(ADDRESS);
  Wire.write(0x1E); // reset
  Wire.endTransmission();
  delay(10);

  
  
 

  for (int i=0; i<6  ; i++) {

    Wire.beginTransmission(ADDRESS);
    Wire.write(0xA2 + (i * 2));
    Wire.endTransmission();

    Wire.beginTransmission(ADDRESS);
    Wire.requestFrom(ADDRESS, 6);
    delay(1);
    if(Wire.available())
    {
       C[i+1] = Wire.read() << 8 | Wire.read();
    }
    else {
      Serial.println("Error reading PROM 1"); // error reading the PROM or communicating with the device
    }
    Serial.println(C[i+1]);
  }
  Serial.println();




//dT   = D2 - ((uint32_t)C[5] << 8);
dT   = D2 - ((uint32_t)C[5] * 256);

//OFF  = ((int64_t)C[2] << 16) + ((dT * C[4]) >> 7);
OFF  = ((int64_t)C[2] * 131072) + ((dT * C[4]) / 64);

//SENS = ((int32_t)C[1] << 15) + ((dT * C[3]) >> 8);
SENS = ((int32_t)C[1] * 65536) + (dT * C[3] / 128);

TEMP = (int64_t)dT * (int64_t)C[6] / 8388608 + 2000;
P  = ((int64_t)D1 * SENS / 2097152 - OFF) / 32768;
P2 = (float)P / 100;

Serial.println();
Serial.print("dT = ");
Serial.println(dT);
Serial.print("OFF = ");
Serial.println(OFF); 
Serial.print("SENS = ");
Serial.println(SENS); 
Serial.print("TEMP = ");
Serial.println(TEMP); 
Serial.print("P = ");
Serial.println(P); 
Serial.print("P2 = ");
Serial.println(P2); 



  delay(1000);

}
