#pragma once
#include <cmath>

namespace Math
{
	float Distance(float x1, float y1, float x2, float y2)
	{
		float xx = x2 - x1;
		float yy = y2 - y1;

		return std::sqrt(xx * xx + yy * yy);
	}

	bool IsCircleColliding(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		//Distance function
		float distance = Distance(x1, y1, x2, y2);
		
		return distance <= r1 + r2;

	}
}