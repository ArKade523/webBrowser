#pragma once
#include <queue>
#include "HTMLDocument.hpp"
#include "FontManager.hpp"
#include "globals.h"

class RenderQueue {
public:
    static RenderQueue& getInstance();
    void push(Node* node);
    void populateQueue(Node* root);
    void render();
    bool isEmpty() { return renderQueue.empty(); }
    Node* front() { return renderQueue.front(); }

    // delete copy constructor
    RenderQueue(const RenderQueue&) = delete;

private:
    RenderQueue();
    void renderNode(Node* node);
    int x = 0, y = 0;
    std::queue<Node*> renderQueue;
    SDL_Renderer *renderer;
    FontManager& fm;
};