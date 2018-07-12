// Prototypes

enum DirectionMoved;
typedef struct Stack;

// Structs

enum DirectionMoved {
	None,
	Forward,
	Left,
	Right
};

typedef struct {
	unsigned int top_of_stack;
	unsigned int len;
	DirectionMoved* arr;
} Stack;

// Functions

void directionMovedToString(DirectionMoved d, string& s) {
	switch(d) {
		case None:
			s = "None";
			break;
		case Left:
			s = "Left";
			break;
		case Right:
			s = "Right";
			break;
		case Forward:
			s = "Forward";
			break;
		default:
			s = "NULL";
	}
}

void initializeStack(Stack s, DirectionMoved* arr, unsigned int len) {
	s.top_of_stack = 0;
	s.len = len;
	s.arr = arr;

	for(unsigned int i = 0; i < s.len; ++i) {
		s.arr[i] = None;
	}
}

void displayStack(Stack s) {
	for(unsigned int i = 0; i < s.len; ++i) {
		string str = "";
		directionMovedToString(s.arr[i], str);
		displayCenteredTextLine(i, str);
	}
}

bool push(Stack s, DirectionMoved d) {
	if(s.top_of_stack == s.len) {
		return false;
	}
	s.arr[s.top_of_stack] = d;
	++s.top_of_stack;
	return true;
}

DirectionMoved pop(Stack s) {
	if(s.top_of_stack == 0) {
		return None;
	}
	DirectionMoved retval = s.arr[s.top_of_stack-1];
	s.arr[s.top_of_stack-1] = None;
	--s.top_of_stack;
	return retval;
}
