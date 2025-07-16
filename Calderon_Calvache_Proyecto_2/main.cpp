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
    setlocale(LC_ALL, " ");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Arbol Binario Visual con TGUI");
    tgui::Gui gui(window);
    //intento de que se pueda desplazar
    sf::View view = window.getDefaultView();
    float scrollSpeed = 30.f;

    sf::Font font;
    if (!font.loadFromFile("fuente/MinecraftTen.ttf")) {
        std::cerr << "No se pudo cargar la fuente MinecraftTen.ttf" << std::endl;
        return -1;
    }

    BinaryTree tree;
    Pantalla pantallaActual = Pantalla::PRINCIPAL;

    bool textoVisible = false;

    auto input = tgui::EditBox::create();
    input->setPosition(20, 20);
    input->setSize(100, 30);
    input->setDefaultText("Numero ejm: 0");
    gui.add(input);

    auto statusLabel = tgui::Label::create();
    statusLabel->setPosition(20, 560);
    statusLabel->setTextSize(18);
    statusLabel->setSize(760, 30);
    statusLabel->getRenderer()->setTextColor(tgui::Color::Black);
    gui.add(statusLabel);

    sf::Clock statusClock;
    bool statusVisible = false;

    auto insertarBtn = tgui::Button::create("Insertar");
    insertarBtn->setPosition(130, 20);
    insertarBtn->setSize(80, 30);

    insertarBtn->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    insertarBtn->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(insertarBtn);
    insertarBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                bool inserted = tree.insert(valor);
                if (inserted) {
                    tree.updatePositions();
                    Traversal::animationQueue.clear();
                    Search::animationQueue.clear();
                    Traversal::currentResult.clear();
                    Search::currentResult.clear();
                    input->setText("");
                    statusLabel->setText("Numero insertado exitosamente");
                } else {
                    statusLabel->setText("Número repetido, intente nuevamente");
                }
                statusClock.restart();
                statusVisible = true;
            } catch (...) {
                statusLabel->setText("Entrada invalida para insertar.");
                statusClock.restart();
                statusVisible = true;
            }
        }
    });

    auto btnRecorrido = tgui::Button::create("Recorridos");
    btnRecorrido->setPosition(230, 20);
    btnRecorrido->setSize(100, 30);

    btnRecorrido->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnRecorrido->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(btnRecorrido);
    btnRecorrido->onPress([&]() {
        pantallaActual = Pantalla::RECORRIDO;
        Traversal::currentResult.clear();
        Search::currentResult.clear();
        textoVisible = false;
    });

    auto btnBusqueda = tgui::Button::create("Busqueda");
    btnBusqueda->setPosition(340, 20);
    btnBusqueda->setSize(100, 30);

    btnBusqueda->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnBusqueda->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(btnBusqueda);
    btnBusqueda->onPress([&]() {
        pantallaActual = Pantalla::BUSQUEDA;
        Traversal::currentResult.clear();
        Search::currentResult.clear();
    });

    auto btnEliminar = tgui::Button::create("Eliminar");
    btnEliminar->setPosition(450, 20);
    btnEliminar->setSize(100, 30);

    btnEliminar->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnEliminar->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(btnEliminar);
    btnEliminar->onPress([&]() { pantallaActual = Pantalla::ELIMINAR; });

    auto btnBalancear = tgui::Button::create("Balancear");
    btnBalancear->setPosition(560, 20);
    btnBalancear->setSize(100, 30);

    btnBalancear->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnBalancear->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(btnBalancear);
    btnBalancear->onPress([&]() {
        pantallaActual = Pantalla::BALANCEO;
        Traversal::animationQueue.clear();
        Search::animationQueue.clear();
        Balancer::animationQueue.clear();

        Traversal::currentResult.clear();
        Search::currentResult.clear();
        Balancer::clear();

        textoVisible = false;
    });

    auto btnVolver = tgui::Button::create("Volver");
    btnVolver->setPosition(680, 20);
    btnVolver->setSize(100, 30);

    btnVolver->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnVolver->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);

    gui.add(btnVolver);
    btnVolver->onPress([&]() { pantallaActual = Pantalla::PRINCIPAL; });

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

        textoVisible = true;//
    });

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
            statusLabel->setText("Entrada invalida para busqueda.");
            statusClock.restart();
            statusVisible = true;
        }
    });

    auto metodoBox = tgui::ComboBox::create();
    metodoBox->setPosition(20, 60);
    metodoBox->setSize(300, 30);
    metodoBox->addItem("Reemplazar por el mayor del subarbol izquierdo");
    metodoBox->addItem("Reemplazar por el menor del subarbol derecho");
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
                statusClock.restart();
                statusVisible = true;
            } catch (...) {
                statusLabel->setText("Error al eliminar nodo.");
                statusClock.restart();
                statusVisible = true;
            }
        }
    });

    auto avlBtn = tgui::Button::create("Aplicar AVL");
    avlBtn->setPosition(20, 60);
    avlBtn->setSize(120, 30);
    gui.add(avlBtn);
    avlBtn->onPress([&]() {
        Balancer::animationQueue.clear();
        Balancer::rotacionIndex = 0;
        tree.setRoot(Balancer::balance(tree.getRoot()));
        tree.updatePositions();
        Traversal::animationQueue.clear();
        Search::animationQueue.clear();
        Traversal::currentResult.clear();
        Search::currentResult.clear();
        statusLabel->setText("AVL aplicado.");
        statusClock.restart();
        statusVisible = true;
        textoVisible = true;
    });

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
    //barrita
        auto sliderHorizontal = tgui::Slider::create();
        sliderHorizontal->setPosition(0, 580);
        sliderHorizontal->setSize(780, 20);
        sliderHorizontal->setMinimum(0);
        sliderHorizontal->setMaximum(1000);
        sliderHorizontal->setValue(500);  // centro
        gui.add(sliderHorizontal);

        auto sliderVertical = tgui::Slider::create();
        sliderVertical->setPosition(780, 0);
        sliderVertical->setSize(20, 560);
        sliderVertical->setMinimum(0);
        sliderVertical->setMaximum(1000);
        sliderVertical->setValue(500); // centro
        sliderVertical->setVerticalScroll(true);
        gui.add(sliderVertical);

        // Vínculo con la vista
        sliderHorizontal->onValueChange([&](float value){
            float centerX = value; // puedes ajustar con un factor si se mueve muy lento o muy rápido
            view.setCenter(centerX, view.getCenter().y);
        });

        sliderVertical->onValueChange([&](float value){
            float centerY = value;
            view.setCenter(view.getCenter().x, centerY);
        });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        setVisibilidad(pantallaActual);
        //pa que todo se renderice bien
        window.setView(view);

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
            if (finalDelayClock.getElapsedTime().asMilliseconds() > 3000) {
                currentHighlight = nullptr;
                animacionTerminada = false;
            }
        } else {
            currentHighlight = nullptr;
        }

        if (textoVisible) {
            Traversal::drawResult(window, font);
            Search::drawResult(window, font);
            Balancer::drawResult(window, font);
        }

        if (statusVisible && statusClock.getElapsedTime().asSeconds() > 2.f) {
            statusLabel->setText("");
            statusVisible = false;
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
