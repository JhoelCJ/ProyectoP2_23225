#ifndef ELIMINAR_H
#define ELIMINAR_H

#include "ArbolBinario.h"
#include <string>

struct EliminacionVisual {
    Node* node;
    float yOriginal;
    float elapsedTime;
};

class Eliminar {
public:
    static std::string mensajes[10];
    static int mensajeCount;
    static EliminacionVisual animacionesEliminacion[10];
    static int animacionesCount;

    static Node* nodosA[10];
    static int nodosPendientes;

    static Node* remove(Node* raiz, int key, bool usarMayor, bool& eliminado) {
        if (!raiz) return nullptr;

        if (key < raiz->valor) {
            raiz->left = remove(raiz->left, key, usarMayor, eliminado);
        } else if (key > raiz->valor) {
            raiz->right = remove(raiz->right, key, usarMayor, eliminado);
        } else {
            if (animacionesCount < 10){
                EliminacionVisual ev;
                ev.node = raiz;
                ev.yOriginal = raiz->y;
                ev.elapsedTime = 0.f;
                animacionesEliminacion[animacionesCount++] = ev;
            }
            if (!raiz->left) {
                Node* temp = raiz->right;
               if (nodosPendientes < 10) {
                    nodosA[nodosPendientes++] = raiz;
                }
                eliminado = true;
                mensajes[mensajeCount++] = "Nodo eliminado.";
                return temp;
            } else if (!raiz->right) {
                Node* temp = raiz->left;
                if (nodosPendientes < 10) {
                    nodosA[nodosPendientes++] = raiz;
                }
                eliminado = true;
                mensajes[mensajeCount++] = "Nodo eliminado.";
                return temp;
            }

            if (usarMayor) {
                Node* maxLeft = findMax(raiz->left);
                raiz->valor = maxLeft->valor;
                raiz->left = remove(raiz->left, maxLeft->valor, usarMayor, eliminado);
                eliminado = true;
                mensajes[mensajeCount++] = "Mayor del Menor";
            } else {
                Node* minRight = findMin(raiz->right);
                raiz->valor = minRight->valor;
                raiz->right = remove(raiz->right, minRight->valor, usarMayor, eliminado);
                eliminado = true;
                mensajes[mensajeCount++] = "Menor del Mayor";
            }
        }
        return raiz;
    }

    static void remove(ArbolBinario& arbol, int key, bool usarMayor, bool& eliminado) {
        mensajeCount = 0;
        eliminado = false;
        animacionesCount = 0;
        nodosPendientes = 0;
        arbol.setRaiz(remove(arbol.obtenerRaiz(), key, usarMayor, eliminado));
        if (!eliminado) {
            mensajes[0] = "El numero no existe";
            mensajeCount = 1;
        }
    }

    static void procesarEliminacionesPendientes() {
        for (int i = 0; i < nodosPendientes; ++i) {
            delete nodosA[i];
            nodosA[i] = nullptr;
        }
        nodosPendientes = 0;
    }

private:
    static Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    static Node* findMax(Node* node) {
        while (node && node->right) node = node->right;
        return node;
    }
};

inline std::string Eliminar::mensajes[10];
inline int Eliminar::mensajeCount = 0;
inline EliminacionVisual Eliminar::animacionesEliminacion[10];
inline int Eliminar::animacionesCount = 0;
inline Node* Eliminar::nodosA[10];
inline int Eliminar::nodosPendientes = 0;

#endif // ELIMINAR_H_H_INCLUDED
