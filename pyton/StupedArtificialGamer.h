#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"

class StupedArtificialGamer : public IGamer
{
public:
    StupedArtificialGamer(Field& field, Snake& snake);
    Direction Comand() override;

private:
    Direction GetComand();

    std::queue<Direction> _comands;
    Field& _field;
    Snake& _snake;
    Point _startPosition;
};
