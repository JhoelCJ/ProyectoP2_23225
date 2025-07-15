#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "BinaryTree.h"
#include "Balanceo.h"
#include "RecorridoIN.h"
#include "Eliminar.h"
#include "Busqueda.h"

int main() {
    setlocale(LC_ALL, "");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Árbol Binario Visual con TGUI");
    tgui::Gui gui(window);

    // Fuente para nodos
    sf::Font font;
    if (!font.loadFromFile("fuente/MinecraftTen.ttf")) return -1;

    // Árbol binario
    BinaryTree tree;

    // Entrada de número
    auto input = tgui::EditBox::create();
    input->setPosition(20, 20);
    input->setSize(100, 30);
    input->setDefaultText("Numero [0]");
    gui.add(input);

    // Botón de insertar
    auto button = tgui::Button::create("Insertar");
    button->setPosition(130, 20);
    button->setSize(80, 30);
    gui.add(button);

    button->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                tree.insert(valor);
                tree.updatePositions();

                // Limpiar animaciones y textos previos
                Traversal::animationQueue.clear();
                Search::animationQueue.clear();
                Traversal::currentResult.clear();
                Search::currentResult.clear();

                input->setText("");
            } catch (...) {}
        }
    });

    // Botón para eliminar nodo
    auto eliminarBtn = tgui::Button::create("Eliminar");
    eliminarBtn->setPosition(220, 20);
    eliminarBtn->setSize(80, 30);
    gui.add(eliminarBtn);

    auto metodoBox = tgui::ComboBox::create();
    metodoBox->setPosition(510, 20);
    metodoBox->setSize(250, 30);
    metodoBox->addItem("Reemplazar por el mayor del subárbol izquierdo");
    metodoBox->addItem("Reemplazar por el menor del subárbol derecho");
    metodoBox->setSelectedItemByIndex(0);
    metodoBox->setTextSize(10);
    gui.add(metodoBox);

    // Etiqueta para mostrar mensajes
    auto statusLabel = tgui::Label::create();
    statusLabel->setPosition(20, 570);
    statusLabel->setTextSize(18);
    statusLabel->setSize(700, 30);
    statusLabel->getRenderer()->setTextColor(tgui::Color::Black);
    gui.add(statusLabel);

    // Botón de eliminar con uso correcto de Deleter y verificacion de index:
    eliminarBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                bool usarMayor = metodoBox->getSelectedItemIndex() == 0;
                Deleter::remove(tree, valor, usarMayor);

                std::string mensaje;
                for (int i = 0; i < Deleter::mensajeCount; ++i) {
                    mensaje += Deleter::mensajes[i] + " ";
                }
                statusLabel->setText(mensaje);

                Traversal::animationQueue.clear();
                Search::animationQueue.clear();
                Traversal::currentResult.clear();
                Search::currentResult.clear();

                input->setText("");
            } catch (...) {
                statusLabel->setText("Error al convertir el valor ingresado para eliminar.");
            }
        }
    });

    // Botón para balancear AVL
    auto avlBtn = tgui::Button::create("Balancear AVL");
    avlBtn->setPosition(600, 60);
    avlBtn->setSize(130, 30);
    gui.add(avlBtn);

    avlBtn->onPress([&]() {
        Balancer::rotacionIndex = 0;
        Balancer::animationQueue.clear();

        tree.setRoot(Balancer::balance(tree.getRoot()));
        tree.updatePositions();

        if (Balancer::rotacionIndex == 0) {
            statusLabel->setText("AVL: No se realizaron rotaciones.");
        }

        Traversal::animationQueue.clear();
        Search::animationQueue.clear();
        Traversal::currentResult.clear();
        Search::currentResult.clear();
    });

    // ComboBox para tipo de recorrido
    auto recorridoBox = tgui::ComboBox::create();
    recorridoBox->setPosition(400, 20);
    recorridoBox->setSize(100, 30);
    recorridoBox->addItem("Inorden");
    recorridoBox->addItem("Preorden");
    recorridoBox->addItem("Postorden");
    recorridoBox->setDefaultText("Recorrido");
    recorridoBox->setTextSize(10);
    gui.add(recorridoBox);

    recorridoBox->onItemSelect([&]() {
        std::string selected = recorridoBox->getSelectedItem().toStdString();
        // Limpiar animaciones y textos previos
        Traversal::animationQueue.clear();
        Search::animationQueue.clear();
        Search::currentResult.clear();

        if (selected == "Inorden") {
            Traversal::inorder(tree.getRoot(), tree.getRoot(), window, font);
        } else if (selected == "Preorden") {
            Traversal::preorder(tree.getRoot(), tree.getRoot(), window, font);
        } else if (selected == "Postorden") {
            Traversal::postorder(tree.getRoot(), tree.getRoot(), window, font);
        }
    });

    // ComboBox para tipo de búsqueda
    auto searchTypeBox = tgui::ComboBox::create();
    searchTypeBox->setPosition(20, 60);
    searchTypeBox->setSize(200, 30);
    searchTypeBox->addItem("Busqueda en Profundidad");
    searchTypeBox->addItem("Busqueda en Anchura");
    searchTypeBox->setDefaultText("Tipo de Busqueda");
    searchTypeBox->setTextSize(12);
    gui.add(searchTypeBox);

    // Botón para buscar
    auto searchBtn = tgui::Button::create("Buscar");
    searchBtn->setPosition(230, 60);
    searchBtn->setSize(80, 30);
    gui.add(searchBtn);

    searchBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        std::string searchType = searchTypeBox->getSelectedItem().toStdString();

        if (texto.empty()) return;

        if (searchType.empty()) {
            std::cout << "Seleccione un tipo de busqueda." << std::endl;
            return;
        }

        try {
            int valor = std::stoi(texto);

            // Limpiar animaciones y textos previos
            Traversal::animationQueue.clear();
            Search::animationQueue.clear();
            Traversal::currentResult.clear();

            if (searchType == "Busqueda en Profundidad") {
                Search::depthFirstSearch(tree.getRoot(), valor, window, font);
            } else if (searchType == "Busqueda en Anchura") {
                Search::breadthFirstSearch(tree.getRoot(), valor, window, font);
            }
            input->setText("");
        } catch (...) {
            std::cout << "Entrada inválida para búsqueda." << std::endl;
        }
    });

    // Bucle principal
    sf::Clock animationClock;
    Node* currentHighlight = nullptr;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        tree.draw(window, font);

        // Mostrar animación y texto
        static bool animacionTerminada = false;
        static sf::Clock finalDelayClock;

        if (!Search::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 700) {
                currentHighlight = Search::animationQueue.dequeue();
                animationClock.restart();
                if (Search::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Search::drawResult(window, font);
        } else if (!Traversal::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 700) {
                currentHighlight = Traversal::animationQueue.dequeue();
                animationClock.restart();
                if (Traversal::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Traversal::drawResult(window, font);
        } else if (animacionTerminada) {
            if (finalDelayClock.getElapsedTime().asMilliseconds() > 2000) {
                currentHighlight = nullptr;
                animacionTerminada = false;
            }
        } else if (!Balancer::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 1000) {
                currentHighlight = Balancer::animationQueue.dequeue();
                animationClock.restart();
                if (Balancer::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Balancer::drawResult(window, font);
        }
        else {
            currentHighlight = nullptr;
        }

        gui.draw();

        if (currentHighlight) {
            sf::CircleShape highlight(20.f);
            highlight.setFillColor(sf::Color::Yellow);
            highlight.setPosition(currentHighlight->x - 20, currentHighlight->y - 20);
            window.draw(highlight);
        }

        window.display();
    }

    return 0;
}
