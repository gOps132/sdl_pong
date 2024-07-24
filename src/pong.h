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

class Pong {
public:
	Pong();
	~Pong();
	void gameLoop();

	void update(double delta_time);
	void draw();

private:
	GameContext		 *s_gc;
	Paddle 			 m_p1;
	Paddle 			 m_p2;
	Ball 			 m_ball;
	
	int score[2] = {0,0};
	bool m_interrupt = true;
};
#endif // __PONG_H__