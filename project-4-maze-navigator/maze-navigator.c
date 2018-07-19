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
const int ROOM_DISTANCE = 465;
const int TURN_DISTANCE = 194;
const int US_DISTANCE_TO_WALL = 5;
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
	Stack previousRooms;
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

	// Correct position for geared wheels
	//moveEncoderAndStop(-65, SPEED/2, -65, SPEED/2);

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

	// Correct position for geared wheels
	// moveEncoderAndStop(-65, SPEED/2, -65, SPEED/2);

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

	// Motor inaccuracy correction for geared wheels
	/*/
	resetMotorEncoder(LeftMotor);
	setMotorTarget(LeftMotor, 50, SPEED/10);
	waitUntilMotorStop(LeftMotor);
	//*/

	sleep(100);

	Location newRoom;
	setLocation(newRoom,
			xAtDirection(r.location, r.direction),
			yAtDirection(r.location, r.direction)
	);

	Location previousRoomOnStack;
	setLocation(previousRoomOnStack,
			peekX(r.previousRooms),
			peekY(r.previousRooms)
	);

	// Update move history for the robot
	// If the current move is repeating the most recent move, then the
	// most recent move is removed
	if(equals(previousRoomOnStack, newRoom)) {
		pop(r.previousRooms);
	}
	else {
		push(r.previousRooms, r.location);
	}
	r.location = newRoom;

	// Move history
	datalogAddShort(0, peekX(r.previousRooms));
	datalogAddShort(1, peekY(r.previousRooms));
	return true;
}

void reverseAlongPreviousRooms(Robot r) {
	turnRight(r);
	turnRight(r);

	Location nextRoom;
	setLocation(nextRoom, peekX(r.previousRooms), peekY(r.previousRooms));
	bool successfulPop = pop(r.previousRooms);

	while(successfulPop) {

		Direction d = directionOfNewLocation(r.location, nextRoom);
		if(r.direction == NORTH) {
			if(d == WEST) turnLeft(r);
			if(d == EAST) turnRight(r);
		}
		else if(r.direction == EAST) {
			if(d == NORTH) turnLeft(r);
			if(d == SOUTH) turnRight(r);
		}
		else if(r.direction == SOUTH) {
			if(d == EAST) turnLeft(r);
			if(d == WEST) turnRight(r);
		}
		else if(r.direction == WEST) {
			if(d == SOUTH) turnLeft(r);
			if(d == NORTH) turnRight(r);
		}

		moveEncoderAndStop(ROOM_DISTANCE, FORWARD_SPEED, ROOM_DISTANCE, FORWARD_SPEED);
		r.location = nextRoom;

		setLocation(nextRoom, peekX(r.previousRooms), peekY(r.previousRooms));
		successfulPop = pop(r.previousRooms);
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
	Location arr[len];
	initializeStack(robot.previousRooms, arr, len);

	setLocation(robot.location, INITIAL_X, INITIAL_Y);
	robot.direction = INITIAL_DIRECTION;

	Location destination;
	setLocation(destination, DESTINATION_X, DESTINATION_Y);

	Location destination;
	setLocation(destination, DESTINATION_X, DESTINATION_Y);

	//turnRight(robot);sleep(500);return;
	//turnRight(robot);sleep(500);turnRight(robot);sleep(500);turnRight(robot);sleep(500);turnRight(robot);sleep(500);return;
	//turnLeft(robot);sleep(500);turnLeft(robot);sleep(500);turnLeft(robot);sleep(500);turnLeft(robot);sleep(500);return;
	//goForwards(robot);sleep(500);return;

	while(!equals(destination, robot.location)) {

		// Debugging: Display recorded moveset
		displayStack(robot.previousRooms);

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

	playTone(soundFastUpwardTones);
	sleep(1000);
	reverseAlongPreviousRooms(robot);
	playTone(soundFastUpwardTones);
	sleep(1000);

	playTone(soundFastUpwardTones);
	sleep(1000);
	reverseAlongPreviousRooms(robot);
	playTone(soundFastUpwardTones);
	sleep(1000);

	datalogClose();
}
