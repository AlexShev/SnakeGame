#pragma once

struct Point
{
	int x;
	int y;

	Point(int x = 0, int y = 0)
	{
		this->x = x;
		this->y = y;
	}

	bool operator== (const Point& right) const
	{
		return x == right.x && y == right.y;
	}
};