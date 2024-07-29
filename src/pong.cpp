#include "pong.h"

#include <SDL3/SDL.h> 
#include <cmath>

#include "game_context.h"
#include "window.h"
#include "timestep.h"

#define SCREEN_WIDTH 	640
#define SCREEN_HEIGHT 	480
#define FPS				60

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
			static_cast<float>((SCREEN_WIDTH / 2) - 50),
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
	reset();
	s_gc = &GameContext::getInstance();
}

Pong::~Pong()
{
	std::cout << "delete pong\n";
	delete s_gc->m_window;
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
end:
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
		while (SDL_PollEvent(&s_gc->m_ev))
		{
			switch (s_gc->m_ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					m_interrupt = false;
					goto end;
				default:
					break;
			}
			m_p1.handleInput(s_gc->m_ev);
			m_p2.handleInput(s_gc->m_ev);
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

		std::cout << "delta time: " << delta_time << "\n";

		// Do rendering loop
		update(delta_time);
		draw();

		++counted_frames;
	}
}

void Pong::reset()
{
	// center ball
	m_ball.init(static_cast<float>(SCREEN_WIDTH / 2), static_cast<float>(SCREEN_HEIGHT / 2));
}

void Pong::update(double delta_time)
{
	m_p1.update(delta_time);
	m_p2.update(delta_time);
	m_ball.checkCollision(m_p1);
	m_ball.checkCollision(m_p2);
	m_ball.update(delta_time);
}

void Pong::draw()
{
	// draw background
	SDL_Rect rect = {0,0,s_gc->m_window->getWidth(),s_gc->m_window->getHeight()};
	SDL_FillSurfaceRect(s_gc->m_screen, &rect, 0x7F3AF9FA);

	m_ball.draw(s_gc->m_screen);
	m_p1.draw(s_gc->m_screen);
	m_p2.draw(s_gc->m_screen);

    // SDL_SetRenderDrawColor( s_gc->m_renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(s_gc->m_renderer);
	SDL_UpdateTexture(s_gc->m_texture, nullptr, s_gc->m_screen->pixels, s_gc->m_screen->w * sizeof(uint32_t));
	SDL_RenderTexture(s_gc->m_renderer, s_gc->m_texture, nullptr, nullptr);
	SDL_RenderPresent(s_gc->m_renderer);
}