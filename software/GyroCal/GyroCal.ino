#include <Wire.h>
#include <L3G4200D.h>

#if defined (CORE_TEENSY)
// Teensy uses Serial2 for data logger
#define BEGIN(x) Serial2.begin((x))
#define PRINT(x) Serial2.print((x))
#define PRINTF(x,y) Serial2.print((x),(y))
#define PRINTLN(x) Serial2.println((x))
#define PRINTLNF(x,y) Serial2.println((x),(y))

#else
// Assume running on an Uno, just use the normal serial port
#define BEGIN(x) Serial.begin((x))
#define PRINT(x) Serial.print((x))
#define PRINTF(x,y) Serial.print((x),(y))
#define PRINTLN(x) Serial.println((x))
#define PRINTLNF(x,y) Serial.println((x),(y))

#endif

L3G4200D gyro(false); // Assume SDO pin is LOW
uint8_t error;
int rate;

void setup() {
  BEGIN(57600);
  
  Wire.begin();
  
  PRINTLN( "Initializing gyro at 2000 dps" );
  rate = 2000;  
  error = gyro.Initialize(2000);
  if ( error ) {
    PRINT("ERROR! ");
    PRINTLN(gyro.GetErrorText(error));
  }
  
  PRINTLN(gyro.CheckWhoAmI() ? 
  "Gyro Who Am I test PASSED" :
  "Gyro Who Am I test FAILED");
  
  PRINTLN("Waiting 10 seconds to begin stationary calibration");
  delay(10000);
}

void loop() {
  float X = 0;
  float Y = 0;
  float Z = 0;
  float MinX = 0;
  float MaxX = 0;
  float MinY = 0;
  float MaxY = 0;  
  float MinZ = 0;
  float MaxZ = 0;
  bool bFirst = true;
  int i = 0;

  PRINT( "Starting calibration at " );
  PRINT( rate );
  PRINTLN( " dps." );
  while (i++ < 100) {
    GyroScaled gyroscaled = gyro.ReadScaledAxis();
    X += gyroscaled.XRate;
    Y += gyroscaled.YRate;    
    Z += gyroscaled.ZRate; 
    
    if ( bFirst || gyroscaled.XRate < MinX )
      MinX = gyroscaled.XRate;
    if ( bFirst || gyroscaled.XRate > MaxX )
      MaxX = gyroscaled.XRate;
    if ( bFirst || gyroscaled.YRate < MinY )
      MinY = gyroscaled.YRate;
    if ( bFirst || gyroscaled.YRate > MaxY )
      MaxY = gyroscaled.YRate;      
    if ( bFirst || gyroscaled.ZRate < MinZ )
      MinZ = gyroscaled.ZRate;
    if ( bFirst || gyroscaled.ZRate > MaxZ )
      MaxZ = gyroscaled.ZRate;
    bFirst = false;
    delay(20);
  }
  PRINT( "X: Avg " );
  PRINT( X / 100.0 );
  PRINT( " Min " );
  PRINT( MinX );  
  PRINT( " Max " );
  PRINT( MaxX ); 
      
  PRINT( "     Y: Avg " );
  PRINT( Y / 100.0 );
  PRINT( " Min " );
  PRINT( MinY );  
  PRINT( " Max " );
  PRINT( MaxY ); 

  PRINT( "     Z: Avg " );
  PRINT( Z / 100.0 );
  PRINT( " Min " );
  PRINT( MinZ );  
  PRINT( " Max " );
  PRINTLN( MaxZ ); 

  if ( rate == 2000 )
    rate = 500;
  else if ( rate == 500 )
    rate = 250;
  else if ( rate == 250 )
    rate = 2000;
  gyro.SetRate( rate );
  delay( 2000 );  
}
