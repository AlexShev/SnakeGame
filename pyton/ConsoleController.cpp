#include "ConsoleController.h"
#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>

bool ConsoleController::AskRestart()
{
    std::cout << MESSAGES[tryAgain] << " [y/n]" << std::endl;

    auto answer = ReadData();

    return answer == "y" || answer == "";
}

GamerType ConsoleController::AskGamerType()
{
    PrintGamerType();

    return ReadGamerType();
}

LevelDifficulty ConsoleController::AskLevelDifficulty()
{
    PrintLevelDifficulty();

    return ReadLevelDifficulty();
}

void ConsoleController::ShowFullFrame(Field& field)
{
    system("cls");  // NOLINT(concurrency-mt-unsafe)

    auto& frame = field.GetField();


    for (auto& line : frame)
    {
        for (const auto simbol : line)
        {
            std::cout << GetSymbol(simbol);
        }
        std::cout << std::endl;
    }
}

void ConsoleController::Reflection(Field& field)
{
    std::queue<Reduction> changes;

    field.MoveReductions(changes);

    while (!changes.empty())
    {
	    const Reduction red = changes.front();
        changes.pop();

        this->SetCursor(red.point.x, red.point.y);

        std::cout << GetSymbol(red.type);
    }

    this->SetCursor(0, GetHeight() + 2);
}

void ConsoleController::ShowScore(const int score, const int timeToDeleteTail, const LevelDifficulty level)
{
    SetCursor(0, GetHeight() + 1);

    std::cout << "Your score: " << score;

    if (level > LevelDifficulty::easy)
    {
        std::cout << " Time to remove the tail: " << timeToDeleteTail << "          ";
    }

    this->SetCursor(0, GetHeight() + 2);
}

void ConsoleController::ShowGemeOver()
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

void ConsoleController::ShowMessage(const char* message)
{
    system("cls");

    std::cout << message << std::endl;
}

bool ConsoleController::IsInterrupt()
{
    return GetKeyState(VK_ESCAPE) < 0;
}

char ConsoleController::GetSymbol(PointType condition)
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

void ConsoleController::SetCursor(int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void ConsoleController::PrintLevelDifficulty()
{
    system("cls");

    std::cout << "Enter level difficulty\n";
    std::cout << LevelDifficulty::easy << " - easy level\n";
    std::cout << LevelDifficulty::middle << " - middle level\n";
    std::cout << LevelDifficulty::hard << " - hard level\n";
}

void ConsoleController::PrintGamerType()
{
    system("cls");

    std::cout << "Enter gamer type\n";
    std::cout << GamerType::human << " - you are a gamer\n";
    std::cout << GamerType::artificial << " - our AI is a gamer\n";
}


std::string ConsoleController::ReadData()
{
    std::cout << ">> ";
    std::string answer;
    std::getline(std::cin, answer);
    return answer;
}

void ConsoleController::ClearConsoleAfterPress()
{
    system("PAUSE"); system("cls");
};

int ConsoleController::ReadInt()
{
    int input = -1;

    try
    {
        input = std::stoi(ReadData());
    }
    catch (const std::exception&) {}

    return input;
}

LevelDifficulty ConsoleController::ReadLevelDifficulty()
{
    int level = ReadInt();

    if (level < LevelDifficulty::easy || level > LevelDifficulty::hard)
    {
        return LevelDifficulty::levelDifficultyError;
    }

    return LevelDifficulty(level);
}

GamerType ConsoleController::ReadGamerType()
{
    int type = ReadInt();

    if (type < GamerType::human || type > GamerType::artificial)
    {
        return GamerType::gamerTypeError;
    }

    return GamerType(type);
}
