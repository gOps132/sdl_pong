#include "pong.h"

#include <SDL3/SDL.h> 

#include "game_context.h"
#include "window.h"

#define SCREEN_WIDTH 640	//window height
#define SCREEN_HEIGHT 480	//window width

Pong::Pong()
	: 	m_p1(
			Paddle::Type::LEFT,
			20.0,
			static_cast<double>(SCREEN_HEIGHT / 2) - 50,
			10, 50, 0xffffffff
		),
		m_p2(
			Paddle::Type::RIGHT, 
			// 20.0,20.0,
			static_cast<double>(SCREEN_WIDTH) - 20 - 10,
			static_cast<double>((SCREEN_WIDTH / 2) - 50),
			10, 50, 0xffffffff
		),
		m_ball(
			static_cast<double>(SCREEN_WIDTH / 2), static_cast<double>(SCREEN_HEIGHT / 2),
			10, 10, 1, 1, 0xffffffff
		)
{
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
end:
	while (m_interrupt)
	{
		while (SDL_PollEvent(&s_gc->m_ev))
		{
			switch (s_gc->m_ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					m_interrupt = false;
					goto end;
					// break;
				default:
					break;
			}
			// std::cout << "poll pong\n";
			m_p1.handleInput(s_gc->m_ev);
			m_p2.handleInput(s_gc->m_ev);
		}
		update(1.0/60.0);
		draw();
	}
}

void Pong::update(double delta_time)
{
	m_ball.update(delta_time);
	m_p1.update(delta_time);
	m_p2.update(delta_time);
}

void Pong::draw()
{
	// draw background
	SDL_Rect rect = {0,0,s_gc->m_window->getWidth(),s_gc->m_window->getHeight()};
	SDL_FillSurfaceRect(s_gc->m_screen, &rect, 0x7F3AF9FA);

	m_ball.draw(s_gc->m_screen);
	m_p1.draw(s_gc->m_screen);
	m_p2.draw(s_gc->m_screen);

	SDL_RenderClear(s_gc->m_renderer);
	SDL_UpdateTexture(s_gc->m_texture, nullptr, s_gc->m_screen->pixels, s_gc->m_screen->w * sizeof(uint32_t));
	SDL_RenderTexture(s_gc->m_renderer, s_gc->m_texture, nullptr, nullptr);
	SDL_RenderPresent(s_gc->m_renderer);
}