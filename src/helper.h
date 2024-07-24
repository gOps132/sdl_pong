#ifndef __HELPER_H__
#define __HELPER_H__

#include <SDL3/SDL.h>

uint32_t colorConvert(SDL_Surface* surface, int p_color)
{
    // Extract RGB components
    uint8_t r = (p_color >> 16) & 0xFF;
    uint8_t g = (p_color >> 8) & 0xFF;
    uint8_t b = p_color & 0xFF;

	return SDL_MapSurfaceRGB(surface, r, g, b);
}

#endif // __HELPER_H__