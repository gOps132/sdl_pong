#ifndef __BOX_OBJECT_H__
#define __BOX_OBJECT_H__

#include <SDL3/SDL.h>
#include "vector2d.h"

class AABB {
public:
	AABB() : x_min(0.0f), y_min(0.0f), x_max(0.0f), y_max(0.0f) {}	

	float x_min, y_min; // bottom left
	float x_max, y_max; // top right
};

class BoxObject
{
public:
	BoxObject(float p_x, float p_y, float p_w, float p_h, float p_dx, float p_dy, unsigned int p_color) 
		: m_position{p_x, p_y}, m_dimensions{p_w, p_h}, m_velocity{p_dx, p_dy}, m_color(p_color) {};
	virtual ~BoxObject() {};
	virtual void handleInput(SDL_Event const &p_event) = 0;
	virtual void update(float p_delta_time) = 0;
	virtual void draw(SDL_Surface *p_surface) = 0;
public:
	AABB 	 m_bounding_box;
	Vector2D m_position;
	Vector2D m_dimensions;
	Vector2D m_velocity;
protected:
	unsigned int m_color;
	SDL_Rect m_rect;
};

#endif // __BOX_OBJECT_H__