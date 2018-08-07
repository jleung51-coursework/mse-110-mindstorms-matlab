#pragma config(Sensor, S3,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

/*
 * This Lego Mindstorms EV3 ROBOTC program follows a black line, using a
 * PID controller.
 *
 */

// #define NOTROBOTC  // Comment this out if running with the ROBOTC compiler

// #include "../robotc_dummy.h"

 // Replace ROBOTC-specific variables defined at top of file
 #ifdef NOTROBOTC
 const int ColorSensor = 0;
 const int UltrasonicSensor = 0;
 const int LeftMotor = 0;
 const int RightMotor = 0;
 #endif

// ---------- CONFIGURATION VARIABLES ----------

// Max speed is 100 or -100
const int speed_baseline = 25;
const int speed_differential = 12;

const int reflection_black = 8;
const int reflection_white = 40;
const int reflection_centered = 20;
// Counts of pure white or pure black before turning sharply to find line again
const int turn_threshold = 20;

const float integral_accumulated_dampener = 0.8;  // Must be less than 1
const float integral_effect_multiplier = 1;  // Must be greater than 1

const float derivative_effect_dampener = 0.7;  // Must be less than 1

// ---------- STRUCTS ----------
typedef struct {
	int left_motor;
	int right_motor;
} speed;

// ---------- FUNCTION PROTOTYPES ----------
bool isBlack(int val);
void copySpeed(speed* to, speed from);
void setSpeed(speed* s, int left_motor, int right_motor);
void addSpeed(speed* s, speed add);
void applySpeed(speed s);
float getPercentDeviation(int color_reflected);

// ---------- FUNCTIONS ----------

bool isBlack(int val) {
	return val < reflection_centered;
}

void copySpeed(speed* to, speed from) {
	to->left_motor = from.left_motor;
	to->right_motor = from.right_motor;
}

void setSpeed(speed* s, int left_motor, int right_motor) {
	s->left_motor = left_motor;
	s->right_motor = right_motor;
}

void addSpeed(speed* s, speed add) {
	s->left_motor += add.left_motor;
	s->right_motor += add.right_motor;
}

void applySpeed(speed s) {
	setMotorSpeed(LeftMotor, s.left_motor);
	setMotorSpeed(RightMotor, s.right_motor);
}

// Negative means darker than center, positive means lighter than center
float getPercentDeviation(int color_reflected) {
	if (isBlack(color_reflected)) {
		return -1 * ((float)reflection_centered - color_reflected)
		/(reflection_centered - reflection_black);
	}
	else {
		return ((float)color_reflected - reflection_centered)
		/(reflection_white - reflection_centered);
	}
}

task main()
{
	displayCenteredTextLine(1, "Version 2.3");

	// setMotorBrakeMode(LeftMotor, motorBrake);

	speed speed_current;
	setSpeed(&speed_current, speed_baseline, speed_baseline);
	applySpeed(speed_current);
	while(!isBlack(getColorReflected(ColorSensor))) {
	}

	speed speed_previous;
	copySpeed(&speed_current, speed_previous);

	float error_accumulated = 0;
	int persistent_black = 0;
	int persistent_white = 0;

	#ifdef NOTROBOTC
	for(int notRobotcCounter; notRobotcCounter < 100; ++notRobotcCounter) {
	#else
	while(true) {
	#endif

		int color_reflected = getColorReflected(ColorSensor);
		// Cap sensor values
		if(color_reflected < reflection_black) {
			color_reflected = reflection_black;
		}
		else if(color_reflected > reflection_white) {
			color_reflected = reflection_white;
		}

		float percent_deviation = getPercentDeviation(color_reflected);

		// Calculate proportional speed
		//
		// If the value is negative, there is been too much black
		// and there needs to be more white. The robot should turn right, so
		// the left motor should increase and the right motor should decrease.
		//
		// The left motor is inversely proportional to the percent deviation
		// and the right motor is proportional to the percent deviation.
		speed speed_proportional;
		setSpeed(
		&speed_proportional,
		speed_differential * percent_deviation * -1,
		speed_differential * percent_deviation
		);

		// Calculate integral speed
		error_accumulated *= integral_accumulated_dampener;
		error_accumulated += percent_deviation;
		speed speed_integral;

		// If the value is negative, there is been too much black
		// and there needs to be more white. The robot should turn right, so
		// the left motor should increase and the right motor should decrease.
		//
		// The left motor is inversely proportional to the percent deviation
		// and the right motor is proportional to the percent deviation.
		setSpeed(
		&speed_integral,
		error_accumulated * integral_effect_multiplier * -1,
		error_accumulated * integral_effect_multiplier
		);

		// Calculate derivative speed
		speed speed_derivative;
		setSpeed(
		&speed_derivative,
		(speed_current.left_motor - speed_previous.left_motor)
		* derivative_effect_dampener,
		(speed_current.right_motor - speed_previous.right_motor)
		* derivative_effect_dampener
		);
		copySpeed(&speed_previous, speed_current);

		speed new_speed;
		setSpeed(&new_speed, speed_baseline, speed_baseline);
		addSpeed(&new_speed, speed_proportional);
		addSpeed(&new_speed, speed_integral);
		addSpeed(&new_speed, speed_derivative);

		// Hard-correct sensor value at reflective limits
		if(color_reflected <= reflection_black) {
			++persistent_black;
			if(persistent_black > turn_threshold) {
				setSpeed(&new_speed, speed_baseline*2/3, -speed_baseline*2/3);
			}
		}
		else {
			persistent_black = 0;
		}

		if(color_reflected >= reflection_white) {
			++persistent_white;
			if(persistent_white > turn_threshold) {
				setSpeed(&new_speed, -speed_baseline*2/3, speed_baseline);
			}
		}
		else {
			persistent_white = 0;
		}

		applySpeed(new_speed);

		displayCenteredTextLine(3, "Color reflected: %d", color_reflected);
		displayCenteredTextLine(4, "Percentage deviation: %f", percent_deviation);
		displayCenteredTextLine(5, "Left speed: %d", new_speed.left_motor);
		displayCenteredTextLine(6, "Right speed: %d", new_speed.right_motor);
		displayCenteredTextLine(7, "Integral difference: %f", error_accumulated);
		displayCenteredTextLine(8, "Derivative left: %d", speed_derivative.left_motor);
		displayCenteredTextLine(9, "Derivative right: %d", speed_derivative.right_motor);
		displayCenteredTextLine(10, "Persistent black count: %d", persistent_black);
		displayCenteredTextLine(11, "Persistent white count: %d", persistent_white);
	}
}
