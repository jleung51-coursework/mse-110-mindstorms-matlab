/**
 * This ROBOTC header file provides a basic data structure representing a
 * stack of Directions.
 *
 */

#pragma once

#include "map-essentials.h"

// Structs

typedef struct {
	unsigned int top_of_stack;
	unsigned int len;
	Direction* arr;
} Stack;

// Function prototypes

void initializeStack(Stack s, Direction* arr, unsigned int len);
void displayStack(Stack s);
Direction peek(Stack s);
bool push(Stack s, Direction d);
Direction pop(Stack s);

// Functions

void initializeStack(Stack s, Direction* arr, unsigned int len) {
	s.top_of_stack = 0;
	s.len = len;
	s.arr = arr;

	for(unsigned int i = 0; i < s.len; ++i) {
		s.arr[i] = NONE_DIRECTION;
	}
}

void displayStack(Stack s) {
	for(unsigned int i = 0; i < s.len; ++i) {
		string str = "";
		directionToString(s.arr[i], &str);
		displayCenteredTextLine(i, str);
	}
}

Direction peek(Stack s) {
	if(s.top_of_stack == 0) {
		return NONE_DIRECTION;
	}
	return s.arr[s.top_of_stack-1];
}

bool push(Stack s, Direction d) {
	if(s.top_of_stack == s.len) {
		return false;
	}
	s.arr[s.top_of_stack] = d;
	++s.top_of_stack;
	return true;
}

Direction pop(Stack s) {
	if(s.top_of_stack == 0) {
		return NONE_DIRECTION;
	}
	Direction retval = s.arr[s.top_of_stack-1];
	s.arr[s.top_of_stack-1] = NONE_DIRECTION;
	--s.top_of_stack;
	return retval;
}
