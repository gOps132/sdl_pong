
#include <iostream>

#include "paddle.h"
#include "game_context.h"

Paddle::Paddle(Paddle::Type p_type, float p_x, float p_y, float p_w, float p_h, float p_dy, int p_color)
	: m_type(p_type), m_position{p_x, p_y}, m_dimensions{p_w, p_h}, m_velocity{0.0f, p_dy}, m_color(p_color)
{
	m_direction = Direction::NONE;
}

void Paddle::handleInput(SDL_Event const &p_event)
{
	switch(p_event.type)
	{
		case SDL_EventType::SDL_EVENT_KEY_DOWN:
			// std::cout << p_event.key.key  << "\n";
            if(m_type == Type::LEFT)
            {
				switch (p_event.key.key)
				{
					case SDLK_W:
						m_direction = Direction::UP;
						break;
					case SDLK_S:
						m_direction = Direction::DOWN;
						break;
					default:
						break;
				}
            } else if(m_type == Type::RIGHT)
            {
				// std::cout << "key down" << "\n";
				switch (p_event.key.key)
				{
					case SDLK_UP:
						m_direction = Direction::UP;
						break;
					case SDLK_DOWN:
						m_direction = Direction::DOWN;
						break;
					default:
						break;
				}
			}
			break;
		case SDL_EventType::SDL_EVENT_KEY_UP:
			m_direction = Direction::NONE;
			break;
	}
}

// move at specified direction at percentage of delta time
void Paddle::update(float p_delta_time)
{
	if(m_direction == Direction::UP)
    {
        if(m_position.m_y >= 0)
        {
            m_position.m_y -= m_velocity.m_y * p_delta_time;
        }
    }
    else if(m_direction == Direction::DOWN)
    {
        if(m_position.m_y <= GameContext::getInstance().m_screen->h - m_dimensions.m_y)
        {
            m_position.m_y += m_velocity.m_y * p_delta_time;
        }
    }
}

void Paddle::draw(SDL_Surface *p_surface)
{
	m_rect.x = m_position.m_x;
	m_rect.y = m_position.m_y;
	m_rect.w = m_dimensions.m_x;
	m_rect.h = m_dimensions.m_y;

	// render fill rect?
	int r = SDL_FillSurfaceRect(p_surface, &m_rect, m_color);
	if (r != 0)
	{
		throw SDL_GetError();
	}
}