#include "GameEngine.h"

class BrickBreaker : public olc::PixelGameEngine
{
	enum BrickStructureShape
	{
		SQUARE, RECTANGLE, CIRCLE, TRIANGLE
	};

	class Paddle : public shape::Rectangle
	{
		private:
			float velocity;
			olc::Key left, right;

			Paddle(float x, float y, float width, float height, olc::Pixel color, olc::Key left, olc::Key right) : shape::Rectangle(x, y, width, height, color)
			{
				velocity = 10.0f;

				this->left = left;
				this->right = right;
			}

			void OnUserUpdate(float fElapsedTime, BrickBreaker *b)
			{
				if (b->GetKey(left).bHeld && x > 0)
				{
					x -= velocity * fElapsedTime;
				}
				else if (b->GetKey(right).bHeld && x < b->ScreenWidth())
				{
					x += velocity * fElapsedTime;
				}

				b->FillRect(x, y, width, height, color);
			}
	};

	class Brick : public shape::Rectangle
	{
		public:
			int lives;

			Brick(float x, float y, float width, float height, olc::Pixel color) : shape::Rectangle(x, y, width, height, color)
			{
				lives = 3;
			}

			void removeALife()
			{
				lives--;
				
			}
	};

	class Ball : public shape::Circle
	{
		public:
			olc::vf2d velocity;

			Ball(float x, float y, float radius, olc::Pixel color, olc::vf2d velocity) : shape::Circle(x, y, radius, color)
			{
				this->velocity = velocity;
			}

			void OnUserUpdate(float fElapsedTime, BrickBreaker* b)
			{
				x += velocity.x * fElapsedTime;
				y += velocity.y * fElapsedTime;

				if (x < 0 || x > b->ScreenHeight())
				{
					velocity.x = -velocity.x;
				}
				if (y < 0)
				{
					velocity.y = -velocity.y;
					y = radius / 2;
				}
				else if (y > b->ScreenHeight())
				{
					// GameOver or possibility just the removal of itself inside an std::vector<Ball>
				}

				// Figure out closest brick and then check collision with
				Brick closestBrick = *new Brick(0, 0, 0, 0, olc::WHITE);
				if (Collides(closestBrick))
				{
					closestBrick.removeALife();
				}

				b->FillCircle(x, y, radius, color);
			}

			bool Collides(shape::Rectangle p)
			{
				// The ball doesn't exist.
				// Deal with the bounce here.
				// Or just make two methods.
				// Checking specific collisions.
				// (Horizontal and Vertical)
				return false;
			}
	};

	public:
		std::vector<Brick> bricks;
		int brickAmt;

		int brick_width, brick_height;

		Paddle* player;
		Ball* ball;

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
	BrickBreaker brickBreaker;
	if (brickBreaker.Construct(450, 300, 2, 2, false, true, true))
	{
		brickBreaker.Start();
	}
	return 0;
}