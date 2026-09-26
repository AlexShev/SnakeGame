#include "HardArtificialGamer.h"
#include <algorithm>
#include <array>
#include <deque>
#include <limits>
#include <queue>
#include <vector>

namespace
{
    struct Step { Direction direction; int dx; int dy; };
    const std::array<Step, 4> steps{{ {right, 1, 0}, {down, 0, 1},
        {left, -1, 0}, {up, 0, -1} }};

    bool Reverse(Direction a, Direction b)
    {
        return (a == left && b == right) || (a == right && b == left)
            || (a == up && b == down) || (a == down && b == up);
    }

    bool Inside(Point p, int w, int h)
    {
        return p.x > 0 && p.y > 0 && p.x < w - 1 && p.y < h - 1;
    }

    // Search accessible space after a proposed sequence of moves. A path to
    // the tail is a useful escape route when the snake has grown long.
    int Space(Point head, const std::deque<Point>& body, int w, int h, bool& tailReachable)
    {
        std::vector<bool> blocked(w * h, false), seen(w * h, false);
        const auto id = [w](Point p) { return p.y * w + p.x; };
        for (const Point& p : body) blocked[id(p)] = true;
        if (!body.empty()) blocked[id(body.back())] = false;
        std::queue<Point> pending;
        pending.push(head);
        seen[id(head)] = true;
        int area = 0;
        tailReachable = body.empty();
        while (!pending.empty())
        {
            const Point p = pending.front();
            pending.pop();
            ++area;
            if (!body.empty() && p == body.back()) tailReachable = true;
            for (const Step& s : steps)
            {
                const Point n(p.x + s.dx, p.y + s.dy);
                if (Inside(n, w, h) && !blocked[id(n)] && !seen[id(n)])
                {
                    seen[id(n)] = true;
                    pending.push(n);
                }
            }
        }
        return area;
    }
}

Direction HardArtificialGamer::Command()
{
    const int w = static_cast<int>(_field.GetWidth());
    const int h = static_cast<int>(_field.GetHeight());
    const Point head = _snake.GetHead();
    const auto id = [w](Point p) { return p.y * w + p.x; };
    std::deque<Point> initial(_snake.GetTail().begin(), _snake.GetTail().end());
    const auto& cells = _field.GetField();

    struct Node
    {
        Point head;
        std::deque<Point> body;
        Direction first;
        Direction last;
        int depth;
    };
    // Bounded lookahead simulates the actual body, including growth on food.
    // It is rebuilt every tick, accommodating moving food.
    std::queue<Node> pending;
    pending.push({head, initial, nothing, _snake.GetDirection(), 0});
    const int horizon = w * h;
    const int maxNodes = w * h;
    std::vector<bool> visited(w * h, false);
    visited[id(head)] = true;
    int explored = 0;
    Direction bestFood = nothing;
    int bestFoodDepth = std::numeric_limits<int>::max();
    Direction fallback = nothing;
    int fallbackArea = -1;

    while (!pending.empty() && explored++ < maxNodes)
    {
        Node state = pending.front();
        pending.pop();
        if (state.depth >= horizon || state.depth >= bestFoodDepth) continue;
        for (const Step& s : steps)
        {
            if (!state.body.empty() && Reverse(state.last, s.direction)) continue;
            const Point next(state.head.x + s.dx, state.head.y + s.dy);
            if (!Inside(next, w, h)) continue;
            const bool meal = cells[next.y][next.x] == food;
            const bool tailLeaves = !meal && !state.body.empty();
            bool collision = false;
            for (size_t i = 0; i < state.body.size(); ++i)
                if (next == state.body[i] && !(tailLeaves && i + 1 == state.body.size()))
                    collision = true;
            if (collision) continue;

            Node candidate = state;
            candidate.body.push_front(state.head);
            if (!meal) candidate.body.pop_back();
            candidate.head = next;
            candidate.last = s.direction;
            candidate.first = state.depth == 0 ? s.direction : state.first;
            candidate.depth = state.depth + 1;
            bool tailReachable = false;
            const int area = (state.depth == 0 || meal)
                ? Space(next, candidate.body, w, h, tailReachable) : 0;
            if (state.depth == 0 && area > fallbackArea)
            {
                fallbackArea = area;
                fallback = s.direction;
            }
            if (meal)
            {
                // Avoid taking food inside a pocket without a route to the tail.
                if (tailReachable || candidate.body.empty())
                {
                    bestFood = candidate.first;
                    bestFoodDepth = candidate.depth;
                }
                continue;
            }
            if (!visited[id(next)] && candidate.depth < bestFoodDepth)
            {
                visited[id(next)] = true;
                pending.push(std::move(candidate));
            }
        }
    }
    return bestFood != nothing ? bestFood : fallback;
}
