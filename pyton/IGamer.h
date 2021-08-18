#pragma once
#include "Enums.h"

class IGamer
{
public:
	virtual ~IGamer() = default;
	virtual Direction Command() = 0;
};