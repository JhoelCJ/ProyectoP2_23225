#ifndef BALANCEO_H_INCLUDED
#define BALANCEO_H_INCLUDED

#include "BinaryTree.h"
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

struct NodeQueueBal {
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

class Balancer {
public:
    static std::string mensajes[10];
    static int rotacionIndex;
    static NodeQueueBal animationQueue;

    static Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    static Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }

    static Node* balance(Node* node) {
        if (!node) return nullptr;

        node->left = balance(node->left);
        node->right = balance(node->right);

        int balanceFactor = getHeight(node->left) - getHeight(node->right);

        if (balanceFactor > 1) {
            if (getHeight(node->left->left) >= getHeight(node->left->right)) {
                record("II", node);
                return rotateRight(node);
            } else {
                record("ID", node);
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        } else if (balanceFactor < -1) {
            if (getHeight(node->right->right) >= getHeight(node->right->left)) {
                record("DD", node);
                return rotateLeft(node);
            } else {
                record("DI", node);
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }

        return node;
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
        if (rotacionIndex == 0) return;

        std::ostringstream mensaje;
        mensaje << "Rotaciones: ";
        for (int i = 0; i < rotacionIndex; ++i) {
            mensaje << mensajes[i];
            if (i < rotacionIndex - 1) mensaje << ", ";
        }

        std::string wrapped = wrapText(mensaje.str(), 60);
        sf::Text resultText(wrapped, font, 18);
        resultText.setFillColor(sf::Color::Green);
        resultText.setPosition(20, 500);

        window.draw(resultText);
    }

    static void clear() {
        rotacionIndex = 0;
        animationQueue.clear();
    }

private:
    static void record(const char* msg, Node* n) {
        if (rotacionIndex < 10) {
            mensajes[rotacionIndex] = msg;
            ++rotacionIndex;
            if (n) animationQueue.enqueue(n);
        }
    }

    static int getHeight(Node* node) {
        if (!node) return 0;
        int lh = getHeight(node->left);
        int rh = getHeight(node->right);
        return 1 + (lh > rh ? lh : rh);
    }
};

inline std::string Balancer::mensajes[10];
inline int Balancer::rotacionIndex = 0;
inline NodeQueueBal Balancer::animationQueue;

#endif // BALANCEO_H_INCLUDED
