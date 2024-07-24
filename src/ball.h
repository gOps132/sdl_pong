#ifndef __BALL_H__
#define __BALL_H__

#include <SDL3/SDL.h>

class Ball {
public:
	Ball(double p_x, double p_y, int p_w, int p_h, int p_dx, int p_dy, int p_color);
	~Ball() = default;

	void update(double p_delta_time);
	void draw(SDL_Surface *p_surface);
private:
	double m_x,m_y;
	int m_w,m_h;
	int m_dx, m_dy;
	SDL_Rect m_rect;
	int m_color;
};

#endif // __BALL_H__