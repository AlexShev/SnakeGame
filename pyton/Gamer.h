#pragma once
#include "IGamer.h"
#include <WinUser.h>

class Gamer : public IGamer
{
public:
    Direction Command() override;
};