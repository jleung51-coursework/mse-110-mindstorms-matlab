with open('datalog-0.txt', 'r') as f:
    for line in f:
        c = chr(int(line))
        if c == 'F':
            print("Forward")
        elif c == 'R':
		    print("Right")
        elif c == 'L':
		    print("Left")
        else:
			print("Invalid character: " + c)