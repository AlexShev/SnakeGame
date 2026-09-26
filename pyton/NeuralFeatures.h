#pragma once
#include "Field.h"
#include "Snake.h"
#include <array>
#include <queue>
#include <vector>
#include <algorithm>
#include <cmath>

// Shared representation for training and inference; order: L, R, U, D.
inline std::array<float, 21> NeuralFeatures(const Field& field, const Snake& snake)
{
    const int w = static_cast<int>(field.GetWidth()), h = static_cast<int>(field.GetHeight());
    const Point head = snake.GetHead();
    const Point offsets[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    std::array<float, 21> result{};
    const auto& cells = field.GetField();
    std::vector<Point> foods;
    for (int y = 1; y < h - 1; ++y)
        for (int x = 1; x < w - 1; ++x)
            if (cells[y][x] == food) foods.emplace_back(x, y);
    for (int d = 0; d < 4; ++d)
    {
        const Point next(head.x + offsets[d].x, head.y + offsets[d].y);
        const bool inside = next.x > 0 && next.y > 0 && next.x < w - 1 && next.y < h - 1;
        const bool tailLeaves = !snake.GetTail().empty() && next == snake.GetTail().back()
            && inside && cells[next.y][next.x] != food;
        const bool legal = inside && (cells[next.y][next.x] == emptiness
            || cells[next.y][next.x] == food || tailLeaves);
        result[4*d] = legal ? 1.f : 0.f;
        int nearest = w + h;
        for (const Point& p : foods)
            nearest = std::min(nearest, std::abs(next.x - p.x) + std::abs(next.y - p.y));
        result[4*d + 1] = 1.f - static_cast<float>(nearest) / (w + h);
        if (!legal) continue;
        std::vector<bool> blocked(w*h, false), seen(w*h, false);
        for (const Point& p : snake.GetTail()) blocked[p.y*w+p.x] = true;
        blocked[head.y*w+head.x] = true;
        if (tailLeaves || (!snake.GetTail().empty() && cells[next.y][next.x] != food))
        {
            const Point tail = snake.GetTail().back();
            blocked[tail.y*w+tail.x] = false;
        }
        std::queue<Point> q;
        q.push(next);
        seen[next.y*w+next.x] = true;
        int area = 0;
        while (!q.empty())
        {
            const Point p = q.front(); q.pop(); ++area;
            for (const Point& delta : offsets)
            {
                const Point n(p.x+delta.x, p.y+delta.y);
                if (n.x < 1 || n.y < 1 || n.x >= w-1 || n.y >= h-1) continue;
                const int index = n.y*w+n.x;
                if (!seen[index] && !blocked[index])
                { seen[index] = true; q.push(n); }
            }
        }
        result[4*d + 2] = static_cast<float>(area) / ((w-2)*(h-2));
        result[4*d + 3] = static_cast<float>(
            d < 2 ? (d == 0 ? next.x : w-1-next.x)
                  : (d == 2 ? next.y : h-1-next.y)) / std::max(w, h);
    }
    const Direction dirs[4] = {left, right, up, down};
    for (int i = 0; i < 4; ++i) result[16+i] = snake.GetDirection() == dirs[i] ? 1.f : 0.f;
    result[20] = static_cast<float>(snake.GetTimeToDeleteTail()) / (w+h);
    return result;
}
