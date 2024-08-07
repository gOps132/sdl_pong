#ifndef __GAME_CONTEXT_H__
#define __GAME_CONTEXT_H__

#include <iostream>
#include <SDL3/SDL.h>

#include "window.h"

// singleton
class GameContext {
private:
	static GameContext *s_instance;
	GameContext();
	GameContext(GameContext const&) = delete;
	GameContext& operator=(GameContext const&) = delete;
public:
	~GameContext();

	inline static GameContext *getInstance() {
		if (!s_instance)
			s_instance = new GameContext();
		return s_instance;
	}

	Window 			*m_window 	= nullptr;
	SDL_Event 		 m_ev;
	SDL_Renderer 	*m_renderer = nullptr;	
	SDL_Surface 	*m_screen	= nullptr;
	SDL_Texture 	*m_texture 	= nullptr;
};

// GameContext *GameContext::s_instance = nullptr;

#endif // __GAME_CONTEXT_H__