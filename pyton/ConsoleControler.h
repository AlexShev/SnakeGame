#pragma once
#include "IControler.h"

class ConsoleControler : public IControler
{
public:
    ConsoleControler(size_t height, size_t width) : IControler(width, height) { }

    bool AskRestart() override;
    GamerType AskGamerType() override;
    LevelDifficulty AskLevelDifficulty() override;

    void ShowInit(Field& fielde) override;
    void Reflection(std::queue<Reduction>& changes) override;
    void ShowScore(int score, int timeToDeleteTail, LevelDifficulty level) override;
    void ShowGemeOver() override;
    void ShowError() override;

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

    const char* _GAME_OVER = " Game Over!!! ";
    const char* _ERROR = "Somthing is not good, restart the game, please";
    const char* _TRY_AGAIN = "Woulde you like try again? [y/n]";
};

