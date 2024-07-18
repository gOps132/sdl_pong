#include "SDL3/SDL.h"

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <chrono>

#define SCREEN_WIDTH 640	//window height
#define SCREEN_HEIGHT 480	//window width

// TODO: REFRACTOR USING THE C_NAMING CONVENTION

class Window {
public:
	Window(
		int p_width = SCREEN_WIDTH,
		int p_height = SCREEN_HEIGHT,
		std::string p_title = "Pong"
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
	
	SDL_Window *getInstance() { return m_window_instance; }
	void updateWindowSize() 
	{
		SDL_GetWindowSize(m_window_instance, &m_width, &m_height);
	}
	const int getWidth() { return m_width; }
	const int getHeight() { return m_height; }

private:
	SDL_Window *m_window_instance;
	std::string m_title;
	int m_width;
	int m_height;
};

typedef struct Paddle { 
	int x,y;
	int width,height;
} Paddle;

typedef struct Ball {
	int x,y;
	int width,height;
	int dx, dy;
} Ball;

// std::unique_ptr<int *> colorConvert(int p_color)
// {
//     // Extract RGB components
//     uint8_t r = (p_color >> 16) & 0xFF;
//     uint8_t g = (p_color >> 8) & 0xFF;
//     uint8_t b = p_color & 0xFF;

//     // Allocate memory for the array
//     std::unique_ptr<int *> color = std::make_unique<int *>(new int[3]{r, g, b});
//     return color;
// }

uint32_t colorConvert(SDL_Surface* surface, int p_color)
{
    // Extract RGB components
    uint8_t r = (p_color >> 16) & 0xFF;
    uint8_t g = (p_color >> 8) & 0xFF;
    uint8_t b = p_color & 0xFF;

	return SDL_MapSurfaceRGB(surface, r, g, b);
}

class Pong {
public:
	Pong() 
	{
		m_window = new Window();

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

		m_screen = SDL_CreateSurface(m_window->getWidth(), m_window->getHeight(), SDL_PIXELFORMAT_RGBA8888);
		// m_screen = SDL_CreateSurface(m_window->getWidth(), m_window->getHeight(), SDL_PIXELFORMAT_RGBA32);
		if (!m_screen)
		{
			throw SDL_GetError();
		}

		initGame();
	}
	~Pong()
	{
		SDL_DestroySurface(m_screen);
		SDL_DestroyRenderer(m_renderer);
		delete m_window;
		SDL_Quit(); 
	};

	void initGame()
	{
		// center ball
		m_ball.x = m_screen->w/2; 
		m_ball.y = m_screen->h/2;
		m_ball.width = 10;
		m_ball.height = 10;
		m_ball.dy = 1;
		m_ball.dx = 1;

		m_paddle[0].x = 20;
		m_paddle[0].y = (m_screen->h/2)-50;
		m_paddle[0].width = 10;
		m_paddle[0].height = 50;
		
		m_paddle[1].x = m_screen->w - 20 - 10;
		m_paddle[1].y = (m_screen->h/2)-50;
		m_paddle[1].width = 10;
		m_paddle[1].height = 50;
		
		m_white_color = colorConvert(m_screen, 0xfffffff);
	}

	/**
	 *	poll events
	 *	check for  
	 *	Update ball position
	 */
	void gameLoop()
	{
#ifndef DEBUG
		auto start_time = std::chrono::steady_clock::now();
#endif
		while (m_interrupt)
		{
			update();
#ifdef DEBUG
			auto now = std::chrono::steady_clock::now();
    		auto time = std::chrono::duration_cast<std::chrono::seconds>(now-start_time);

			m_time_elapsed = time.count();
			std::cout << "time elapsed: " << m_time_elapsed << "\n";
#endif
			// 	frame rate
			//	time it takes to render  frame in milliseconds
			next_game_tick += 1000 / 60;
			sleep = next_game_tick - SDL_GetTicks();
		
			if( sleep >= 0 ) {
				SDL_Delay(sleep);
			}
		}
	}

