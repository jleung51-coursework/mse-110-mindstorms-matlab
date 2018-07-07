/**
 * This C header file provides dummy definitions for types and functions
 * specific to the Lego Mindstorms EV3 ROBOTC version of C.
 * Including this file allows a file written in ROBOTC to be succesfully
 * compiled (but likely not run);
 *
 */

#pragma once

#ifdef NOTROBOTC

#include <stdarg.h>
#include <stdio.h>

// Types
typedef int task;

typedef enum {
  false,
  true
} bool;

// Display
void displayTextLine(const int nLineNumber, char* sFormatString, ...);
void displayTextLine(const int nLineNumber, char* sFormatString, ...) {
  va_list args;
  va_start(args, sFormatString);
  vprintf(sFormatString, args);
  va_end(args);
  printf("\n");
};

void displayCenteredTextLine(const int nLineNumber, char* sFormatString, ...);
void displayCenteredTextLine(const int nLineNumber, char* sFormatString, ...) {
  va_list args;
  va_start(args, sFormatString);
  vprintf(sFormatString, args);
  va_end(args);
  printf("\n");
};

void displayCenteredBigTextLine(
  const int nLineNumber,
  char* sFormatString, ...
);
void displayCenteredBigTextLine(
  const int nLineNumber,
  char* sFormatString, ...) {
  va_list args;
  va_start(args, sFormatString);
  vprintf(sFormatString, args);
  va_end(args);
  printf("\n");
}

void drawCircle(const int centerX, const int centerY, const int radius);
void drawCircle(const int centerX, const int centerY, const int radius) {};

void drawRect(
  const int left, const int top,
  const int right, const int bottom
);
void drawRect(
  const int left, const int top,
  const int right, const int bottom
) {};

void eraseDisplay();
void eraseDisplay(){};

// Datalog
bool datalogOpen(long nDatalogIndex, long nColumns, bool bAppend);
bool datalogOpen(long nDatalogIndex, long nColumns, bool bAppend) {
  return true;
}

bool datalogAddShort(long nIndex, short data);
bool datalogAddShort(long nIndex, short data) {
  return true;
}

void datalogFlush();
void datalogFlush(){};

void datalogClose();
void datalogClose(){};

// Sensors (buttons)
typedef enum
{
  buttonAny,
  buttonLeft,
  buttonRight
} buttonTypes;
bool getButtonPress(int button);
bool getButtonPress(int button) {
  return true;
}

// Sensors
short getColorReflected(int sensor);
short getColorReflected(int sensor) {
  return 0;
};

// Actuators (motors)
void setMotorSpeed(int motor, int speed);
void setMotorSpeed(int motor, int speed) {};

// Other functions
void sleep();
void sleep(){};

int random();
int random() {
  return 1;
}

#endif
