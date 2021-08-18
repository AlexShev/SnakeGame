#pragma once
#include <vector>
#include <queue>
#include "Enums.h"
#include "Reduction.h"
#include <random>
#include <set>

class Field
{
public:
	explicit Field(LevelDifficulty level, size_t height = 20, size_t width = 40);

    void ChangeField(std::queue<Reduction>& reductions);
    
    void MoveFood(int snakeLength);
    void DisappearFood(Point pointToDisappear);
    void DisappearFood();
    void GenerateFood(int snakeLength);

    void MoveReductions(std::queue<Reduction>& toMove) { toMove = std::move(_reductions); }
    
    PointType operator ()(size_t height, size_t width) const;
    const std::vector<std::vector<PointType>>& GetField() const { return _field; }
    size_t GetWidth() const { return _width; }
    size_t GetHeight() const { return _height; }

private:
    void Init();
    int RandomInt(int min, int max);
    Point GeneratePoint();

    std::vector<std::vector<PointType>> _field;
    std::vector<std::set<int>> _foods;
    std::queue<Reduction> _reductions;

    int _counterFood;
    int _maxFoodNumbers;
    LevelDifficulty _level;

    size_t _width;
    size_t _height;

    std::mt19937 _randomGenerator;
};
