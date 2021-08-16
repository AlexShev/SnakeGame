#pragma once
#include <vector>
#include <queue>
#include "Enums.h"
#include "Reduction.h"
#include <random>
#include <set>

struct Field
{
public:
    Field(LevelDifficulty level, size_t height = 20, size_t width = 40);
    void Init();

    void ChengeField(std::queue<Reduction>& reductions);
    
    void MoveFood(int snakeLengh);
    void DisappearFood(Point pointToDisapper);
    void DisappearFood();
    void GeneratFood(int snakeLengh);

    std::vector<std::vector<PointType>> field;
    std::queue<Reduction> reductions;
    std::vector<std::set<int>> foods;

    size_t width;
    size_t height;

private:
    int RandomInt(int min, int max);
    Point GeneratPoint();

    int _counterFood;
    int _maxFoodNumbers;
    LevelDifficulty _level;

    std::mt19937 _randomGenerator;
};
