#include "HTMLDocument.hpp"
#include <iostream>
#include <stack>

/**
 * @author Kade Angell <kade.angell@icloud.com>
 * @param html
 * Creates a new HTMLDocument object from the given HTML string.
*/
HTMLDocument::HTMLDocument(std::string html) {
    root = nullptr;
    this->html = html;
    tokens = tokenize();
    root = parse();
}

/**
 * @return void
 * Deconstructs the HTMLDocument object.
*/
HTMLDocument::~HTMLDocument() {
    deleteTree(root);
}

/**
 * @return Node*
 * Returns the root node of the tree.
*/
Node* HTMLDocument::getRoot() {
    return root;
}

/**
 * @author Kade Angell <kade.angell@icloud.com>
 * @param node
 * @return void
 * Deletes the tree of nodes recusively, starting at the given node.
*/
void HTMLDocument::deleteTree(Node* node) {
    if (node) {
        for (Node* child : node->children) {
            deleteTree(child);
        }
        delete node;
    }
}

std::vector<Token> HTMLDocument::tokenize() {
    std::vector<Token> tokens;
    std::string tokenValue;
    bool inTag = false;

    for (size_t i = 0; i < html.size(); ++i) {
        char c = html[i];

        if (c == '<') {
            // start of a tag
            if (!tokenValue.empty()) {
                // Save the text from the previous token
                tokens.push_back({TokenType::TEXT, tokenValue});
                tokenValue.clear();
            }
            inTag = true;
            tokenValue += c;
        } else if (c == '>') {
            // End of a tag
            tokenValue += c;
            if (tokenValue[1] == '/') {
                // Close tag
                tokens.push_back({TokenType::TAG_CLOSE, tokenValue});
            } else if (tokenValue[tokenValue.size() - 2] == '/') {
                // Self-closing tag
                tokens.push_back({TAG_SELF_CLOSE, tokenValue});
            } else if (tokenValue[tokenValue.size() - 2] == '!') {
                // Comment tag
                tokenValue.clear();
            } else {
                // Open tag
                tokens.push_back({TAG_OPEN, tokenValue});
            }
            tokenValue.clear();
            inTag = false;
        } else {
            // Text or inside of a tag
            tokenValue += c;
            if (!inTag && i == html.size() - 1) {
                // Save the text from the last token
                tokens.push_back({TokenType::TEXT, tokenValue});
            }
        }
    }

    return tokens;
}

Node* HTMLDocument::parse() {
    // TODO: Implement the parsing of the tokens into a tree of nodes.
    std::stack<Node*> stack;

    for (const Token& token : tokens) {
        switch(token.type) {
            case TokenType::TAG_OPEN:
                {
                    Node* node = new Node();
                    node->type = getElementType(token.value);
                    node->birthOrder = stack.empty() ? 0 : stack.top()->children.size();
                    if (!stack.empty()) {
                        stack.top()->children.push_back(node);
                    } else {
                        root = node;
                    }
                    stack.push(node);
                }
                break;
            case TokenType::TAG_CLOSE:
                {
                    if (!stack.empty()) {
                        stack.pop();
                    }
                }
                break;
            case TokenType::TAG_SELF_CLOSE:
                {
                    Node* node = new Node();
                    node->type = getElementType(token.value);
                    if (!stack.empty()) {
                        stack.top()->children.push_back(node);
                    } else {
                        root = node;
                    }
                }
                break;
            case TokenType::TEXT:
                {
                    if (!stack.empty()) {
                        stack.top()->textContent = token.value;
                    }
                }
                break;
        }
    }

    return root;
}

