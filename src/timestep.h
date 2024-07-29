/*
 *	from: https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php
 */ 


#ifndef __TIMESTEP_H__
#define __TIMESTEP_H__

#include <cstdint>

class Timestep
{
public:	
	Timestep();

	void start();
	void stop();
	void pause();
	void unpause();

	uint32_t get_ticks();
	
	bool is_started() const { return m_started; }
	bool is_paused() const { return m_started && m_paused; }
private:
	uint32_t m_start_ticks;
	uint32_t m_paused_ticks;

	bool m_paused;
	bool m_started; 
};

#endif // __TIMESTEP_H__