#include "../pyton/Field.h"
#include "../pyton/Snake.h"
#include "../pyton/HardArtificialGamer.h"
#include "../pyton/NeuralArtificialGamer.h"
#include "../pyton/NeuralFeatures.h"
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

// collect <games> <teacher|mix|neural> <teacher_probability> <seed>
// Labels always come from the teacher, actions may come from the student.
int main(int argc, char** argv)
{
    const int games = argc > 1 ? std::atoi(argv[1]) : 20;
    const std::string mode = argc > 2 ? argv[2] : "teacher";
    const double beta = argc > 3 ? std::atof(argv[3]) : 1.0;
    const unsigned seed = argc > 4 ? static_cast<unsigned>(std::strtoul(argv[4],nullptr,10)) : 2026;
    std::mt19937 random(seed);
    std::uniform_real_distribution<double> uniform(0.0,1.0);
    const Direction dirs[4]={left,right,up,down};
    for (int game=0;game<games;++game)
    {
        Field field(hard,20,40);
        field.Seed(seed+game);
        Snake snake(20,10,right);
        snake.SetMaxHangryLevel(hard,20,40);
        field.ChangeField(snake.GetReductions());
        field.GenerateFood();
        HardArtificialGamer teacher(field,snake);
        NeuralArtificialGamer student(field,snake);
        if (mode != "teacher" && !student.Loaded()) return 2;
        for (int tick=0;tick<1200;++tick)
        {
            const Direction label=teacher.Command();
            if (tick%2==0 && label != nothing)
            {
                const auto features=NeuralFeatures(field,snake);
                for (float value:features) std::cout<<value<<',';
                for (int i=0;i<4;++i)
                    if (dirs[i]==label) { std::cout<<i<<'\n'; break; }
            }
            Direction action=label;
            if (mode=="neural" || (mode=="mix" && uniform(random)>beta))
                action=student.Command();
            if (snake.Move(field,action)==dedth) break;
            field.GenerateFood();
            field.MoveFood();
        }
    }
}
