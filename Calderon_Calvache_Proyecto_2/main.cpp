#include <TGUI/TGUI.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "ArbolBinario.h"
#include "Balanceo.h"
#include "Recorrido.h"
#include "Eliminar.h"
#include "Busqueda.h"

enum class Pantalla { PRINCIPAL, RECORRIDO, BUSQUEDA, ELIMINAR, BALANCEO };

int main() {
    setlocale(LC_ALL, " ");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Arbol Binario Visual con TGUI");
    tgui::Gui gui(window);

    sf::View view = window.getDefaultView();
    float scrollSpeed = 30.f;

    sf::Font font;
    if (!font.loadFromFile("fuente/MinecraftTen.ttf")) {
        std::cerr << "No se pudo cargar la fuente MinecraftTen.ttf" << std::endl;
        return -1;
    }

    ArbolBinario arbol;
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
    bool estaVisible = false;

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
                bool inserted = arbol.insert(valor);
                if (inserted) {
                    arbol.actualizarPosicion();
                    Recorrido::animacionQueue.clear();
                    Buscar::animacionQueue.clear();
                    Recorrido::resultadoReal.clear();
                    Buscar::resultadoReal.clear();
                    input->setText("");
                    statusLabel->setText("Numero insertado exitosamente");
                } else {
                    statusLabel->setText("Numero repetido, intente nuevamente");
                }
                statusClock.restart();
                estaVisible = true;
            } catch (...) {
                statusLabel->setText("Entrada invalida para insertar.");
                statusClock.restart();
                estaVisible = true;
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
        Recorrido::resultadoReal.clear();
        Buscar::resultadoReal.clear();
        Balancear::clear();
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
        Recorrido::resultadoReal.clear();
        Buscar::resultadoReal.clear();
        Balancear::clear();
        textoVisible = false;
    });

    auto btnEliminar = tgui::Button::create("Eliminar");
    btnEliminar->setPosition(450, 20);
    btnEliminar->setSize(100, 30);
    btnEliminar->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnEliminar->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
    gui.add(btnEliminar);
    btnEliminar->onPress([&]() {
        pantallaActual = Pantalla::ELIMINAR;
        Balancear::clear();
        textoVisible = false;
    });

    auto btnBalancear = tgui::Button::create("Balancear");
    btnBalancear->setPosition(560, 20);
    btnBalancear->setSize(100, 30);
    btnBalancear->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnBalancear->getRenderer()->setBackgroundColorHover(tgui::Color::Yellow);
    gui.add(btnBalancear);
    btnBalancear->onPress([&]() {
        pantallaActual = Pantalla::BALANCEO;
        Recorrido::animacionQueue.clear();
        Buscar::animacionQueue.clear();
        Balancear::animacionQueue.clear();
        Recorrido::resultadoReal.clear();
        Buscar::resultadoReal.clear();
        Balancear::clear();
        estaVisible = false;
        textoVisible = false;
    });

    auto btnSalir = tgui::Button::create("Salir :(");
    btnSalir->setPosition(680, 20);
    btnSalir->setSize(100, 30);
    btnSalir->getRenderer()->setBackgroundColor(tgui::Color::Cyan);
    btnSalir->getRenderer()->setBackgroundColorHover(tgui::Color::Red);
    gui.add(btnSalir);
    btnSalir->onPress([&]() { window.close(); });

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
        Recorrido::animacionQueue.clear();
        Recorrido::resultadoReal.clear();
        Buscar::animacionQueue.clear();
        Buscar::resultadoReal.clear();

        if (selected == "Inorden")
            Recorrido::inOrden(arbol.obtenerRaiz(), arbol.obtenerRaiz(), window, font);
        else if (selected == "Preorden")
            Recorrido::preOrdenRecursivo(arbol.obtenerRaiz(), arbol.obtenerRaiz(), window, font);
        else if (selected == "Postorden")
            Recorrido::postOrden(arbol.obtenerRaiz(), arbol.obtenerRaiz(), window, font);

        textoVisible = true;//
    });

    auto cajaBusqueda = tgui::ComboBox::create();
    cajaBusqueda->setPosition(20, 60);
    cajaBusqueda->setSize(200, 30);
    cajaBusqueda->addItem("Busqueda en Profundidad");
    cajaBusqueda->addItem("Busqueda en Anchura");
    cajaBusqueda->setDefaultText("Tipo de Busqueda");
    gui.add(cajaBusqueda);

    auto BuscarBtn = tgui::Button::create("Buscar");
    BuscarBtn->setPosition(230, 60);
    BuscarBtn->setSize(80, 30);
    gui.add(BuscarBtn);
    BuscarBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        std::string tipo = cajaBusqueda->getSelectedItem().toStdString();
        if (texto.empty() || tipo.empty()) return;
        try {
            int valor = std::stoi(texto);
            Recorrido::animacionQueue.clear();
            Buscar::animacionQueue.clear();
            Recorrido::resultadoReal.clear();
            Buscar::resultadoReal.clear();
            if (tipo == "Busqueda en Profundidad")
                Buscar::depthFirstBuscar(arbol.obtenerRaiz(), valor, window, font);
            else
                Buscar::breadthFirstBuscar(arbol.obtenerRaiz(), valor, window, font);
            input->setText("");
        } catch (...) {
            statusLabel->setText("Entrada invalida para busqueda.");
            statusClock.restart();
            estaVisible = true;
        }
    });

    auto metodoBox = tgui::ComboBox::create();
    metodoBox->setPosition(20, 60);
    metodoBox->setSize(175, 30);
    metodoBox->addItem("Mayor del Menor");
    metodoBox->addItem("Menor del Mayor");
    metodoBox->setSelectedItemByIndex(0);
    gui.add(metodoBox);

    auto eliminarRealBtn = tgui::Button::create("Eliminar Nodo");
    eliminarRealBtn->setPosition(200, 60);
    eliminarRealBtn->setSize(100, 30);
    gui.add(eliminarRealBtn);
    eliminarRealBtn->onPress([&]() {
        std::string texto = input->getText().toStdString();
        if (!texto.empty()) {
            try {
                int valor = std::stoi(texto);
                bool usarMayor = metodoBox->getSelectedItemIndex() == 0;
                bool eliminado = false;
                Eliminar::remove(arbol, valor, usarMayor, eliminado);
                arbol.actualizarPosicion();
                std::string mensaje;
                for (int i = 0; i < Eliminar::mensajeCount; ++i)
                    mensaje += Eliminar::mensajes[i] + " ";
                statusLabel->setText(mensaje);
                Recorrido::animacionQueue.clear();
                Buscar::animacionQueue.clear();
                Recorrido::resultadoReal.clear();
                Buscar::resultadoReal.clear();
                input->setText("");
                statusClock.restart();
                estaVisible = true;
            } catch (...) {
                statusLabel->setText("Error al eliminar nodo.");
                statusClock.restart();
                estaVisible = true;
            }
        }
    });

    auto avlBtn = tgui::Button::create("Aplicar AVL");
    avlBtn->setPosition(20, 60);
    avlBtn->setSize(120, 30);
    gui.add(avlBtn);
    avlBtn->onPress([&]() {
        Balancear::animacionQueue.clear();
        Balancear::rotacionIndex = 0;
        arbol.setRaiz(Balancear::balance(arbol.obtenerRaiz()));
        arbol.actualizarPosicion();
        Recorrido::animacionQueue.clear();
        Buscar::animacionQueue.clear();
        Recorrido::resultadoReal.clear();
        Buscar::resultadoReal.clear();
        statusLabel->setText("AVL aplicado.");
        statusClock.restart();
        estaVisible = true;
        textoVisible = true;
    });

    auto setVisibilidad = [&](Pantalla pantalla) {
        recorridoBox->setVisible(pantalla == Pantalla::RECORRIDO);
        cajaBusqueda->setVisible(pantalla == Pantalla::BUSQUEDA);
        BuscarBtn->setVisible(pantalla == Pantalla::BUSQUEDA);
        metodoBox->setVisible(pantalla == Pantalla::ELIMINAR);
        eliminarRealBtn->setVisible(pantalla == Pantalla::ELIMINAR);
        avlBtn->setVisible(pantalla == Pantalla::BALANCEO);
    };

    sf::Clock animationClock;
    Node* currentHighlight = nullptr;
    bool animacionTerminada = false;
    sf::Clock finalDelayClock;

    auto sliderHorizontal = tgui::Slider::create();
    sliderHorizontal->setMinimum(0);
    sliderHorizontal->setMaximum(1000);
    sliderHorizontal->setValue(500);
    gui.add(sliderHorizontal);

    auto sliderVertical = tgui::Slider::create();
    sliderVertical->setVerticalScroll(true);
    sliderVertical->setMinimum(0);
    sliderVertical->setMaximum(1000);
    sliderVertical->setValue(500);
    gui.add(sliderVertical);

    auto actualizarSliders = [&](unsigned int width, unsigned int height) {
    sliderHorizontal->setSize(width - 10, 10);
    sliderHorizontal->setPosition(0, height - 10);

    sliderVertical->setSize(10, height - 10);
    sliderVertical->setPosition(width - 10, 0);

    sliderHorizontal->setMaximum(width * 2);
        sliderVertical->setMaximum(height * 2);
    };

    actualizarSliders(window.getSize().x, window.getSize().y);

    sliderHorizontal->onValueChange([&](float valor){
            view.setCenter(valor, view.getCenter().y);
    });

    sliderVertical->onValueChange([&](float valor){
            view.setCenter(view.getCenter().x, valor);
    });

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            if (event.type == sf::Event::Closed){
                window.close();
            }else if (event.type == sf::Event::Resized) {
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
                actualizarSliders(event.size.width, event.size.height);
            }
        }

        setVisibilidad(pantallaActual);

        window.setView(view);
        window.clear(sf::Color::White);
        arbol.draw(window, font);

        if (Eliminar::animacionesCount > 0) {
            EliminacionVisual& ev = Eliminar::animacionesEliminacion[0];
            ev.elapsedTime += animationClock.restart().asSeconds();
            float offset = 100.f * ev.elapsedTime;

            sf::CircleShape circulo(20.f);
            circulo.setFillColor(sf::Color::Red);
            circulo.setPosition(ev.node->x - 20, ev.yOriginal + offset);
            window.draw(circulo);

            if (offset > 200.f) {
                for (int i = 1; i < Eliminar::animacionesCount; ++i) {
                    Eliminar::animacionesEliminacion[i - 1] = Eliminar::animacionesEliminacion[i];
                }
                Eliminar::animacionesCount--;

                if (Eliminar::animacionesCount == 0) {
                    Eliminar::procesarEliminacionesPendientes();
                    arbol.actualizarPosicion();
                }
            }
}

        if (!Buscar::animacionQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Buscar::animacionQueue.dequeue();
                animationClock.restart();
                if (Buscar::animacionQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Buscar::mostrarResultado(window, font);
        } else if (!Recorrido::animacionQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Recorrido::animacionQueue.dequeue();
                animationClock.restart();
                if (Recorrido::animacionQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Recorrido::mostrarResultado(window, font);
        } else if (!Balancear::animacionQueue.isEmpty()) {
            if (animationClock.getElapsedTime().asMilliseconds() >= 500) {
                currentHighlight = Balancear::animacionQueue.dequeue();
                animationClock.restart();
                if (Balancear::animacionQueue.isEmpty()) {
                    animacionTerminada = true;
                    finalDelayClock.restart();
                }
            }
            Balancear::mostrarResultado(window, font);

        } else if (animacionTerminada) {
            if (finalDelayClock.getElapsedTime().asMilliseconds() > 3000) {
                currentHighlight = nullptr;
                animacionTerminada = false;
            }
        } else {
            currentHighlight = nullptr;
        }

        if (textoVisible) {
            Recorrido::mostrarResultado(window, font);
            Buscar::mostrarResultado(window, font);
            Balancear::mostrarResultado(window, font);
        }

        if (estaVisible && statusClock.getElapsedTime().asSeconds() > 2.f) {
            statusLabel->setText("");
            estaVisible = false;
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
