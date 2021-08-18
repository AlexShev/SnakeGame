#pragma once
#include "IController.h"

class ConsoleController final : public IController
{
public:
    ConsoleController(size_t height, size_t width);

    bool AskRestart() override;
    GamerType AskGamerType() override;
    LevelDifficulty AskLevelDifficulty() override;

    void ShowFullFrame(Field& field) override;
    void Reflection(Field& field) override;
    void ShowScore(int score, int timeToDeleteTail, LevelDifficulty level) override;
    void ShowGameOver() override;
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

