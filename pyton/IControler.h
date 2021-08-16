#pragma once
#include "Field.h"
#include "Reduction.h"
#include "Enums.h"
#include <string>
#include <queue>

class IControler
{
public:
    IControler(size_t width, size_t height) : _width(width), _height(height) { }

    virtual bool AskRestart() = 0;
    virtual GamerType AskGamerType() = 0;
    virtual LevelDifficulty AskLevelDifficulty() = 0;

    virtual bool IsInterrupt() = 0;

    virtual void ShowInit(Field& fielde) = 0;
    virtual void Reflection(std::queue<Reduction>& changes) = 0;
    virtual void ShowScore(int score, int timeToDeleteTail, LevelDifficulty level) = 0;
    virtual void ShowGemeOver() = 0;
    virtual void ShowMessage(const char* message) = 0;

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }

private:
    size_t _width;
    size_t _height;
};
