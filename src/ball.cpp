#include "ball.h"

#include <SDL3/SDL.h>
#include "game_context.h"

Ball::Ball(float p_w, float p_h, float p_dx, float p_dy, int p_color)
	: m_w(p_w), m_h(p_h), m_dx(p_dx), m_dy(p_dy), m_color(p_color)
{
}

void Ball::init(float p_x, float p_y)
{
	m_x = p_x;
	m_y = p_y;
}

void Ball::checkCollision(Paddle &p_paddle)
{
	// define collision boxes
	float left_a, left_b;
	float right_a, right_b;
	float top_a, top_b;
	float bottom_a, bottom_b;
	
	left_a = m_x;
	right_a = m_x + m_w;
	top_a = m_y;
	bottom_a = m_y + m_h;

	left_b = p_paddle.m_x;
	right_b = p_paddle.m_x + p_paddle.m_w;
	top_b = p_paddle.m_y;
	bottom_b = p_paddle.m_y + p_paddle.m_h;
	
	if (left_a > right_b)
	{
		m_in_collision = false;
	}
	
	if (right_a > left_b)
	{
		m_in_collision = false;
	}

	if (top_a > bottom_b)
	{
		m_in_collision = false;
	}
	
	if (bottom_a > top_b)
	{
		m_in_collision = false;
	}
	
	m_in_collision = true;
}

// TODO: collision detection, boundary checking, getting position of
// the paddles
void Ball::update(float p_delta_time)
{
	// std::cout << "is colliding: " << m_in_collision << "\n";

	/* move the ball according to its motion vector */
	m_x += m_dx * p_delta_time;
	m_y += m_dy * p_delta_time;

	// std::cout << m_x << ", " << m_y << "\n";
	// std::cout << p_delta_time << "\n";

	/* turn the ball around if it hits the edge of the screen */
	if (m_x < 0)
	{
		// score
		m_dx = -m_dx;
	}
	if (m_x > GameContext::getInstance().m_screen->w - 10)
	{
		m_dx = -m_dx;
	}
	if ((m_y < 0 || m_y > GameContext::getInstance().m_screen->h - 10))
	{
		m_dy = -m_dy;
	}

	/* check the collision of the paddle*/
	
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
