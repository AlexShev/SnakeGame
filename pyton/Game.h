#pragma once
#include <windows.h>

#include "Snake.h"
#include "Field.h"
#include "IController.h"
#include "Gamer.h"

class Game
{
public:
	Game(IController& controler) : _controler(controler) { }
	void Start();

private:
	int CalculateTactTime(LevelDifficulty level, GamerType type);
	bool AskParam(LevelDifficulty& level, GamerType& type);

	IController& _controler;
};
