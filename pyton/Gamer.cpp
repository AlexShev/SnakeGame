#include "Game.h"

Direction Gamer::Command()
{
    if (GetAsyncKeyState(VK_UP) < 0)
    {
        return up;
    }
    if (GetAsyncKeyState(VK_DOWN) < 0)
    {
        return down;
    }
    if (GetAsyncKeyState(VK_LEFT) < 0)
    {
        return left;
    }
    if (GetAsyncKeyState(VK_RIGHT) < 0)
    {
        return right;
    }

    if (GetAsyncKeyState('W') < 0)
    {
        return up;
    }
    if (GetAsyncKeyState('S') < 0)
    {
        return down;
    }
    if (GetAsyncKeyState('A') < 0)
    {
        return left;
    }
    if (GetAsyncKeyState('D') < 0)
    {
        return right;
    }

    return nothing;
}
