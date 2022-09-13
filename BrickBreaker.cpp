#define OLC_PGE_APPLICATION
#include "../vendor/olcPixelGameEngine/olcPixelGameEngine.h"

class BrickBreaker : public olc::PixelGameEngine
{
	class Brick
	{
		public:
			float x, y, width, height;
			olc::Pixel color;

			Brick(float x, float y, float width, float height, olc::Pixel color)
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
				this->color = color;
			}
	};

	public:
		std::vector<Brick> bricks;
		int brickAmt;

		BrickBreaker()
		{
			sAppName = "Brick Breaker";
		}
		
		bool OnUserCreate() override
		{
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			return true;
		}
};

int main()
{
	BrickBreaker brickBreacker;
	if (brickBreacker.Construct(450, 300, 2, 2, false, true, false))
	{
		brickBreacker.Start();
	}
	return 0;
}