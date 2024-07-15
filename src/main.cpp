#include "SDL3/SDL.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <exception>
#include <memory>

class Window {
public:
	Window(
		int p_width = 1080,
		int p_height = 720,
		std::string p_title = "title"
	)
	: m_width(p_width), m_height(p_height), m_title(p_title)
	{
		// CREATE WINDOW
		m_window_instance = SDL_CreateWindow(
			m_title.c_str(),
			m_width, m_height,
			SDL_WINDOW_RESIZABLE
		);

		if (!m_window_instance)
		{
			throw SDL_GetError();
		}
	};
	~Window() 
	{
		SDL_DestroyWindow(m_window_instance);
	};
	
	SDL_Window *get_instance() { return m_window_instance; }
	void update_window_size() 
	{
		SDL_GetWindowSize(m_window_instance, &m_width, &m_height);
	}
	const int get_width() { return m_width; }
	const int get_height() { return m_height; }

private:
	SDL_Window *m_window_instance;
	std::string m_title;
	int m_width;
	int m_height;
};

class Pong {
public:
	Pong() 
	{
		init();
	}
	~Pong()
	{
		SDL_DestroyRenderer(m_renderer);
		SDL_Quit(); 
		delete m_window;
	};
	void init()
	{
		m_window = new Window();

		// initialize sdl subsystems
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
		{ 
			throw SDL_GetError(); 
		}

		// CREATE RENDERER
		m_renderer = SDL_CreateRenderer(m_window->get_instance(), nullptr);
		if (!m_renderer)
		{
			throw SDL_GetError();
		}
	}
	/**
	 * poll events
	 * check for  
	 * Update ball position
	 */
	void game_loop()
	{
		while (interrupt)
		{
			// POLL EVENTS
			while (SDL_PollEvent(&ev))
				switch (ev.type)
				{
					case SDL_EventType::SDL_EVENT_QUIT:
						throw "Application shutdown\n";
						interrupt = false;
						break;
				}
	
			m_window->update_window_size();

			SDL_Surface *m_surface = SDL_CreateSurface(m_window->get_width(), m_window->get_height(), SDL_PIXELFORMAT_RGBA8888);
			if (!m_surface)
			{
				throw SDL_GetError();
			}

			// Define the color 0x2ad873
			uint32_t color = 0x2ad873;

			// Extract RGB components
			uint8_t r = (color >> 16) & 0xFF;
			uint8_t g = (color >> 8) & 0xFF;
			uint8_t b = color & 0xFF;
		
			// map the color to the pixel format of the surface
			uint32_t pixel_color = SDL_MapSurfaceRGB(m_surface, r, g, b);

			// std::vector<uint32_t> pixels(m_window.width * m_window.height, 0);
			// std::fill_n(pixels.data(), m_window.width * m_window.height, pixel_color);

			SDL_Rect rect = {0,0,m_window->get_width(),m_window->get_height()}; // rectangle covering the entire surface
			SDL_FillSurfaceRect(m_surface, &rect, pixel_color);

			SDL_Texture *m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
			if (!m_texture)
			{
				throw SDL_GetError();
			}
			SDL_DestroySurface(m_surface);

			// RENDER

			// SDL_UpdateTexture(m_texture, nullptr, pixels.data(), 4 * m_window.width);
			SDL_RenderClear(m_renderer);
			SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
			SDL_RenderPresent(m_renderer);
		}
	}
private:
	Window *m_window = nullptr;
	SDL_Renderer *m_renderer = nullptr;
	SDL_Event ev;
	bool interrupt = true;
};

int main(int argc, char **argv)
{
		Pong *app = new Pong();
		try {
			app->game_loop();
		}
		catch (const char* e) {
			std::cout << e;
			delete app;
			return -1;
		}
		delete app;

    return 0;
}