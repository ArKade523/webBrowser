#include "renderQueue.hpp"
#include "renderHTML.h"

RenderQueue& RenderQueue::getInstance() {
    static RenderQueue instance;
    return instance;
}

void RenderQueue::push(Node* node) {
    renderQueue.push(node);
}

void RenderQueue::populateQueue(Node* root) {
    x = y = 0;
    renderQueue.push(root);
    for (Node* child : root->children) {
        populateQueue(child);
    }
}

void RenderQueue::render() {
    while (!renderQueue.empty()) {
        Node* node = renderQueue.front();
        this->renderNode(node);
        // renderNode()
        renderQueue.pop();
    }
}

RenderQueue::RenderQueue() : fm(FontManager::getInstance()), renderer(g_renderer) {
}

void RenderQueue::renderNode(Node* node) {
    // determine which type of node we have
    ElementNode *elementNode = dynamic_cast<ElementNode*>(node);
    TextNode *textNode = dynamic_cast<TextNode*>(node);
    if (elementNode) {
        #ifndef NDEBUG
            std::cout << "Rendering element node" << std::endl;
        #endif
        // TODO
    } else if (textNode) {
        #ifndef NDEBUG
            std::cout << "Rendering text node at x: " << x << " and y: " << y << std::endl;
            std::cout << "Text: " << textNode->textContent << std::endl;
        #endif

        TTF_Font* font = fm.getFont("Montserrat", 18);
        // Verify that the font is loaded
        if (!font) {
            std::cerr << "Font not loaded" << std::endl;
            return;
        }

        y += TTF_FontHeight(font) + 5;

        // renderText(g_renderer, font, textNode->textContent, x, y);

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, textNode->textContent.c_str(), text_BLACK);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = {100, 100, textSurface->w, textSurface->h};
                SDL_RenderCopy(g_renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}
