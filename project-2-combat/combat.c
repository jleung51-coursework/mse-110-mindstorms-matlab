#pragma config(Sensor, S1,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

/*
 * This Lego Mindstorms EV3 ROBOTC program navigates a sumo ring to remove
 * an opponent robot from the ring.
 *
 */

 #define NOTROBOTC  // Comment this out if running with the ROBOTC compiler

 #include "../robotc_dummy.h"

// Replace ROBOTC-specific variables defined at top of file
#ifdef NOTROBOTC
const int ColorSensor = 0;
const int UltrasonicSensor = 0;
const int LeftMotor = 0;
const int RightMotor = 0;
#endif

// Function prototypes
void setSpeed(int left_motor_speed, int right_motor_speed);

const int threshold_white = 10;

void setSpeed(int left_motor_speed, int right_motor_speed) {
	setMotorSpeed(LeftMotor, -left_motor_speed);
	setMotorSpeed(RightMotor, -right_motor_speed);
}

task main()
{
	const int min = 10;
	const int max = 70;
	int oscillation = min;
	bool increasing = true;

	#ifdef NOTROBOTC
	for(int notRobotcCounter; notRobotcCounter < 200; ++notRobotcCounter) {
	#else
	while(true) {
	#endif

		int color_reflected = getColorReflected(ColorSensor);
		displayCenteredTextLine(3, "Color: %d", color_reflected);

		// Avoid the white border
		if(color_reflected > threshold_white) {
			setSpeed(-70, -70);
			sleep(800);
			setSpeed(-60, 60);
			sleep(3000);
			setSpeed(0, 0);
			sleep(300);
		}

		displayCenteredTextLine(4, "Oscillation: %d", oscillation);

		// Charge at another robot
		int distance = getUSDistance(UltrasonicSensor);
		if (distance < 50) {
			setSpeed(100, 100);
		}
		else {
			setSpeed(oscillation, max+min-oscillation);
			sleep(30);
			oscillation += increasing ? 1 : -1;
			if(oscillation > max) {
				increasing = false;
			}
			else if(oscillation < min) {
				increasing = true;
			}
		}

	}

}
