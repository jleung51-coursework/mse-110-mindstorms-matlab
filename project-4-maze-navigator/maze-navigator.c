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

// Add header files with other functions
#include "map-essentials.h"
#include "stack.h"

// Constants

// Set original x and y coordinates, as well as direction of robot
const unsigned int INITIAL_X = 0;
const unsigned int INITIAL_Y = 5;
const Direction INITIAL_DIRECTION = EAST;

// Set destination x and y coordinates
const unsigned int DESTINATION_X = 3;
const unsigned int DESTINATION_Y = 0;

// Constants for ungeared medium wheels
const int ROOM_DISTANCE = 490;  // Distance of one cell
const int TURN_DISTANCE_RIGHT = 182;  // Wheel encoder value to turn right
const int TURN_DISTANCE_LEFT = 180;  // Wheel encoder value to turn left
const int US_DISTANCE_TO_WALL = 11;  // Distance between ultrasonic sensor and wall
const int FORWARD_SPEED = 30;  // Speed for which the robot should go forwards and backwards
const int TURN_SPEED = 40; // Speed for which the robot should turn right or left

// Structs

typedef enum TurnDirection {  // Holds a single history of a turn
	NONE,
	LEFT,
	RIGHT
} TurnDirection;

typedef struct {  // Holds all values relating to a robot's location, direction, and previous moves.
	Direction direction;
	TurnDirection previousTurn;  // Which way it turned on the previous move
	Location location;
	Stack previousMoves;  // History of optimal previous moves and turns so it can return to the original position
} Robot;

// Function prototypes

void moveEncoderAndStop(
		float leftMotorPosition, int leftMotorSpeed,
		float rightMotorPosition, int rightMotorSpeed);
void turnRight(Robot r);
void turnLeft(Robot r);
void turn180(Robot r);
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
	resetMotorEncoder(LeftMotor);  // Set encoder values to 0
	resetMotorEncoder(RightMotor);
	setMotorTarget(LeftMotor, leftMotorPosition, leftMotorSpeed);  // Run motors to a position at a set speed
	setMotorTarget(RightMotor, rightMotorPosition, rightMotorSpeed);
	waitUntilMotorStop(LeftMotor);  // Stop motors fully before returning the function
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
	if(r.previousTurn != NONE && r.previousTurn != RIGHT) {  // If the robot turned left and is now turning right
		turnDistance += 2;  // Turn a little more to correct for encoder issues
	}

	moveEncoderAndStop(turnDistance, TURN_SPEED/2, -turnDistance, TURN_SPEED/2);  // Turn robot to the right
	r.direction = getDirectionRight(r.direction);  // Shift the stored robot direction to the right
	r.previousTurn = RIGHT;  // Store the turn for future processing
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
	if(r.previousTurn != NONE && r.previousTurn != LEFT) {  // If the robot turned right and is now turning left
		turnDistance += 2;  // Turn a little more to correct for encoder issues
	}

	moveEncoderAndStop(-turnDistance, TURN_SPEED/2, turnDistance, TURN_SPEED/2);  // Turn robot to the left
	r.direction = getDirectionLeft(r.direction);  // Shift the stored robot direction to the right
	r.previousTurn = LEFT;  // Store the turn for future processing
	sleep(100);
}

// This function turns the robot 180 degrees.
//
// The direction of Robot r will be updated accordingly.
//
// Parameters:
//   Robot r - Robot object which can be moved and turned
void turn180(Robot r) {
	moveEncoderAndStop(  // Turns the robot all the way around to face the other way
			TURN_DISTANCE_RIGHT*2, TURN_SPEED/2,
			-TURN_DISTANCE_RIGHT*2, TURN_SPEED/2
	);
	r.direction = getOppositeDirection(r.direction);  // Shift the stored robot direction to the opposite direction
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
	resetMotorEncoder(LeftMotor);  // Set motor encoder values to 0
 	resetMotorEncoder(RightMotor);
 	setMotorTarget(LeftMotor, ROOM_DISTANCE, FORWARD_SPEED);  // Move robot forwards
	setMotorTarget(RightMotor, ROOM_DISTANCE, FORWARD_SPEED);

	// Move forwards and stop after one cell
	while(getMotorEncoder(LeftMotor) < ROOM_DISTANCE*0.9 ||
				getMotorEncoder(RightMotor) < ROOM_DISTANCE*0.9) {

		if (getTouchValue(TouchSensor)){  // If there is a wall in front

			// Stop the motors
			resetMotorEncoder(LeftMotor);
			resetMotorEncoder(RightMotor);
			setMotorSpeed(LeftMotor, 0);
			setMotorSpeed(RightMotor, 0);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);

			// Correct robot direction by pushing forcefully against the wall
			moveEncoderAndStop(120, 30, 120, 30);
			resetMotorEncoder(LeftMotor);
			resetMotorEncoder(RightMotor);
			sleep(200);

			// Move backwards to center of cell
			setMotorTarget(LeftMotor, -110, FORWARD_SPEED);
			setMotorTarget(RightMotor, -110, FORWARD_SPEED);
			waitUntilMotorStop(LeftMotor);
			waitUntilMotorStop(RightMotor);

			sleep(100);
			return false;  // Tell the caller that the robot did not move forward successfully
		}
	}

	waitUntilMotorStop(LeftMotor);
	waitUntilMotorStop(RightMotor);

	// Update move history for the robot
	if(r.direction == getOppositeDirection(peek(r.previousMoves))) {
		// If the current move is reversing the most recent move, then the
		// most recent move is removed
		pop(r.previousMoves);
	}
	else {
		// Add new optimal path to history
		push(r.previousMoves, r.direction);
	}

	// Update location for robot to the new, current cell
	setLocation(
			&r.location,
			xAtDirection(r.location, r.direction),
			yAtDirection(r.location, r.direction)
	);

	// Change previous turn history to nothing
	r.previousTurn = NONE;

	// Store move history in datalog for debugging
	string s;
	directionToString(peek(r.previousMoves), &s);
	stringDelete(s, 1, 10);
	datalogAddChar(0, (char)atoi(s));
	return true;
}

