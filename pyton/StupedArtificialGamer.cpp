#include "StupedArtificialGamer.h"

StupedArtificialGamer::StupedArtificialGamer(Field& field, Snake& snake) : _field(field), _snake(snake), _startPosition(1, _field.height - 1)
{
    int x = _snake.GetHead().x;

    if (x > _startPosition.x)
    {
        for (int i = x - 1; i > _startPosition.x; i--)
        {
            _comands.push(Direction::left);
        }
    }
    else
    {
        for (size_t i = x; i < _startPosition.x; i++)
        {
            _comands.push(Direction::right);
        }
    }

    int y = _snake.GetHead().y;

    if (y > _startPosition.y)
    {
        for (int i = y - 1; i >= _startPosition.y; i--)
        {
            _comands.push(Direction::up);
        }
    }
    else
    {
        for (size_t i = y + 1; i < _startPosition.y; i++)
        {
            _comands.push(Direction::down);
        }
    }
}

Direction StupedArtificialGamer::Comand()
{
    if (_comands.empty())
    {
        if (_snake.GetHead() == Point(2, 1))
        {
            _comands.push(Direction::left);

            for (size_t i = 1; i < _startPosition.y - 1; i++)
            {
                _comands.push(Direction::down);
            }

            _comands.push(Direction::right);
        }
        else
        {
            for (size_t i = _startPosition.x; i < _field.width - 3; ++i)
            {
                _comands.push(Direction::right);
            }

            if (_snake.GetHead().y != 1)
            {
                _comands.push(Direction::up);
            }

            for (int i = _field.width - 3; i > _startPosition.x; --i)
            {
                _comands.push(Direction::left);
            }

            if (_snake.GetHead().y != 2)
            {
                _comands.push(Direction::up);
            }
        }
    }

    return GetComand();
}

Direction StupedArtificialGamer::GetComand()
{
    Direction answer = _comands.front();
    _comands.pop();
    return answer;
}