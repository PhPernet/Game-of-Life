#include "grid.h"
#include "renderer_sfml.h"
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

int main()
{
    int width = 80;
    int height = 60;
    int windowWidth = 800;
    int windowHeight = 600;
    int cellSize = 10;

    Grid grid(width, height);
    grid.random_init();

    RendererSFML renderer(windowWidth, windowHeight, cellSize);

    while (renderer.isOpen())
    {
        renderer.handleEvents();
        grid.evolve();
        renderer.drawGrid(grid);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        std::cout << "Win size: " << renderer.getWindow().getSize().x << "x" << renderer.getWindow().getSize().y << "\n";
    }

    return 0;
}