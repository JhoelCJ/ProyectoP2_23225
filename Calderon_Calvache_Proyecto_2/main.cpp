#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "BinaryTree.h"
#include "Balanceo.h"
#include "RecorridoIN.h"
#include "Eliminar.h"
#include "Busqueda.h"

enum class Pantalla { PRINCIPAL, RECORRIDO, BUSQUEDA, ELIMINAR, BALANCEO };

int main() {
    setlocale(LC_ALL, "");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Árbol Binario Visual con TGUI");
    tgui::Gui gui(window);

    // Cargar fuente personalizada
    sf::Font font;
    if (!font.loadFromFile("fuente/MinecraftTen.ttf")) {
        std::cerr << "No se pudo cargar la fuente MinecraftTen.ttf" << std::endl;
        return -1;
    }

    BinaryTree tree;
    Pantalla pantallaActual = Pantalla::PRINCIPAL;

    // Crear elementos reutilizables
    auto input = tgui::EditBox::create();
    input->setPosition(20, 20);
    input->setSize(100, 30);
    input->setDefaultText("Numero [0]");
    gui.add(input);

    auto statusLabel = tgui::Label::create();
    statusLabel->setPosition(20, 560);
    statusLabel->setTextSize(18);
    statusLabel->setSize(760, 30);
    statusLabel->getRenderer()->setTextColor(tgui::Color::Black);
    gui.add(statusLabel);

    // Botón Insertar
    auto insertarBtn = tgui::Button::create("Insertar");
    insertarBtn->setPosition(130, 20);
    insertarBtn->setSize(80, 30);
    gui.add(insertarBtn);
    insertarBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                tree.insert(valor);
                tree.updatePositions();
                Traversal::animationQueue.clear();
                Search::animationQueue.clear();
                Traversal::currentResult.clear();
                Search::currentResult.clear();
                input->setText("");
            } catch (...) {
                statusLabel->setText("Entrada inválida para insertar.");
            }
        }
    });

    // Botones de navegación
    auto btnRecorrido = tgui::Button::create("Recorridos");
    btnRecorrido->setPosition(230, 20);
    btnRecorrido->setSize(100, 30);
    gui.add(btnRecorrido);
    btnRecorrido->onPress([&]() { pantallaActual = Pantalla::RECORRIDO; });

    auto btnBusqueda = tgui::Button::create("Busqueda");
    btnBusqueda->setPosition(340, 20);
    btnBusqueda->setSize(100, 30);
    gui.add(btnBusqueda);
    btnBusqueda->onPress([&]() { pantallaActual = Pantalla::BUSQUEDA; });

    auto btnEliminar = tgui::Button::create("Eliminar");
    btnEliminar->setPosition(450, 20);
    btnEliminar->setSize(100, 30);
    gui.add(btnEliminar);
    btnEliminar->onPress([&]() { pantallaActual = Pantalla::ELIMINAR; });

    auto btnBalancear = tgui::Button::create("Balancear");
    btnBalancear->setPosition(560, 20);
    btnBalancear->setSize(100, 30);
    gui.add(btnBalancear);
    btnBalancear->onPress([&]() { pantallaActual = Pantalla::BALANCEO; });

    auto btnVolver = tgui::Button::create("Volver");
    btnVolver->setPosition(680, 20);
    btnVolver->setSize(100, 30);
    gui.add(btnVolver);
    btnVolver->onPress([&]() { pantallaActual = Pantalla::PRINCIPAL; });

    // ComboBox para Recorridos
    auto recorridoBox = tgui::ComboBox::create();
    recorridoBox->setPosition(20, 60);
    recorridoBox->setSize(200, 30);
    recorridoBox->addItem("Inorden");
    recorridoBox->addItem("Preorden");
    recorridoBox->addItem("Postorden");
    recorridoBox->setDefaultText("Recorrido");
    gui.add(recorridoBox);
    recorridoBox->onItemSelect([&]() {
        std::string selected = recorridoBox->getSelectedItem().toStdString();
        Traversal::animationQueue.clear();
        Traversal::currentResult.clear();
        Search::animationQueue.clear();
        Search::currentResult.clear();

        if (selected == "Inorden")
            Traversal::inorder(tree.getRoot(), tree.getRoot(), window, font);
        else if (selected == "Preorden")
            Traversal::preorder(tree.getRoot(), tree.getRoot(), window, font);
        else if (selected == "Postorden")
            Traversal::postorder(tree.getRoot(), tree.getRoot(), window, font);
    });

    // ComboBox + botón para Búsqueda
    auto searchTypeBox = tgui::ComboBox::create();
    searchTypeBox->setPosition(20, 60);
    searchTypeBox->setSize(200, 30);
    searchTypeBox->addItem("Busqueda en Profundidad");
    searchTypeBox->addItem("Busqueda en Anchura");
    searchTypeBox->setDefaultText("Tipo de Busqueda");
    gui.add(searchTypeBox);

    auto searchBtn = tgui::Button::create("Buscar");
    searchBtn->setPosition(230, 60);
    searchBtn->setSize(80, 30);
    gui.add(searchBtn);
    searchBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        std::string tipo = searchTypeBox->getSelectedItem().toStdString();
        if (texto.empty() || tipo.empty()) return;
        try {
            int valor = std::stoi(texto);
            Traversal::animationQueue.clear();
            Search::animationQueue.clear();
            Traversal::currentResult.clear();
            Search::currentResult.clear();
            if (tipo == "Busqueda en Profundidad")
                Search::depthFirstSearch(tree.getRoot(), valor, window, font);
            else
                Search::breadthFirstSearch(tree.getRoot(), valor, window, font);
            input->setText("");
        } catch (...) {
            statusLabel->setText("Entrada inválida para búsqueda.");
        }
    });

    // ComboBox para método de eliminación
    auto metodoBox = tgui::ComboBox::create();
    metodoBox->setPosition(20, 60);
    metodoBox->setSize(300, 30);
    metodoBox->addItem("Reemplazar por el mayor del subárbol izquierdo");
    metodoBox->addItem("Reemplazar por el menor del subárbol derecho");
    metodoBox->setSelectedItemByIndex(0);
    gui.add(metodoBox);

    auto eliminarRealBtn = tgui::Button::create("Eliminar Nodo");
    eliminarRealBtn->setPosition(340, 60);
    eliminarRealBtn->setSize(100, 30);
    gui.add(eliminarRealBtn);
    eliminarRealBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                bool usarMayor = metodoBox->getSelectedItemIndex() == 0;
                Deleter::remove(tree, valor, usarMayor);
                tree.updatePositions();
                std::string mensaje;
                for (int i = 0; i < Deleter::mensajeCount; ++i)
                    mensaje += Deleter::mensajes[i] + " ";
                statusLabel->setText(mensaje);
                Traversal::animationQueue.clear();
                Search::animationQueue.clear();
                Traversal::currentResult.clear();
                Search::currentResult.clear();
                input->setText("");
            } catch (...) {
                statusLabel->setText("Error al eliminar nodo.");
            }
        }
    });

    auto avlBtn = tgui::Button::create("Aplicar AVL");
    avlBtn->setPosition(20, 60);
    avlBtn->setSize(120, 30);
    gui.add(avlBtn);
    avlBtn->onPress([&]() {
        Balancer::animationQueue.clear();
        tree.setRoot(Balancer::balance(tree.getRoot()));
        tree.updatePositions();
        Traversal::animationQueue.clear();
        Search::animationQueue.clear();
        Traversal::currentResult.clear();
        Search::currentResult.clear();
        statusLabel->setText("AVL aplicado.");
    });

    // Mostrar u ocultar widgets según la pantalla actual
    auto setVisibilidad = [&](Pantalla pantalla) {
        recorridoBox->setVisible(pantalla == Pantalla::RECORRIDO);
        searchTypeBox->setVisible(pantalla == Pantalla::BUSQUEDA);
        searchBtn->setVisible(pantalla == Pantalla::BUSQUEDA);
        metodoBox->setVisible(pantalla == Pantalla::ELIMINAR);
        eliminarRealBtn->setVisible(pantalla == Pantalla::ELIMINAR);
        avlBtn->setVisible(pantalla == Pantalla::BALANCEO);
    };

    sf::Clock animationClock;
    Node* currentHighlight = nullptr;
    bool animacionTerminada = false;
    sf::Clock finalDelayClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        setVisibilidad(pantallaActual);

        window.clear(sf::Color::White);
        tree.draw(window, font);

        if (!Search::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Search::animationQueue.dequeue();
                animationClock.restart();
                if (Search::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Search::drawResult(window, font);
        } else if (!Traversal::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Traversal::animationQueue.dequeue();
                animationClock.restart();
                if (Traversal::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Traversal::drawResult(window, font);
        } else if (!Balancer::animationQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Balancer::animationQueue.dequeue();
                animationClock.restart();
                if (Balancer::animationQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Balancer::drawResult(window, font);
        } else if (animacionTerminada) {
            if (finalDelayClock.getElapsedTime().asMilliseconds() > 2000) {
                currentHighlight = nullptr;
                animacionTerminada = false;
            }
        } else {
            currentHighlight = nullptr;
        }

        if (currentHighlight) {
            sf::CircleShape highlight(20.f);
            highlight.setFillColor(sf::Color::Yellow);
            highlight.setPosition(currentHighlight->x - 20, currentHighlight->y - 20);
            window.draw(highlight);
        }

        gui.draw();
        window.display();
    }

    return 0;
}
