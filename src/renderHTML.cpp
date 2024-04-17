#include "renderHTML.h"

void renderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    // Verify that the font is loaded
    if (!font) {
        std::cerr << "Font not loaded" << std::endl;
        return;
    }

    // Set font hinting
    TTF_SetFontHinting(font, TTF_HINTING_NONE);

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

void renderHeader(SDL_Renderer* renderer, ElementNode* node, int x, int y) {
    if (!node) {
        return;
    }

    const int headerSize = 24 + 6 - 4 * (node->elementType - ElementType::H1);

    FontManager& font_manager = FontManager::getInstance();

    TTF_Font* font = font_manager.getFont("Helvetica", headerSize, FontStyle::BOLD);

    for (Node* child : node->children) {
        // check if the child is a text node
        TextNode* textNode = dynamic_cast<TextNode*>(child);
        if (textNode) {
            renderText(renderer, font, textNode->textContent, x, y);
        }
    }
}

void renderBlock(SDL_Renderer* renderer, int x, int y, int width, int height, const SDL_Color& color) {
    SDL_Rect blockRect = {x, y, width, height};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &blockRect);
}

void renderNode(SDL_Renderer* renderer, Node* node) {
    static int x = 0;
    static int y = 0;
    FontManager& fm = FontManager::getInstance();

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
            std::cout << "Rendering text node" << std::endl;
            std::cout << "Text: " << textNode->textContent << std::endl;
        #endif

        TTF_Font* font = fm.getFont("Montserrat", 18);
        // Verify that the font is loaded
        if (!font) {
            std::cerr << "Font not loaded" << std::endl;
            return;
        }

        y += TTF_FontHeight(font) + 5;

        renderText(g_renderer, font, textNode->textContent, x, y);
    }
}

// void renderNode(SDL_Renderer* renderer, Node* node, int x, int y) {
//     if (!node) {
//         return;
//     }

//     FontManager& font_manager = FontManager::getInstance();

//     // Only render elements that are children of the body
//     if (node->elementType == ElementType::HEAD) {
//         for (Node* child : node->children) {
//             // check if the node is an element node
//             ElementNode* elementNode = dynamic_cast<ElementNode*>(child);
//             if (elementNode) {
//                 renderNode(renderer, elementNode, x, y);
//             }
//         }
//         return;
//     }

//     TTF_Font* font = font_manager.getFont("Montserrat", 18);

//     if (node->elementType == ElementType::H1 || 
//         node->elementType == ElementType::H2 || 
//         node->elementType == ElementType::H3 || 
//         node->elementType == ElementType::H4 || 
//         node->elementType == ElementType::H5 || 
//         node->elementType == ElementType::H6) {
//         renderHeader(renderer, node, x, y);
//     } else if (
//         node->elementType == ElementType::META || 
//         node->elementType == ElementType::LINK || 
//         node->elementType == ElementType::SCRIPT || 
//         node->elementType == ElementType::STYLE || 
//         node->elementType == ElementType::TITLE) {
//         // Do nothing
//     } else {
//         for (Node* child : node->children) {
//             // check if the child is a text node
//             TextNode* textNode = dynamic_cast<TextNode*>(child);
//             if (textNode) {
//                 renderText(renderer, font, textNode->textContent, x, y);
//             }
//         }
//     }

//     int childY = y + TTF_FontHeight(font) + 5;

//     for (Node* child : node->children) {
//         // check if the child is an element node
//         ElementNode* elementNode = dynamic_cast<ElementNode*>(child);
//         if (elementNode) {
//             renderNode(renderer, elementNode, x, childY);
//             childY += 30;
//         }
//     }
// }
