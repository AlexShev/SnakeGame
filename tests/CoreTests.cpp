#include "pyton/Field.h"
#include "pyton/Snake.h"
#include "pyton/AverageArtificialGamer.h"
#include "pyton/HardArtificialGamer.h"
#include <cassert>
#include <iostream>
#include <stdexcept>

static void Put(Field& field, Point point, PointType type)
{
    std::queue<Reduction> changes;
    changes.emplace(point, type);
    field.ChangeField(changes);
}

int main()
{
    try
    {
        Field invalid(easy, 2, 4);
        assert(false);
    }
    catch (const std::invalid_argument&) { }

    Field field(easy, 8, 8);
    Snake snake(3, 3, right);
    snake.SetMaxHangryLevel(easy, 8, 8);
    field.ChangeField(snake.GetReductions());

    Put(field, {4, 3}, food);
    assert(snake.Move(field, right) == live);
    assert(snake.GetLenght() == 1);
    assert(snake.Move(field, left) == live);
    assert(snake.GetHead() == Point(5, 3)); // reverse input ignored

    Field tailField(easy, 8, 8);
    Snake tailSnake(3, 3, right);
    tailSnake.SetMaxHangryLevel(easy, 8, 8);
    tailField.ChangeField(tailSnake.GetReductions());
    const Point meals[] = {{4, 3}, {4, 2}, {3, 2}};
    const Direction moves[] = {right, up, left};
    for (int i = 0; i < 3; ++i)
    {
        Put(tailField, meals[i], food);
        assert(tailSnake.Move(tailField, moves[i]) == live);
    }
    assert(tailSnake.GetTail().back() == Point(3, 3));
    assert(tailSnake.Move(tailField, down) == live);
    assert(tailSnake.GetHead() == Point(3, 3));
    assert(tailField(3, 3) == snakeHead);

    Field nearlyFull(easy, 5, 5);
    for (int y = 1; y < 4; ++y)
        for (int x = 1; x < 4; ++x)
            if (x != 2 || y != 2)
                Put(nearlyFull, {x, y}, snakeBody);
    nearlyFull.GenerateFood();
    assert(nearlyFull(2, 2) == food);
    nearlyFull.GenerateFood(); // no free cells; must return

    Field noFood(middle, 8, 8);
    Snake noFoodSnake(3, 3, right);
    noFood.ChangeField(noFoodSnake.GetReductions());
    AverageArtificialGamer medium(noFood, noFoodSnake);
    HardArtificialGamer hardBot(noFood, noFoodSnake);
    assert(medium.Command() != nothing);
    assert(hardBot.Command() != nothing);

    for (int game = 0; game < 3; ++game)
    {
        Field board(hard, 20, 40);
        Snake player(20, 10, right);
        player.SetMaxHangryLevel(hard, 20, 40);
        board.ChangeField(player.GetReductions());
        board.GenerateFood();
        HardArtificialGamer bot(board, player);
        int survived = 0;
        for (int tick = 0; tick < 1500; ++tick)
        {
            const Direction decision = bot.Command();
            if (player.Move(board, decision) == dedth)
                break;
            ++survived;
            board.GenerateFood();
            board.MoveFood();
            assert(board(player.GetHead().y, player.GetHead().x) == snakeHead);
        }
        std::cout << "hard bot game " << game << ": " << survived << " ticks, score " << player.GetLenght() << "\n";
        // A distant initial meal can cause hunger after 20 moves; log the
        // observed run instead of treating random survival as a guarantee.
    }

    std::cout << "core checks passed\n";
}
