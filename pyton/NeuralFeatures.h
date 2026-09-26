#pragma once
#include "Field.h"
#include "Snake.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <queue>
#include <vector>

constexpr int NeuralInputSize = 103;
constexpr int NeuralHiddenSize = 64;

// Shared, deterministic representation used by both C++ training and play.
// First 21 inputs retain the old ordering: 4 candidate directions x 4,
// direction one-hot, and time to tail loss.
inline std::array<float, NeuralInputSize> NeuralFeatures(const Field& field, const Snake& snake)
{
    const int w = static_cast<int>(field.GetWidth()), h = static_cast<int>(field.GetHeight());
    const Point head = snake.GetHead();
    const Point offsets[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    const auto& cells = field.GetField();
    std::array<float, NeuralInputSize> result{};
    std::vector<Point> foods;
    for (int y = 1; y < h-1; ++y)
        for (int x = 1; x < w-1; ++x)
            if (cells[y][x] == food) foods.emplace_back(x, y);

    for (int d = 0; d < 4; ++d)
    {
        const Point next(head.x+offsets[d].x, head.y+offsets[d].y);
        const bool inside = next.x > 0 && next.y > 0 && next.x < w-1 && next.y < h-1;
        const bool tailLeaves = inside && !snake.GetTail().empty()
            && next == snake.GetTail().back() && cells[next.y][next.x] != food;
        const bool legal = inside && (cells[next.y][next.x] == emptiness
            || cells[next.y][next.x] == food || tailLeaves);
        result[4*d] = legal ? 1.f : 0.f;
        int nearest = w+h;
        for (const Point& p : foods)
            nearest = std::min(nearest, std::abs(next.x-p.x)+std::abs(next.y-p.y));
        result[4*d+1] = 1.f - static_cast<float>(nearest)/(w+h);
        if (!legal) continue;

        std::vector<bool> blocked(w*h, false), seen(w*h, false);
        for (const Point& p : snake.GetTail()) blocked[p.y*w+p.x] = true;
        blocked[head.y*w+head.x] = true;
        if (!snake.GetTail().empty() && cells[next.y][next.x] != food)
        {
            const Point tail = snake.GetTail().back();
            blocked[tail.y*w+tail.x] = false;
        }
        std::queue<std::pair<Point,int>> q;
        q.push({next,0});
        seen[next.y*w+next.x] = true;
        int area = 0, foodDistance = w+h;
        while (!q.empty())
        {
            const Point p = q.front().first;
            const int distance = q.front().second;
            q.pop(); ++area;
            if (cells[p.y][p.x] == food)
                foodDistance = std::min(foodDistance, distance);
            for (const Point& delta : offsets)
            {
                const Point n(p.x+delta.x,p.y+delta.y);
                if (n.x < 1 || n.y < 1 || n.x >= w-1 || n.y >= h-1) continue;
                const int index = n.y*w+n.x;
                if (!seen[index] && !blocked[index])
                { seen[index] = true; q.push({n,distance+1}); }
            }
        }
        result[4*d+2] = static_cast<float>(area)/((w-2)*(h-2));
        result[4*d+3] = static_cast<float>(d < 2 ?
            (d == 0 ? next.x : w-1-next.x) : (d == 2 ? next.y : h-1-next.y))
            / std::max(w,h);
        result[21+d] = 1.f - static_cast<float>(foodDistance)/(w+h);
    }
    const Direction dirs[4] = {left,right,up,down};
    for (int i=0;i<4;++i) result[16+i] = snake.GetDirection()==dirs[i] ? 1.f : 0.f;
    result[20] = static_cast<float>(snake.GetTimeToDeleteTail())/(w+h);

    // Body geometry around the head: occupancy, food, and wall per cell.
    for (int dy=-2;dy<=2;++dy)
        for (int dx=-2;dx<=2;++dx)
        {
            const int x=head.x+dx,y=head.y+dy;
            const int base=25+((dy+2)*5+(dx+2))*3;
            if (x<1 || y<1 || x>=w-1 || y>=h-1) result[base+2]=1.f;
            else
            {
                result[base] = (cells[y][x]==snakeBody || cells[y][x]==snakeHead) ? 1.f : 0.f;
                result[base+1] = cells[y][x]==food ? 1.f : 0.f;
            }
        }
    if (!foods.empty())
    {
        const auto nearest = *std::min_element(foods.begin(),foods.end(),
            [&](Point a, Point b) {
                return std::abs(a.x-head.x)+std::abs(a.y-head.y)
                    < std::abs(b.x-head.x)+std::abs(b.y-head.y);
            });
        result[100]=static_cast<float>(nearest.x-head.x)/w;
        result[101]=static_cast<float>(nearest.y-head.y)/h;
    }
    result[102]=static_cast<float>(snake.GetLenght())/((w-2)*(h-2));
    return result;
}
