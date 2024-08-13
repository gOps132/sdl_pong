#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <SDL3/SDL.h>
#include "vector2d.h"
#include "box_object.h"

class Paddle : public BoxObject {
public:
	enum class Type {LEFT = 0,RIGHT};
	enum class Direction {NONE=0, UP, DOWN};

	Paddle(Type p_type, float p_x, float p_y, float p_dx, float p_dy, float p_w, float p_h, unsigned int p_color);
	~Paddle() = default;
	
	virtual void handleInput(SDL_Event const &p_event) override;
	virtual void update(float p_delta_time) override;
	virtual void draw(SDL_Surface *p_surface) override;
private:
	Type m_type;
	Direction m_direction;
};

#endif // __PADDLE_H__