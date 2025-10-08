#pragma once
#include <SFML/Graphics.hpp>
#include "grid.h"

class RendererSFML {
private:
    sf::RenderWindow window;
    sf::View view;
    int windowWidth;
    int windowHeight;
    float zoomLevel;
    float moveSpeed;
    int cellSize;

public:
    RendererSFML(int w, int h, int cellSize = 10);
    void drawGrid(const Grid& grid);
    void handleEvents();
    bool isOpen() const { return window.isOpen(); }
    // void updateView();
    sf::RenderWindow& getWindow() { return window; }
};
