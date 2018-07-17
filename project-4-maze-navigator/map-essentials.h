#pragma once

enum Direction {
	NONE,
	NORTH,
	EAST,
	SOUTH,
	WEST
};

typedef struct {
	unsigned int x;
	unsigned int y;
} Location;

void directionToString(Direction l, string& s) {
	switch(l) {
		case NORTH:
			s = "NORTH";
			break;
		case WEST:
			s = "WEST";
			break;
		case SOUTH:
			s = "SOUTH";
			break;
		case EAST:
			s = "EAST";
			break;
		default:
		  s = "";
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
			return NONE;
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
			return NONE;
	}
}

void locationToString(Location l, string& s) {
	string x = "";
	sprintf(x, "%d", l.x);

	string y = "";
	sprintf(y, "%d", l.y);

	sprintf(s, "(%d, %d)", l.x, l.y);
}

void setLocation(Location& l, unsigned int x, unsigned int y) {
	l.x = x;
	l.y = y;
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

Direction directionOfNewLocation(Location old, Location new) {
	if(new.x < old.x) {
		return WEST;
	}
	else if(new.x > old.x) {
		return EAST;
	}
	else if(new.y < old.y) {
		return NORTH;
	}
	else if(new.y > old.y) {
		return SOUTH;
	}
	else {
		return NONE;
	}
}