#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <SDL3/SDL.h>

class Paddle {
public:
	enum class Type {LEFT = 0,RIGHT};
	enum class Direction {NONE=0, UP, DOWN};

	Paddle(Type p_type, float p_x, float p_y, float p_w, float p_h, float p_dy, int p_color);
	~Paddle() = default;
	
	void handleInput(SDL_Event const &p_event);
	void update(float p_delta_time);
	void draw(SDL_Surface *p_surface);
public:
	float m_x,m_y;
	float m_w,m_h;
	float m_dy;
	int m_color;
	Type m_type;
private:
	SDL_Rect m_rect;
	Direction m_direction;
};

#endif // __PADDLE_H__