#include "pong.h"

#include <SDL3/SDL.h> 
#include <cmath>

#include "game_context.h"
#include "window.h"
#include "timestep.h"
#include "helper.h"
#include "vector2d.h"

Pong::Pong()
	: 	m_p1(
			Paddle::Type::LEFT,
			20.0f,
			static_cast<float>(SCREEN_HEIGHT / 2) - 50,
			10.0f, 50.0f,
			200.0f,
			0xffffffff
		),
		m_p2(
			Paddle::Type::RIGHT, 
			// 20.0,20.0,
			static_cast<float>(SCREEN_WIDTH) - 20 - 10,
			static_cast<float>((SCREEN_HEIGHT / 2) - 50),
			10.0f, 50.0f,
			200.0f,
			0xffffffff
		),
		m_ball(
			10.0f, 10.0f,
			130.0f, 130.0f,
			0xffffffff
		)
{
	reset({static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)});
}

Pong::~Pong()
{
	std::cout << "delete pong\n";
	// FIXME:  POSSIBLE MEMORY LEAK FOR GAME CONTEXT IF UNINITIALIZED
	delete GameContext::getInstance().m_window;
	SDL_Quit();
}

void Pong::gameLoop()
{
	m_interrupt = true;

	constexpr int frame_delay = 1000 / FPS;
	unsigned int next_game_tick = 0;
	unsigned int sleep = 0;
	
	uint32_t counted_frames = 0;
	Timestep fps_timer;
	fps_timer.start();
	while (m_interrupt)
	{
		uint32_t frame_start = fps_timer.get_ticks();
		float avg_fps = counted_frames / (fps_timer.get_ticks() / 1000.0f);
		if (avg_fps > 2000000)
		{
			avg_fps = 0;
		}

		// std::cout << "Average Frames Per Second: " << avg_fps << "\n";

		// Do event loop
		while (SDL_PollEvent(&GameContext::getInstance().m_ev))
		{
			switch (GameContext::getInstance().m_ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					m_interrupt = false;
					break;
				default:
					break;
			}
			m_p1.handleInput(GameContext::getInstance().m_ev);
			m_p2.handleInput(GameContext::getInstance().m_ev);
		}

		// FRAMERATE CAP
		uint32_t frame_time = fps_timer.get_ticks() - frame_start;
		if (frame_time < frame_delay)
		{
// TODO: SDL DELAY only takes in milliseconds and can cause noticable sync issues 
			SDL_Delay(frame_delay - frame_time);
		}

		// std::cout << "Timer: " << fps_timer.get_ticks() << "\n";
		float delta_time = 1.0f/avg_fps;
// TODO: quick fix for initial nan issue
		if (std::isnan(delta_time))
		{
			delta_time = 0.01;
		}

		// std::cout << "delta time: " << delta_time << "\n";

		// Do rendering loop
		update(delta_time);

		draw();

		++counted_frames;
	}
}

void Pong::reset(Vector2D p_position)
{
	// center ball
	m_ball.init(p_position);

	// decrement the speed of the ball if its beyond the initial speed
	m_ball.m_velocity.m_x, m_ball.m_velocity.m_y -= (m_ball.m_velocity.m_x > m_iv || m_ball.m_velocity.m_y > m_iv) ? 10.0f : 0.0f;
}

