#pragma config(Sensor, S1,     TouchSensor1,   sensorEV3_Touch)
#pragma config(Sensor, S2,     TouchSensor2,   sensorEV3_Touch)

bool paddleBounce(int ballXPos, int paddleLeft, int paddleRight) {
	return paddleLeft <= ballXPos && ballXPos <= paddleRight;
}

void menuScreen() {
	eraseDisplay();
	displayCenteredBigTextLine(5, "PONG");
	displayCenteredTextLine(9, "Press any button to begin");
	while(!getButtonPress(buttonAny)) {sleep(50);}
}

task main()
{
	int score = 0;
	int level = 1;
	int lives = 3;
	const int livesReset = 3;

	const int xMax = 175;
	const int yMax = 127;

	float ballYPosInitial = 10;
	float ballXPos = random(xMax);
	float ballYPos = ballYPosInitial;

	const float speedInitial = 0.5;
	float xSpeed = speedInitial;
	float ySpeed = speedInitial;
	float speedIncrease = 0.2;

	const int ballDiameter = 6;

	// Paddle
	const int paddleTop = 4;
	const int paddleBottom = 2;

	int paddleSize = 50;
	int paddleLeft = xMax/2 - paddleSize/2;
	int paddleRight = paddleLeft + paddleSize;

	bool paddle2Active = false;
	int paddle2Left = 0;
	int paddle2Right = 0;

	const int paddleSpeed = 1;

	menuScreen();

	while(true) {

		sleep(10);
		eraseDisplay();
		displayTextLine(1, "Level: %d", level);
		displayTextLine(2, "Score: %d", score);
		drawCircle(ballXPos, ballYPos, ballDiameter);
		drawRect(paddleLeft, paddleTop, paddleRight, paddleBottom);

		if(getButtonPress(buttonLeft)) {
			paddleLeft -= paddleSpeed;
			paddleRight -= paddleSpeed;
		}
		if(getButtonPress(buttonRight)) {
			paddleLeft += paddleSpeed;
			paddleRight += paddleSpeed;
		}

		paddle2Active = false;
		if(paddleLeft < 0) {
			if(paddleRight < 0) {
				paddleLeft += xMax;
				paddleRight += xMax;
			}
			else {
				paddle2Active = true;
				paddle2Left = paddleLeft + xMax;
				paddle2Right = paddleRight + xMax;
				drawRect(paddle2Left, paddleTop, paddle2Right, paddleBottom);
			}
		}
		else if(paddleRight > xMax) {
			if(paddleLeft > xMax) {
				paddleLeft -= xMax;
				paddleRight -= xMax;
			}
			else {
				paddle2Active = true;
				paddle2Left = paddleLeft - xMax;
				paddle2Right = paddleRight - xMax;
				drawRect(paddle2Left, paddleTop, paddle2Right, paddleBottom);
			}
		}

		ballXPos += xSpeed;
		ballYPos += ySpeed;

		// Bounce ball
		if(ballXPos <= 0 || ballXPos >= xMax) {
			xSpeed = -xSpeed;
		}
		if(ballYPos >= yMax) {
			ySpeed = -ySpeed;
		}

		if(ballYPos-(ballDiameter/2) <= paddleTop && ySpeed < 0) {

			if(paddleBounce(ballXPos, paddleLeft, paddleRight)) {
				ySpeed = -ySpeed;
				++score;
				if(score % 3 == 0) {
					++level;
					xSpeed += xSpeed > 0 ? speedIncrease : -speedIncrease;
					ySpeed += speedIncrease;
				}
			}
			else if(paddle2Active && paddleBounce(ballXPos, paddle2Left, paddle2Right)) {
				ySpeed = -ySpeed;
				++score;
				if(score % 3 == 0) {
					++level;
					xSpeed += xSpeed > 0 ? speedIncrease : -speedIncrease;
					ySpeed += speedIncrease;
				}
			}
			else {
				ballXPos = random(xMax);
				ballYPos = ballYPosInitial;
				ySpeed = -ySpeed;
				--lives;

				eraseDisplay();
				displayCenteredBigTextLine(6, "Lives: %d", lives+1);
				sleep(1500);
				eraseDisplay();
				displayCenteredBigTextLine(6, "Lives: %d", lives);
				sleep(1500);

				if(lives == 0) {
					eraseDisplay();
					displayCenteredBigTextLine(5, "GAME OVER");
					displayCenteredTextLine(9, "Returning you to");
					displayCenteredTextLine(10, "the home screen...");
					sleep(3000);
					xSpeed = speedInitial;
					ySpeed = speedInitial;
					score = 0;
					level = 1;
					lives = livesReset;
					menuScreen();
				}
			}
		}

	}


}
