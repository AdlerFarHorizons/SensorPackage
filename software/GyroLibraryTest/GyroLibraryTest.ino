#include <Wire.h>
#include <L3G4200D.h>

L3G4200D gyro(false); // Assume SDO pin is LOW
uint8_t error;

void setup() {
  Serial.begin(57600);
  
  Wire.begin();
  
  Serial.println( "Initializing gyro at 2000 dps" );
  error = gyro.Initialize(2000);
  if ( error ) {
    Serial.print("ERROR! ");
    Serial.println(gyro.GetErrorText(error));
  }
  
  Serial.println(gyro.CheckWhoAmI() ? 
  "Gyro Who Am I test PASSED" :
  "Gyro Who Am I test FAILED");
}

void loop() {
  GyroRaw gyroraw = gyro.ReadRawAxis();
  GyroScaled gyroscaled = gyro.Raw2Scaled(gyroraw);
  
  Serial.print( gyroraw.XRate );
  Serial.print( "," );  
  Serial.print( gyroraw.YRate );
  Serial.print( "," );
  Serial.print( gyroraw.ZRate );  
  Serial.print( "," );
  Serial.print( gyroscaled.XRate );
  Serial.print( "," );  
  Serial.print( gyroscaled.YRate );
  Serial.print( "," );
  Serial.println( gyroscaled.ZRate );  
  delay(100);  
}
