#include "renderHTML.h"

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), text_BLACK);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}

void renderHeader(SDL_Renderer* renderer, Node* node, int x, int y) {
    if (!node) {
        return;
    }

    const int headerSize = 24 + 6 - 4 * (node->type - ElementType::H1);

    FontManager& font_manager = FontManager::getInstance();

    TTF_Font* font = font_manager.getFont("Roboto", headerSize, FontStyle::BOLD);

    renderText(renderer, font, node->textContent, x, y);
}

void renderBlock(SDL_Renderer* renderer, int x, int y, int width, int height, const SDL_Color& color) {
    SDL_Rect blockRect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &blockRect);
}

void renderNode(SDL_Renderer* renderer, Node* node, int x, int y) {
    if (!node) {
        return;
    }

    FontManager& font_manager = FontManager::getInstance();

    TTF_Font* font = font_manager.getFont("Roboto", 18);
    if (node->type == ElementType::H1 || 
        node->type == ElementType::H2 || 
        node->type == ElementType::H3 || 
        node->type == ElementType::H4 || 
        node->type == ElementType::H5 || 
        node->type == ElementType::H6) {
        renderHeader(renderer, node, x, y);
    } else if (
        node->type == ElementType::META || 
        node->type == ElementType::LINK || 
        node->type == ElementType::SCRIPT || 
        node->type == ElementType::STYLE || 
        node->type == ElementType::TITLE) {
        // Do nothing
    } else {
        renderText(renderer, font, node->textContent, x, y);    
    }

    int childY = y + TTF_FontHeight(font) + 5;

    for (Node* child : node->children) {
        renderNode(renderer, child, x, childY);
        childY += 30;
    }
}
