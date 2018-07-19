#pragma once

#include <map-essentials.h>

// Prototypes

typedef struct Stack;

// Structs

typedef struct {
	unsigned int top_of_stack;
	unsigned int len;
	Direction* arr;
} Stack;

// Functions

void initializeStack(Stack s, Direction* arr, unsigned int len) {
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
		directionToString(s.arr[i], str);
		displayCenteredTextLine(i, str);
	}
}

Direction peek(Stack s) {
	if(s.top_of_stack == 0) {
		return NONE;
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
		return NONE;
	}
	Direction retval = s.arr[s.top_of_stack-1];
	s.arr[s.top_of_stack-1] = NONE;
	--s.top_of_stack;
	return retval;
}