	void drawBall() 
	{
		SDL_Rect src;  

		src.x = m_ball.x;
		src.y = m_ball.y;
		src.w = m_ball.width;
		src.h = m_ball.height;

// TODO: precompile dis
		int r  = SDL_FillSurfaceRect(m_screen, &src, m_white_color);

		if (r !=0){
			throw SDL_GetError();
		}
	}

	void moveBall()
	{
		/* Move the ball by its motion vector. */
		// m_ball.x += m_ball.dx;
		// m_ball.y += m_ball.dy;
	}

	void drawPaddle()
	{
		SDL_Rect src;
		
		for(int i = 0; i < 2; i++)
		{
			src.x = m_paddle[i].x; 
			src.y = m_paddle[i].y; 
			src.w = m_paddle[i].width; 
			src.h = m_paddle[i].height; 

			int r  = SDL_FillSurfaceRect(m_screen, &src, m_white_color);
			
			if (r !=0){
				throw SDL_GetError();
			}
		}
	}

	void movePaddle(const int p_direction, Paddle &p_paddle)
	{
		// paddle moving down
		if (p_direction <= 0)
		{ 
			if (p_paddle.y >= m_screen->h - p_paddle.height)
			{
				// stay
				p_paddle.y = m_screen->h - p_paddle.height;	
			}
			else
			{
				p_paddle.y += 5;
			} 
		}
		// paddle moving up
		if (p_direction > 0)
		{
			if (p_paddle.y <= 0)
			{
				// stay at current position
				p_paddle.y = 0;	
			} 
			else 
			{
				p_paddle.y -= 5;
			} 
		}
	}

	void drawBackground()
	{
		SDL_Rect rect = {0,0,m_window->getWidth(),m_window->getHeight()}; // rectangle covering the entire surface
		SDL_FillSurfaceRect(m_screen, &rect, 0x7F3AF9FA);
	}

// TODO: window resizing event implementation later??
	void update()
	{
		// POLL EVENTS
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
				case SDL_EventType::SDL_EVENT_QUIT:
					m_interrupt = false;
					throw "Application shutdown\n";
					break;
				default:
					break;
			}
			// TODO: PADDLES WONT MOVE INDEPENDENTLY OF EACH OTHER IN A SINGLE THREAD
			switch (ev.key.key)
			{
				// move right paddle down
				case SDLK_DOWN:
					movePaddle(-1, m_paddle[1]);
					std::cout << "MOVE RIGHT PADDLE DOWN\n";
					break;
				case SDLK_UP:
					movePaddle(1, m_paddle[1]);
					std::cout << "MOVE RIGHT PADDLE UP\n";
					break;
				case SDLK_S:
					movePaddle(-1, m_paddle[0]);
					std::cout << "MOVE LEFT PADDLE DOWN\n";
					break;
				case SDLK_W:
					movePaddle(1, m_paddle[0]);
					std::cout << "MOVE LEFT PADDLE UP\n";
					break;
				default:
					break;
			}
		}
		// m_window->updateWindowSize();
		
		// main menu
		// when game starts
			drawBackground();
			drawBall();
			moveBall();
			drawPaddle();
		// end

		SDL_Texture *m_texture = SDL_CreateTextureFromSurface(m_renderer, m_screen);
		if (!m_texture)
		{
			throw SDL_GetError();
		}

		// RENDER
		SDL_RenderClear(m_renderer);
		// TODO: Redundant?
		// SDL_UpdateTexture(m_texture, nullptr, m_screen->pixels, m_screen->w * sizeof(uint32_t));
		SDL_RenderTexture(m_renderer, m_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);  
	}
private:
	Window *m_window = nullptr;
	SDL_Renderer *m_renderer = nullptr;
	SDL_Event ev;

	SDL_Surface *m_screen;

	Paddle m_paddle[2];
	Ball m_ball;
	int score[2] = {0,0};

	bool m_interrupt = true;
	double m_time_elapsed;
	int sleep = 0;
	uint32_t next_game_tick = SDL_GetTicks();

	int m_white_color;
};

int main(int argc, char **argv)
{
		Pong *app = new Pong();
		try {
			app->gameLoop();
		}
		catch (const char* e) {
			std::cout << e;
			delete app;
			return -1;
		}
		delete app;

    return 0;
}