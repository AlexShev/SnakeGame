#pragma once
#include <windows.h>

#include "Snake.h"
#include "Field.h"
#include "IControler.h"
#include "Gamer.h"

class Game
{
public:
	Game(IControler& controler) : _controler(controler) { }
	void Start();

private:
	int CalculateTactTime(LevelDifficulty level, GamerType type);
	bool AskParam(LevelDifficulty& level, GamerType& type);

	IControler& _controler;
};
