#pragma config(Sensor, S1,     TouchSensor,    sensorEV3_Touch)
#pragma config(Sensor, S2,     UltrasonicSensor, sensorEV3_Ultrasonic)
#pragma config(Motor,  motorA,          RightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
#pragma config(Motor,  motorB,          LeftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)

/*
 * This Lego Mindstorms EV3 ROBOTC program navigates a maze of 4x6 cells.
 * Given an initial location and direction, it searches for a target location,
 * notifies the user when found, and returns from the target location to the
 * initial location using the quickest path found.
 *
 */

#include <map-essentials.h>
#include <stack.h>

// Constants

const unsigned int INITIAL_X = 0;
const unsigned int INITIAL_Y = 6;
const Direction INITIAL_DIRECTION = EAST;

const unsigned int DESTINATION_X = 3;
const unsigned int DESTINATION_Y = 6;

/* // Constants for geared wheels
const int ROOM_DISTANCE = 3175;
const int TURN_DISTANCE = 1275;
const int US_DISTANCE_TO_WALL = 15;
const int SPEED = 100;
*/

// Constants for ungeared medium wheels
const int ROOM_DISTANCE = 488;
const int TURN_DISTANCE = 194;
const int US_DISTANCE_TO_WALL = 8;
const int FORWARD_SPEED = 30;
const int TURN_SPEED = 40;

// Enums and structs

/*
enum WallStatus {
	NONE,
	NORMAL,
	EDGE,
	UNKNOWN
};

/*
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
*/

typedef struct {
	Direction direction;
	Location location;
	Stack previousMoves;
} Robot;

// Functions

void moveEncoderAndStop(
		float leftMotorPosition, int leftMotorSpeed,
		float rightMotorPosition, int rightMotorSpeed) {
	resetMotorEncoder(LeftMotor);
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, leftMotorPosition, leftMotorSpeed);
	setMotorTarget(RightMotor, rightMotorPosition, rightMotorSpeed);
	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);
}

// This funtion turns the robot 90 degrees right.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turnRight(Robot r){
	moveEncoderAndStop(TURN_DISTANCE, TURN_SPEED/2, -TURN_DISTANCE, TURN_SPEED/2);
	r.direction = getDirectionRight(r.direction);
	sleep(500);
}

// This funtion turns the robot 90 degrees left.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turnLeft(Robot r){
	moveEncoderAndStop(-TURN_DISTANCE, TURN_SPEED/2, TURN_DISTANCE, TURN_SPEED/2);
	r.direction = getDirectionLeft(r.direction);
	sleep(500);
}

// This function moves the robot forwards exactly one cell.
//
// The location and move history of Robot r will be updated accordingly.
// If there is a wall in front, the robot will return to its original position
// and return false.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned.
// Returns: Whether the robot successfully moved forward one cell.
bool goForwards(Robot r) {
	resetMotorEncoder(LeftMotor);
 	resetMotorEncoder(RightMotor);
 	setMotorTarget(LeftMotor, ROOM_DISTANCE, FORWARD_SPEED);
	setMotorTarget(RightMotor, ROOM_DISTANCE, FORWARD_SPEED);

	// Move forwards and stop after one cell
	while(getMotorEncoder(LeftMotor) < ROOM_DISTANCE ||
				getMotorEncoder(RightMotor) < ROOM_DISTANCE) {
		int distanceMoved = getMotorEncoder(LeftMotor);

		if (getTouchValue(TouchSensor)){
			resetMotorEncoder(LeftMotor);
			resetMotorEncoder(RightMotor);
			setMotorSpeed(LeftMotor, 0);
			setMotorSpeed(RightMotor, 0);
			sleep(1000);

			setMotorTarget(LeftMotor, -distanceMoved, FORWARD_SPEED);
			setMotorTarget(RightMotor, -distanceMoved, FORWARD_SPEED);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);

			sleep(500);
			return false;
		}
	}

	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	sleep(100);

	// Update move history for the robot
	// If the current move is reversing the most recent move, then the
	// most recent move is removed
	if(r.direction == getOppositeDirection(peek(r.previousMoves))) {
		pop(r.previousMoves);
	}
	else {
		push(r.previousMoves, r.direction);
	}

	setLocation(
			r.location,
			xAtDirection(r.location, r.direction),
			yAtDirection(r.location, r.direction)
	);

	// Move history
	string s;
	directionToString(peek(r.previousMoves), s);
	stringDelete(s, 1, 10);
	datalogAddChar(0, (char)atoi(s));
	return true;
}

void reverseAlongPreviousRooms(Robot r) {
	turnRight(r);
	turnRight(r);

	Direction nextDirection = getOppositeDirection(pop(r.previousMoves));

	while(nextDirection != NONE) {

		if(r.direction == NORTH) {
			if(nextDirection == WEST) turnLeft(r);
			if(nextDirection == EAST) turnRight(r);
		}
		else if(r.direction == EAST) {
			if(nextDirection == NORTH) turnLeft(r);
			if(nextDirection == SOUTH) turnRight(r);
		}
		else if(r.direction == SOUTH) {
			if(nextDirection == EAST) turnLeft(r);
			if(nextDirection == WEST) turnRight(r);
		}
		else if(r.direction == WEST) {
			if(nextDirection == SOUTH) turnLeft(r);
			if(nextDirection == NORTH) turnRight(r);
		}

		moveEncoderAndStop(ROOM_DISTANCE, FORWARD_SPEED, ROOM_DISTANCE, FORWARD_SPEED);
		setLocation(
				r.location,
				xAtDirection(r.location, r.direction),
				yAtDirection(r.location, r.direction)
		);

		nextDirection = getOppositeDirection(pop(r.previousMoves));
	}

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

	// Create and initialize robot
	Robot robot;

	const unsigned int len = 50;
	Direction arr[len];
	initializeStack(robot.previousMoves, arr, len);

	setLocation(robot.location, INITIAL_X, INITIAL_Y);
	robot.direction = INITIAL_DIRECTION;

	Location destination;
	setLocation(destination, DESTINATION_X, DESTINATION_Y);

	while(!equals(destination, robot.location)) {

		// Debugging: Display recorded moveset
		displayStack(robot.previousMoves);

		// Debugging: Display robot location and direction
		/*displayCenteredTextLine(1, "Current X: %d", robot.location.x);
		displayCenteredTextLine(2, "Current Y: %d", robot.location.y);
		displayCenteredTextLine(3, "New X: %d", xAtDirection(robot.location, robot.direction));
		displayCenteredTextLine(4, "New Y: %d", yAtDirection(robot.location, robot.direction));
		string s;
		directionToString(robot.direction, s);
		displayCenteredTextLine(5, "Direction: %s", s);
		*/

		// No right wall
		if(getUSDistance(UltrasonicSensor) > US_DISTANCE_TO_WALL) {
			turnRight(robot);
		}

		bool result = goForwards(robot);
		if(!result) {
			turnLeft(robot);
		}
	}

	playSound(soundFastUpwardTones);
	sleep(1000);

	reverseAlongPreviousRooms(robot);
	playSound(soundFastUpwardTones);
	sleep(1000);

	playTone(soundFastUpwardTones);
	sleep(1000);
	reverseAlongPreviousRooms(robot);
	playTone(soundFastUpwardTones);
	sleep(1000);

	datalogClose();
}
