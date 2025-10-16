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

    // Textures / image d'état et textures pour alive/dead
    sf::Image stateImage;
    sf::Texture stateTexture;
    sf::Texture aliveTexture;
    sf::Texture deadTexture;
    sf::Shader shader;

public:
    RendererSFML(int w, int h, int cellSize = 10);
    void drawGrid(const Grid& grid);
    void handleEvents();
    bool isOpen() const { return window.isOpen(); }
    sf::RenderWindow& getWindow() { return window; }

private:
    void updateStateTexture(const Grid& grid);
};
