#include "../pyton/Field.h"
#include "../pyton/Snake.h"
#include "../pyton/HardArtificialGamer.h"
#include "../pyton/NeuralArtificialGamer.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <vector>

// benchmark <games> <first_seed> <teacher:0|1>
int main(int argc,char** argv)
{
    const int games=argc>1?std::atoi(argv[1]):50;
    const unsigned seed=argc>2?static_cast<unsigned>(std::strtoul(argv[2],nullptr,10)):100000;
    const bool teacher=argc>3 && std::atoi(argv[3])!=0;
    if(games<=0)return 1;
    std::vector<int> ticks,scores;
    int longRuns=0,early=0;
    for(int game=0;game<games;++game)
    {
        Field board(hard,20,40);
        board.Seed(seed+game);
        Snake snake(20,10,right);
        snake.SetMaxHangryLevel(hard,20,40);
        board.ChangeField(snake.GetReductions());
        board.GenerateFood();
        NeuralArtificialGamer student(board,snake);
        HardArtificialGamer expert(board,snake);
        if(!student.Loaded()) { std::cerr<<"model not found\n";return 2; }
        int tick=0;
        for(;tick<1200;++tick)
        {
            const Direction action=teacher?expert.Command():student.Command();
            if(snake.Move(board,action)==dedth)break;
            board.GenerateFood();
            board.MoveFood();
        }
        ticks.push_back(tick);
        scores.push_back(snake.GetLenght());
        if(tick>=1200)++longRuns;
        if(tick<=20)++early;
    }
    std::sort(ticks.begin(),ticks.end());
    std::cout<<(teacher?"teacher":"neural")
        <<" games="<<games
        <<" mean_ticks="<<std::accumulate(ticks.begin(),ticks.end(),0)/games
        <<" median="<<ticks[games/2]
        <<" reached_1200="<<longRuns
        <<" died_by_20="<<early
        <<" mean_score="<<std::accumulate(scores.begin(),scores.end(),0)/games
        <<'\n';
}
