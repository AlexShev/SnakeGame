#include "Game.h"
#include "GamerFactory.h"

void Game::Start()
{
    LevelDifficulty level;
    GamerType type;

    if (AskParam(level, type))
    {
        do
        {
            int timeSliping = CalculateTactTime(level, type);

            Snake snake(_controler.GetWidth() / 2, _controler.GetHeight() / 2, Direction::right);
            snake.SetMaxHangryLevel(level, _controler.GetHeight(), _controler.GetWidth());

            Field field(level, _controler.GetHeight(), _controler.GetWidth());

            IGamer* gamer = GamerFactory().CreateIGamer(type, field, snake, level);

            if (gamer == nullptr)
            {
                _controler.ShowMessage(MESSAGES[MessageType::error]);

                continue;
            }

            field.ChangeField(snake.GetReductions());
            field.GenerateFood(1);

            _controler.ShowFullFrame(field);

            Condition condition = Condition::live;

            while (condition == Condition::live && !_controler.IsInterrupt())
            {
                Direction dir = gamer->Command();

                condition = snake.Move(field, dir);

                field.GenerateFood(snake.GetLenght() + 1);
                field.MoveFood(snake.GetLenght() + 1);
                
                _controler.Reflection(field);
                _controler.ShowScore(snake.GetLenght(), snake.GetTimeToDeleteTail(), level);

                Sleep(timeSliping);
            }

            Sleep(1000);

            _controler.ShowGameOver();

            delete gamer;

        } while (_controler.AskRestart() && AskParam(level, type));
    }
}

bool Game::AskParam(LevelDifficulty &level, GamerType& type)
{

    level = _controler.AskLevelDifficulty();

    while (level == LevelDifficulty::levelDifficultyError)
    {
        _controler.ShowMessage(MESSAGES[inputError]);

        if (!_controler.AskRestart())
        {
            return false;
        }

        level = _controler.AskLevelDifficulty();
    }

    type = _controler.AskGamerType();

    while (type == GamerType::gamerTypeError)
    {
        _controler.ShowMessage(MESSAGES[inputError]);

        if (!_controler.AskRestart())
        {
            return false;
        }

        type = _controler.AskGamerType();
    }

    return true;
}

int Game::CalculateTactTime(LevelDifficulty level, GamerType type)
{
    if (type == GamerType::human)
    {
        switch (level)
        {
        case easy:
            return 450;
            break;
        case middle:
            return 300;
            break;
        case hard:
            return 150;
            break;
        }
    }

    return 10;
}
