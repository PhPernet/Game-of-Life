#include "renderer_sfml.h"
#include <iostream>
#include <optional>

RendererSFML::RendererSFML(int w, int h, int cellSize_)
    : windowWidth(w), windowHeight(h), cellSize(cellSize_),
      window(sf::VideoMode({static_cast<unsigned int>(w), static_cast<unsigned int>(h)}), "Game of Life"),
      view({0.f, 0.f}, {static_cast<float>(w), static_cast<float>(h)}),
      zoomLevel(1.f), moveSpeed(500.f)
{
    window.setFramerateLimit(60);
}

void RendererSFML::drawGrid(const Grid& grid) {
    window.clear(sf::Color::Black);

    const auto& cells = grid.getCells();
    for (int y = 0; y < grid.getHeight(); ++y) {
        for (int x = 0; x < grid.getWidth(); ++x) {
            if (cells[y][x].getState() == 1) {
                sf::RectangleShape rect;
                rect.setSize({static_cast<float>(cellSize), static_cast<float>(cellSize)});
                rect.setPosition({static_cast<float>(x * cellSize - windowWidth / 2),
                    static_cast<float>(y * cellSize - windowHeight / 2)});
                rect.setFillColor(sf::Color::White);
                window.draw(rect);
            }
        }
    }

    window.setView(view);
    window.display();
}

void RendererSFML::handleEvents() {
    float dt = 5.f / 60.f;
    
    while (auto eventOpt = window.pollEvent()) {
        const sf::Event& event = *eventOpt;
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* resized = event.getIf<sf::Event::Resized>()) {
            sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
            window.setView(sf::View(visibleArea));
            std::cout << "New size: " << resized->size.x << "x" << resized->size.y << "\n";
        }
        // else if (event.isMouseWheelScrolled()) {
        //     const auto& scroll = event.asMouseWheelScroll();
        //     if (scroll.wheel == sf::Mouse::VerticalWheel) {
        //         zoomLevel *= (scroll.delta > 0) ? 0.9f : 1.1f;
        //         view.setSize(windowWidth * zoomLevel, windowHeight * zoomLevel);
        //     }
        // }

        // Keyboard view movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            view.move({-moveSpeed * dt, 0});
            std::cout << "View center: (" << view.getCenter().x << ", " << view.getCenter().y << ")\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            view.move({moveSpeed * dt, 0});
            std::cout << "View center: (" << view.getCenter().x << ", " << view.getCenter().y << ")\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            view.move({0, -moveSpeed * dt});
            std::cout << "View center: (" << view.getCenter().x << ", " << view.getCenter().y << ")\n";
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            view.move({0, moveSpeed * dt});
            std::cout << "View center: (" << view.getCenter().x << ", " << view.getCenter().y << ")\n";
        }

    }

}

// void RendererSFML::updateView() {
//     window.setView(view);
// }
