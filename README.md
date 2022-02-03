# nRF52_Slow_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/nRF52_Slow_PWM.svg?)](https://www.ardu-badge.com/nRF52_Slow_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/nRF52_Slow_PWM.svg)](https://github.com/khoih-prog/nRF52_Slow_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/nRF52_Slow_PWM/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/nRF52_Slow_PWM.svg)](http://github.com/khoih-prog/nRF52_Slow_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Buy Me A Coffee" style="height: 60px !important;width: 217px !important;" ></a>


---
---

## Table of Contents

* [Why do we need this nRF52_Slow_PWM library](#why-do-we-need-this-nRF52_Slow_PWM-library)
  * [Features](#features)
  * [Why using ISR-based PWM is better](#why-using-isr-based-pwm-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nRF52840-and-nRF52832-boards)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [Usage](#usage)
  * [1. Init Hardware Timer](#1-init-hardware-timer)
  * [2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions](#2-Set-PWM-Frequency-dutycycle-attach-irqCallbackStartFunc-and-irqCallbackStopFunc-functions)
* [Examples](#examples)
  * [ 1. ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
  * [ 2. ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
  * [ 3. ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
  * [ 4. ISR_Changing_PWM](examples/ISR_Changing_PWM)
  * [ 5. ISR_Modify_PWM](examples/ISR_Modify_PWM)
* [Example ISR_16_PWMs_Array_Complex](#Example-ISR_16_PWMs_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_PWMs_Array_Complex on NRF52840_ITSYBITSY](#1-ISR_16_PWMs_Array_Complex-on-NRF52840_ITSYBITSY)
  * [2. ISR_16_PWMs_Array on NRF52840_ITSYBITSY](#2-ISR_16_PWMs_Array-on-NRF52840_ITSYBITSY)
  * [3. ISR_16_PWMs_Array_Simple on NRF52840_ITSYBITSY](#3-ISR_16_PWMs_Array_Simple-on-NRF52840_ITSYBITSY)
  * [4. ISR_Modify_PWM on NRF52840_ITSYBITSY](#4-ISR_Modify_PWM-on-NRF52840_ITSYBITSY)
  * [5. ISR_Changing_PWM on NRF52840_ITSYBITSY](#5-ISR_Changing_PWM-on-NRF52840_ITSYBITSY)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [nRF52_Slow_PWM library](https://github.com/khoih-prog/nRF52_Slow_PWM)

### Features

This library enables you to use ISR-based PWM channels on an nRF52-based board, such as `Itsy-Bitsy nRF52840`, using [`Adafruit nRF52 core`](https://github.com/adafruit/Adafruit_nRF52_Arduino) core  to create and output PWM any GPIO pin. Because this library doesn't use the powerful purely hardware-controlled PWM with many limitations, the maximum PWM frequency is currently limited at **1000Hz**, which is still suitable for many real-life applications. Now you can also modify PWM settings on-the-fly.

---

This library enables you to use Interrupt from Hardware Timers on nRF52-based boards to create and output PWM to pins. It now supports 16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer. PWM interval can be very long (uint64_t microsecs / millisecs). The most important feature is they're ISR-based PWM channels. Therefore, their executions are not blocked by bad-behaving functions or tasks. This important feature is absolutely necessary for mission-critical tasks. These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software PWM using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based PWM-channels**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software PWM channels. 

The most important feature is they're ISR-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_PWMs_Array_Complex**](examples/ISR_16_PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy, compared to software PWM, by printing the actual period / duty-cycle in `microsecs` of each of PWM-channels.

Being ISR-based PWM, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet or Blynk services. You can also have many `(up to 16)` PWM channels to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see `software-based` SimpleTimer is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

#### Why using ISR-based PWM is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software PWM channels using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software PWM channels, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. **nRF52-based boards** such as **AdaFruit Itsy-Bitsy nRF52840, Feather nRF52840 Express**, etc., using [`Adafruit nRF52 core`](https://github.com/adafruit/Adafruit_nRF52_Arduino)

---

#### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Adafruit nRF52 v1.3.0+`](https://github.com/adafruit/Adafruit_nRF52_Arduino) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest)
 
 3. To use with certain example
   - [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) for [ISR_16_PWMs_Array_Complex example](examples/ISR_16_PWMs_Array_Complex).
---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**nRF52_Slow_PWM**](https://github.com/khoih-prog/nRF52_Slow_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/nRF52_Slow_PWM.svg?)](https://www.ardu-badge.com/nRF52_Slow_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**nRF52_Slow_PWM**](https://github.com/khoih-prog/nRF52_Slow_PWM) page.
2. Download the latest release `nRF52_Slow_PWM-master.zip`.
3. Extract the zip file to `nRF52_Slow_PWM-master` directory 
4. Copy whole `nRF52_Slow_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**nRF52_Slow_PWM** library](https://platformio.org/lib/show/12879/nRF52_Slow_PWM) by using [Library Manager](https://platformio.org/lib/show/12879/nRF52_Slow_PWM/installation). Search for **nRF52_Slow_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 1.1.0](Packages_Patches/adafruit/hardware/nrf52/1.1.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0). 

Supposing the Adafruit nRF52 version is 1.1.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/cores/nRF5/Udp.h`**
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/cores/nRF5/Print.h`**      <===== To be able to print `uint64_t`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/1.1.0/cores/nRF5/Print.cpp`**    <===== To be able to print `uint64_t`

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.h`**       <===== To be able to print `uint64_t`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Print.cpp`**     <===== To be able to print `uint64_t`


---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---
---


## Usage


Before using any Timer, you have to make sure the Timer has not been used by any other purpose. Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)


#### 1. Init Hardware Timer

```
// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER2
NRF52Timer ITimer(NRF_TIMER_2);

// Init nRF52_Slow_PWM, each can service 16 different ISR-based PWM channels
NRF52_Slow_PWM ISR_PWM;
```

#### 2. Set PWM Frequency, dutycycle, attach irqCallbackStartFunc and irqCallbackStopFunc functions

```
void irqCallbackStartFunc()
{

}

void irqCallbackStopFunc()
{

}

void setup()
{
  ....
  
  // You can use this with PWM_Freq in Hz
  ISR_PWM.setPWM(PWM_Pin, PWM_Freq, PWM_DutyCycle, irqCallbackStartFunc, irqCallbackStopFunc);
                   
  ....                 
}  
```

---
---

### Examples: 

 1. [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
 2. [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
 3. [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
 4. [ISR_Changing_PWM](examples/ISR_Changing_PWM)
 5. [ISR_Modify_PWM](examples/ISR_Modify_PWM)

 
---
---

### Example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)

```
#if !(defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
      defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
      defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
      defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
#error This code is designed to run on nRF52 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      4

#define USING_MICROS_RESOLUTION       true    //false 

#include "nRF52_Slow_PWM.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#define LED_OFF             HIGH
#define LED_ON              LOW

#ifndef LED_BUILTIN
  #define LED_BUILTIN       25
#endif

#ifndef LED_BLUE
  #define LED_BLUE          10
#endif

#ifndef LED_RED
  #define LED_RED           11
#endif


#define HW_TIMER_INTERVAL_US      5L

volatile uint64_t startMicros = 0;

// Depending on the board, you can select NRF52 Hardware Timer from NRF_TIMER_1-NRF_TIMER_4 (1 to 4)
// If you select the already-used NRF_TIMER_0, it'll be auto modified to use NRF_TIMER_1

// Init NRF52 timer NRF_TIMER3
NRF52Timer ITimer(NRF_TIMER_3);

// Init nRF52_Slow_PWM, each can service 16 different ISR-based PWM channels
NRF52_Slow_PWM ISR_PWM;

//////////////////////////////////////////////////////

void TimerHandler()
{
  ISR_PWM.run();
}

/////////////////////////////////////////////////

#define NUMBER_ISR_PWMS         16

#define PIN_D0      0
#define PIN_D1      1
#define PIN_D2      2
#define PIN_D3      3
#define PIN_D4      4
#define PIN_D5      5
#define PIN_D6      6
#define PIN_D7      7
#define PIN_D8      8
#define PIN_D9      9
#define PIN_D10     10
#define PIN_D11     11
#define PIN_D12     12

typedef void (*irqCallback)  ();

//////////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#define USING_PWM_FREQUENCY     true

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

typedef struct
{
  uint32_t      PWM_Pin;
  irqCallback   irqCallbackStartFunc;
  irqCallback   irqCallbackStopFunc;

#if USING_PWM_FREQUENCY
  uint32_t      PWM_Freq;
#else
  uint32_t      PWM_Period;
#endif

  uint32_t      PWM_DutyCycle;

  uint64_t      deltaMicrosStart;
  uint64_t      previousMicrosStart;

  uint64_t      deltaMicrosStop;
  uint64_t      previousMicrosStop;

} ISR_PWM_Data;

// In nRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomethingStart(int index);

void doingSomethingStop(int index);

#else   // #if USE_COMPLEX_STRUCT

volatile unsigned long deltaMicrosStart    [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMicrosStart [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

volatile unsigned long deltaMicrosStop     [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMicrosStop  [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign pins here. Be carefull to select good pin to use or crash, e.g pin 6-11
uint32_t PWM_Pin[NUMBER_ISR_PWMS] =
{
  LED_BUILTIN, LED_BLUE, LED_RED, PIN_D0, PIN_D1,  PIN_D2,  PIN_D3,  PIN_D4,
  PIN_D5,   PIN_D6,  PIN_D7, PIN_D8, PIN_D9, PIN_D10, PIN_D11, PIN_D12
};

// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period[NUMBER_ISR_PWMS] =
{
  1000000L,   500000L,   333333L,   250000L,   200000L,   166667L,   142857L,   125000L,
  111111L,   100000L,    66667L,    50000L,    40000L,   33333L,     25000L,    20000L
};

// You can assign any interval for any timer here, in Hz
double PWM_Freq[NUMBER_ISR_PWMS] =
{
  1.0f,  2.0f,  3.0f,  4.0f,  5.0f,  6.0f,  7.0f,  8.0f,
  9.0f, 10.0f, 15.0f, 20.0f, 25.0f, 30.0f, 40.0f, 50.0f
};

// You can assign any interval for any timer here, in milliseconds
uint32_t PWM_DutyCycle[NUMBER_ISR_PWMS] =
{
  5, 10, 20, 25, 30, 35, 40, 45,
  50, 45, 40, 35, 30, 25, 20, 15
};

void doingSomethingStart(int index)
{
  unsigned long currentMicros  = micros();

  deltaMicrosStart[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStart[index] = currentMicros;
}

void doingSomethingStop(int index)
{
  unsigned long currentMicros  = micros();

  // Count from start to stop PWM pulse
  deltaMicrosStop[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStop[index] = currentMicros;
}

#endif    // #if USE_COMPLEX_STRUCT

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomethingStart0()
{
  doingSomethingStart(0);
}

void doingSomethingStart1()
{
  doingSomethingStart(1);
}

void doingSomethingStart2()
{
  doingSomethingStart(2);
}

void doingSomethingStart3()
{
  doingSomethingStart(3);
}

void doingSomethingStart4()
{
  doingSomethingStart(4);
}

void doingSomethingStart5()
{
  doingSomethingStart(5);
}

void doingSomethingStart6()
{
  doingSomethingStart(6);
}

void doingSomethingStart7()
{
  doingSomethingStart(7);
}

void doingSomethingStart8()
{
  doingSomethingStart(8);
}

void doingSomethingStart9()
{
  doingSomethingStart(9);
}

void doingSomethingStart10()
{
  doingSomethingStart(10);
}

void doingSomethingStart11()
{
  doingSomethingStart(11);
}

void doingSomethingStart12()
{
  doingSomethingStart(12);
}

void doingSomethingStart13()
{
  doingSomethingStart(13);
}

void doingSomethingStart14()
{
  doingSomethingStart(14);
}

void doingSomethingStart15()
{
  doingSomethingStart(15);
}

//////////////////////////////////////////////////////

void doingSomethingStop0()
{
  doingSomethingStop(0);
}

void doingSomethingStop1()
{
  doingSomethingStop(1);
}

void doingSomethingStop2()
{
  doingSomethingStop(2);
}

void doingSomethingStop3()
{
  doingSomethingStop(3);
}

void doingSomethingStop4()
{
  doingSomethingStop(4);
}

void doingSomethingStop5()
{
  doingSomethingStop(5);
}

void doingSomethingStop6()
{
  doingSomethingStop(6);
}

void doingSomethingStop7()
{
  doingSomethingStop(7);
}

void doingSomethingStop8()
{
  doingSomethingStop(8);
}

void doingSomethingStop9()
{
  doingSomethingStop(9);
}

void doingSomethingStop10()
{
  doingSomethingStop(10);
}

void doingSomethingStop11()
{
  doingSomethingStop(11);
}

void doingSomethingStop12()
{
  doingSomethingStop(12);
}

void doingSomethingStop13()
{
  doingSomethingStop(13);
}

void doingSomethingStop14()
{
  doingSomethingStop(14);
}

void doingSomethingStop15()
{
  doingSomethingStop(15);
}

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

#if USING_PWM_FREQUENCY

ISR_PWM_Data curISR_PWM_Data[NUMBER_ISR_PWMS] =
{
  // pin, irqCallbackStartFunc, irqCallbackStopFunc, PWM_Freq, PWM_DutyCycle, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
  { LED_BUILTIN,  doingSomethingStart0,    doingSomethingStop0,    1,   5, 0, 0, 0, 0 },
  { LED_BLUE,     doingSomethingStart1,    doingSomethingStop1,    2,  10, 0, 0, 0, 0 },
  { LED_RED,      doingSomethingStart2,    doingSomethingStop2,    3,  20, 0, 0, 0, 0 },
  { PIN_D0,       doingSomethingStart3,    doingSomethingStop3,    4,  25, 0, 0, 0, 0 },
  { PIN_D1,       doingSomethingStart4,    doingSomethingStop4,    5,  30, 0, 0, 0, 0 },
  { PIN_D2,       doingSomethingStart5,    doingSomethingStop5,    6,  35, 0, 0, 0, 0 },
  { PIN_D3,       doingSomethingStart6,    doingSomethingStop6,    7,  40, 0, 0, 0, 0 },
  { PIN_D4,       doingSomethingStart7,    doingSomethingStop7,    8,  45, 0, 0, 0, 0 },
  { PIN_D5,       doingSomethingStart8,    doingSomethingStop8,    9,  50, 0, 0, 0, 0 },
  { PIN_D6,       doingSomethingStart9,    doingSomethingStop9,   10,  45, 0, 0, 0, 0 },
  { PIN_D7,       doingSomethingStart10,   doingSomethingStop10,  15,  40, 0, 0, 0, 0 },
  { PIN_D8,       doingSomethingStart11,   doingSomethingStop11,  20,  35, 0, 0, 0, 0 },
  { PIN_D9,       doingSomethingStart12,   doingSomethingStop12,  25,  30, 0, 0, 0, 0 },
  { PIN_D10,      doingSomethingStart13,   doingSomethingStop13,  30,  25, 0, 0, 0, 0 },
  { PIN_D11,      doingSomethingStart14,   doingSomethingStop14,  40,  20, 0, 0, 0, 0 },
  { PIN_D12,      doingSomethingStart15,   doingSomethingStop15,  50,  15, 0, 0, 0, 0 }
};

#else   // #if USING_PWM_FREQUENCY

ISR_PWM_Data curISR_PWM_Data[NUMBER_ISR_PWMS] =
{
  // pin, irqCallbackStartFunc, irqCallbackStopFunc, PWM_Period, PWM_DutyCycle, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
  { LED_BUILTIN,  doingSomethingStart0,     doingSomethingStop0,   1000000L,  5, 0, 0, 0, 0 },
  { LED_BLUE,     doingSomethingStart1,     doingSomethingStop1,    500000L, 10, 0, 0, 0, 0 },
  { LED_RED,      doingSomethingStart2,     doingSomethingStop2,    333333L, 20, 0, 0, 0, 0 },
  { PIN_D0,       doingSomethingStart3,     doingSomethingStop3,    250000L, 25, 0, 0, 0, 0 },
  { PIN_D1,       doingSomethingStart4,     doingSomethingStop4,    200000L, 30, 0, 0, 0, 0 },
  { PIN_D2,       doingSomethingStart5,     doingSomethingStop5,    166667L, 35, 0, 0, 0, 0 },
  { PIN_D3,       doingSomethingStart6,     doingSomethingStop6,    142857L, 40, 0, 0, 0, 0 },
  { PIN_D4,       doingSomethingStart7,     doingSomethingStop7,    125000L, 45, 0, 0, 0, 0 },
  { PIN_D5,       doingSomethingStart8,     doingSomethingStop8,    111111L, 50, 0, 0, 0, 0 },
  { PIN_D6,       doingSomethingStart9,     doingSomethingStop9,    100000L, 45, 0, 0, 0, 0 },
  { PIN_D7,       doingSomethingStart10,    doingSomethingStop10,    66667L, 40, 0, 0, 0, 0 },
  { PIN_D8,       doingSomethingStart11,    doingSomethingStop11,    50000L, 35, 0, 0, 0, 0 },
  { PIN_D9,       doingSomethingStart12,    doingSomethingStop12,    40000L, 30, 0, 0, 0, 0 },
  { PIN_D10,      doingSomethingStart13,    doingSomethingStop13,    33333L, 25, 0, 0, 0, 0 },
  { PIN_D11,      doingSomethingStart14,    doingSomethingStop14,    25000L, 20, 0, 0, 0, 0 },
  { PIN_D12,      doingSomethingStart15,    doingSomethingStop15,    20000L, 15, 0, 0, 0, 0 }
};

#endif  // #if USING_PWM_FREQUENCY

void doingSomethingStart(int index)
{
  unsigned long currentMicros  = micros();

  curISR_PWM_Data[index].deltaMicrosStart    = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStart = currentMicros;
}

void doingSomethingStop(int index)
{
  unsigned long currentMicros  = micros();

  //curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStop;
  // Count from start to stop PWM pulse
  curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStop  = currentMicros;
}

#else   // #if USE_COMPLEX_STRUCT

irqCallback irqCallbackStartFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStart0,  doingSomethingStart1,  doingSomethingStart2,  doingSomethingStart3,
  doingSomethingStart4,  doingSomethingStart5,  doingSomethingStart6,  doingSomethingStart7,
  doingSomethingStart8,  doingSomethingStart9,  doingSomethingStart10, doingSomethingStart11,
  doingSomethingStart12, doingSomethingStart13, doingSomethingStart14, doingSomethingStart15
};

irqCallback irqCallbackStopFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStop0,  doingSomethingStop1,  doingSomethingStop2,  doingSomethingStop3,
  doingSomethingStop4,  doingSomethingStop5,  doingSomethingStop6,  doingSomethingStop7,
  doingSomethingStop8,  doingSomethingStop9,  doingSomethingStop10, doingSomethingStop11,
  doingSomethingStop12, doingSomethingStop13, doingSomethingStop14, doingSomethingStop15
};

#endif    // #if USE_COMPLEX_STRUCT

//////////////////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMicrosStart = startMicros;

  unsigned long currMicros = micros();

  Serial.print(F("SimpleTimer (ms): ")); Serial.print(SIMPLE_TIMER_MS);
  Serial.print(F(", us : ")); Serial.print(currMicros);
  Serial.print(F(", Dus : ")); Serial.println(currMicros - previousMicrosStart);

  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("PWM Channel : ")); Serial.print(i);
    Serial.print(F(", programmed Period (us): "));

#if USING_PWM_FREQUENCY
    Serial.print(1000000 / curISR_PWM_Data[i].PWM_Freq);
#else
    Serial.print(curISR_PWM_Data[i].PWM_Period);
#endif

    Serial.print(F(", actual : ")); Serial.print(curISR_PWM_Data[i].deltaMicrosStart);

    Serial.print(F(", programmed DutyCycle : "));

    Serial.print(curISR_PWM_Data[i].PWM_DutyCycle);

    Serial.print(F(", actual : ")); Serial.println((float) curISR_PWM_Data[i].deltaMicrosStop * 100.0f / curISR_PWM_Data[i].deltaMicrosStart);
    //Serial.print(F(", actual deltaMicrosStop : ")); Serial.println(curISR_PWM_Data[i].deltaMicrosStop);
    //Serial.print(F(", actual deltaMicrosStart : ")); Serial.println(curISR_PWM_Data[i].deltaMicrosStart);

#else

    Serial.print(F("PWM Channel : ")); Serial.print(i);

#if USING_PWM_FREQUENCY
    Serial.print(1000000 / PWM_Freq[i]);
#else
    Serial.print(PWM_Period[i]);
#endif

    Serial.print(F(", programmed Period (us): ")); Serial.print(PWM_Period[i]);
    Serial.print(F(", actual : ")); Serial.print(deltaMicrosStart[i]);

    Serial.print(F(", programmed DutyCycle : "));

    Serial.print(PWM_DutyCycle[i]);

    Serial.print(F(", actual : ")); Serial.println( (float) deltaMicrosStop[i] * 100.0f / deltaMicrosStart[i]);
    //Serial.print(F(", actual deltaMicrosStop : ")); Serial.println(deltaMicrosStop[i]);
    //Serial.print(F(", actual deltaMicrosStart : ")); Serial.println(deltaMicrosStart[i]);
#endif
  }

  previousMicrosStart = currMicros;
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_16_PWMs_Array_Complex on ")); Serial.println(BOARD_NAME);
  Serial.println(NRF52_SLOW_PWM_VERSION);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMicros = micros();
    Serial.print(F("Starting ITimer OK, micros() = ")); Serial.println(startMicros);
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  startMicros = micros();

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_PWM

  //for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  for (uint16_t i = 0; i < 8; i++)
  {
#if USE_COMPLEX_STRUCT
    curISR_PWM_Data[i].previousMicrosStart = startMicros;
    //ISR_PWM.setInterval(curISR_PWM_Data[i].PWM_Period, curISR_PWM_Data[i].irqCallbackStartFunc);

    //void setPWM(uint32_t pin, uint32_t frequency, uint32_t dutycycle
    // , timer_callback_p StartCallback = nullptr, timer_callback_p StopCallback = nullptr)

#if USING_PWM_FREQUENCY
    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(curISR_PWM_Data[i].PWM_Pin, curISR_PWM_Data[i].PWM_Freq, curISR_PWM_Data[i].PWM_DutyCycle,
                   curISR_PWM_Data[i].irqCallbackStartFunc, curISR_PWM_Data[i].irqCallbackStopFunc);
#else
    // Or You can use this with PWM_Period in us
    ISR_PWM.setPWM_Period(curISR_PWM_Data[i].PWM_Pin, curISR_PWM_Data[i].PWM_Period, curISR_PWM_Data[i].PWM_DutyCycle,
                          curISR_PWM_Data[i].irqCallbackStartFunc, curISR_PWM_Data[i].irqCallbackStopFunc);
#endif

#else
    previousMicrosStart[i] = micros();

#if USING_PWM_FREQUENCY
    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq[i], PWM_DutyCycle[i], irqCallbackStartFunc[i], irqCallbackStopFunc[i]);
#else
    // Or You can use this with PWM_Period in us
    ISR_PWM.setPWM_Period(PWM_Pin[i], PWM_Period[i], PWM_DutyCycle[i], irqCallbackStartFunc[i], irqCallbackStopFunc[i]);
#endif

#endif
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_PWM still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_PWM is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_PWM.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

### 1. ISR_16_PWMs_Array_Complex on NRF52840_ITSYBITSY

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) to demonstrate how to use multiple PWM channels with complex callback functions, the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods and duty-cycles**


```
Starting ISR_16_PWMs_Array_Complex on NRF52840_ITSYBITSY
NRF52_Slow_PWM v1.1.0
[PWM] NRF52TimerInterrupt: F_CPU (MHz) = 64 , Timer =  NRF_TIMER2
[PWM] Frequency = 1000000.00 , _count = 10
Starting ITimer OK, micros() = 2910156
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2910156
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2910156
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2910156
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2910156
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2910156
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 2910156
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2910156
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2910156
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 2910156
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 2910156
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 2910156
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 2910156
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 2910156
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 2910156
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 2910156
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 2910156
PWM Channel : 0, programmed Period (us): 1000000, actual : 992188, programmed DutyCycle : 5, actual : 5.12
PWM Channel : 1, programmed Period (us): 500000, actual : 493164, programmed DutyCycle : 10, actual : 11.68
PWM Channel : 2, programmed Period (us): 333333, actual : 344726, programmed DutyCycle : 20, actual : 17.85
PWM Channel : 3, programmed Period (us): 250000, actual : 262695, programmed DutyCycle : 30, actual : 25.65
PWM Channel : 4, programmed Period (us): 200000, actual : 192383, programmed DutyCycle : 40, actual : 38.58
PWM Channel : 5, programmed Period (us): 166666, actual : 177734, programmed DutyCycle : 45, actual : 38.46
PWM Channel : 6, programmed Period (us): 142857, actual : 143554, programmed DutyCycle : 50, actual : 46.94
PWM Channel : 7, programmed Period (us): 125000, actual : 128906, programmed DutyCycle : 55, actual : 47.73
PWM Channel : 8, programmed Period (us): 111111, actual : 113281, programmed DutyCycle : 60, actual : 54.31
PWM Channel : 9, programmed Period (us): 100000, actual : 106445, programmed DutyCycle : 65, actual : 57.80
PWM Channel : 10, programmed Period (us): 66666, actual : 69336, programmed DutyCycle : 70, actual : 56.34
PWM Channel : 11, programmed Period (us): 50000, actual : 46875, programmed DutyCycle : 75, actual : 66.67
PWM Channel : 12, programmed Period (us): 40000, actual : 46875, programmed DutyCycle : 80, actual : 66.67
PWM Channel : 13, programmed Period (us): 33333, actual : 31250, programmed DutyCycle : 85, actual : 75.00
PWM Channel : 14, programmed Period (us): 25000, actual : 91797, programmed DutyCycle : 90, actual : 18.09
PWM Channel : 15, programmed Period (us): 20000, actual : 30274, programmed DutyCycle : 95, actual : 64.52
SimpleTimer (ms): 2000, us : 33451171, Dus : 10166015
PWM Channel : 0, programmed Period (us): 1000000, actual : 993164, programmed DutyCycle : 5, actual : 5.60
PWM Channel : 1, programmed Period (us): 500000, actual : 510742, programmed DutyCycle : 10, actual : 8.41
PWM Channel : 2, programmed Period (us): 333333, actual : 346680, programmed DutyCycle : 20, actual : 20.85
PWM Channel : 3, programmed Period (us): 250000, actual : 259766, programmed DutyCycle : 30, actual : 26.69
PWM Channel : 4, programmed Period (us): 200000, actual : 205079, programmed DutyCycle : 40, actual : 35.24
PWM Channel : 5, programmed Period (us): 166666, actual : 167969, programmed DutyCycle : 45, actual : 44.19
PWM Channel : 6, programmed Period (us): 142857, actual : 144531, programmed DutyCycle : 50, actual : 44.59
PWM Channel : 7, programmed Period (us): 125000, actual : 128906, programmed DutyCycle : 55, actual : 48.48
PWM Channel : 8, programmed Period (us): 111111, actual : 114258, programmed DutyCycle : 60, actual : 53.85
PWM Channel : 9, programmed Period (us): 100000, actual : 91797, programmed DutyCycle : 65, actual : 67.02
PWM Channel : 10, programmed Period (us): 66666, actual : 69336, programmed DutyCycle : 70, actual : 56.34
PWM Channel : 11, programmed Period (us): 50000, actual : 44921, programmed DutyCycle : 75, actual : 67.39
PWM Channel : 12, programmed Period (us): 40000, actual : 44921, programmed DutyCycle : 80, actual : 67.39
PWM Channel : 13, programmed Period (us): 33333, actual : 38085, programmed DutyCycle : 85, actual : 56.41
PWM Channel : 14, programmed Period (us): 25000, actual : 80078, programmed DutyCycle : 90, actual : 97.56
PWM Channel : 15, programmed Period (us): 20000, actual : 20507, programmed DutyCycle : 95, actual : 95.24
```

---

### 2. ISR_16_PWMs_Array on NRF52840_ITSYBITSY

The following is the sample terminal output when running example [**ISR_16_PWMs_Array**](examples/ISR_16_PWMs_Array) on **nRF52-based NRF52840_ITSYBITSY** to demonstrate how to use multiple PWM channels with simple callback functions.

```
Starting ISR_16_PWMs_Array on NRF52840_ITSYBITSY
NRF52_Slow_PWM v1.1.0
[PWM] NRF52TimerInterrupt: F_CPU (MHz) = 64 , Timer =  NRF_TIMER2
[PWM] Frequency = 1000000.00 , _count = 20
Starting ITimer OK, micros() = 3693359
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 3693359
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 3693359
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 3693359
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 3693359
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 3693359
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 3693359
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 3693359
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 3693359
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 3693359
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 3693359
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 3693359
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 3693359
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 3693359
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 3693359
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 3693359
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 3693359
```

---

### 3. ISR_16_PWMs_Array_Simple on NRF52840_ITSYBITSY

The following is the sample terminal output when running example [**ISR_16_PWMs_Array_Simple**](examples/ISR_16_PWMs_Array_Simple) on **nRF52-based NRF52840_ITSYBITSY** to demonstrate how to use multiple PWM channels.

```
Starting ISR_16_PWMs_Array_Simple on NRF52840_ITSYBITSY
NRF52_Slow_PWM v1.1.0
[PWM] NRF52TimerInterrupt: F_CPU (MHz) = 64 , Timer =  NRF_TIMER2
[PWM] Frequency = 1000000.00 , _count = 20
Starting ITimer OK, micros() = 2906250
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2906250
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2906250
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2906250
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2906250
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2906250
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 2906250
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2906250
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2906250
Channel : 8	Period : 111111		OnTime : 66666	Start_Time : 2906250
Channel : 9	Period : 100000		OnTime : 65000	Start_Time : 2906250
Channel : 10	Period : 66666		OnTime : 46666	Start_Time : 2906250
Channel : 11	Period : 50000		OnTime : 37500	Start_Time : 2906250
Channel : 12	Period : 40000		OnTime : 32000	Start_Time : 2906250
Channel : 13	Period : 33333		OnTime : 28333	Start_Time : 2906250
Channel : 14	Period : 25000		OnTime : 22500	Start_Time : 2906250
Channel : 15	Period : 20000		OnTime : 19000	Start_Time : 2906250
```

---

### 4. ISR_Modify_PWM on NRF52840_ITSYBITSY

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) on **nRF52-based NRF52840_ITSYBITSY** to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on NRF52840_ITSYBITSY
NRF52_Slow_PWM v1.1.0
[PWM] NRF52TimerInterrupt: F_CPU (MHz) = 64 , Timer =  NRF_TIMER2
[PWM] Frequency = 16000000.00 , _count = 320
Starting ITimer OK, micros() = 2670898
Using PWM Freq = 1.00, PWM DutyCycle = 10
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 2671875
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 12672851
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 22673828
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 32674804
```

---

### 5. ISR_Changing_PWM on NRF52840_ITSYBITSY

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) on **nRF52-based NRF52840_ITSYBITSY** to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on NRF52840_ITSYBITSY
NRF52_Slow_PWM v1.1.0
[PWM] NRF52TimerInterrupt: F_CPU (MHz) = 64 , Timer =  NRF_TIMER2
[PWM] Frequency = 16000000.00 , _count = 320
Starting ITimer OK, micros() = 2670898
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 2671875
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 12683593
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 22689453
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 32711914
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [nRF52_Slow_PWM issues](https://github.com/khoih-prog/nRF52_Slow_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards

---

## DONE

1. Basic hardware multi-channel PWM for **nRF52-based AdaFruit Itsy-Bitsy nRF52840, Feather nRF52840 Express, etc.** using [`Adafruit nRF52 core`](https://github.com/adafruit/Adafruit_nRF52_Arduino)
2. Add Table of Contents
3. Add functions to modify PWM settings on-the-fly

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/nRF52_Slow_PWM/blob/main/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


