#ifndef __HELPER_H__
#define __HELPER_H__

#include <SDL3/SDL.h>
#include <random>

uint32_t colorConvert(SDL_Surface* surface, int p_color)
{
    // Extract RGB components
    uint8_t r = (p_color >> 16) & 0xFF;
    uint8_t g = (p_color >> 8) & 0xFF;
    uint8_t b = p_color & 0xFF;

	return SDL_MapSurfaceRGB(surface, r, g, b);
}

template<typename T>
T random_uniform(const T p_range_min, const T p_range_max)
{
	std::random_device						rand_dev;
	// std::mt19937							generator(rand_dev());
	std::default_random_engine				generator(rand_dev());
	std::uniform_int_distribution<T> 		distr(p_range_min, p_range_max);

	return distr(generator);
}

#endif // __HELPER_H__