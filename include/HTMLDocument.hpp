#pragma once
#include <string>
#include <vector>
#include "HTMLElementTypes.h"

struct Node {
    ElementType type;
    std::vector<Node*> children;
    // std::vector<std::string, std::string> attributes;
    std::string textContent;
    uint32_t birthOrder; // Used to distinguish between several children of the same parent
};

enum TokenType {
    TAG_OPEN,
    TAG_CLOSE,
    TAG_SELF_CLOSE,
    TEXT
};

struct Token {
    TokenType type;
    std::string value;
};

class HTMLDocument {
public:
    Node* root;
    std::string html;
    std::vector<Token> tokens;

    HTMLDocument(std::string html);
    ~HTMLDocument();
    Node* getRoot();

private:
    std::vector<Token> tokenize();
    Node* parse();
    ElementType getElementType(std::string tag);
    void deleteTree(Node* node);
};