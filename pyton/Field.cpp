#include "Field.h"
#include <time.h>
#include <cassert>

Field::Field(LevelDifficulty level, size_t height, size_t width) : _level(level), _width(width), _height(height)
{
    Init();
    std::random_device device;
    _randomGenerator.seed(device());
    _maxFoodNumbers = std::max(1., std::sqrt(width + height ) / level);
}

void Field::ChengeField(std::queue<Reduction>& reductions)
{
    while (!reductions.empty())
    {
        Reduction red = reductions.front();

        reductions.pop();

        this->reductions.push(red);

        _field[red.point.y][red.point.x] = red.type;
    }
}

void Field::MoveFood(int snakeLengh)
{
    if (_level >= LevelDifficulty::hard && RandomInt(0, _height) == 0)
    {
        DisappearFood();
        GeneratFood(snakeLengh);
    }
}

int Field::RandomInt(int min, int max)
{
    std::uniform_int_distribution<int> range(min, max);

    return range(_randomGenerator);
}

void Field::DisappearFood(Point pointToDisapper)
{
    --_counterFood;

    foods[pointToDisapper.y].erase(pointToDisapper.x);

    _field[pointToDisapper.y][pointToDisapper.x] = PointType::emptiness;
}

void Field::DisappearFood()
{
    Point pointToDisapper;
    
    do {
        pointToDisapper.y = RandomInt(1, _height - 1);
    } while (foods[pointToDisapper.y].empty());

    pointToDisapper.x = *foods[pointToDisapper.y].begin();

    reductions.emplace(pointToDisapper, PointType::emptiness);

    DisappearFood(pointToDisapper);
}

Point Field::GeneratPoint()
{
    return Point(RandomInt(1, _width - 1), RandomInt(1, _height - 1));
}

void Field::GeneratFood(int snakeLengh)
{
    Point food;

    if (snakeLengh == (_height - 2) * (_width - 2) - _maxFoodNumbers)
    {
        _maxFoodNumbers = 1;
    }

    if(snakeLengh == (_height - 2) * (_width - 2))
    {
        _maxFoodNumbers = 0;
    }

    for (_counterFood; _counterFood < _maxFoodNumbers; _counterFood++)
    {
        food = GeneratPoint();

        if (_field[food.y][food.x] == PointType::emptiness)
        {
            _field[food.y][food.x] = PointType::food;

            reductions.emplace(food, PointType::food);

            foods[food.y].insert(food.x);
        }
        else
        {
            --_counterFood;
        }
    }
}

PointType Field::operator()(size_t height, size_t width) const
{
    assert(height < _height && width < _width);

    return _field[height][width];
}

void Field::Init()
{
    _field = std::vector<std::vector<PointType>>(_height, std::vector<PointType>(_width, PointType::emptiness));

    foods.resize(_height);

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

    _counterFood = 0;
}
