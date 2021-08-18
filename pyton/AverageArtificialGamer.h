#pragma once
#include "IGamer.h"
#include "Field.h"
#include "Snake.h"

class AverageArtificialGamer : public IGamer
{
public:
    AverageArtificialGamer(Field& field, Snake& snake) : _field(field), _snake(snake) { }
    Direction Command() override;

private:
    Direction GetComand();
    static inline bool IsFood(const std::vector<std::vector<int>>& myMap, const Point& point);
    bool AddPointsToNextWave(std::vector<std::vector<int>>& myMap, std::queue<Point>& nextWave, Point currCenter, int d,
                             Point& food, bool& isFreeSpaseNeibor);

    std::list<Direction> _comands;
    Field& _field;
    Snake& _snake;

    const int dx[4] = { 1, 0, -1, 0 };   // смещени€, соответствующие сосед€м €чейки
    const int dy[4] = { 0, 1, 0, -1 };   // справа, снизу, слева и сверху
};
