#include <Wire.h>
#include <math.h>
#include <HMC5883LFH_T.h>


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

// Store our compass as a variable.
HMC5883L compass;
int error = 0;
#define mag 0x1E //0011110b, I2C 7bit address of HMC5883

int16_t maxX;
int16_t minX;
int16_t maxY;
int16_t minY;
int16_t maxZ;
int16_t minZ;
bool bFirst = true;

#define ISVALIDMAG(x) ( (x) < 2047 && (x) > -2048 )

//#define ISVALIDMAG(x) ( 1 )

void setup() {
  
  BEGIN(57600);
  delay(1000); //Give the open log a second to get in gear. 

  Wire.begin();
  compass = HMC5883L(); // Construct a new HMC5883 compass.

  PRINTLN("Setting scale to +/- 1.3 Ga"); 
  error = compass.SetScale(1.3F); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    PRINTLN(compass.GetErrorText(error));
  
  PRINTLN("Setting measurement mode to continous.");
  compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  
  PRINT("Setting config register A to 0x");
  PRINTLNF((Averaging_1 | Rate_15 | Bias_Normal), HEX );
  compass.SetConfiguration( Averaging_4, Rate_15, Bias_Normal );
}


void loop()
{
  MagnetometerRaw raw = compass.ReadRawAxis();
  
  bool bChange = false;
  
  if ( ! ISVALIDMAG(raw.XAxis) || ! ISVALIDMAG(raw.YAxis) || ! ISVALIDMAG(raw.ZAxis) ) {
    PRINT( "Invalid data: " );
    PRINT( raw.XAxis );
    PRINT( ", " );
    PRINT( raw.YAxis );
    PRINT( ", " );
    PRINTLN( raw.ZAxis );
    return; 
  }
  
  if ( bFirst ) {
    maxX = raw.XAxis;
    minX = raw.XAxis;   
    maxY = raw.YAxis;
    minY = raw.YAxis;  
    maxZ = raw.ZAxis;
    minZ = raw.ZAxis;
    bFirst = false;     
  } else {  
    if ( raw.XAxis > maxX ) {
      maxX = raw.XAxis;
      bChange = true;
    }
    if ( raw.XAxis < minX ) {
      minX = raw.XAxis;
      bChange = true;
    }

    if (raw.YAxis > maxY ) {
      maxY = raw.YAxis;
      bChange = true;
    }
    if ( raw.YAxis < minY ) {
      minY = raw.YAxis;
      bChange = true;
    }  
    if ( raw.ZAxis > maxZ ) {
      maxZ = raw.ZAxis;
      bChange = true;
    }
    if ( raw.ZAxis < minZ ) {
      minZ = raw.ZAxis;
      bChange = true;
    }
    
    /*
    PRINT( raw.XAxis );
    PRINT( ", " );
    PRINT( raw.YAxis );
    PRINT( ", " );
    PRINT( raw.ZAxis );    
    PRINT( "     " );
    */
    
    if ( bChange ) {
      PRINT( "X: Min " );
      PRINT( minX );
      PRINT( " Max " );
      PRINT( maxX );
      PRINT( " Range " );
      PRINT( maxX - minX );
      PRINT( " Offset " );
      PRINT( ( maxX + minX ) / -2 );

      PRINT( "  Y: Min " );
      PRINT( minY );
      PRINT( " Max " );
      PRINT( maxY );
      PRINT( " Range " );
      PRINT( maxY - minY );
      PRINT( " Offset " );
      PRINT( ( maxY + minY ) / -2 );      

      PRINT( "  Z: Min " );
      PRINT( minZ );
      PRINT( " Max " );
      PRINT( maxZ );
      PRINT( " Range " );
      PRINT( maxZ - minZ );
      PRINT( " Offset " );
      PRINTLN( ( maxZ + minZ ) / -2 );     
    }     
  }
  
  delay(50);
}
