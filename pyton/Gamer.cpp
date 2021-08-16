#include "Game.h"

Direction Gamer::Comand()
{
    if (GetKeyState(VK_UP) < 0)
    {
        return up;
    }
    if (GetKeyState(VK_DOWN) < 0)
    {
        return down;
    }
    if (GetKeyState(VK_LEFT) < 0)
    {
        return left;
    }
    if (GetKeyState(VK_RIGHT) < 0)
    {
        return right;
    }

    if (GetKeyState('W') < 0)
    {
        return up;
    }
    if (GetKeyState('S') < 0)
    {
        return down;
    }
    if (GetKeyState('A') < 0)
    {
        return left;
    }
    if (GetKeyState('D') < 0)
    {
        return right;
    }

    return nothing;
}
