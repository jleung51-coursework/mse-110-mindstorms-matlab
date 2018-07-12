#pragma config(Sensor, S1,     TouchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

int cellDistance = 3175;
int cellDistanceTurn = 1275;
int speed = 100;

typedef struct {
	int x;
	int y;
} coordinate;

typdef enum {
	none, normal, edge, unknown	
} wallStatus;

typedef struct {
	wallStatus north;
	wallStatus east;
	wallStatus south;
	wallStatus west;
	bool traversed;
} room;

typedef struct {
	
} stack;

typedef enum {
	north, south, east, west
} direction;

typedef struct {
	room Rooms[4][6];
} maze;

typedef struct {
	direction direction;
	coordinate coordinate;
	stack previousMoves;
} robot;

void goRight(){
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, cellDistanceTurn + 20, speed/2);
	setMotorTarget(RightMotor, -(cellDistanceTurn + 20), speed/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);
	
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, -65, speed/2);
	setMotorTarget(RightMotor, -65, speed/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);
	
	sleep(100);
}

void goLeft(){
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	
	setMotorTarget(LeftMotor, -cellDistanceTurn, speed/2);
	setMotorTarget(RightMotor, cellDistanceTurn, speed/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);
	
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, -65, speed/2);
	setMotorTarget(RightMotor, -65, speed/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);
	
	sleep(100);
}

int goForwards(){

	resetMotorEncoder(LeftMotor);
 	resetMotorEncoder(RightMotor);

 	setMotorTarget(LeftMotor, -cellDistance, speed);
	setMotorTarget(RightMotor, -cellDistance, speed);
	
	while(getMotorEncoder(LeftMotor) < cellDistance && getMotorEncoder(RightMotor) < cellDistance) {
			int distance = getMotorEncoder(LeftMotor);
			displayCenteredTextLine(5, "Starting: %d", getMotorEncoder(LeftMotor));
			
			if (getTouchValue(TouchSensor)){
				resetMotorEncoder(LeftMotor);
				resetMotorEncoder(RightMotor);
				
				setMotorTarget(LeftMotor, distance, speed);
				setMotorTarget(RightMotor, distance, speed);
				
				waitUntilMotorStop(LeftMotor);
				waitUntilMotorStop(RightMotor);
				
				
				displayCenteredTextLine(7, "End; %d", getMotorEncoder(LeftMotor));
				displayCenteredTextLine(11, "Difference: %d", distance - getMotorEncoder(LeftMotor));
				sleep(10000);
				return 1;
		}
	}
	
	sleep(100);
	return 0;
	
	//waitUntilMotorStop(LeftMotor);
	//waitUntilMotorStop(RightMotor);
	
}

task main()
{
	goForwards();
  sleep(500);
}