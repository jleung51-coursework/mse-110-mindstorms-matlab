#pragma once

#ifdef NOTROBOTC
typedef char* string;
#endif

typedef enum Direction {
	NONE_DIRECTION,
	NORTH,
	EAST,
	SOUTH,
	WEST
} Direction;

typedef struct {
	unsigned int x;
	unsigned int y;
} Location;

// Function prototypes

void directionToString(Direction l, string* s);
Direction getOppositeDirection(Direction d);
Direction getDirectionLeft(Direction d);
Direction getDirectionRight(Direction d);

void setLocation(Location* l, unsigned int x, unsigned int y);
bool equals(Location l1, Location l2);
unsigned int xAtDirection(Location l, Direction d);
unsigned int yAtDirection(Location l, Direction d);

// Functions

void directionToString(Direction l, string* s) {
	switch(l) {
		case NORTH:
			*s = "NORTH";
			break;
		case WEST:
			*s = "WEST";
			break;
		case SOUTH:
			*s = "SOUTH";
			break;
		case EAST:
			*s = "EAST";
			break;
		default:
		  *s = "";
	}
}

Direction getOppositeDirection(Direction d) {
	if(d == NORTH) return SOUTH;
	else if(d == SOUTH) return NORTH;
	else if(d == EAST) return WEST;
	else if(d == WEST) return EAST;
	else return NONE_DIRECTION;

	switch(d) {
		case NORTH:
			return SOUTH;
		case EAST:
			return WEST;
		case SOUTH:
			return NORTH;
		case WEST:
			return EAST;
		default:
			return NONE_DIRECTION;
	}
}

Direction getDirectionLeft(Direction d) {
	switch(d) {
		case NORTH:
			return WEST;
		case WEST:
			return SOUTH;
		case SOUTH:
			return EAST;
		case EAST:
			return NORTH;
		default:
			return NONE_DIRECTION;
	}
}

Direction getDirectionRight(Direction d) {
	switch(d) {
		case NORTH:
			return EAST;
		case EAST:
			return SOUTH;
		case SOUTH:
			return WEST;
		case WEST:
			return NORTH;
		default:
			return NONE_DIRECTION;
	}
}

void setLocation(Location* l, unsigned int x, unsigned int y) {
	l->x = x;
	l->y = y;
}

bool equals(Location l1, Location l2) {
	return l1.x == l2.x && l1.y == l2.y;
}

// If the room is at an edge and the direction points towards the edge,
// returns 0
unsigned int xAtDirection(Location l, Direction d) {
	switch(d) {
		case EAST:
			return l.x + 1;
		case WEST:
			if(l.x == 0) {
				return 0;
			}
			return l.x - 1;
		default:
			return l.x;
	}
}

// If the room is at an edge and the direction points towards the edge,
// returns 0
unsigned int yAtDirection(Location l, Direction d) {
	switch(d) {
		case SOUTH:
			return l.y + 1;
		case NORTH:
			if(l.y == 0) {
				return 0;
			}
			return l.y - 1;
		default:
			return l.y;
	}
}
