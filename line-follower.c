#pragma config(Sensor, S3,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

/*
* This ROBOTC for Lego Mindstorms program follows a black line.
*
*/

// ---------- CONFIGURATION VARIABLES ----------

// Max speed is 100 or -100
const int speed_baseline = 25;
const int speed_differential = 15;

const int reflection_black = 6;
const int reflection_white = 40;
const int reflection_centered = 20;

bool isBlack(int val) {
	return val < reflection_centered;
}

// ---------- STRUCTS ----------
typedef struct {
	int left_motor;
	int right_motor;
} speed;

// ---------- FUNCTION PROTOTYPES ----------
void initializeSpeed(speed* s);
void copySpeed(speed* to, speed from);
void setSpeed(speed* s, int left_motor, int right_motor);
void applySpeed(speed s);

// ---------- FUNCTIONS ----------

void initializeSpeed(speed* s){
	setSpeed(s, 0, 0);
}

void copySpeed(speed* to, speed from) {
	to->left_motor = from.left_motor;
	to->right_motor = from.right_motor;
}

void setSpeed(speed* s, int left_motor, int right_motor) {
	s->left_motor = left_motor;
	s->right_motor = right_motor;
}

void applySpeed(speed s) {
	setMotorSpeed(LeftMotor, s.left_motor);
	setMotorSpeed(RightMotor, s.right_motor);
}

task main()
{
	displayCenteredTextLine(1, "Version 3.2");

	// setMotorBrakeMode(LeftMotor, motorBrake);

	speed current_speed;
	setSpeed(&current_speed, speed_baseline, speed_baseline);
	applySpeed(current_speed);

	while(!isBlack(getColorReflected(ColorSensor))) {
	}

	speed previous_speed;
	copySpeed(&current_speed, previous_speed);

	while (true) {

		speed new_speed;
		initializeSpeed(&new_speed);

		float percent_deviation;
		int color_reflected = getColorReflected(ColorSensor);

		// Cap sensor value at reflective limits
		if(color_reflected < reflection_black) {
			color_reflected = reflection_black;
		}
		else if(color_reflected > reflection_white) {
			color_reflected = reflection_white;
		}

		if (isBlack(color_reflected)) {
			percent_deviation = ((float)reflection_centered - color_reflected)
					/(reflection_centered - reflection_black);

			setSpeed(&new_speed,
				speed_baseline + speed_differential * percent_deviation,
				speed_baseline + speed_differential * percent_deviation * -1
			);
		}
		else {
			percent_deviation = ((float)color_reflected - reflection_centered)
					/(reflection_white - reflection_centered);

			setSpeed(&new_speed,
				speed_baseline + speed_differential * percent_deviation * -1,
				speed_baseline + speed_differential * percent_deviation
			);
		}

		displayCenteredTextLine(3, "Color reflected: %d", color_reflected);
		if(isBlack(color_reflected)) {
			displayCenteredTextLine(4, "Percentage deviation: - %f", percent_deviation);
		}
		else {
			displayCenteredTextLine(4, "Percentage deviation: %f", percent_deviation);
		}
		displayCenteredTextLine(5, "Left speed: %d", new_speed.left_motor);
		displayCenteredTextLine(6, "Right speed: %d", new_speed.right_motor);

		applySpeed(new_speed);
	}
}
