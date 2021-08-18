#include "Game.h"
#include "ConsoleController.h"

int main()
{
    ConsoleController consoleController(20, 40);

    Game(consoleController).Start();

    return 0;
}
