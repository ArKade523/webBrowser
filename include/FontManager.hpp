#pragma once
#include <SDL.h>
// #include <SDL_ttf.h>
#include "/usr/local/Cellar/sdl2_ttf/2.22.0/include/SDL2/SDL_ttf.h"
#include <string>
#include <map>
#include <iostream>
#include <filesystem>
#include <cstdlib> // for getenv()
#include <fstream>
#include "Font_TTCParsing.h"

enum class FontStyle {
    BLACK,
    BLACK_ITALIC,
    BOLD,
    BOLD_ITALIC,
    ITALIC,
    LIGHT,
    LIGHT_ITALIC,
    MEDIUM,
    MEDIUM_ITALIC,
    REGULAR,
    THIN,
    THIN_ITALIC
};

class FontManager {
public:
    // Delete copy constructor 
    FontManager(const FontManager&) = delete;

    TTF_Font* getFont(const std::string& fontName, int size, FontStyle style = FontStyle::REGULAR);
    static FontManager& getInstance();
    ~FontManager();

private:
    std::vector<std::string> fontPaths;
    std::map<std::tuple<std::string, FontStyle, int>, TTF_Font*> fontCache;

    FontManager();
    TTF_Font* loadFont(const std::string& fontPath, int size, int index = 0);
    std::string getStyleString(FontStyle style);
};
