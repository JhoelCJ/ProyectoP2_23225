#ifndef RECORRIDO_H_INCLUDED
#define RECORRIDO_H_INCLUDED

#include "ArbolBinario.h"
#include <SFML/Graphics.hpp>
#include <sstream>

struct NodeQueue {
    Node* pasos[100];
    int front = 0;
    int ultimo = 0;

    void enqueue(Node* n) {
        pasos[ultimo] = n;
        ultimo = (ultimo + 1) % 100;
    }

    Node* dequeue() {
        if (isEmpty()) return nullptr;
        Node* n = pasos[front];
        front = (front + 1) % 100;
        return n;
    }

    bool isEmpty() {
        return front == ultimo;
    }

    void clear() {
        front = ultimo = 0;
    }
};

class Recorrido {
public:
    static std::string resultadoReal;
    static NodeQueue animacionQueue;

    static void inOrden(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animacionQueue.clear();
        inOrdenRecursivo(node, result);
        resultadoReal = "InOrden: " + result.str();
    }

    static void preOrdenRecursivo(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animacionQueue.clear();
        preOrdenRecursive(node, result);
        resultadoReal = "Preorden: " + result.str();
    }

    static void postOrden(Node* node, Node* root, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream result;
        animacionQueue.clear();
        postOrdenRecursive(node, result);
        resultadoReal = "Postorden: " + result.str();
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

    static void mostrarResultado(sf::RenderWindow& window, sf::Font& font) {
        if (resultadoReal.empty())
            return;

        std::string wrapped = wrapText(resultadoReal, 80);
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
    static void inOrdenRecursivo(Node* node, std::ostringstream& result) {
        if (!node) return;
        inOrdenRecursivo(node->left, result);
        animacionQueue.enqueue(node);
        result << node->valor << ", ";
        inOrdenRecursivo(node->right, result);
    }

    static void preOrdenRecursive(Node* node, std::ostringstream& result) {
        if (!node) return;
        animacionQueue.enqueue(node);
        result << node->valor << ", ";
        preOrdenRecursive(node->left, result);
        preOrdenRecursive(node->right, result);
    }

    static void postOrdenRecursive(Node* node, std::ostringstream& result) {
        if (!node) return;
        postOrdenRecursive(node->left, result);
        postOrdenRecursive(node->right, result);
        animacionQueue.enqueue(node);
        result << node->valor << ", ";
    }
};

std::string Recorrido::resultadoReal = "";
NodeQueue Recorrido::animacionQueue;

#endif // RECORRIDO_H_INCLUDED
