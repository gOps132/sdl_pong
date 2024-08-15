#include "pong.h"

#include <SDL3/SDL.h> 
#include <cmath>

#include "game_context.h"
#include "window.h"
#include "timestep.h"
#include "helper.h"
#include "vector2d.h"
#include "ball.h"

Pong::Pong()
	: 	m_p1(
			Paddle::Type::LEFT,
			20.0f,
			static_cast<float>(SCREEN_HEIGHT / 2) - 50,
			0.0f, 200.0f,
			10.0f, 50.0f,
			0xffffffff
		),
		m_p2(
			Paddle::Type::RIGHT, 
			// 20.0,20.0,
			static_cast<float>(SCREEN_WIDTH) - 20 - 10,
			static_cast<float>((SCREEN_HEIGHT / 2) - 50),
			0.0f, 200.0f,
			10.0f, 50.0f,
			0xffffffff
		)
{
	Vector2D pos = {static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2)};

	m_ball.reserve(200);
	// create 100 balls
	for (int i = 0; i < 200; i++)
	{
		m_ball.emplace_back(
			10.0f, 10.0f,
			130.0f + (float)i, 130.0f + (float)i,
			0xffffffff
		);
	}

	reset(pos);
}

Pong::~Pong()
{
	std::cout << "delete pong\n";
	delete GameContext::getInstance();
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
		while (SDL_PollEvent(&GameContext::getInstance()->m_ev))
		{
			switch (GameContext::getInstance()->m_ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					m_interrupt = false;
					break;
				default:
					break;
			}
			m_p1.handleInput(GameContext::getInstance()->m_ev);
			m_p2.handleInput(GameContext::getInstance()->m_ev);
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

void Pong::reset(Vector2D &p_position)
{
	// center ball
	for (auto& ball : m_ball)
	{
		ball.init(p_position);

		// decrement the speed of the ball if its beyond the initial speed
		ball.m_velocity.m_x, ball.m_velocity.m_y -= (ball.m_velocity.m_x > m_iv || ball.m_velocity.m_y > m_iv) ? 10.0f : 0.0f;
	}
}

bool Pong::checkCollisions(BoxObject &p_ball, BoxObject &p_paddle)
{
	 m_x_overlap = p_ball.m_bounding_box.x_min < p_paddle.m_bounding_box.x_max	&&
					p_ball.m_bounding_box.x_max > p_paddle.m_bounding_box.x_min;

	 m_y_overlap = p_ball.m_bounding_box.y_min < p_paddle.m_bounding_box.y_max	&&
					p_ball.m_bounding_box.y_max > p_paddle.m_bounding_box.y_min;

	return (m_x_overlap && m_y_overlap);
}

Vector2D Pong::calcNormal(BoxObject &p_ball, BoxObject &p_paddle)
{
	if (m_x_overlap < m_y_overlap) {
		// Collision is more likely on the left/right side
		if (p_ball.m_bounding_box.x_max > p_paddle.m_bounding_box.x_min) {
			// Collision on the left side of Box B (normal points left)
			return {1, 0};
		} else {
			// Collision on the right side of Box B (p_normal points right)
			return {-1, 0};
		}
	} else {
		// Collision is more likely on the top/bottom side
		if (p_ball.m_bounding_box.y_max > p_paddle.m_bounding_box.y_min) {
			// Collision on the bottom side of Box B (p_normal points down)
			return {0, -1};
		} else {
			// Collision on the top side of Box B (p_normal points up)
			return {0, 1};
		}
	}
}

// TODO: Render score
void Pong::update(double delta_time)
{
	// // TODO: Have iterations of point checking, bounding vectors for collisions
	// Vector2D incident = {m_ball.m_velocity.m_x, m_ball.m_velocity.m_y};
	// Vector2D normal;
	// Vector2D new_trajectory;

	m_p1.update(delta_time);
	m_p2.update(delta_time);
	
	for (auto& ball : m_ball)
	{
		ball.update(delta_time);
	}

	
	for(auto it = m_ball.begin(); it != m_ball.end(); /*no incremeent*/)
	{
		if(checkCollisions(*it, m_p1) || checkCollisions(*it, m_p2))
		{
			// TODO: IMPLEMENT MORE SOPHISTICATED COLLISION REACTION

			// normal = calcNormal(ball, m_p1);
			// new_trajectory = reflect(incident, normal);

			// it->m_velocity.m_x = new_trajectory.m_x + 10.0;;
			// it->m_velocity.m_y = new_trajectory.m_y + 10.0;;

			// std::cout << "Old Trajectory: (" << it->m_velocity.m_x << ", " << it->m_velocity.m_y << ")\n";
			// std::cout << "New Trajectory: (" << new_trajectory.m_x << ", " << new_trajectory.m_y << ")\n";
			
			it->m_velocity.m_x = -it->m_velocity.m_x;
		}
		/* turn the ball around if it hits the edge of the screen */
		if ((it->m_position.m_y < 0 || it->m_position.m_y > GameContext::getInstance()->m_screen->h - 10))
		{
			it->m_velocity.m_y = -it->m_velocity.m_y;
		}
		if (it->m_position.m_x > GameContext::getInstance()->m_screen->w - 10)
		{
			score[0]++;
			std::cout << "P1: " << score[0] << " P2: " << score[1] << "\n";

			// it->m_velocity.m_x = -it->m_velocity.m_x;
			// Vector2D pos = {static_cast<float>(SCREEN_WIDTH / 2), (float)random_uniform<int>(20, SCREEN_HEIGHT-20)};
			// reset(pos);	
			it = m_ball.erase(it);
		} else if (it->m_position.m_x < 0)
		{
			score[1]++;
			std::cout << "P1: " << score[0] << " P2: " << score[1] << "\n";
			
			// it->m_velocity.m_x = -it->m_velocity.m_x;
			// Vector2D pos = {static_cast<float>(SCREEN_WIDTH / 2), (float)random_uniform<int>(20, SCREEN_HEIGHT-20)};
			// reset(pos);
			it = m_ball.erase(it);
		} 
		else
		{
			++it;
		}
	}
	if (m_ball.empty())
	{
		std::cout << "Game Over\n";
		std::cout << "P1: " << score[0] << " P2: " << score[1] << "\n";
	}
}

void Pong::draw()
{
	// draw background
	SDL_Rect rect = {0,0,GameContext::getInstance()->m_window->getWidth(),GameContext::getInstance()->m_window->getHeight()};
	SDL_FillSurfaceRect(GameContext::getInstance()->m_screen, &rect, 0x7F3AF9FA);

	m_p1.draw(GameContext::getInstance()->m_screen);
	m_p2.draw(GameContext::getInstance()->m_screen);
	for (auto& ball : m_ball)
	{
		ball.draw(GameContext::getInstance()->m_screen);
	}

    // SDL_SetRenderDrawColor( GameContext::getInstance()->m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(GameContext::getInstance()->m_renderer);
	SDL_UpdateTexture(GameContext::getInstance()->m_texture, nullptr, GameContext::getInstance()->m_screen->pixels, GameContext::getInstance()->m_screen->w * sizeof(uint32_t));
	SDL_RenderTexture(GameContext::getInstance()->m_renderer, GameContext::getInstance()->m_texture, nullptr, nullptr);
	SDL_RenderPresent(GameContext::getInstance()->m_renderer);
}