#pragma once
#include "Point.h"
#include "Enums.h"

struct Reduction
{
	Point point;
	PointType type;
	Reduction(Point point, PointType type)
	{
		this->point = point;
		this->type = type;
	}
};