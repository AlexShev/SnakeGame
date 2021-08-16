#include "Game.h"
#include "ConsoleControler.h"

int main()
{
    ConsoleControler consoleControler(20, 40);

    Game(consoleControler).Start();

    return 0;
}
