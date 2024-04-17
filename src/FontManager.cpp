#include "FontManager.hpp"

#define NDEBUG

namespace fs = std::filesystem;

// Helper function to resolve the home directory
std::string getHomeDirectory() {
    char* homeDir = getenv("HOME");
    if (homeDir == nullptr) { // Home directory not found
        homeDir = getenv("USERPROFILE"); // Try Windows environment
    }
    return std::string(homeDir ? homeDir : "");
}

// Platform-specific font paths initialization
std::vector<std::string> getFontPaths() {
    std::vector<std::string> paths;
    #ifdef _WIN32
    paths.push_back("C:\\Windows\\Fonts");
    #elif defined(__APPLE__)
    paths.push_back("/Library/Fonts");
    paths.push_back("/System/Library/Fonts");
    paths.push_back(getHomeDirectory() + "/Library/Fonts");
    #else
    paths.push_back("/usr/share/fonts");
    paths.push_back("/usr/local/share/fonts");
    paths.push_back(getHomeDirectory() + "/.fonts");
    #endif
    return paths;
}

FontManager::FontManager() {
    #ifndef NDEBUG
        std::cout << "FontManager constructor called" << std::endl;
    #endif

    fontPaths = getFontPaths();
}

FontManager::~FontManager() {
    #ifndef NDEBUG
        std::cout << "FontManager destructor called" << std::endl;
    #endif

    for (auto& font : fontCache) {
        TTF_CloseFont(font.second);
    }
    fontCache.clear();
}

/**
 * @author Kade Angell <kade.angell@icloud.com>
 * @return FontManager& The FontManager instance
*/
FontManager& FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

/**
 * @author Kade Angell <kade.angell@icloud.com>
 * @param fontName The name of the font to load
 * @param size The size of the font to load
 * @param style The style of the font to load
 * @return TTF_Font* The loaded font
 * This function attempts to search the system for the specified font and load it.
 * If the typeface is not found, it will return a default font (Helvetica)
*/
TTF_Font* FontManager::getFont(const std::string& fontName, int size, FontStyle style) {
    #ifndef NDEBUG
        std::cout << "FontManager getFont called, looking for " << fontName << std::endl;
    #endif

    auto key = std::make_tuple(fontName + getStyleString(style), style, size);
    if (fontCache.find(key) != fontCache.end()) {
        return fontCache[key];
    }

    #ifndef NDEBUG
        std::cout << "Font not found in cache" << std::endl;
    #endif

    for (const auto& baseDir : fontPaths) {
        for (const auto& entry : fs::recursive_directory_iterator(baseDir)) {
            #ifndef NDEBUG
                std::cout << "Checking file: " << entry.path() << std::endl;
            #endif
            if (entry.path().filename().string().find(fontName) != std::string::npos &&
                (entry.path().extension() == ".ttf" || entry.path().extension() == ".ttc")) {
                #ifndef NDEBUG
                    std::cout << "Font found: " << entry.path() << std::endl;
                #endif

                // Found a TrueTypeCollection font, try to load it
                if (entry.path().extension() == ".ttc") {
                    try {
                        // Determine the index for the style specified
                        int index = findFontStyleIndex(entry.path().string().c_str(), getStyleString(style).c_str());

                        TTF_Font* font = loadFont(entry.path().string(), size, index);
                        if (font) {
                            fontCache[key] = font;
                            return font;
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "Error loading font: " << e.what() << std::endl;
                    }
                }

                #ifndef NDEBUG
                    std::cout << "Found .ttf font: " << entry.path() << std::endl;
                #endif

                // Found a TrueType font, try to load it
                try {

                    TTF_Font* font = loadFont(entry.path().string(), size);
                    // Apply styles if necessary
                    if (style == FontStyle::BOLD) {
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
                    } else if (style == FontStyle::ITALIC) {
                        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
                    } else if (style == FontStyle::BOLD_ITALIC) {
                        TTF_SetFontStyle(font, TTF_STYLE_BOLD | TTF_STYLE_ITALIC);
                    } else if (style == FontStyle::REGULAR) {
                        TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
                    }
                    if (font) {
                        fontCache[key] = font;
                        return font;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error loading font: " << e.what() << std::endl;
                }
            }
        }
    }
    static bool fontNotFound = false;
    if (!fontNotFound) {
        std::cerr << "Font not found: " << fontName << std::endl;
        std::cerr << "Using default font" << std::endl;
        fontNotFound = true;
    }

    // Return a default font if the font is not found
    return getFont("Helvetica", size, style);

    return nullptr;
}

TTF_Font* FontManager::loadFont(const std::string& fontPath, int size, int index) {
    #ifndef NDEBUG
        std::cout << "FontManager loadFont called" << std::endl;
    #endif

    if (!fs::exists(fontPath)) {
        throw std::runtime_error("Font file does not exist: " + fontPath);
    }

    TTF_Font* font = TTF_OpenFontIndex(fontPath.c_str(), size, index);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return nullptr;
    }
    return font;
}

std::string FontManager::getStyleString(FontStyle style) {
    switch (style) {
        case FontStyle::BLACK: return "Black";
        case FontStyle::BLACK_ITALIC: return "BlackItalic";
        case FontStyle::BOLD: return "Bold";
        case FontStyle::BOLD_ITALIC: return "BoldItalic";
        case FontStyle::ITALIC: return "Italic";
        case FontStyle::LIGHT: return "Light";
        case FontStyle::LIGHT_ITALIC: return "LightItalic";
        case FontStyle::MEDIUM: return "Medium";
        case FontStyle::MEDIUM_ITALIC: return "MediumItalic";
        case FontStyle::REGULAR: return "Regular";
        case FontStyle::THIN: return "Thin";
        case FontStyle::THIN_ITALIC: return "ThinItalic";
        default: return "Regular";
    }
}
