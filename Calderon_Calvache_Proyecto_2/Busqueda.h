#ifndef BUSQUEDA_H_INCLUDED
#define BUSQUEDA_H_INCLUDED

#include "BinaryTree.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <queue>

struct NodeQueuee {
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

class Search {
public:
    static std::string currentResult;
    static NodeQueuee animationQueue;

    static void depthFirstSearch(Node* node, int target, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animationQueue.clear();
        bool found = dfsRecursive(node, target, result);
        if (found) {
            currentResult = "Busqueda en Profundidad: Encontrado " + std::to_string(target) + " -> Camino: " + result.str();
        } else {
            currentResult = "Busqueda en Profundidad: No se encontró " + std::to_string(target) + " -> Camino: " + result.str();
        }
    }

    static void breadthFirstSearch(Node* root, int target, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animationQueue.clear();
        if (!root) {
            currentResult = "Arbol vacio";
            return;
        }

        std::queue<Node*> q;
        q.push(root);
        bool found = false;

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();

            animationQueue.enqueue(current);
            result << current->value << ", ";

            if (current->value == target) {
                found = true;
                break;
            }

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }

        if (found) {
            currentResult = "Busqueda en Anchura: Encontrado " + std::to_string(target) + " -> Camino: " + result.str();
        } else {
            currentResult = "Busqueda en Anchura: No se encontro " + std::to_string(target) + " -> Camino: " + result.str();
        }
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
        if (currentResult.empty()) return;

        std::string wrapped = wrapText(currentResult, 60);
        sf::Text resultText(wrapped, font, 18);
        resultText.setFillColor(sf::Color::Blue);
        resultText.setPosition(20, 520);

        if (resultText.getLocalBounds().width > 700) {
            float scale = 700.f / resultText.getLocalBounds().width;
            resultText.setScale(scale, scale);
        }

        window.draw(resultText);
    }

private:
    static bool dfsRecursive(Node* node, int target, std::ostringstream& result) {
        if (!node) return false;

        animationQueue.enqueue(node);
        result << node->value << ", ";

        if (node->value == target) return true;

        if (dfsRecursive(node->left, target, result)) return true;
        if (dfsRecursive(node->right, target, result)) return true;

        return false;
    }
};


std::string Search::currentResult = "";
NodeQueuee Search::animationQueue;

#endif // BUSQUEDA_H_INCLUDED
