#include "../pyton/Field.h"
#include "../pyton/Snake.h"
#include "../pyton/HardArtificialGamer.h"
#include "../pyton/NeuralFeatures.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char** argv)
{
    const int games = argc > 1 ? std::atoi(argv[1]) : 20;
    const Direction dirs[4] = {left, right, up, down};
    for (int game = 0; game < games; ++game)
    {
        Field field(hard, 20, 40);
        Snake snake(20, 10, right);
        snake.SetMaxHangryLevel(hard, 20, 40);
        field.ChangeField(snake.GetReductions());
        field.GenerateFood();
        HardArtificialGamer teacher(field, snake);
        for (int tick = 0; tick < 1200; ++tick)
        {
            Direction action = teacher.Command();
            if (tick % 2 == 0)
            {
                const auto features = NeuralFeatures(field, snake);
                for (float value : features) std::cout << value << ',';
                int label = 0;
                while (label < 4 && dirs[label] != action) ++label;
                std::cout << label << '\n';
            }
            if (snake.Move(field, action) == dedth) break;
            field.GenerateFood();
            field.MoveFood();
        }
    }
}
