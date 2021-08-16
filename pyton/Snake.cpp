#include "Snake.h"

Snake::Snake(int x, int y, Direction dir)
{
    _dir = dir;
    _hangryLevel = 0;
    _maxHangryLevel = 1;

    _head.x = x;
    _head.y = y;

    _reductions.push(Reduction(_head, PointType::snakeHead));
}

void Snake::AddTail(Point newHead)
{
    _reductions.push(Reduction(newHead, PointType::snakeHead));
    _reductions.push(Reduction(_head, PointType::snakeBody));

    _tail.push_front(_head);
    _head = newHead;
}

void Snake::SetMaxHangryLevel(LevelDifficulty level, size_t height, size_t widht)
{
    if (level == LevelDifficulty::easy)
    {
        _maxHangryLevel = height * widht;
    }
    else
    {
        _maxHangryLevel = (height + widht) / level;
    }
}

void Snake::MoveTail(Point newHead)
{
    _reductions.push(Reduction(newHead, PointType::snakeHead));
    _reductions.push(Reduction(_head, PointType::snakeBody));

    _tail.push_front(_head);
    _head = newHead;

    _reductions.push(Reduction(_tail.back(), PointType::emptiness));
    _tail.pop_back();
}

bool Snake::DeleteTail()
{
    if (_tail.empty())
    {
        return false;
    }
    else
    {
        _reductions.push(Reduction(_tail.back(), PointType::emptiness));
        _tail.pop_back();
        return true;
    }
}

Condition Snake::Move(Field& field, Direction dir)
{
    Condition res = live;

    if (dir != Direction::nothing)
    {
        _dir = dir;
    }

    Point newHead = _head;

    switch (_dir)
    {
    case left:
        --newHead.x;
        break;
    case right:
        ++newHead.x;
        break;
    case up:
        --newHead.y;
        break;
    case down:
        ++newHead.y;
        break;
    }

    if ((newHead.x < 1) || (newHead.x >= field.GetWidth() - 1) || (newHead.y < 1) || (newHead.y >= field.GetHeight() - 1))
    {
        _hangryLevel = 0;
        res = Condition::dedth;
    }
    else if (field(newHead.y, newHead.x) == food)
    {
        AddTail(newHead);
        _hangryLevel = 0;
        field.DisappearFood(newHead);
    }
    else if (field(newHead.y, newHead.x) == PointType::emptiness)
    {
        MoveTail(newHead);
        ++_hangryLevel;
    }
    else
    {
        res = Condition::dedth;
    }

    if (_hangryLevel > _maxHangryLevel)
    {
        _hangryLevel = 0;
        res = DeleteTail() ? Condition::live : Condition::dedth;
    }

    field.ChengeField(_reductions);

    return res;
}
