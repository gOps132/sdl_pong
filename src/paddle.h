#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <SDL3/SDL.h>

class Paddle {
public:
	enum class Type {LEFT = 0,RIGHT};
	enum class Direction {NONE=0, UP, DOWN};

	Paddle(Type p_type, double p_x, double p_y, int p_w, int p_h, int p_color);
	~Paddle() = default;

	void handleInput(SDL_Event const &p_event);
	void update(double p_delta_time);
	void draw(SDL_Surface *p_surface);
private:
	double m_x,m_y;
	int m_w,m_h;
	Type m_type;
	SDL_Rect m_rect;
	int m_color;
	Direction m_direction;
};

#endif // __PADDLE_H__