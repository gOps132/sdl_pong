#include "window.h"

#include <string>

#include <iostream>
#include <exception>

Window::Window(int p_width, int p_height, std::string p_title)
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

Window::~Window() 
{
	try
	{
	SDL_DestroyWindow(m_window_instance);
	} catch (const char* e)
	{
		std::cout << e << "\n";
	}
};

SDL_Window *Window::getInstance() { return m_window_instance; }

void Window::updateWindowSize() 
{
	SDL_GetWindowSize(m_window_instance, &m_width, &m_height);
}

const int Window::getWidth() { return m_width; }
const int Window::getHeight() { return m_height; }