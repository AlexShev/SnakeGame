#include "Field.h"
#include <time.h>

Field::Field(LevelDifficulty level, size_t height, size_t width) : _level(level), width(width), height(height)
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

        field[red.point.y][red.point.x] = red.type;
    }
}

void Field::MoveFood(int snakeLengh)
{
    if (_level >= LevelDifficulty::hard && RandomInt(0, height) == 0)
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

    field[pointToDisapper.y][pointToDisapper.x] = PointType::emptiness;
}

void Field::DisappearFood()
{
    Point pointToDisapper;
    
    do {
        pointToDisapper.y = RandomInt(1, height - 1);
    } while (foods[pointToDisapper.y].empty());

    pointToDisapper.x = *foods[pointToDisapper.y].begin();

    reductions.emplace(pointToDisapper, PointType::emptiness);

    DisappearFood(pointToDisapper);
}

Point Field::GeneratPoint()
{
    return Point(RandomInt(1, width - 1), RandomInt(1, height - 1));
}

void Field::GeneratFood(int snakeLengh)
{
    Point food;

    if (snakeLengh == (height - 2) * (width - 2) - _maxFoodNumbers)
    {
        _maxFoodNumbers = 1;
    }

    if(snakeLengh == (height - 2) * (width - 2))
    {
        _maxFoodNumbers = 0;
    }

    for (_counterFood; _counterFood < _maxFoodNumbers; _counterFood++)
    {
        food = GeneratPoint();

        if (field[food.y][food.x] == PointType::emptiness)
        {
            field[food.y][food.x] = PointType::food;

            reductions.emplace(food, PointType::food);

            foods[food.y].insert(food.x);
        }
        else
        {
            --_counterFood;
        }
    }
}

void Field::Init()
{
    field = std::vector<std::vector<PointType>>(height, std::vector<PointType>(width, PointType::emptiness));

    foods.resize(height);

    for (auto& i : field[0])
    {
        i = PointType::border;
    }

    for (size_t i = 1; i < height - 1; i++)
    {
        field[i][0] = field[i][width - 1] = PointType::border;
    }

    for (auto& i : field[height - 1])
    {
        i = PointType::border;
    }

    _counterFood = 0;
}
