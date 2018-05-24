#pragma config(Sensor, S3,     ColorSensor,    sensorEV3_Color)
#pragma config(Sensor, S4,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)


/*
* This ROBOTC for Lego Mindstorms program follows a black line.
*
*/

// Max speed is 100 or -100
const int speed = 25;
// Black if less than threshold; white if greater than threshold
const int reflectionThreshold = 35;

const int turn_threshold = 30;  // Counts of pure white or pure black before turning sharply to find line again
int persistent_black = 0;
int persistent_white = 0;

bool isBlack(int val) {
	return val < reflectionThreshold;
}

task main()
{
	// setMotorBrakeMode(LeftMotor, motorBrake);

	setMotorSpeed(LeftMotor, speed);
	setMotorSpeed(RightMotor, speed);
	//while(!isBlack(getColorReflected(ColorSensor))) {}

	while (true) {

		int colorReflected = getColorReflected(ColorSensor);
		displayCenteredTextLine(5, "%d", colorReflected);

		/*
		if(colorReflected < 10) {
			setMotorSpeed(LeftMotor, speed*0.5);
			setMotorSpeed(RightMotor, speed*-0.5);
		}
		else if(colorReflected > 40) {
			setMotorSpeed(LeftMotor, speed*-0.5);
			setMotorSpeed(RightMotor, speed*0.5);
		}
		*/
		
		
		if (getUSDistance(UltrasonicSensor) <= 15){
		
		
		
			setMotorSpeed(LeftMotor, 0);
			setMotorSpeed(RightMotor, 0);
			
			sleep(2000);
			playTone(500, 5);
	
			resetMotorEncoder(LeftMotor);
      resetMotorEncoder(RightMotor);
			setMotorTarget(LeftMotor, 120, 30);
      setMotorTarget(RightMotor, 120, 30);	
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);
			sleep(500);
			
			resetMotorEncoder(LeftMotor);
      resetMotorEncoder(RightMotor);
			
			setMotorTarget(LeftMotor, 180, 30);
      setMotorTarget(RightMotor, -180, 30);	
      
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);
			
			sleep(500);
			
			setMotorTarget(LeftMotor, 30, 30);
      setMotorTarget(RightMotor, -30, 30);	
      
      waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);
			
			sleep(500);
			
			resetMotorEncoder(LeftMotor);
      resetMotorEncoder(RightMotor);
			setMotorTarget(LeftMotor, -120, 30);
      setMotorTarget(RightMotor, -120, 30);	
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);
			sleep(500);
			
		}

		bool path_chosen = false;

		// Hard-correct sensor value at reflective limits
		if(colorReflected <= 10) {
			++persistent_black;
			if(persistent_black > turn_threshold) {
				setMotorSpeed(LeftMotor, speed*0.5);
				setMotorSpeed(RightMotor, speed*-0.6);
			}
			path_chosen = true;
		}
		else {
			persistent_black = 0;
		}

		if(colorReflected >= 40) {
			++persistent_white;
			if(persistent_white > turn_threshold) {
				setMotorSpeed(LeftMotor, speed*-0.6);
				setMotorSpeed(RightMotor, speed*0.5);
			}
			path_chosen = true;
		}
		else {
			persistent_white = 0;
		}

		if(path_chosen == false) {
			if(18 < colorReflected && colorReflected < 36) {
				setMotorSpeed(LeftMotor, speed*0.75);
				setMotorSpeed(RightMotor, speed*0.75);
			}
			else if(isBlack(colorReflected)) {
				setMotorSpeed(LeftMotor, speed*0.50);
				setMotorSpeed(RightMotor, speed*0.30);
			}
			else {
				setMotorSpeed(LeftMotor, speed*0.30);
				setMotorSpeed(RightMotor, speed*0.50);
			}
		}

		displayCenteredTextLine(2, "Version 3.6");
		displayCenteredTextLine(4, "Color reflected: %d", colorReflected);
		displayCenteredTextLine(5, "Left speed: %d", getMotorSpeed(LeftMotor));
		displayCenteredTextLine(6, "Right speed: %d", getMotorSpeed(RightMotor));
		
	}
}