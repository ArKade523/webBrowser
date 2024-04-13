#pragma once
#include "globals.h"
#include "HTMLDocument.hpp"
#include "FontManager.hpp"
#include <SDL.h>
// #include <SDL_ttf.h>
#include "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2/SDL_ttf.h"
#include <string>

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y);

void renderHeader(SDL_Renderer* renderer, Node* node, int x, int y);

void renderBlock(SDL_Renderer* renderer, int x, int y, int width, int height, const SDL_Color& color);

void renderNode(SDL_Renderer* renderer, Node* node, int x, int y);