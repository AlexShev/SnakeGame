#include "Field.h"
#include <ctime>
#include <cassert>

Field::Field(const LevelDifficulty level, const size_t height, const size_t width)
: _level(level), _width(width), _height(height)
{
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

void Field::MoveFood(int snakeLength)
{
    if (_level >= LevelDifficulty::hard && RandomInt(0, _height) == 0)
    {
        DisappearFood();
        GenerateFood(snakeLength);
    }
}

int Field::RandomInt(const int min, const int max)
{
    std::uniform_int_distribution<int> range(min, max);

    return range(_randomGenerator);
}

void Field::DisappearFood(Point pointToDisappear)
{
    --_counterFood;

    _foods[pointToDisappear.y].erase(pointToDisappear.x);

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

Point Field::GeneratePoint()
{
    return Point(RandomInt(1, _width - 1), RandomInt(1, _height - 1));
}

void Field::GenerateFood(int snakeLength)
{
	if (snakeLength == static_cast<int>(_height - 2) * static_cast<int>(_width - 2) - _maxFoodNumbers)
    {
        _maxFoodNumbers = 1;
    }

    if(snakeLength == static_cast<int>(_height - 2) * static_cast<int>(_width - 2))
    {
        _maxFoodNumbers = 0;
    }

    for (; _counterFood < _maxFoodNumbers; _counterFood++)
    {
        Point food = GeneratePoint();

        if (_field[food.y][food.x] == PointType::emptiness)
        {
            _field[food.y][food.x] = PointType::food;

            _reductions.emplace(food, PointType::food);

            _foods[food.y].insert(food.x);
        }
        else
        {
            --_counterFood;
        }
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
