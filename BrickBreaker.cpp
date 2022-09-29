#include "GameEngine.h"

class BrickBreaker : public olc::PixelGameEngine
{
	enum BrickStructureShape
	{
		SQUARE, RECTANGLE, CIRCLE, TRIANGLE
	};

	class Paddle : public shape::Rectangle
	{
		public:
			float velocity;
			olc::Key left, right;

			Paddle(float x, float y, float width, float height, olc::Pixel color, olc::Key left, olc::Key right) : shape::Rectangle(x, y, width, height, color)
			{
				velocity = 150.0f;

				this->left = left;
				this->right = right;
			}
	};

	class Brick : public shape::Rectangle
	{
		public:
			int lives;

			Brick(float x, float y, float width, float height, olc::Pixel color) : shape::Rectangle(x, y, width, height, color)
			{
				lives = 1;
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

			bool Collides(shape::Rectangle r)
			{
				olc::vf2d rPos =
				{
					Clamp(x, r.x, r.x + r.width),
					Clamp(y, r.y, r.y + r.height)
				};
				float distance = abs(sqrt(pow(rPos.x - x, 2) + pow(rPos.y - y, 2)));
				return (distance <= radius);
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
			player = new Paddle(float(ScreenWidth() >> 1), float(ScreenHeight() - 30), 50.0f, 10.0f, olc::WHITE, olc::Key::A, olc::Key::D);
			ball = new Ball(float(ScreenWidth() >> 1), float(ScreenHeight() >> 1), 3.0f, olc::WHITE, { 100.0f, 100.0f });

			brick_width = 30;
			brick_height = 10;

			brickAmt = 10;

			GenerateBricks();
			return true;
		}

		bool OnUserUpdate(float fElapsedTime) override
		{
			if (bricks.size() == 0)
			{
				// YOU WON
			}

			Clear(olc::BLACK);

			// Player OnUserUpdate();
			if (GetKey(player->left).bHeld && player->x > 0)
			{
				player->x -= player->velocity * fElapsedTime;
			}
			else if (GetKey(player->right).bHeld && player->x < ScreenWidth())
			{
				player->x += player->velocity * fElapsedTime;
			}
			FillRect(player->x, player->y, player->width, player->height, player->color);
			

			// Ball OnUserUpdate();
			ball->x += ball->velocity.x * fElapsedTime;
			ball->y += ball->velocity.y * fElapsedTime;

			if (ball->x < 0 || ball->x > ScreenWidth())
			{
				reverseBallMovement(ball, fElapsedTime);
				ball->velocity.x = -ball->velocity.x;
			}
			if (ball->y < 0)
			{
				reverseBallMovement(ball, fElapsedTime);
				ball->velocity.y = -ball->velocity.y;
			}
			else if (ball->y > ScreenHeight())
			{
				// GameOver or possibility just the removal of itself inside an std::vector<Ball>
			}

			if (ball->Collides(*player))
			{
				reverseBallMovement(ball, fElapsedTime);
				ball->velocity.y *= -1;
			}
			else
			{
				// Figure out closest brick and then check collision with
				for (int i = 1; i < bricks.size(); i++)
				{
					Brick* brick = &bricks[i];
					if (ball->Collides(*brick))
					{
						brick->removeALife();
						reverseBallMovement(ball, fElapsedTime);
						ball->velocity.y *= -1;
					}
				}
			}
			FillCircle(ball->x, ball->y, ball->radius, ball->color);

			// Rendering bricks
			for (int i = 0; i < bricks.size(); i++)
			{
				Brick b = bricks[i];
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

		void reverseBallMovement(Ball *ball, float fElapsedTime)
		{
			ball->x -= ball->velocity.x * fElapsedTime;
			ball->y -= ball->velocity.y * fElapsedTime;
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