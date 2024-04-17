#pragma once
#include <SDL.h>
// #include <SDL_ttf.h>
#include "/usr/local/Cellar/sdl2_ttf/2.22.0/include/SDL2/SDL_ttf.h" // personal laptop
// #include "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2/SDL_ttf.h" // work laptop

static SDL_Window* g_window = nullptr;
static SDL_Renderer* g_renderer = nullptr;
static SDL_Color text_BLACK = {0, 0, 0}; // black