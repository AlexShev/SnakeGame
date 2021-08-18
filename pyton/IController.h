#pragma once
#include "Field.h"
#include "Reduction.h"
#include "Enums.h"
#include <string>
#include <queue>

class IController
{
public:
	virtual ~IController() = default;
	IController(size_t width, size_t height) : _width(width), _height(height) { }

    virtual bool AskRestart() = 0;
    virtual GamerType AskGamerType() = 0;
    virtual LevelDifficulty AskLevelDifficulty() = 0;

    virtual bool IsInterrupt() = 0;

    virtual void ShowFullFrame(Field& fielde) = 0;
    virtual void Reflection(Field& fielde) = 0;
    virtual void ShowScore(const int score, const int timeToDeleteTail, const LevelDifficulty level) = 0;
    virtual void ShowGemeOver() = 0;
    virtual void ShowMessage(const char* message) = 0;

    int GetWidth() const { return _width; }
    int GetHeight() const { return _height; }

private:
    size_t _width;
    size_t _height;
};
