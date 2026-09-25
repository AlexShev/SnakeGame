#include "AverageArtificialGamer.h"

Direction AverageArtificialGamer::Command()

{
    if (_comands.empty())
    {
        std::list<Point> tempTail = _snake.GetTail();

        std::vector<std::vector<int>> myMap(_field.GetHeight(), std::vector<int>(_field.GetWidth()));

        auto& field = _field.GetField();

        for (size_t i = 0; i < _field.GetHeight(); i++)
        {
            for (size_t j = 0; j < _field.GetWidth(); j++)
            {
                myMap[i][j] = static_cast<int>(field[i][j]);
            }
        }

        Point food;

        std::queue<Point> nextWave;
        nextWave.push(_snake.GetHead());

        int d = 0;

        myMap[_snake.GetHead().y][_snake.GetHead().x] = 0;

        bool isFoodFunded = false;
        bool isFreeSpaceNeighbor = true;
        auto tailNode = tempTail.rbegin();

        while (!isFoodFunded && isFreeSpaceNeighbor)
        {
            std::queue<Point> wave = std::move(nextWave);
            isFreeSpaceNeighbor = false;

            while (!wave.empty() && !isFoodFunded)
            {
                isFoodFunded = AddPointsToNextWave(myMap, nextWave, wave.front(), d, food, isFreeSpaceNeighbor);

                wave.pop();
            }

            ++d;

            if (tailNode != tempTail.rend())
            {
                myMap[tailNode->y][tailNode->x] = PointType::emptiness;
                ++tailNode;
            }
        }

        if (!isFoodFunded)
        {
            const Point head = _snake.GetHead();
            const Direction directions[4] = { right, down, left, up };
            const int dx[4] = { 1, 0, -1, 0 };
            const int dy[4] = { 0, 1, 0, -1 };

            for (int i = 0; i < 4; ++i)
            {
                const Point next(head.x + dx[i], head.y + dy[i]);
                const Direction current = _snake.GetDirection();
                const bool reverse = (current == left && directions[i] == right)
                    || (current == right && directions[i] == left)
                    || (current == up && directions[i] == down)
                    || (current == down && directions[i] == up);

                if (reverse && !_snake.GetTail().empty())
                {
                    continue;
                }
                if (next.x > 0 && next.y > 0
                    && next.x < static_cast<int>(_field.GetWidth()) - 1
                    && next.y < static_cast<int>(_field.GetHeight()) - 1)
                {
                    const PointType cell = _field(next.y, next.x);
                    if (cell == emptiness || cell == PointType::food
                        || (!_snake.GetTail().empty() && next == _snake.GetTail().back()))
                    {
                        return directions[i];
                    }
                }
            }

            return nothing;
        }

        const int len = myMap[food.y][food.x];            
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
	    const Direction answer = _comands.front();
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
    for (int k = 0; k < 4; ++k)                    // ïðîõîäèì ïî âñåì íåïîìå÷åííûì ñîñåäÿì
    {
        int iy = currCenter.y + dy[k], ix = currCenter.x + dx[k];

        if (iy >= 0 && iy < myMap.size() && ix >= 0 && ix < myMap[0].size())
        {
            if (myMap[iy][ix] == PointType::emptiness)
            {
                isFreeSpaseNeibor = true;
                myMap[iy][ix] = d + 1;      // ðàñïðîñòðàíÿåì âîëíó
                nextWave.emplace(ix, iy);
            }
            else if (myMap[iy][ix] == PointType::food)
            {
                myMap[iy][ix] = d + 1;      // ðàñïðîñòðàíÿåì âîëíó
                food.y = iy;
                food.x = ix;
                return true;
            }
        }
    }

    return false;
}
