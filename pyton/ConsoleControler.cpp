#include "ConsoleControler.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

bool ConsoleControler::AskRestart()
{
    std::cout << MESSAGES[tryAgain] << " [y/n]" << std::endl;

    auto answer = ReadData();

    return answer == "y" || answer == "";
}

GamerType ConsoleControler::AskGamerType()
{
    PrintGamerType();

    return ReadGamerType();
}

LevelDifficulty ConsoleControler::AskLevelDifficulty()
{
    PrintLevelDifficulty();

    return ReadLevelDifficulty();
}

void ConsoleControler::ShowFullFrame(Field& field)
{
    system("cls");

    auto& frame = field.GetField();


    for (auto& line : frame)
    {
        for (auto simbol : line)
        {
            std::cout << GetSymbol(simbol);
        }
        std::cout << std::endl;
    }
}

void ConsoleControler::Reflection(Field& fielde)
{
    std::queue<Reduction> changes;

    fielde.MoveReductions(changes);

    while (!changes.empty())
    {
        Reduction red = changes.front();
        changes.pop();

        this->SetCursor(red.point.x, red.point.y);

        std::cout << GetSymbol(red.type);
    }

    this->SetCursor(0, GetHeight() + 2);
}

void ConsoleControler::ShowScore(int score, int timeToDeleteTail, LevelDifficulty level)
{
    SetCursor(0, GetHeight() + 1);

    std::cout << "Your score: " << score;

    if (level > LevelDifficulty::easy)
    {
        std::cout << " Time to remove the tail: " << timeToDeleteTail << "          ";
    }

    this->SetCursor(0, GetHeight() + 2);
}

void ConsoleControler::ShowGemeOver()
{
    int width = GetWidth();
    int height = GetHeight();

    for (size_t i = 0; i < 2; i++)
    {
        SetCursor(width / 2 - 7, height / 2 - 1);

        std::cout << MESSAGES[gameOver];

        SetCursor(0, height + 2);

        Sleep(500);

        SetCursor(width / 2 - 7, height / 2 - 1);

        std::cout << "              ";

        SetCursor(0, height + 2);

        Sleep(500);
    }

    SetCursor(width / 2 - 7, height / 2 - 1);

    std::cout << MESSAGES[gameOver];

    SetCursor(0, height + 3);

    ClearConsoleAfterPress();
}

void ConsoleControler::ShowMessage(const char* message)
{
    system("cls");

    std::cout << message << std::endl;
}

bool ConsoleControler::IsInterrupt()
{
    return GetKeyState(VK_ESCAPE) < 0;
}

char ConsoleControler::GetSymbol(PointType condition)
{
    switch (condition)
    {
    case PointType::border:
        return '#';
    case PointType::emptiness:
        return ' ';
    case PointType::snakeBody:
        return '0';
    case PointType::snakeHead:
        return '@';
    case PointType::food:
        return '$';
    default:
        break;
    }
    return ' ';
}

void ConsoleControler::SetCursor(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void ConsoleControler::PrintLevelDifficulty()
{
    system("cls");

    std::cout << "Enter level difficulty\n";
    std::cout << LevelDifficulty::easy << " - easy level\n";
    std::cout << LevelDifficulty::middle << " - middle level\n";
    std::cout << LevelDifficulty::hard << " - hard level\n";
}

void ConsoleControler::PrintGamerType()
{
    system("cls");

    std::cout << "Enter gamer type\n";
    std::cout << GamerType::human << " - you are a gamer\n";
    std::cout << GamerType::artificial << " - our AI is a gamer\n";
}


std::string ConsoleControler::ReadData()
{
    std::cout << ">> ";
    std::string answer;
    std::getline(std::cin, answer);
    return answer;
}

void ConsoleControler::ClearConsoleAfterPress()
{
    system("PAUSE"); system("cls");
};

int ConsoleControler::ReadInt()
{
    int input = -1;

    try
    {
        input = std::stoi(ReadData());
    }
    catch (const std::exception&) {}

    return input;
}

LevelDifficulty ConsoleControler::ReadLevelDifficulty()
{
    int level = ReadInt();

    if (level < LevelDifficulty::easy || level > LevelDifficulty::hard)
    {
        return LevelDifficulty::levelDifficultyError;
    }

    return LevelDifficulty(level);
}

GamerType ConsoleControler::ReadGamerType()
{
    int type = ReadInt();

    if (type < GamerType::human || type > GamerType::artificial)
    {
        return GamerType::gamerTypeError;
    }

    return GamerType(type);
}
