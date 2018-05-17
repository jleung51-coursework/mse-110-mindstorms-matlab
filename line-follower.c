/*
 * This ROBOTC for Lego Mindstorms program follows a black line.
 *
 */

#pragma config(Sensor, S3,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

// Max speed is 100 or -100
const int speed = 15;
// Black if less than threshold; white if greater than threshold
const int reflectionThreshold = 40;

bool isBlack(int val) {
	return val < reflectionThreshold;
}

task main()
{
	setMotorSpeed(LeftMotor, speed);
	setMotorSpeed(RightMotor, speed);
	while(isWhite(getColorReflected(ColorSensor)) {}

	while (true) {
		int colorReflected = getColorReflected(ColorSensor);
		displayCenteredTextLine(5, "%d", colorReflected);

		if (isBlack(colorReflected)){
			setMotorSpeed(LeftMotor, -speed);
			setMotorSpeed(RightMotor, speed);
		} else {
			setMotorSpeed(LeftMotor, speed);
			setMotorSpeed(RightMotor, -speed);
		}
	}
}
