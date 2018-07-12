#pragma once

#include <map-essentials.h>

// Prototypes

typedef struct Stack;

// Structs

typedef struct {
	unsigned int top_of_stack;
	unsigned int len;
	Location* arr;
} Stack;

// Functions

void initializeStack(Stack s, Location* arr, unsigned int len) {
	s.top_of_stack = 0;
	s.len = len;
	s.arr = arr;

	for(unsigned int i = 0; i < s.len; ++i) {
		s.arr[i] = NONE;
	}
}

void displayStack(Stack s) {
	for(unsigned int i = 0; i < s.len; ++i) {
		string str = "";
		Location extraLocationBecauseRobotcSucks;
		extraLocationBecauseRobotcSucks.x = s.arr[i].x;
		extraLocationBecauseRobotcSucks.y = s.arr[i].y;
		locationToString(extraLocationBecauseRobotcSucks, str);
		displayCenteredTextLine(i, str);
	}
}

unsigned int peekX(Stack s) {
	if(s.top_of_stack == 0) {
		return NONE;
	}
	return s.arr[s.top_of_stack-1].x;
}

unsigned int peekY(Stack s) {
	if(s.top_of_stack == 0) {
		return NONE;
	}
	return s.arr[s.top_of_stack-1].y;
}


bool push(Stack s, Location l) {
	if(s.top_of_stack == s.len) {
		return false;
	}
	s.arr[s.top_of_stack] = l;
	++s.top_of_stack;
	return true;
}

bool pop(Stack s) {
	if(s.top_of_stack == 0) {
		return false;
	}
	s.arr[s.top_of_stack-1] = NONE;
	--s.top_of_stack;
	return true;
}
