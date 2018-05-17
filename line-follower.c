#pragma config(Sensor, S3,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

/*
* This ROBOTC for Lego Mindstorms program follows a black line.
*
*/

// Max speed is 100 or -100
const int speed = 40;
// Black if less than threshold; white if greater than threshold
const int reflectionThreshold = 35;

bool isBlack(int val) {
	return val < reflectionThreshold;
}

task main()
{
	// setMotorBrakeMode(LeftMotor, motorBrake);

	setMotorSpeed(LeftMotor, speed);
	setMotorSpeed(RightMotor, speed);
	while(!isBlack(getColorReflected(ColorSensor))) {}


	int previousLeftSpeed = speed;
	int previousRightSpeed = speed;

	while (true) {
		displayCenteredTextLine(2, "Version 2.8");
		int colorReflected = getColorReflected(ColorSensor);
		displayCenteredTextLine(5, "%d", colorReflected);

		if(colorReflected < 10) {
			int newLeftSpeed = speed*0.5;
			int newRightSpeed = speed*-0.5;
		
			setMotorSpeed(LeftMotor, speed*0.5);
			setMotorSpeed(RightMotor, speed*-0.5);
		}
		else if(colorReflected > 42) {
			setMotorSpeed(LeftMotor, speed*-0.5);
			setMotorSpeed(RightMotor, speed*0.5);
		}

		else if(isBlack(colorReflected)) {
			float difference = reflectionThreshold - colorReflected;
			float ratio = 0.5;

			setMotorSpeed(LeftMotor, (1/2)*speed);
			setMotorSpeed(RightMotor, (1/4)*speed);
		}
		else {
			float difference = colorReflected - reflectionThreshold;
			float ratio = 0.5;

			setMotorSpeed(LeftMotor, (1/4)*speed);
			setMotorSpeed(RightMotor, (1/2)*speed);
		}
	}
}