// TODO: add more hitboxes
// TODO: REFRACTOR THE CODE IN A MORE GENERAL AND MODULAR DESIGN PATTERN
// TODO: change to return the vector of the ball direction
bool Pong::checkCollisions(Ball &p_ball, Paddle &p_paddle)
{
	// depends if paddle is left or right
	if (p_paddle.m_type == Paddle::Type::LEFT)
	{
		// cross x boundary
		// hits the height of the paddle offset of the y pos
		// and less than the y offset of the paddle plus the height of the paddle
		if (
			(p_paddle.m_position.m_x + p_paddle.m_dimensions.m_x > m_ball.m_position.m_x)		&& 
			(p_ball.m_position.m_y > p_paddle.m_position.m_y) 					&& 
			(p_ball.m_position.m_y < p_paddle.m_position.m_y + p_paddle.m_dimensions.m_y))
		{
			return true;
		}
	}

	if (p_paddle.m_type == Paddle::Type::RIGHT)
	{
		// cross right x boundary
		// hits the height of the paddle offset of the y pos
		// and less than the y offset of hte paddle plus the height of the paddle
		if(
			(p_paddle.m_position.m_x - p_paddle.m_dimensions.m_x < m_ball.m_position.m_x)		&&
			(p_ball.m_position.m_y > p_paddle.m_position.m_y) 					&& 
			(p_ball.m_position.m_y < p_paddle.m_position.m_y + p_paddle.m_dimensions.m_y)
		)
		{
			return true;
		}
	}

	return false;
}

// TODO: Render score
void Pong::update(double delta_time)
{


	m_p1.update(delta_time);
	m_p2.update(delta_time);
	m_ball.update(delta_time);

	Vector2D incident = {m_ball.m_velocity.m_x, m_ball.m_velocity.m_y};
	Vector2D normal = {0.0, 1.0};
	Vector2D new_trajectory = reflect(incident, normal);

    std::cout << "New Trajectory: (" << new_trajectory.m_x << ", " << new_trajectory.m_y << ")\n";

	if(checkCollisions(m_ball, m_p1) || checkCollisions(m_ball, m_p2))
	{
		// TODO: Get the vector normal of the surface of something it hits

		m_ball.m_velocity.m_x += 10.0;
		m_ball.m_velocity.m_y += 10.0;
		m_ball.m_velocity.m_x = -m_ball.m_velocity.m_x;
	}
	/* turn the ball around if it hits the edge of the screen */
	if (m_ball.m_position.m_x < 0)
	{
		score[1]++;
		// std::cout << "P1: " << score[0] << " P2: " << score[1] << "\n";
		m_ball.m_velocity.m_x = -m_ball.m_velocity.m_x;
		reset({static_cast<float>(SCREEN_WIDTH / 2), (float)random_uniform<int>(20, SCREEN_HEIGHT-20)});
	}
	if (m_ball.m_position.m_x > GameContext::getInstance().m_screen->w - 10)
	{
		score[0]++;
		// std::cout << "P1: " << score[0] << " P2: " << score[1] << "\n";
		m_ball.m_velocity.m_x = -m_ball.m_velocity.m_x;
		reset({static_cast<float>(SCREEN_WIDTH / 2), (float)random_uniform<int>(20, SCREEN_HEIGHT-20)});
	}
	if ((m_ball.m_position.m_y < 0 || m_ball.m_position.m_y > GameContext::getInstance().m_screen->h - 10))
	{
		m_ball.m_velocity.m_y = -m_ball.m_velocity.m_y;
	}
}

void Pong::draw()
{
	// draw background
	SDL_Rect rect = {0,0,GameContext::getInstance().m_window->getWidth(),GameContext::getInstance().m_window->getHeight()};
	SDL_FillSurfaceRect(GameContext::getInstance().m_screen, &rect, 0x7F3AF9FA);

	m_p1.draw(GameContext::getInstance().m_screen);
	m_p2.draw(GameContext::getInstance().m_screen);
	m_ball.draw(GameContext::getInstance().m_screen);

    // SDL_SetRenderDrawColor( GameContext::getInstance().m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(GameContext::getInstance().m_renderer);
	SDL_UpdateTexture(GameContext::getInstance().m_texture, nullptr, GameContext::getInstance().m_screen->pixels, GameContext::getInstance().m_screen->w * sizeof(uint32_t));
	SDL_RenderTexture(GameContext::getInstance().m_renderer, GameContext::getInstance().m_texture, nullptr, nullptr);
	SDL_RenderPresent(GameContext::getInstance().m_renderer);
}