#ifndef BUSQUEDA_H_INCLUDED
#define BUSQUEDA_H_INCLUDED

#include "ArbolBinario.h"
#include <SFML/Graphics.hpp>
#include <sstream>

struct NodeQueuee {
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

class Buscar {
public:
    static std::string resultadoReal;
    static NodeQueuee animacionQueue;

    static void depthFirstBuscar(Node* node, int target, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream resultado;
        animacionQueue.clear();
        bool encontrar = dfsRecursive(node, target, resultado);
        if (encontrar) {
            resultadoReal = "Busqueda en Profundidad: Encontrado " + std::to_string(target) + " -> Camino: " + resultado.str();
        } else {
            resultadoReal = "!!!No se encontro el valor " + std::to_string(target);// + " -> Camino: " + resultado.str();
        }
    }

    static void breadthFirstBuscar(Node* raiz, int target, sf::RenderWindow& window, sf::Font& font) {
        std::ostringstream resultado;
        animacionQueue.clear();
        if (!raiz) {
            resultadoReal = "Arbol vacio";
            return;
        }

        Node* queue[100];
        int front = 0, ultimo = 0;
        queue[ultimo++] = raiz;
        bool encontrar = false;

        while (front != ultimo) {
            Node* actual = queue[front++];

            animacionQueue.enqueue(actual);
            resultado << actual->valor << ", ";

            if (actual->valor == target) {
                encontrar = true;
                break;
            }

            if (actual->left) queue[ultimo++] = actual->left;
            if (actual->right) queue[ultimo++] = actual->right;
        }

        if (encontrar) {
            resultadoReal = "Busqueda en Anchura: Encontrado " + std::to_string(target) + " -> Camino: " + resultado.str();
        } else {
            resultadoReal = "!!!No se encontro el valor " + std::to_string(target);// + " -> Camino: " + resultado.str();
        }
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
        if (resultadoReal.empty()) return;

        std::string wrapped = wrapText(resultadoReal, 60);
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
    static bool dfsRecursive(Node* node, int target, std::ostringstream& resultado) {
        if (!node) return false;

        animacionQueue.enqueue(node);
        resultado << node->valor << ", ";

        if (node->valor == target) return true;

        if (dfsRecursive(node->left, target, resultado)) return true;
        if (dfsRecursive(node->right, target, resultado)) return true;

        return false;
    }
};

std::string Buscar::resultadoReal = "";
NodeQueuee Buscar::animacionQueue;

#endif // BUSQUEDA_H_INCLUDED
