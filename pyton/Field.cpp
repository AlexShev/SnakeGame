#include "Field.h"
#include <ctime>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <stdexcept>

Field::Field(const LevelDifficulty level, const size_t height, const size_t width)
: _level(level), _width(width), _height(height)
{
    if (width < 3 || height < 3)
    {
        throw std::invalid_argument("Field must be at least 3 by 3");
    }

    Init();

	std::random_device device;

	_randomGenerator.seed(device());
    _counterFood = 0;
	_maxFoodNumbers = static_cast<int>(std::max(1., std::sqrt(width + height) / level));
}

void Field::ChangeField(std::queue<Reduction>& reductions)
{
    while (!reductions.empty())
    {
        Reduction red = reductions.front();

        reductions.pop();

        this->_reductions.push(red);

        _field[red.point.y][red.point.x] = red.type;
    }
}

void Field::MoveFood()
{
    if (_counterFood > 0 && _level >= LevelDifficulty::hard && RandomInt(0, static_cast<int>(_height)) == 0)
    {
        DisappearFood();
        GenerateFood();
    }
}

int Field::RandomInt(const int min, const int max)
{
    std::uniform_int_distribution<int> range(min, max);

    return range(_randomGenerator);
}

void Field::DisappearFood(Point pointToDisappear)
{
    if (_foods[pointToDisappear.y].erase(pointToDisappear.x) == 0)
    {
        return;
    }

    --_counterFood;

    _field[pointToDisappear.y][pointToDisappear.x] = PointType::emptiness;
}

void Field::DisappearFood()
{
    Point pointToDisapper;
    
    do {
        pointToDisapper.y = RandomInt(1, _height - 1);
    } while (_foods[pointToDisapper.y].empty());

    pointToDisapper.x = *_foods[pointToDisapper.y].begin();

    _reductions.emplace(pointToDisapper, PointType::emptiness);

    DisappearFood(pointToDisapper);
}

void Field::GenerateFood()
{
    std::vector<Point> freeCells;
    for (size_t y = 1; y + 1 < _height; ++y)
    {
        for (size_t x = 1; x + 1 < _width; ++x)
        {
            if (_field[y][x] == PointType::emptiness)
            {
                freeCells.emplace_back(static_cast<int>(x), static_cast<int>(y));
            }
        }
    }

    while (_counterFood < _maxFoodNumbers && !freeCells.empty())
    {
        const int index = RandomInt(0, static_cast<int>(freeCells.size()) - 1);
        const Point food = freeCells[index];
        freeCells[index] = freeCells.back();
        freeCells.pop_back();

        _field[food.y][food.x] = PointType::food;
        _reductions.emplace(food, PointType::food);
        _foods[food.y].insert(food.x);
        ++_counterFood;
    }
}

PointType Field::operator()(const size_t height, const size_t width) const
{
    assert(height < _height && width < _width);

    return _field[height][width];
}

void Field::Init()
{
    _field = std::vector<std::vector<PointType>>(_height, std::vector<PointType>(_width, PointType::emptiness));

    _foods.resize(_height);

    for (auto& i : _field[0])
    {
        i = PointType::border;
    }

    for (size_t i = 1; i < _height - 1; i++)
    {
        _field[i][0] = _field[i][_width - 1] = PointType::border;
    }

    for (auto& i : _field[_height - 1])
    {
        i = PointType::border;
    }
}
