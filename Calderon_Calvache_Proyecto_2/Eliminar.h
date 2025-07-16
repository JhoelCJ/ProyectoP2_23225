#ifndef ELIMINAR_H
#define ELIMINAR_H

#include "BinaryTree.h"
#include <string>

struct EliminacionVisual {
    Node* node;
    float yOriginal;
    float elapsedTime;
};

class Deleter {
public:
    static std::string mensajes[10];
    static int mensajeCount;
    static EliminacionVisual animacionesEliminacion[10];
    static int animacionesCount;

    static Node* nodosAEliminar[10];
    static int nodosPendientes;

    static Node* remove(Node* root, int key, bool usarMayor, bool& eliminado) {
        if (!root) return nullptr;

        if (key < root->value) {
            root->left = remove(root->left, key, usarMayor, eliminado);
        } else if (key > root->value) {
            root->right = remove(root->right, key, usarMayor, eliminado);
        } else {
            if (animacionesCount < 10){
                EliminacionVisual ev;
                ev.node = root;
                ev.yOriginal = root->y;
                ev.elapsedTime = 0.f;
                animacionesEliminacion[animacionesCount++] = ev;
            }
            if (!root->left) {
                Node* temp = root->right;
               // delete root;
               if (nodosPendientes < 10) {
                    nodosAEliminar[nodosPendientes++] = root;
                }
                eliminado = true;
                mensajes[mensajeCount++] = "Nodo eliminado.";
                return temp;
            } else if (!root->right) {
                Node* temp = root->left;
                if (nodosPendientes < 10) {
                    nodosAEliminar[nodosPendientes++] = root;
                }
                //delete root;
                eliminado = true;
                mensajes[mensajeCount++] = "Nodo eliminado.";
                return temp;
            }

            if (usarMayor) {
                Node* maxLeft = findMax(root->left);
                root->value = maxLeft->value;
                root->left = remove(root->left, maxLeft->value, usarMayor, eliminado);
                eliminado = true;
                mensajes[mensajeCount++] = "Reemplazado por mayor del subarbol izquierdo.";
            } else {
                Node* minRight = findMin(root->right);
                root->value = minRight->value;
                root->right = remove(root->right, minRight->value, usarMayor, eliminado);
                eliminado = true;
                mensajes[mensajeCount++] = "Reemplazado por menor del subarbol derecho.";
            }
        }
        return root;
    }

    static void remove(BinaryTree& tree, int key, bool usarMayor, bool& eliminado) {
        mensajeCount = 0;
        eliminado = false;
        animacionesCount = 0;
        nodosPendientes = 0;
        tree.setRoot(remove(tree.getRoot(), key, usarMayor, eliminado));
        if (!eliminado) {
            mensajes[0] = "El numero no existe";
            mensajeCount = 1;
        }
    }

    static void procesarEliminacionesPendientes() {
        for (int i = 0; i < nodosPendientes; ++i) {
            delete nodosAEliminar[i];
            nodosAEliminar[i] = nullptr;
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

inline std::string Deleter::mensajes[10];
inline int Deleter::mensajeCount = 0;
inline EliminacionVisual Deleter::animacionesEliminacion[10];
inline int Deleter::animacionesCount = 0;
inline Node* Deleter::nodosAEliminar[10];
inline int Deleter::nodosPendientes = 0;

#endif // ELIMINAR_H_INCLUDED
