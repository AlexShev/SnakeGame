#pragma once
#include <list>
#include <queue>
#include "Enums.h"
#include "Point.h"
#include "Reduction.h"
#include "Field.h"

class Snake
{
public:
    Snake(int x, int y, Direction dir);
    void SetMaxHangryLevel(LevelDifficulty _level, size_t height, size_t widht);

    Condition Move(Field& field, Direction dir);

    std::queue<Reduction>& GetReductions() { return _reductions; }
    int GetLenght() const { return _tail.size(); }
    int GetTimeToDeleteTail() const { return _maxHangryLevel - _hangryLevel; }
    Point GetHead() const { return _head; }
    const std::list<Point>& GetTail() const { return _tail; }

private:
    void AddTail(Point newHead);
    void MoveTail(Point newHead);
    bool DeleteTail();

    Direction _dir;
    Point _head;
    std::list<Point> _tail;
    std::queue<Reduction> _reductions;
    int _hangryLevel;
    int _maxHangryLevel;
};