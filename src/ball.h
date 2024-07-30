#ifndef __BALL_H__
#define __BALL_H__

#include <SDL3/SDL.h>
#include "paddle.h"

class Ball {
public:
	Ball(float p_w, float p_h, float p_dx, float p_dy, int p_color);
	~Ball() = default;
 
	void init(float p_x, float p_y);
	void update(float p_delta_time);
	void draw(SDL_Surface *p_surface);
public:
	float m_x,m_y;
	float m_w,m_h;
	float m_dx, m_dy;
	int m_color;
private:
	SDL_Rect m_rect;
};

#endif // __BALL_H__