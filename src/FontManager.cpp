#include "FontManager.hpp"

#define NDEBUG

FontManager::FontManager() {
    #ifndef NDEBUG
        std::cout << "FontManager constructor called" << std::endl;
    #endif
}

FontManager::~FontManager() {
    #ifndef NDEBUG
        std::cout << "FontManager destructor called" << std::endl;
    #endif

    for (auto& font : fonts) {
        TTF_CloseFont(font.second);
    }
    fonts.clear();
}

FontManager& FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

void FontManager::initialize(const std::string& fontsPath) {
    #ifndef NDEBUG
        std::cout << "FontManager initialized" << std::endl;
        std::cout << "Passed with fonts path: " << fontsPath << std::endl;
    #endif

    static bool initialized = false;

    if (!initialized) {
        this->fontsPath = fontsPath;
        initialized = true;
    }

    #ifndef NDEBUG
        std::cout << "FontManager fonts path: " << this->fontsPath << std::endl;
    #endif
}

TTF_Font* FontManager::getFont(const std::string& fontName, int size, FontStyle style) {
    #ifndef NDEBUG
        std::cout << "FontManager getFont called" << std::endl;
    #endif

    // if (!initialized) {
    //     std::cerr << "FontManager not initialized!" << std::endl;
    //     return nullptr;
    // }

    std::pair<std::string, FontStyle> key = {fontName + std::to_string(size), style};
    if (fonts.find(key) == fonts.end()) {
        std::string styleStr;
        switch (style) {
            case FontStyle::BLACK:
                styleStr = "Black";
                break;
            case FontStyle::BLACK_ITALIC:
                styleStr = "BlackItalic";
                break;
            case FontStyle::BOLD:
                styleStr = "Bold";
                break;
            case FontStyle::BOLD_ITALIC:
                styleStr = "BoldItalic";
                break;
            case FontStyle::ITALIC:
                styleStr = "Italic";
                break;
            case FontStyle::LIGHT:
                styleStr = "Light";
                break;
            case FontStyle::LIGHT_ITALIC:
                styleStr = "LightItalic";
                break;
            case FontStyle::MEDIUM:
                styleStr = "Medium";
                break;
            case FontStyle::MEDIUM_ITALIC:
                styleStr = "MediumItalic";
                break;
            case FontStyle::REGULAR:
                styleStr = "Regular";
                break;
            case FontStyle::THIN:
                styleStr = "Thin";
                break;
            case FontStyle::THIN_ITALIC:
                styleStr = "ThinItalic";
                break;
            default:
                styleStr = "Regular";
                break;
        }

        std::string fontPath = this->fontsPath + "/" + fontName + "/" + fontName + "-" + styleStr + ".ttf";
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), size);
        if (!font) {
            std::cerr << "Font could not be loaded! TTF_Error: " << TTF_GetError() << std::endl;
            return nullptr;
        }
        fonts[key] = font;
    }

    return fonts[key];
}
