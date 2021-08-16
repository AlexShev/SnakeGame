#pragma once

enum MessageType 
{
	gameOver,
	error,
	inputError,
	tryAgain,
};

static const char* MESSAGES[4]
{
	" Game Over!!! ",
	"Somthing is not good, restart the game, please",
	"The entered parameters are not correct",
	"Woulde you like try again?",
};

enum GamerType
{
	human = 1,
	artificial = 2,
	gamerTypeError = -1
};

enum LevelDifficulty
{
	easy = 1,
	middle = 2,
	hard = 3,
	levelDifficultyError = -1
};

enum PointType
{
	emptiness = -1,
	border = -2,
	snakeBody = -3,
	snakeHead = -4,
	food = -5
};

enum Direction
{
	left,
	right,
	up,
	down,
	nothing
};	

enum Condition
{
	live,
	dedth
};