#ifndef __BALL_H__
#define __BALL_H__

#include <SDL3/SDL.h>
#include "paddle.h"
#include "vector2d.h"

class Ball {
public:
	Ball(float p_w, float p_h, float p_dx, float p_dy, int p_color);
	~Ball() = default;
 
	void Ball::init(Vector2D &p_position);
	void update(float p_delta_time);
	void draw(SDL_Surface *p_surface);
public:
	Vector2D m_position;
	Vector2D m_dimensions;
	Vector2D m_velocity;

	int m_color;
private:
	SDL_Rect m_rect;
};

#endif // __BALL_H__