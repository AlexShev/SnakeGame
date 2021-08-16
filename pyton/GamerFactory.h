#pragma once
#include "IGamer.h"
#include "Gamer.h"
#include "StupedArtificialGamer.h"
#include "AverageArtificialGamer.h"

class GamerFactory
{
public:
	IGamer* CreateIGamer(GamerType type, Field& field, Snake& snake, LevelDifficulty level)
	{
		if (type == human)
		{
			return new Gamer();
		}
		else if (type == artificial)
		{
			if (level == LevelDifficulty::easy)
			{
				return new StupedArtificialGamer(field, snake);
			}
			else if (level == LevelDifficulty::middle)
			{
				return new AverageArtificialGamer(field, snake);
			}
		}

		return nullptr;
	}
};