#pragma config(Sensor, S1,     TouchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

const int ROOM_DISTANCE = 3175;
const int TURN_DISTANCE = 1275;
const int SPEED = 100;

enum WallStatus {
	NONE,
	NORMAL,
	EDGE,
	UNKNOWN
};

enum Direction{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

typedef struct {
	int x;
	int y;
} Location;

typedef struct {
	WallStatus north;
	WallStatus east;
	WallStatus south;
	WallStatus west;
	bool traversed;
} Room;

typedef struct {
	Room rooms[4][6];
} Maze;

typedef struct {
  // TODO: Implement stack
  int placeholder;
} Stack;

typedef struct {
	Direction direction;
	Location location;
	Stack previousMoves;
} Robot;

void goRight(){
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, TURN_DISTANCE + 20, SPEED/2);
	setMotorTarget(RightMotor, -(TURN_DISTANCE + 20), SPEED/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, -65, SPEED/2);
	setMotorTarget(RightMotor, -65, SPEED/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	sleep(100);
}

void goLeft(){
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);

	setMotorTarget(LeftMotor, -TURN_DISTANCE, SPEED/2);
	setMotorTarget(RightMotor, TURN_DISTANCE, SPEED/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, -65, SPEED/2);
	setMotorTarget(RightMotor, -65, SPEED/2);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	sleep(100);
}

int goForwards(){

	resetMotorEncoder(LeftMotor);
 	resetMotorEncoder(RightMotor);

 	setMotorTarget(LeftMotor, -ROOM_DISTANCE, SPEED);
	setMotorTarget(RightMotor, -ROOM_DISTANCE, SPEED);

	while(getMotorEncoder(LeftMotor) < ROOM_DISTANCE && getMotorEncoder(RightMotor) < ROOM_DISTANCE) {
			int distanceMoved = getMotorEncoder(LeftMotor);
			displayCenteredTextLine(5, "Starting: %d", getMotorEncoder(LeftMotor));

			if (getTouchValue(TouchSensor)){
				resetMotorEncoder(LeftMotor);
				resetMotorEncoder(RightMotor);

				setMotorTarget(LeftMotor, distanceMoved, SPEED);
				setMotorTarget(RightMotor, distanceMoved, SPEED);

				waitUntilMotorStop(LeftMotor);
				waitUntilMotorStop(RightMotor);


				displayCenteredTextLine(7, "End; %d", getMotorEncoder(LeftMotor));
				displayCenteredTextLine(11, "Difference: %d", distanceMoved - getMotorEncoder(LeftMotor));
				sleep(10000);
				return 1;
		}
	}

	sleep(100);
	return 0;
}

task main()
{
	goForwards();
  sleep(500);
}
