#pragma config(Sensor, S1,     TouchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorB,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)

#include <map-essentials.h>
#include <stack.h>

const unsigned int INITIAL_X = 0;
const unsigned int INITIAL_Y = 6;
const Direction INITIAL_DIRECTION = EAST;

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
	r.direction = getDirectionRight(r.direction);
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
	r.direction = getDirectionLeft(r.direction);
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

	Location newRoom;
	newRoom.x = xAtDirection(r.location, r.direction);
	newRoom.y = yAtDirection(r.location, r.direction);

	Location previousRoomOnStack;
	previousRoomOnStack.x = peekX(r.previousMoves);
	previousRoomOnStack.y = peekY(r.previousMoves);

	if(equals(previousRoomOnStack, newRoom)) {
		pop(r.previousMoves);
	}
	else {
		push(r.previousMoves, r.location);
	}
	r.location = newRoom;

	datalogAddShort(0, peekX(r.previousMoves));
	datalogAddShort(1, peekY(r.previousMoves));
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
	Location arr[len];
	initializeStack(robot.previousMoves, arr, len);

	setLocation(robot.location, INITIAL_X, INITIAL_Y);
	robot.direction = INITIAL_DIRECTION;

	while(!getButtonPress(buttonAny)) {

		displayStack(robot.previousMoves); sleep(1000);
		// No right wall
		if(getUSDistance(UltrasonicSensor) > US_DISTANCE_TO_WALL) {
			turnRight(robot);
		}

		displayCenteredTextLine(1, "Current X: %d", robot.location.x);
		displayCenteredTextLine(2, "Current Y: %d", robot.location.y);
		displayCenteredTextLine(3, "New X: %d", xAtDirection(robot.location, robot.direction));
		displayCenteredTextLine(4, "New Y: %d", yAtDirection(robot.location, robot.direction));
		string s;
		directionToString(robot.direction, s);
		displayCenteredTextLine(5, "Direction: %s", s);



		bool result = goForwards(robot);
		if(!result) {
			turnLeft(robot);
		}
	}

	datalogClose();
}
