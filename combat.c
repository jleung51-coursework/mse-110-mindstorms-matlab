#pragma config(Sensor, S1,     ColorSensor,    sensorEV3_Color)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

const int threshold_white = 10;

void setSpeed(int left_motor_speed, int right_motor_speed) {
	setMotorSpeed(LeftMotor, left_motor_speed);
	setMotorSpeed(RightMotor, right_motor_speed);
}

task main()
{
	 
	while(true) {
		int color_reflected = getColorReflected(ColorSensor);
		displayCenteredBigTextLine(3, "Color: %d", color_reflected);
		
		if(color_reflected > threshold_white) {
			setSpeed(-100, -100);
			sleep(250);
			setSpeed(-40, 40);
			sleep(800);
			setSpeed(0, 0);
			sleep(300);
		}
		setSpeed(60, 6 0);
	}

}