ElementType HTMLDocument::getElementType(std::string tag) {
    static const std::unordered_map<std::string, ElementType> elementTypes = {
        {"a", ElementType::A},
        {"abbr", ElementType::ABBR},
        {"address", ElementType::ADDRESS},
        {"area", ElementType::AREA},
        {"article", ElementType::ARTICLE},
        {"aside", ElementType::ASIDE},
        {"audio", ElementType::AUDIO},
        {"b", ElementType::B},
        {"base", ElementType::BASE},
        {"bdi", ElementType::BDI},
        {"bdo", ElementType::BDO},
        {"blockquote", ElementType::BLOCKQUOTE},
        {"body", ElementType::BODY},
        {"br", ElementType::BR},
        {"button", ElementType::BUTTON},
        {"canvas", ElementType::CANVAS},
        {"caption", ElementType::CAPTION},
        {"cite", ElementType::CITE},
        {"code", ElementType::CODE},
        {"col", ElementType::COL},
        {"colgroup", ElementType::COLGROUP},
        {"data", ElementType::DATA},
        {"datalist", ElementType::DATALIST},
        {"dd", ElementType::DD},
        {"del", ElementType::DEL},
        {"details", ElementType::DETAILS},
        {"dfn", ElementType::DFN},
        {"dialog", ElementType::DIALOG},
        {"div", ElementType::DIV},
        {"dl", ElementType::DL},
        {"dt", ElementType::DT},
        {"em", ElementType::EM},
        {"embed", ElementType::EMBED},
        {"fieldset", ElementType::FIELDSET},
        {"figcaption", ElementType::FIGCAPTION},
        {"figure", ElementType::FIGURE},
        {"footer", ElementType::FOOTER},
        {"form", ElementType::FORM},
        {"h1", ElementType::H1},
        {"h2", ElementType::H2},
        {"h3", ElementType::H3},
        {"h4", ElementType::H4},
        {"h5", ElementType::H5},
        {"h6", ElementType::H6},
        {"head", ElementType::HEAD},
        {"header", ElementType::HEADER},
        {"hr", ElementType::HR},
        {"html", ElementType::HTML},
        {"i", ElementType::I},
        {"iframe", ElementType::IFRAME},
        {"img", ElementType::IMG},
        {"input", ElementType::INPUT},
        {"ins", ElementType::INS},
        {"kbd", ElementType::KBD},
        {"label", ElementType::LABEL},
        {"legend", ElementType::LEGEND},
        {"li", ElementType::LI},
        {"link", ElementType::LINK},
        {"main", ElementType::MAIN},
        {"map", ElementType::MAP},
        {"mark", ElementType::MARK},
        {"meta", ElementType::META},
        {"meter", ElementType::METER},
        {"nav", ElementType::NAV},
        {"noscript", ElementType::NOSCRIPT},
        {"object", ElementType::OBJECT},
        {"ol", ElementType::OL},
        {"optgroup", ElementType::OPTGROUP},
        {"option", ElementType::OPTION},
        {"output", ElementType::OUTPUT},
        {"p", ElementType::P},
        {"picture", ElementType::PICTURE},
        {"pre", ElementType::PRE},
        {"progress", ElementType::PROGRESS},
        {"q", ElementType::Q},
        {"rp", ElementType::RP},
        {"rt", ElementType::RT},
        {"ruby", ElementType::RUBY},
        {"s", ElementType::S},
        {"samp", ElementType::SAMP},
        {"script", ElementType::SCRIPT},
        {"section", ElementType::SECTION},
        {"select", ElementType::SELECT},
        {"small", ElementType::SMALL},
        {"source", ElementType::SOURCE},
        {"span", ElementType::SPAN},
        {"strong", ElementType::STRONG},
        {"style", ElementType::STYLE},
        {"sub", ElementType::SUB},
        {"summary", ElementType::SUMMARY},
        {"sup", ElementType::SUP},
        {"table", ElementType::TABLE},
        {"tbody", ElementType::TBODY},
        {"td", ElementType::TD},
        {"template", ElementType::TEMPLATE},
        {"textarea", ElementType::TEXTAREA},
        {"tfoot", ElementType::TFOOT},
        {"th", ElementType::TH},
        {"thead", ElementType::THEAD},
        {"time", ElementType::TIME},
        {"title", ElementType::TITLE},
        {"tr", ElementType::TR},
        {"track", ElementType::TRACK},
        {"u", ElementType::U},
        {"ul", ElementType::UL},
        {"var", ElementType::VAR},
        {"video", ElementType::VIDEO},
        {"wbr", ElementType::WBR}
    };

    // Strip '<' and '>' from the tag and only take the first word
    tag = tag.substr(1, tag.size() - 2);
    size_t space = tag.find(' ');
    if (space != std::string::npos) {
        tag = tag.substr(0, space);
    }

    auto it = elementTypes.find(tag);
    if (it != elementTypes.end()) {
        return it->second;
    }

    #ifndef NDEBUG
        std::cerr << "Unknown element type: " << tag << std::endl;
    #endif

    return ElementType::UNKNOWN;    
}