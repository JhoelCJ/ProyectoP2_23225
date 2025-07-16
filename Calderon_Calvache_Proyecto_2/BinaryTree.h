#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <SFML/Graphics.hpp>
#include <cmath>

struct Node {
    int value;
    float x, y;
    Node* left;
    Node* right;

    Node(int val) : value(val), x(0), y(0), left(nullptr), right(nullptr) {}
};

class BinaryTree {
private:
    Node* root;
    const float radius = 20.0f;
    const float verticalSpacing = 75.0f;

    int getDepth(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        int profundidadIzquierda = getDepth(node->left);
        int profundidadDerecha = getDepth(node->right);

        if (profundidadIzquierda > profundidadDerecha) {
            return 1 + profundidadIzquierda;
        } else {
            return 1 + profundidadDerecha;
        }
    }

public:

    Node* getRoot() { return root; }

    BinaryTree() : root(nullptr) {}

    void setRoot(Node* newRoot) { root = newRoot; }

    bool insert(int value) {
        bool inserted = false;
        root = insertRecursive(root, value, 400, 125, 200, inserted);
        return inserted;
    }

    void draw(sf::RenderWindow& window, sf::Font& font) {
        drawRecursive(window, font, root, true);
    }

    struct PositionTracker {
        int currentX = 0;
    };

    void updatePositions() {
        PositionTracker tracker;
        updatePositionsRecursive(root, 0, tracker);
    }

    void updatePositionsRecursive(Node* node, int depth, PositionTracker& tracker) {
        if (!node) return;

        updatePositionsRecursive(node->left, depth + 1, tracker);
        node->x = 100 + tracker.currentX * 75;
        node->y = 150 + depth * verticalSpacing;
        tracker.currentX++;

        updatePositionsRecursive(node->right, depth + 1, tracker);
    }


private:
    Node* insertRecursive(Node* node, int value, float x, float y, float offset, bool& inserted) {
        if (!node) {
            Node* newNode = new Node(value);
            newNode->x = x;
            newNode->y = y;
            inserted = true;
            return newNode;
        }
        if (value < node->value){
            node->left = insertRecursive(node->left, value, x - offset, y + verticalSpacing, offset / 2, inserted);
        }else if (value > node->value){
            node->right = insertRecursive(node->right, value, x + offset, y + verticalSpacing, offset / 2, inserted);
        }else{
            inserted = false;
        }
        return node;
    }

    void updatePositionsRecursive(Node* node, float x, float y, float offset) {
        if (!node) return;

        node->x = x;
        node->y = y;

        updatePositionsRecursive(node->left, x - offset, y + verticalSpacing, offset / 2);
        updatePositionsRecursive(node->right, x + offset, y + verticalSpacing, offset / 2);
    }

    void drawLine(sf::RenderWindow& window, float x1, float y1, float x2, float y2, float radius) {
        sf::Vector2f dir(x2 - x1, y2 - y1);
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len == 0) return;

        dir.x /= len;
        dir.y /= len;

        sf::Vector2f start = sf::Vector2f(x1, y1) + dir * radius;
        sf::Vector2f end = sf::Vector2f(x2, y2) - dir * radius;

        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color::Black),
            sf::Vertex(end, sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }

    void drawRecursive(sf::RenderWindow& window, sf::Font& font, Node* node, bool expandRadius = true) {
        if (!node) return;

        if (node->left)
            drawLine(window, node->x, node->y, node->left->x, node->left->y, radius);

        if (node->right)
            drawLine(window, node->x, node->y, node->right->x, node->right->y, radius);

        int baseFontSize = 14;
        sf::Text text(std::to_string(node->value), font, baseFontSize);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect textBounds = text.getLocalBounds();
        float textWidth = textBounds.width;

        float adjustedRadius = radius;
        int adjustedFontSize = baseFontSize;

        if (expandRadius) {

            if (textWidth > radius * 1.5f) {
                adjustedRadius = textWidth * 0.6f;
            }
        } else {
            while (textWidth > adjustedRadius * 1.5f && adjustedFontSize > 8) {
                adjustedFontSize -= 1;
                text.setCharacterSize(adjustedFontSize);
                textBounds = text.getLocalBounds();
                textWidth = textBounds.width;
            }
        }

        sf::CircleShape circle(adjustedRadius);
        circle.setFillColor(sf::Color(200, 200, 255));
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2);
        circle.setPosition(node->x - adjustedRadius, node->y - adjustedRadius);
        window.draw(circle);

        textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);
        text.setPosition(node->x, node->y);

        window.draw(text);

        drawRecursive(window, font, node->left, expandRadius);
        drawRecursive(window, font, node->right, expandRadius);
    }
};

#endif
