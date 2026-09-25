#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"

// Re-evaluates the board every turn: seek food without entering a small trap.
class HardArtificialGamer : public IGamer
{
public:
    HardArtificialGamer(Field& field, Snake& snake) : _field(field), _snake(snake) { }
    Direction Command() override;

private:
    Field& _field;
    Snake& _snake;
};
