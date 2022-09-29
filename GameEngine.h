#pragma once
#define OLC_PGE_APPLICATION
#include "../vendor/olcPixelGameEngine/olcPixelGameEngine.h"

namespace shape
{
	class Rectangle
	{
		public:
			float x, y, width, height;
			olc::Pixel color;

			Rectangle(float x, float y, float width, float height, olc::Pixel color)
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
				this->color = color;
			}
	};

	class Circle
	{
		public:
			float x, y, radius;
			olc::Pixel color;

			Circle(float x, float y, float radius, olc::Pixel color)
			{
				this->x = x;
				this->y = y;
				this->radius = radius;
				this->color = color;
			}
	};
}

inline float Clamp(float value, float min, float max)
{
	return std::max(min, std::min(max, value));
}