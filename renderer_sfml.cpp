#include "renderer_sfml.h"
#include <iostream>

static const std::string vertexShaderSrc = R"(
    void main()
    {
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
        gl_TexCoord[0] = gl_MultiTexCoord0;
    }
)";

static const std::string fragmentShaderSrc = R"(
    uniform sampler2D stateTex;
    uniform sampler2D aliveTex;
    uniform sampler2D deadTex;
    uniform vec2 gridSize;

    void main()
    {
        // texcoord over full screen quad [0,1]
        vec2 uv = gl_TexCoord[0].xy;

        // map to grid cell coordinates
        vec2 cellCoord = uv * gridSize;

        // sample the state texture at the cell center (nearest cell)
        vec2 cellTexCoord = (floor(cellCoord) + 0.5) / gridSize;
        float state = texture2D(stateTex, cellTexCoord).r;

        // local coordinates inside the cell for sampling alive/dead textures
        vec2 local = fract(cellCoord);

        vec4 aliveColor = texture2D(aliveTex, local);
        vec4 deadColor  = texture2D(deadTex, local);

        vec4 color = mix(deadColor, aliveColor, state);
        gl_FragColor = color;
    }
)";

RendererSFML::RendererSFML(int w, int h, int cellSize_)
    : windowWidth(w), windowHeight(h), cellSize(cellSize_),
      window(sf::VideoMode({static_cast<unsigned int>(w), static_cast<unsigned int>(h)}), "Game of Life"),
      view({0.f, 0.f}, {static_cast<float>(w), static_cast<float>(h)}),
      zoomLevel(1.f), moveSpeed(500.f)
{
    window.setFramerateLimit(60);

    // Compile shader
    if (!shader.loadFromMemory(vertexShaderSrc, fragmentShaderSrc)) {
        std::cerr << "Erreur: impossible de compiler le shader\n";
    }

    // Créer textures alive / dead (procédural, taille = cellSize x cellSize)
    sf::Image imgAlive;
    sf::Image imgDead;
    imgAlive.create(cellSize, cellSize, sf::Color::White);
    imgDead.create(cellSize, cellSize, sf::Color::Black);
    // Optionnel : ajouter un léger motif pour alive (bord sombre) — ici simple blanc/noir
    aliveTexture.loadFromImage(imgAlive);
    deadTexture.loadFromImage(imgDead);
    aliveTexture.setSmooth(false);
    deadTexture.setSmooth(false);

    // stateImage laissé vide ; sera créé à la première mise à jour
}

void RendererSFML::updateStateTexture(const Grid& grid) {
    // Remplir stateImage depuis la grille
    grid.fillImage(stateImage);

    // (re)créer la stateTexture si nécessaire
    if (stateTexture.getSize().x != stateImage.getSize().x || stateTexture.getSize().y != stateImage.getSize().y) {
        stateTexture.create(stateImage.getSize().x, stateImage.getSize().y);
    }
    stateTexture.update(stateImage);
    stateTexture.setSmooth(false);
}

void RendererSFML::drawGrid(const Grid& grid) {
    // Mettre à jour la texture d'état
    updateStateTexture(grid);

    window.clear(sf::Color::Black);
    window.setView(view);

    // Préparer un quad plein écran avec coords [0,1] pour le shader
    sf::VertexArray quad(sf::TrianglesStrip, 4);
    quad[0].position = sf::Vector2f(0.f, 0.f);
    quad[1].position = sf::Vector2f(static_cast<float>(windowWidth), 0.f);
    quad[2].position = sf::Vector2f(0.f, static_cast<float>(windowHeight));
    quad[3].position = sf::Vector2f(static_cast<float>(windowWidth), static_cast<float>(windowHeight));

    quad[0].texCoords = sf::Vector2f(0.f, 0.f);
    quad[1].texCoords = sf::Vector2f(1.f, 0.f);
    quad[2].texCoords = sf::Vector2f(0.f, 1.f);
    quad[3].texCoords = sf::Vector2f(1.f, 1.f);

    // Passer les textures et uniforms au shader
    shader.setUniform("stateTex", stateTexture);
    shader.setUniform("aliveTex", aliveTexture);
    shader.setUniform("deadTex", deadTexture);
    shader.setUniform("gridSize", sf::Glsl::Vec2(static_cast<float>(grid.getWidth()), static_cast<float>(grid.getHeight())));

    // Dessiner le quad plein écran via le shader
    window.draw(quad, &shader);

    window.display();
}

void RendererSFML::handleEvents() {
    float dt = 5.f / 60.f;

    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
            window.setView(sf::View(visibleArea));
            windowWidth = event.size.width;
            windowHeight = event.size.height;
            std::cout << "New size: " << event.size.width << "x" << event.size.height << "\n";
        }
        // mouvements clavier
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            view.move({-moveSpeed * dt, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            view.move({moveSpeed * dt, 0});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            view.move({0, -moveSpeed * dt});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            view.move({0, moveSpeed * dt});
        }
    }
}
