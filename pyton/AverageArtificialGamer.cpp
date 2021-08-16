#include "AverageArtificialGamer.h"

Direction AverageArtificialGamer::Comand()

{
    if (_comands.empty())
    {
        std::list<Point> tempTail = _snake.GetTail();

        std::vector<std::vector<int>> myMap(_field.height, std::vector<int>(_field.width));

        for (size_t i = 0; i < _field.height; i++)
        {
            for (size_t j = 0; j < _field.width; j++)
            {
                myMap[i][j] = int(_field.field[i][j]);
            }
        }

        Point food, currCenter;

        std::queue<Point> wave, nextWave;
        nextWave.push(_snake.GetHead());

        int d = 0;

        myMap[_snake.GetHead().y][_snake.GetHead().x] = 0;

        bool isFoodFinded = false;
        bool isFreeSpaseNeibor = true;
        auto tailNode = tempTail.rbegin();

        while (!isFoodFinded && isFreeSpaseNeibor)
        {
            wave = std::move(nextWave);
            isFreeSpaseNeibor = false;

            while (!wave.empty() && !isFoodFinded)
            {
                currCenter = wave.front();
                wave.pop();

                isFoodFinded = AddPointsToNextWave(myMap, nextWave, currCenter, d, food, isFreeSpaseNeibor);
            }

            ++d;

            if (tailNode != tempTail.rend())
            {
                myMap[tailNode->y][tailNode->x] = PointType::emptiness;
                ++tailNode;
            }
        }

        int len = myMap[food.y][food.x];            // длина кратчайшего пути из (ax, ay) в (bx, by)
        int x = food.x;
        int y = food.y;

        d = len;

        while (d > 0)
        {
            d--;

            if (y - 1 >= 0 && y - 1 < myMap.size() && x >= 0 && x < myMap[0].size() && myMap[y - 1][x] == d)
            {
                _comands.push_front(Direction::down);
                --y;
            }
            else if (y + 1 >= 0 && y + 1 < myMap.size() && x >= 0 && x < myMap[0].size() && myMap[y + 1][x] == d)
            {
                _comands.push_front(Direction::up);
                ++y;
            }
            else if (y >= 0 && y < myMap.size() && x - 1 >= 0 && x - 1 < myMap[0].size() && myMap[y][x - 1] == d)
            {
                _comands.push_front(Direction::right);
                --x;
            }
            else if (y >= 0 && y < myMap.size() && x + 1 >= 0 && x + 1 < myMap[0].size() && myMap[y][x + 1] == d)
            {
                _comands.push_front(Direction::left);
                ++x;
            }
        }
    }

    return GetComand();
}

Direction AverageArtificialGamer::GetComand()
{
    if (_comands.empty())
    {
        return Direction::nothing;
    }
    else
    {
        Direction answer = _comands.front();
        _comands.pop_front();
        return answer;
    }
}

inline bool AverageArtificialGamer::IsFood(const std::vector<std::vector<int>>& myMap, const Point& point)
{
    return myMap[point.y][point.x] == PointType::food;
}

bool AverageArtificialGamer::AddPointsToNextWave(std::vector<std::vector<int>>& myMap, std::queue<Point>& nextWave, Point currCenter, int d, Point& food, bool& isFreeSpaseNeibor)
{
    for (int k = 0; k < 4; ++k)                    // проходим по всем непомеченным соседям
    {
        int iy = currCenter.y + dy[k], ix = currCenter.x + dx[k];

        if (iy >= 0 && iy < myMap.size() && ix >= 0 && ix < myMap[0].size())
        {
            if (myMap[iy][ix] == PointType::emptiness)
            {
                isFreeSpaseNeibor = true;
                myMap[iy][ix] = d + 1;      // распространяем волну
                nextWave.emplace(ix, iy);
            }
            else if (myMap[iy][ix] == PointType::food)
            {
                myMap[iy][ix] = d + 1;      // распространяем волну
                food.y = iy;
                food.x = ix;
                return true;
            }
        }
    }

    return false;
}