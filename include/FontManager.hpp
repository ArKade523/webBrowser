#pragma once
#include <SDL.h>
// #include <SDL_ttf.h>
#include "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2/SDL_ttf.h"
#include <string>
#include <map>
#include <iostream>

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
    void initialize(const std::string& fontsPath);
    FontManager();
    ~FontManager();

private:
    std::string fontsPath;
    std::map<std::pair<std::string, FontStyle>, TTF_Font*> fonts;
    bool initialized = false;
};