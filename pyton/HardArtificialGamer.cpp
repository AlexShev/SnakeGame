#include "HardArtificialGamer.h"
#include <algorithm>
#include <array>
#include <limits>
#include <queue>
#include <vector>

namespace
{
    struct Step
    {
        Direction direction;
        int dx;
        int dy;
    };

    const std::array<Step, 4> steps{ {
        { right, 1, 0 }, { down, 0, 1 }, { left, -1, 0 }, { up, 0, -1 }
    } };

    bool IsReverse(Direction current, Direction requested)
    {
        return (current == left && requested == right) || (current == right && requested == left)
            || (current == up && requested == down) || (current == down && requested == up);
    }

    bool Inside(const Field& field, Point point)
    {
        return point.x > 0 && point.y > 0
            && point.x < static_cast<int>(field.GetWidth()) - 1
            && point.y < static_cast<int>(field.GetHeight()) - 1;
    }

    struct Evaluation
    {
        int area = 0;
        int foodDistance = std::numeric_limits<int>::max();
    };

    Evaluation Evaluate(const Field& field, const Snake& snake, Point next, bool eatsFood)
    {
        const auto& cells = field.GetField();
        const size_t height = field.GetHeight();
        const size_t width = field.GetWidth();
        std::vector<std::vector<bool>> blocked(height, std::vector<bool>(width, false));
        std::vector<std::vector<bool>> seen(height, std::vector<bool>(width, false));

        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                blocked[y][x] = cells[y][x] == snakeBody || cells[y][x] == snakeHead;
            }
        }

        if (!eatsFood && !snake.GetTail().empty())
        {
            const Point tail = snake.GetTail().back();
            blocked[tail.y][tail.x] = false;
        }

        // The old head becomes body unless the snake had no tail and did not eat.
        const Point head = snake.GetHead();
        blocked[head.y][head.x] = eatsFood || !snake.GetTail().empty();
        blocked[next.y][next.x] = false;

        std::queue<std::pair<Point, int>> wave;
        wave.push({ next, 0 });
        seen[next.y][next.x] = true;

        Evaluation result;
        if (eatsFood)
        {
            result.foodDistance = 0;
        }

        while (!wave.empty())
        {
            const Point point = wave.front().first;
            const int distance = wave.front().second;
            wave.pop();
            ++result.area;

            if (cells[point.y][point.x] == food && !(point == next && eatsFood))
            {
                result.foodDistance = std::min(result.foodDistance, distance);
            }

            for (const Step& step : steps)
            {
                const Point neighbor(point.x + step.dx, point.y + step.dy);
                if (Inside(field, neighbor) && !blocked[neighbor.y][neighbor.x]
                    && !seen[neighbor.y][neighbor.x])
                {
                    seen[neighbor.y][neighbor.x] = true;
                    wave.push({ neighbor, distance + 1 });
                }
            }
        }

        return result;
    }
}

Direction HardArtificialGamer::Command()
{
    const Point head = _snake.GetHead();
    const int snakeLength = _snake.GetLenght() + 1;
    const int capacity = static_cast<int>((_field.GetWidth() - 2) * (_field.GetHeight() - 2));

    Direction best = nothing;
    int bestArea = -1;
    int bestDistance = std::numeric_limits<int>::max();
    bool bestSafe = false;
    bool bestHasFood = false;

    for (const Step& step : steps)
    {
        if (!_snake.GetTail().empty() && IsReverse(_snake.GetDirection(), step.direction))
        {
            continue;
        }

        const Point next(head.x + step.dx, head.y + step.dy);
        if (!Inside(_field, next))
        {
            continue;
        }

        const PointType cell = _field(next.y, next.x);
        const bool enteringTail = !_snake.GetTail().empty() && next == _snake.GetTail().back();
        if (cell != emptiness && cell != food && !enteringTail)
        {
            continue;
        }

        const bool eatsFood = cell == food;
        const Evaluation evaluation = Evaluate(_field, _snake, next, eatsFood);
        const int newLength = snakeLength + (eatsFood ? 1 : 0);
        const int requiredArea = std::min(newLength + 1, capacity - newLength + 1);
        const bool safe = evaluation.area >= requiredArea;
        const bool hasFood = evaluation.foodDistance != std::numeric_limits<int>::max();

        // Favor a reachable meal with escape space, then the largest open area.
        if (best == nothing || (safe && hasFood && !(bestSafe && bestHasFood))
            || (safe && hasFood && bestSafe && bestHasFood
                && evaluation.foodDistance < bestDistance)
            || (!(bestSafe && bestHasFood) && evaluation.area > bestArea))
        {
            best = step.direction;
            bestArea = evaluation.area;
            bestDistance = evaluation.foodDistance;
            bestSafe = safe;
            bestHasFood = hasFood;
        }
    }

    return best;
}
