#ifndef RECORRIDOIN_H_INCLUDED
#define RECORRIDOIN_H_INCLUDED

#include "BinaryTree.h"
#include <SFML/Graphics.hpp>
#include <sstream>

struct NodeQueue {
    Node* steps[100];
    int front = 0;
    int rear = 0;

    void enqueue(Node* n) {
        steps[rear] = n;
        rear = (rear + 1) % 100;
    }

    Node* dequeue() {
        if (isEmpty()) return nullptr;
        Node* n = steps[front];
        front = (front + 1) % 100;
        return n;
    }

    bool isEmpty() {
        return front == rear;
    }

    void clear() {
        front = rear = 0;
    }
};

class Traversal {
public:
    static std::string currentResult;
    static NodeQueue animationQueue;

    static void inorder(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animationQueue.clear();
        inorderRecursive(node, result);
        currentResult = "Inorden: " + result.str();
    }

    static void preorder(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animationQueue.clear();
        preorderRecursive(node, result);
        currentResult = "Preorden: " + result.str();
    }

    static void postorder(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animationQueue.clear();
        postorderRecursive(node, result);
        currentResult = "Postorden: " + result.str();
    }

    static std::string wrapText(const std::string& texto, int anchoLinea) {
        std::string result;
        int contador = 0;
        for (char c : texto) {
            result += c;
            contador++;
            if (contador >= anchoLinea && c == ' ') {
                result += '\n';
                contador = 0;
            }
        }
        return result;
    }

    static void drawResult(sf::RenderWindow& window, sf::Font& font) {
        if (currentResult.empty())
            return;

        std::string wrapped = wrapText(currentResult, 80);
        sf::Text resultText(wrapped, font, 18);
        resultText.setFillColor(sf::Color::Red);
        resultText.setPosition(20, 550);

        if (resultText.getLocalBounds().width > 1000) {
            float scale = 700.f / resultText.getLocalBounds().width;
            resultText.setScale(scale, scale);
        }

        window.draw(resultText);
    }

private:
    static void inorderRecursive(Node* node, std::ostringstream& result) {
        if (!node) return;
        inorderRecursive(node->left, result);
        animationQueue.enqueue(node);
        result << node->value << ", ";
        inorderRecursive(node->right, result);
    }

    static void preorderRecursive(Node* node, std::ostringstream& result) {
        if (!node) return;
        animationQueue.enqueue(node);
        result << node->value << ", ";
        preorderRecursive(node->left, result);
        preorderRecursive(node->right, result);
    }

    static void postorderRecursive(Node* node, std::ostringstream& result) {
        if (!node) return;
        postorderRecursive(node->left, result);
        postorderRecursive(node->right, result);
        animationQueue.enqueue(node);
        result << node->value << ", ";
    }
};

std::string Traversal::currentResult = "";
NodeQueue Traversal::animationQueue;

#endif // RECORRIDOIN_H_INCLUDED
