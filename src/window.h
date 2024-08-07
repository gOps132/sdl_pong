#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <SDL3/SDL.h>
#include <string>

class Window {
public:
	Window() : m_width(100), m_height(100), m_title("uninitialized") {}; 
	Window(int p_width, int p_height, std::string p_title);
	~Window();

	SDL_Window *getInstance();
	void updateWindowSize();

	const int getWidth();
	const int getHeight();

private:
	SDL_Window		*m_window_instance;
	std::string 	 m_title;
	int 			 m_width;
	int 			 m_height;
};

#endif // __WINDOW_H__