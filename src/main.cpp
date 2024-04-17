#include "HTMLDocument.hpp"
#include "globals.h"
#include "renderHTML.h"
#include "renderQueue.hpp"
#include <iostream>
#include <fstream>

#define NDEBUG

int initWindow();
int initRenderer();
int initFont();
void checkEvents(SDL_Event event, bool& running);
void close();

int main(int32_t argc, char* argv[]) {
    if (initWindow() == EXIT_FAILURE || 
        initRenderer() == EXIT_FAILURE ||
        initFont() == EXIT_FAILURE) {
        close();
        return EXIT_FAILURE;
    }

    std::string htmlPath = "../html/index.html";
    if (argc > 1) {
        htmlPath = argv[1];
    }

    RenderQueue& rq = RenderQueue::getInstance();

    std::ifstream htmlFile(htmlPath);

    std::string html;

    if (htmlFile.is_open()) {
        std::string line;
        while (std::getline(htmlFile, line)) {
            html += line;
        }
        htmlFile.close();
    } else {
        std::cerr << "Unable to open file: " << htmlPath << std::endl;
        close();
        return EXIT_FAILURE;
    }

    HTMLDocument doc(html);

    bool running = true;
    SDL_Event event;

    // Main loop
    while (running) {
        checkEvents(event, running);

        SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255); // white
        SDL_RenderClear(g_renderer);

        // renderText(g_renderer, roboto_font, "Hello, World!", 10, 10);

        // renderNode(g_renderer, doc.getRoot(), 0, 0);

        rq.populateQueue(doc.getRoot());
        rq.render();

        SDL_RenderPresent(g_renderer);
    }

    close();
    return EXIT_SUCCESS;
}

int initWindow() {
    #ifndef NDEBUG
        std::cout << "Initializing Window" << std::endl;
    #endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    g_window = SDL_CreateWindow(
        "Web Browser",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600, // width, height
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!g_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int initRenderer() {
    #ifndef NDEBUG
        std::cout << "Initializing Renderer" << std::endl;
    #endif

    g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!g_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int initFont() {
    #ifndef NDEBUG
        std::cout << "Initializing TTF" << std::endl;
    #endif

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! \nTTF_Error: " << TTF_GetError() << std::endl;
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void checkEvents(SDL_Event event, bool& running) {
    while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_WINDOWEVENT) {
                switch(event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                        // Window is resized
                        break;
                }

            }
        }
}

void close() {
    TTF_Quit();
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}
