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
        drawRecursive(window, font, root);
    }

    void updatePositions() {
        updatePositionsRecursive(root, 400, 125, 200);
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

    void drawRecursive(sf::RenderWindow& window, sf::Font& font, Node* node) {
        if (!node) return;

        if (node->left)
            drawLine(window, node->x, node->y, node->left->x, node->left->y, radius);

        if (node->right)
            drawLine(window, node->x, node->y, node->right->x, node->right->y, radius);

        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color(200, 200, 255));
        circle.setOutlineColor(sf::Color::Black);
        circle.setOutlineThickness(2);
        circle.setPosition(node->x - radius, node->y - radius);
        window.draw(circle);

        sf::Text text(std::to_string(node->value), font, 16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(node->x - 10, node->y - 12);
        window.draw(text);

        drawRecursive(window, font, node->left);
        drawRecursive(window, font, node->right);
    }
};

#endif
