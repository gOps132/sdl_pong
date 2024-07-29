#include <cstdint>
#include <SDL3/SDL.h>

#include "timestep.h"


Timestep::Timestep()
	: m_start_ticks(0.0f), m_paused_ticks(0.0f), m_paused(false), m_started(false)
{
	
}

void Timestep::start()
{
	m_started = true;
	m_paused = false;

	// get current clock time
	m_start_ticks = SDL_GetTicks();
	m_paused_ticks = 0;
}

void Timestep::stop()
{
	m_started = false;
	m_paused = false;
	
	// clear timer
	m_start_ticks = 0;
	m_paused_ticks = 0;
}

void Timestep::pause()
{
	// if the timer is runing and isn't already paused
	if (m_started && !m_paused)
	{
		m_paused = true;
		
		//  calculate the paused ticks
		m_paused_ticks = SDL_GetTicks() - m_start_ticks;
	}
}

void Timestep::unpause()
{
	if (m_started && m_paused)
	{
		m_paused = false;
		m_start_ticks = SDL_GetTicks() - m_paused_ticks;
		m_paused_ticks = 0;
	}
}

uint32_t Timestep::get_ticks()
{
	// if timer is running
	if (m_started)
	{
		// if timer is paused
		if (m_paused)
		{
			// return the number of ticks when timer is paused
			return m_paused_ticks;
		}
		else
		{
			// return the current time minus the start time
			return SDL_GetTicks() - m_start_ticks;
		}
	}
}