#include "ball.h"

#include <iostream>
#include <SDL3/SDL.h>
#include "game_context.h"
#include "box_object.h"

Ball::Ball(float p_w, float p_h, float p_dx, float p_dy, unsigned int p_color)
	: BoxObject(0.0f, 0.0f, p_w, p_h, p_dx, p_dy, p_color)
{}

Ball::~Ball()
{
	std::cout << "ball erased\n";
}

void Ball::init(Vector2D &p_position)
{
	m_position = p_position;
}

void Ball::update(float p_delta_time)
{
	m_position.m_x += m_velocity.m_x * p_delta_time;
	m_position.m_y += m_velocity.m_y * p_delta_time;

	m_bounding_box.x_min = m_position.m_x;
	m_bounding_box.x_max = m_position.m_x + m_dimensions.m_x;
	m_bounding_box.y_min = m_position.m_y;
	m_bounding_box.y_max = m_position.m_y + m_dimensions.m_y;
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

