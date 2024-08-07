#include "ball.h"

#include <SDL3/SDL.h>
#include "game_context.h"

Ball::Ball(float p_w, float p_h, float p_dx, float p_dy, int p_color)
	: m_dimensions{p_w, p_h}, m_velocity{p_dx, p_dy}, m_color(p_color)
{}

void Ball::init(Vector2D &p_position)
{
	m_position = p_position;
}

void Ball::update(float p_delta_time)
{
	m_position.m_x += m_velocity.m_x * p_delta_time;
	m_position.m_y += m_velocity.m_y * p_delta_time;
}

void Ball::draw(SDL_Surface *p_surface)
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
