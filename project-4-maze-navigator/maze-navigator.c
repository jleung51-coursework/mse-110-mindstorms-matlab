#pragma config(Sensor, S1,     TouchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

#include <stack.h>

const int ROOM_DISTANCE = 3175;
const int TURN_DISTANCE = 1275;
const int US_DISTANCE_TO_WALL = 15;
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
	Direction direction;
	Location location;
	Stack previousMoves;
} Robot;

void turnRight(Robot r){
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

	sleep(500);

	push(r.previousMoves, Right);
}

void turnLeft(Robot r){
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

	sleep(500);

	push(r.previousMoves, Left);
}

bool goForwards(Robot r) {
	resetMotorEncoder(LeftMotor);
 	resetMotorEncoder(RightMotor);

 	setMotorTarget(LeftMotor, -ROOM_DISTANCE, SPEED);
	setMotorTarget(RightMotor, -ROOM_DISTANCE, SPEED);

	while(getMotorEncoder(LeftMotor) > -ROOM_DISTANCE || getMotorEncoder(RightMotor) > -ROOM_DISTANCE) {
		int distanceMoved = getMotorEncoder(LeftMotor);

		if (getTouchValue(TouchSensor)){
			resetMotorEncoder(LeftMotor);
			resetMotorEncoder(RightMotor);
			setMotorSpeed(LeftMotor, 0);
			setMotorSpeed(RightMotor, 0);
			sleep(1000);

			setMotorTarget(LeftMotor, -distanceMoved, SPEED);
			setMotorTarget(RightMotor, -distanceMoved, SPEED);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);

			sleep(500);
			return false;
		}
	}

	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	// Motor inaccuracy correction
	//*/
	resetMotorEncoder(LeftMotor);
	setMotorTarget(LeftMotor, 50, SPEED/10);
	waitUntilMotorStop(LeftMotor);
	//*/

	sleep(100);
	push(r.previousMoves, Forward);
	return true;
}

task main()
{
	datalogClose();
	datalogFlush();
	bool datalogOpenSuccess = datalogOpen(0, 1, false);
	if (!datalogOpenSuccess) {
		displayCenteredTextLine(6, "Unable to open datalog.");
	}
	else {
		displayCenteredTextLine(6, "");
	}

	Robot robot;
	const unsigned int len = 50;
	DirectionMoved arr[len] = {
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None,
		None, None, None, None, None
	};
	initializeStack(robot.previousMoves, arr, len);

	while(!getButtonPress(buttonAny)) {
		displayStack(robot.previousMoves); sleep(500);
		// No right wall
		if(getUSDistance(UltrasonicSensor) > US_DISTANCE_TO_WALL) {
			turnRight(robot);
			datalogAddChar(0, 'R');
		}
		bool result = goForwards(robot);
		datalogAddChar(0, 'F');
		if(!result) {
			turnLeft(robot);
			datalogAddChar(0, 'L');
		}
	}

	datalogClose();
}
