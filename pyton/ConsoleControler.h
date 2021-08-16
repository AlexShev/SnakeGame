#pragma once
#include "IControler.h"

class ConsoleControler : public IControler
{
public:
    ConsoleControler(size_t height, size_t width) : IControler(width, height) { }

    bool AskRestart() override;
    GamerType AskGamerType() override;
    LevelDifficulty AskLevelDifficulty() override;

    void ShowFullFrame(Field& fielde) override;
    void Reflection(Field& fielde) override;
    void ShowScore(int score, int timeToDeleteTail, LevelDifficulty level) override;
    void ShowGemeOver() override;
    void ShowMessage(const char* message) override;

    bool IsInterrupt() override;

private:
    std::string ReadData();
    void ClearConsoleAfterPress();
    LevelDifficulty ReadLevelDifficulty();
    GamerType ReadGamerType();
    int ReadInt();
    void PrintLevelDifficulty();
    void PrintGamerType();
    void SetCursor(int x, int y);
    char GetSymbol(PointType condition);
};

