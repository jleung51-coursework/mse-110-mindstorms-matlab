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

//#define NOTROBOTC  // Comment this out if running with the ROBOTC compiler

//#include "../robotc_dummy.h"

// Replace ROBOTC-specific variables defined at top of file
#ifdef NOTROBOTC
#include <stdlib.h>

const int TouchSensor = 0;
const int UltrasonicSensor = 0;
const int RightMotor = 0;
const int LeftMotor = 0;
#endif

#include "map-essentials.h"
#include "stack.h"

// Constants

const unsigned int INITIAL_X = 0;
const unsigned int INITIAL_Y = 5;
const Direction INITIAL_DIRECTION = EAST;

const unsigned int DESTINATION_X = 3;
const unsigned int DESTINATION_Y = 0;

// Constants for ungeared medium wheels
const int ROOM_DISTANCE = 490;
const int TURN_DISTANCE_RIGHT = 180;
const int TURN_DISTANCE_LEFT = 182;
const int US_DISTANCE_TO_WALL = 11;
const int FORWARD_SPEED = 30;
const int TURN_SPEED = 40;

// Structs

typedef enum TurnDirection {
	NONE,
	LEFT,
	RIGHT
} TurnDirection;

typedef struct {
	Direction direction;
	TurnDirection previousTurn;
	Location location;
	Stack previousMoves;
} Robot;

// Function prototypes

void moveEncoderAndStop(
		float leftMotorPosition, int leftMotorSpeed,
		float rightMotorPosition, int rightMotorSpeed);
void turnRight(Robot r);
void turnLeft(Robot r);
bool goForwards(Robot r);
void reverseAlongPreviousRooms(Robot r);

// Functions

// This function moves the motors to given encoder positions at given speeds
// and returns only when the motion is complete.
//
// Parameters:
//   float leftMotorPosition - Encoder position to move the left motor to.
//   int leftMotorSpeed- Speed at which to move the left motor.
//   float rightMotorPosition - Encoder position to move the right motor to.
//   float rightMotorPosition - Speed at which to move the right motor.
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

// This function turns the robot 90 degrees right.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turnRight(Robot r) {
	unsigned int turnDistance = TURN_DISTANCE_RIGHT;
	if(r.previousTurn != NONE && r.previousTurn != RIGHT) {
		turnDistance += 2;
	}

	moveEncoderAndStop(turnDistance, TURN_SPEED/2, -turnDistance, TURN_SPEED/2);
	r.direction = getDirectionRight(r.direction);
	r.previousTurn = RIGHT;
	sleep(100);
}

// This function turns the robot 90 degrees left.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turnLeft(Robot r) {
	unsigned int turnDistance = TURN_DISTANCE_LEFT;
	if(r.previousTurn != NONE && r.previousTurn != LEFT) {
		turnDistance += 2;
	}

	moveEncoderAndStop(-turnDistance, TURN_SPEED/2, turnDistance, TURN_SPEED/2);
	r.direction = getDirectionLeft(r.direction);
	r.previousTurn = LEFT;
	sleep(100);
}

// This function turns the robot 180 degrees.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turn180(Robot r) {
	moveEncoderAndStop(
			TURN_DISTANCE_RIGHT*2, TURN_SPEED/2,
			-TURN_DISTANCE_RIGHT*2, TURN_SPEED/2
	);
	r.direction = getOppositeDirection(r.direction);
	sleep(100);
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
	while(getMotorEncoder(LeftMotor) < ROOM_DISTANCE*0.9 ||
				getMotorEncoder(RightMotor) < ROOM_DISTANCE*0.9) {
		int distanceMoved = getMotorEncoder(LeftMotor);

		if (getTouchValue(TouchSensor)){
			resetMotorEncoder(LeftMotor);
			resetMotorEncoder(RightMotor);
			setMotorSpeed(LeftMotor, 0);
			setMotorSpeed(RightMotor, 0);
			sleep(200);

			setMotorTarget(LeftMotor, -distanceMoved, FORWARD_SPEED);
			setMotorTarget(RightMotor, -distanceMoved, FORWARD_SPEED);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);

			sleep(100);
			return false;
		}
	}

	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

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
			&r.location,
			xAtDirection(r.location, r.direction),
			yAtDirection(r.location, r.direction)
	);

	r.previousTurn = NONE;

	// Move history
	string s;
	directionToString(peek(r.previousMoves), &s);
	stringDelete(s, 1, 10);
	datalogAddChar(0, (char)atoi(s));
	return true;
}

void reverseAlongPreviousRooms(Robot r) {
	turn180(r);

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

		unsigned int consecutiveRooms = 1;
		while(getOppositeDirection(peek(r.previousMoves)) == nextDirection) {
			++consecutiveRooms;
			pop(r.previousMoves);
		}

		moveEncoderAndStop(
				ROOM_DISTANCE * consecutiveRooms, FORWARD_SPEED,
				ROOM_DISTANCE * consecutiveRooms, FORWARD_SPEED
		);
		setLocation(
				&r.location,
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

	setLocation(&robot.location, INITIAL_X, INITIAL_Y);
	robot.direction = INITIAL_DIRECTION;
	robot.previousTurn = NONE;

	Location destination;
	setLocation(&destination, DESTINATION_X, DESTINATION_Y);

	while(!equals(destination, robot.location)) {

		// Debugging: Display recorded moveset
		displayStack(robot.previousMoves);

		// Debugging: Display robot location and direction
		displayCenteredTextLine(1, "Current X: %d", robot.location.x);
		displayCenteredTextLine(2, "Current Y: %d", robot.location.y);
		displayCenteredTextLine(3, "New X: %d", xAtDirection(robot.location, robot.direction));
		displayCenteredTextLine(4, "New Y: %d", yAtDirection(robot.location, robot.direction));
		string s;
		directionToString(robot.direction, s);
		displayCenteredTextLine(5, "Direction: %s", s);

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
	sleep(300);

	reverseAlongPreviousRooms(robot);
	playSound(soundFastUpwardTones);
	sleep(300);

	datalogClose();
}