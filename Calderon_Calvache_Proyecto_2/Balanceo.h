#ifndef BALANCEO_H_INCLUDED
#define BALANCEO_H_INCLUDED

#include "ArbolBinario.h"
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

struct NodeQueueBal {
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

class Balancear {
public:
    static std::string mensajes[10];
    static int rotacionIndex;
    static NodeQueueBal animacionQueue;

    static Node* rotarRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    static Node* rotarLeft(Node* x) {
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

        int balanceFactor = obtenerAnchura(node->left) - obtenerAnchura(node->right);

        if (balanceFactor > 1) {
            if (obtenerAnchura(node->left->left) >= obtenerAnchura(node->left->right)) {
                grabar("II", node);
                return rotarRight(node);
            } else {
                grabar("ID", node);
                node->left = rotarLeft(node->left);
                return rotarRight(node);
            }
        } else if (balanceFactor < -1) {
            if (obtenerAnchura(node->right->right) >= obtenerAnchura(node->right->left)) {
                grabar("DD", node);
                return rotarLeft(node);
            } else {
                grabar("DI", node);
                node->right = rotarRight(node->right);
                return rotarLeft(node);
            }
        }

        return node;
    }

    static std::string wrapText(const std::string& texto, int anchoLinea) {
        std::string resultado;
        int contador = 0;
        for (char c : texto) {
            resultado += c;
            contador++;
            if (contador >= anchoLinea && c == ' ') {
                resultado += '\n';
                contador = 0;
            }
        }
        return resultado;
    }

    static void mostrarResultado(sf::RenderWindow& window, sf::Font& font) {
        if (rotacionIndex == 0) return;

        std::ostringstream mensaje;
        mensaje << "Rotaciones: ";
        for (int i = 0; i < rotacionIndex; ++i) {
            mensaje << mensajes[i];
            if (i < rotacionIndex - 1) mensaje << ", ";
        }

        std::string wrapped = wrapText(mensaje.str(), 60);
        sf::Text resultadoText(wrapped, font, 18);
        resultadoText.setFillColor(sf::Color::Green);
        resultadoText.setPosition(20, 500);

        window.draw(resultadoText);
    }

    static void clear() {
        rotacionIndex = 0;
        animacionQueue.clear();
    }

private:
    static void grabar(const char* msg, Node* n) {
        if (rotacionIndex < 10) {
            mensajes[rotacionIndex] = msg;
            ++rotacionIndex;
            if (n) animacionQueue.enqueue(n);
        }
    }

    static int obtenerAnchura(Node* node) {
        if (!node) return 0;
        int lh = obtenerAnchura(node->left);
        int rh = obtenerAnchura(node->right);
        return 1 + (lh > rh ? lh : rh);
    }
};

inline std::string Balancear::mensajes[10];
inline int Balancear::rotacionIndex = 0;
inline NodeQueueBal Balancear::animacionQueue;

#endif // BALANCEO_H_INCLUDED
