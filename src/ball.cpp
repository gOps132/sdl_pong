#include "ball.h"

#include <SDL3/SDL.h>

Ball::Ball(double p_x, double p_y, int p_w, int p_h, int p_dx, int p_dy, int p_color)
	: m_x(p_x), m_y(p_y), m_w(p_w), m_h(p_h), m_dx(p_dx), m_dy(p_dy), m_color(p_color)
{
}

void Ball::update(double p_delta_time)
{
	m_x += m_dx * p_delta_time;
	m_y += m_dy * p_delta_time;
}

void Ball::draw(SDL_Surface *p_surface)
{
	m_rect.x = m_x;
	m_rect.y = m_y;
	m_rect.w = m_w;
	m_rect.h = m_h;

	// render fill rect?
	int r = SDL_FillSurfaceRect(p_surface, &m_rect, m_color);
	if (r != 0)
	{
		throw SDL_GetError();
	}
}
