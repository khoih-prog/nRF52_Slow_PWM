/****************************************************************************************************************************
  multiFileProject.ino
  
  For nRF52-based boards using Adafruit_nRF52_Arduino core
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/nRF52_Slow_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !(defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
      defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
      defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  #error This code is designed to run on nRF52 platform! Please check your Tools->Board setting.
#endif

#define NRF52_SLOW_PWM_VERSION_MIN_TARGET      "NRF52_Slow_PWM v1.2.1"
#define NRF52_SLOW_PWM_VERSION_MIN             1002001

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "nRF52_Slow_PWM.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(NRF52_SLOW_PWM_VERSION);

#if defined(NRF52_SLOW_PWM_VERSION_MIN)
  if (NRF52_SLOW_PWM_VERSION_INT < NRF52_SLOW_PWM_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(NRF52_SLOW_PWM_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