// This function reverses the robot along the stored history of previous moves.
// The robot will first turn 180 degrees to face the other direction.
//
// Parameters:
//   Robot r -- Robot object which can be moved and turned
void reverseAlongPreviousRooms(Robot r) {
	turn180(r);  // Face the other way

	Direction nextDirection = getOppositeDirection(pop(r.previousMoves));

	while(nextDirection != NONE) {  // Go through the entire stack of moves

		if(r.direction == NORTH) {  // Correct for new direction
			if(nextDirection == WEST) turnLeft(r);
			if(nextDirection == EAST) turnRight(r);
		}
		else if(r.direction == EAST) {  // Correct for new direction
			if(nextDirection == NORTH) turnLeft(r);
			if(nextDirection == SOUTH) turnRight(r);
		}
		else if(r.direction == SOUTH) {  // Correct for new direction
			if(nextDirection == EAST) turnLeft(r);
			if(nextDirection == WEST) turnRight(r);
		}
		else if(r.direction == WEST) {  // Correct for new direction
			if(nextDirection == SOUTH) turnLeft(r);
			if(nextDirection == NORTH) turnRight(r);
		}

		// Go forwards (multiple times without turning if possible)
		unsigned int consecutiveRooms = 1;
		while(getOppositeDirection(peek(r.previousMoves)) == nextDirection) {
			++consecutiveRooms;  // Add another room to go forwards
			pop(r.previousMoves);
			// Update location
			setLocation(
					&r.location,
					xAtDirection(r.location, r.direction),
					yAtDirection(r.location, r.direction)
			);
		}

		// Move forwards through 1+ rooms
		moveEncoderAndStop(
				ROOM_DISTANCE * consecutiveRooms, FORWARD_SPEED,
				ROOM_DISTANCE * consecutiveRooms, FORWARD_SPEED
		);

		// Get next direction from the stack
		nextDirection = getOppositeDirection(pop(r.previousMoves));
	}

}

// Main function which is run by default
task main()
{
	// Set up datalog for debugging output later
	datalogClose();
	datalogFlush();
	bool datalogOpenSuccess = datalogOpen(0, 1, false);
	if (!datalogOpenSuccess) {
		displayCenteredTextLine(6, "Unable to open datalog.");
	}
	else {
		displayCenteredTextLine(6, "");
	}

	// Create robot
	Robot robot;

	// Generate stack for the history of previous moves
	const unsigned int len = 50;
	Direction arr[len];  // Create new array
	initializeStack(robot.previousMoves, arr, len);  // Set the array to be the stack

	setLocation(&robot.location, INITIAL_X, INITIAL_Y);  // Initialize location
	robot.direction = INITIAL_DIRECTION;  // Initialize direction
	robot.previousTurn = NONE;

	Location destination;
	setLocation(&destination, DESTINATION_X, DESTINATION_Y);  // Create destination location with x and y coordinates set above

	while(!equals(destination, robot.location)) {

		// Debugging: Display recorded moveset
		displayStack(robot.previousMoves);

		// Debugging: Display robot location and direction
		displayCenteredTextLine(1, "Current X: %d", robot.location.x);
		displayCenteredTextLine(2, "Current Y: %d", robot.location.y);
		displayCenteredTextLine(3, "New X: %d", xAtDirection(robot.location, robot.direction));
		displayCenteredTextLine(4, "New Y: %d", yAtDirection(robot.location, robot.direction));
		string s;
		directionToString(robot.direction, &s);
		displayCenteredTextLine(5, "Direction: %s", s);

		// No right wall
		if(getUSDistance(UltrasonicSensor) > US_DISTANCE_TO_WALL) {
			turnRight(robot);
		}

		// Try to move forwards
		bool result = goForwards(robot);
		if(!result) {  // Wall was in front
			turnLeft(robot);  // Turn left to go around the wall
		}
	}

	// Reached destination
	playSound(soundFastUpwardTones);
	sleep(300);

	// Go back to original location along optimal path
	reverseAlongPreviousRooms(robot);
	playSound(soundFastUpwardTones);
	sleep(300);

	datalogClose();
}
