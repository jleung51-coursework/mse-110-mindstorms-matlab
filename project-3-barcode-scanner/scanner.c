#pragma config(Sensor, S1,     ColorSensor,    sensorEV3_Color)
#pragma config(Motor,  motorA,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorD,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)

/**
 * This Lego Mindstorms EV3 ROBOTC program moves forward to scan a barcode and
 * outputs the values to the datalog (datalog-0.txt).
 *
 */

// #define NOTROBOTC  // Comment this out if running with the ROBOTC compiler

// #include "../robotc_dummy.h"

// Replace ROBOTC-specific variables defined at top of file
#ifdef NOTROBOTC
const int ColorSensor = 0;
const int rightMotor = 0;
const int leftMotor = 0;
#endif

const unsigned int samplingDelay = 4;
const int speed = 25;

task main()
{
	displayCenteredTextLine(3, "BARCODE SCANNER");

	datalogFlush();
	datalogClose();
	// Argument 1: Datalog index
	// Argument 2: Columns
	// Argument 3: Append option
	bool datalogOpenSuccess = datalogOpen(0, 1, false);
	if (!datalogOpenSuccess) {
		displayCenteredTextLine(6, "Unable to open datalog.");
	}
	else {
		displayCenteredTextLine(6, "");
	}

	unsigned int samplingCount = 0;
	int colorValue = 0;
	unsigned long scanCount = 0;

	while(!getButtonPress(buttonAny)) {
		setMotorSpeed(leftMotor, -speed);  // Gears causing reversed rotation
		setMotorSpeed(rightMotor, -speed); // Gears causing reversed rotation

		if(samplingCount % samplingDelay == 0) {
			colorValue = getColorReflected(ColorSensor);
			datalogAddShort(0, colorValue);
			++scanCount;
			samplingCount = 0;
		}
		++samplingCount;

		displayCenteredTextLine(7, "Color value: %d", colorValue);
		displayCenteredTextLine(8, "Scan count: %d", scanCount);
	}

	datalogClose();
	return 0;
}
