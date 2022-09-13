#define OLC_PGE_APPLICATION
#include "../vendor/olcPixelGameEngine/olcPixelGameEngine.h"

class BrickBreaker : public olc::PixelGameEngine
{
	enum BrickStructureShape
	{
		SQUARE, RECTANGLE, CIRCLE, TRIANGLE
	};

	class Paddle
	{
		public:
			float x, y, width, height;
			olc::Pixel color;

			Paddle(float x, float y, float width, float height, olc::Pixel color)
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
				this->color = color;
			}
	};

	class Brick : public Paddle
	{
		public:
			int lives;

			Brick(float x, float y, float width, float height, olc::Pixel color) : Paddle(x, y, width, height, color)
			{
				lives = 3;
			}

			void removeALife()
			{
				lives--;
			}
	};

	class Ball
	{
		public:
			olc::vf2d velocity;
			float x, y, radius;

			Ball(float x, float y, float radius, olc::vf2d velocity)
			{
				this->x = x;
				this->y = y;
				this->radius = radius;
				this->velocity = velocity;
			}

			bool Collides(Paddle p)
			{
				// The ball doesn't exist.
				return false;
			}
	};

	public:
		std::vector<Brick> bricks;
		int brickAmt;

		int brick_width, brick_height;

		BrickBreaker()
		{
			sAppName = "Brick Breaker";
		}
		
		bool OnUserCreate() override
		{
			brick_width = 30;
			brick_height = 10;

			brickAmt = 10;

			GenerateBricks();
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			for (int i = 0; i < bricks.size(); i++)
			{
				Brick b = bricks[i];

				// Collision with the ball

				if (b.lives == 0)
				{
					bricks.erase(bricks.begin() + i);
					continue;
				}

				FillRect(b.x, b.y, b.width, b.height, b.color);
			}
			return true;
		}

		void GenerateBricks()
		{
			int length = brickAmt >> 1;
			olc::Pixel color;

			for (int x = 0; x < length; x++)
			{
				for (int y = 0; y < length; y++)
				{
					color = olc::Pixel(rand() % 155 + 100, rand() % 155 + 100, rand() % 155 + 100);
					// Those threes is added there so as to see how each brick is different from eachother. 
					// Prob will be removed if I do borders, sprites, etc.

					// First paranthesis is the offset from the gui border, second is the calculating where they are, and third is the offset from each other.
					bricks.push_back(
						*new Brick(
							(brick_width) + (x * brick_width) + (1 * x), 
							(brick_height * 2) + (y * brick_height) + (1 * y), 
							brick_width, brick_height, color
						)
					);
				}
			}
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