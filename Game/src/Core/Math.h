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

	bool PointInPolygon(const SDL_FPoint& point, const std::vector<SDL_FPoint>& polygon)
	{
		bool isInside = false;
		int n = polygon.size();

		for (int i = 0, j = n - 1; i < n; j = i++)
		{
			const SDL_FPoint& pointi = polygon[i];
			const SDL_FPoint& pointj = polygon[j];

			if (((pointi.y > point.y) != (pointj.y > point.y)) &&
				(point.x < (pointj.x - pointi.x) * (point.y - pointi.y) / (pointj.y - pointi.y) + pointi.x))
			{
				isInside = !isInside;
			}
		}

		return isInside;

	}

	float PointToSegmentDistance(const SDL_FPoint& p, const SDL_FPoint& a, const SDL_FPoint& b) {
		float l2 = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
		if (l2 == 0.0f) return Distance(p.x, p.y, a.x, a.y);
		float t = std::max(0.0f, std::min(1.0f, ((p.x - a.x) * (b.x - a.x) + (p.y - a.y) * (b.y - a.y)) / l2));
		SDL_FPoint projection = { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) };
		return Distance(p.x, p.y, projection.x, projection.y);
	}
}