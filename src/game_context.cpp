#include <iostream>

#include "game_context.h"
#include "window.h"

#define SCREEN_WIDTH 640	//window height
#define SCREEN_HEIGHT 480	//window width

GameContext::GameContext()
{
	std::cout << "instance created" << "\n";

	m_window = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "pong");

	// initialize sdl subsystems
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{
		throw SDL_GetError(); 
	}

	// CREATE RENDERER
	m_renderer = SDL_CreateRenderer(m_window->getInstance(), nullptr);
	if (!m_renderer)
	{
		throw SDL_GetError();
	}

	// CREATE SCREEN TO DRAW
	m_screen = SDL_CreateSurface(m_window->getWidth(), m_window->getHeight(), SDL_PIXELFORMAT_RGBA8888);
	// m_screen = SDL_CreateSurface(m_window->getWidth(), m_window->getHeight(), SDL_PIXELFORMAT_RGBA32);
	if (!m_screen)
	{
		throw SDL_GetError();
	}

	// prolly inefficent to recreate a texture every update
	// TODO: change to update texture later
	m_texture = SDL_CreateTextureFromSurface(m_renderer, m_screen);
	if (!m_texture)
	{
		throw SDL_GetError;
	}
}

GameContext::~GameContext()
{
	delete m_window;
	delete m_texture;
	SDL_DestroySurface(m_screen);
	SDL_DestroyRenderer(m_renderer);
}

