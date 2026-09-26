#pragma once
#include "IGamer.h"
#include "Gamer.h"
#include "StupedArtificialGamer.h"
#include "AverageArtificialGamer.h"
#include "HardArtificialGamer.h"
#include "NeuralArtificialGamer.h"
#include <memory>

class GamerFactory
{
public:
	std::unique_ptr<IGamer> CreateIGamer(GamerType type, Field& field, Snake& snake, LevelDifficulty level)
	{
		if (type == neural)
		{
			return std::make_unique<NeuralArtificialGamer>(field, snake);
		}
		if (type == human)
		{
			return std::make_unique<Gamer>();
		}
		else if (type == artificial)
		{
			if (level == LevelDifficulty::easy)
			{
				return std::make_unique<StupedArtificialGamer>(field, snake);
			}
			else if (level == LevelDifficulty::middle)
			{
				return std::make_unique<AverageArtificialGamer>(field, snake);
			}
			else if (level == LevelDifficulty::hard)
			{
				return std::make_unique<HardArtificialGamer>(field, snake);
			}
		}

		return nullptr;
	}
};
