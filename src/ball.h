#ifndef __BALL_H__
#define __BALL_H__

#include <SDL3/SDL.h>
#include "paddle.h"
#include "vector2d.h"
#include "box_object.h"

class Ball : public BoxObject {
public:
	Ball(float p_w, float p_h, float p_dx, float p_dy, unsigned int p_color);
	~Ball();
 
	void Ball::init(Vector2D &p_position);

	virtual void handleInput(SDL_Event const &p_event) override {};
	virtual void update(float p_delta_time) override;
	virtual void draw(SDL_Surface *p_surface) override;
};

#endif // __BALL_H__