#ifndef __PONG_H__
#define __PONG_H__

#include <SDL3/SDL.h>

#include <iostream>
#include <string>
#include <vector>

#include "game_context.h"

#include "Window.h"
#include "Paddle.h"
#include "ball.h"
#include "vector2d.h"
#include "box_object.h"

#define SCREEN_WIDTH 	640
#define SCREEN_HEIGHT 	480
#define FPS				60

class Pong {
public:
	Pong();
	~Pong();

	void gameLoop();
	void reset(Vector2D &p_position);
	bool checkCollisions(BoxObject &p_ball, BoxObject &p_paddle);
	Vector2D calcNormal(BoxObject &p_ball, BoxObject &p_paddle);

	void update(double delta_time);
	void draw();

private:
	Paddle 			 m_p1;
	Paddle 			 m_p2;
	Ball 			 m_ball;

	float	 m_x_overlap;
	float	 m_y_overlap;

	float m_iv = 130.0f;
	float m_last_frame_time = 1.0f;
	int score[2] = {0,0};
	bool m_interrupt = true;
};

#endif // __PONG_H__