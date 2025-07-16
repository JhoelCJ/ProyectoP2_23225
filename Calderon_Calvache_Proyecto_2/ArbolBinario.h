#ifndef ARBOLBINARIO_H_INCLUDED
#define ARBOLBINARIO_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <cmath>

struct Node {
    int valor;
    float x, y;
    Node* left;
    Node* right;

    Node(int val) : valor(val), x(0), y(0), left(nullptr), right(nullptr) {}
};

class ArbolBinario {
private:
    Node* raiz;
    const float radio = 20.0f;
    const float espacioVertical = 75.0f;

    int obtenerProfundidad(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        int profundidadIzquierda = obtenerProfundidad(node->left);
        int profundidadDerecha = obtenerProfundidad(node->right);

        if (profundidadIzquierda > profundidadDerecha) {
            return 1 + profundidadIzquierda;
        } else {
            return 1 + profundidadDerecha;
        }
    }

public:

    Node* obtenerRaiz() { return raiz; }

    ArbolBinario() : raiz(nullptr) {}

    void setRaiz(Node* newraiz) { raiz = newraiz; }

    bool insert(int valor) {
        bool insertado = false;
        raiz = insertarRecursivo(raiz, valor, 400, 125, 200, insertado);
        return insertado;
    }

    void draw(sf::RenderWindow& window, sf::Font& font) {
        drawRecursivo(window, font, raiz, true);
    }

    struct seguirPosicion {
        int currentX = 0;
    };

    void actualizarPosicion() {
        seguirPosicion seguir;
        actualizarPosicionRecursiva(raiz, 0, seguir);
    }

    void actualizarPosicionRecursiva(Node* node, int profundidad, seguirPosicion& seguir) {
        if (!node) return;

        actualizarPosicionRecursiva(node->left, profundidad + 1, seguir);
        node->x = 100 + seguir.currentX * 75;
        node->y = 150 + profundidad * espacioVertical;
        seguir.currentX++;

        actualizarPosicionRecursiva(node->right, profundidad + 1, seguir);
    }

private:
    Node* insertarRecursivo(Node* node, int valor, float x, float y, float offset, bool& insertado) {
        if (!node) {
            Node* newNode = new Node(valor);
            newNode->x = x;
            newNode->y = y;
            insertado = true;
            return newNode;
        }
        if (valor < node->valor){
            node->left = insertarRecursivo(node->left, valor, x - offset, y + espacioVertical, offset / 2, insertado);
        }else if (valor > node->valor){
            node->right = insertarRecursivo(node->right, valor, x + offset, y + espacioVertical, offset / 2, insertado);
        }else{
            insertado = false;
        }
        return node;
    }

    void actualizarPosicionRecursiva(Node* node, float x, float y, float offset) {
        if (!node) return;

        node->x = x;
        node->y = y;

        actualizarPosicionRecursiva(node->left, x - offset, y + espacioVertical, offset / 2);
        actualizarPosicionRecursiva(node->right, x + offset, y + espacioVertical, offset / 2);
    }

    void drawLinea(sf::RenderWindow& window, float x1, float y1, float x2, float y2, float radio) {
        sf::Vector2f dir(x2 - x1, y2 - y1);
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len == 0) return;

        dir.x /= len;
        dir.y /= len;

        sf::Vector2f start = sf::Vector2f(x1, y1) + dir * radio;
        sf::Vector2f end = sf::Vector2f(x2, y2) - dir * radio;

        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color::Black),
            sf::Vertex(end, sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }

    void drawRecursivo(sf::RenderWindow& window, sf::Font& font, Node* node, bool expandradio = true) {
        if (!node) return;

        if (node->left)
            drawLinea(window, node->x, node->y, node->left->x, node->left->y, radio);

        if (node->right)
            drawLinea(window, node->x, node->y, node->right->x, node->right->y, radio);

        int tamFuenteBase = 14;
        sf::Text text(std::to_string(node->valor), font, tamFuenteBase);
        text.setFillColor(sf::Color::Black);

        sf::FloatRect limiteTexto = text.getLocalBounds();
        float anchuraTexto = limiteTexto.width;

        float ajustarRadio = radio;
        int ajustarTamFuente = tamFuenteBase;

        if (expandradio) {

            if (anchuraTexto > radio * 1.5f) {
                ajustarRadio = anchuraTexto * 0.6f;
            }
        } else {
            while (anchuraTexto > ajustarRadio * 1.5f && ajustarTamFuente > 8) {
                ajustarTamFuente -= 1;
                text.setCharacterSize(ajustarTamFuente);
                limiteTexto = text.getLocalBounds();
                anchuraTexto = limiteTexto.width;
            }
        }

        sf::CircleShape circulo(ajustarRadio);
        circulo.setFillColor(sf::Color(200, 200, 255));
        circulo.setOutlineColor(sf::Color::Black);
        circulo.setOutlineThickness(2);
        circulo.setPosition(node->x - ajustarRadio, node->y - ajustarRadio);
        window.draw(circulo);

        limiteTexto = text.getLocalBounds();
        text.setOrigin(limiteTexto.left + limiteTexto.width / 2.0f,
                       limiteTexto.top + limiteTexto.height / 2.0f);
        text.setPosition(node->x, node->y);

        window.draw(text);

        drawRecursivo(window, font, node->left, expandradio);
        drawRecursivo(window, font, node->right, expandradio);
    }
};


#endif // ARBOLBINARIO_H_INCLUDED
