#include "SDL3/SDL.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

typedef struct Window { 
	std::string title = "title";
	int width = 1080;
	int height = 720;
} Window;

int main(int argc, char **argv)
{

	Window m_window;

	// initialize sdl subsystems
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
	{ 
		std::cout << "Failed to initailize SDL3 Library SDL_Error: " << SDL_GetError();

		return -1;	
	}
	

	// CREATE WINDOW
	SDL_Window *m_window_instance = SDL_CreateWindow(
		m_window.title.c_str(),
		m_window.width, m_window.height,
		SDL_WINDOW_RESIZABLE
	);

	if (!m_window_instance)
	{
		std::cout << "Failed to create a window! SDL_Error: " <<  SDL_GetError();
		return -1;
	}

	// CREATE RENDERER
	SDL_Renderer *m_renderer = SDL_CreateRenderer(m_window_instance, m_window.title.c_str());
	if (!m_renderer)
	{
		std::cout << "Renderer can't be created! SDL_Error: " << SDL_GetError();
		SDL_DestroyWindow(m_window_instance);
		return -1;
	}

	// MAIN LOOP
	for (bool interrupt = false; !interrupt;)
	{
		// POLL EVENTS
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
			switch (ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					std::cout << "Application shutdown \n";
					interrupt = true;
					break;
			}

		SDL_GetWindowSize(m_window_instance, &m_window.width, &m_window.height);

	    SDL_Surface *m_surface = SDL_CreateSurface(m_window.width, m_window.height, SDL_PIXELFORMAT_ARGB8888);
		if (!m_surface)
		{
			printf("Surface could not be created! SDL_Error: %s\n", SDL_GetError());
			interrupt = true;
			break;
		}

		SDL_Texture *m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
		if (!m_texture)
		{
			std::cout << "Unable to create texture! SDL_Error: " << SDL_GetError();
			interrupt = true;
			break;
		}
		SDL_DestroySurface(m_surface);

		// RENDER
		std::vector<uint32_t> pixels(m_window.width * m_window.height, 0);

		// default color
		std::fill_n(pixels.data(), m_window.width * m_window.height, 0x2ad873);
		
		SDL_UpdateTexture(m_texture, nullptr, pixels.data(), 4 * m_window.width);

		SDL_RenderClear(m_renderer);
		SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);
	}

	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window_instance);
	SDL_Quit();

    return 0;
}