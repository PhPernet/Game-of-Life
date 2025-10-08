g++ -std=c++20 game.cpp renderer_sfml.cpp grid.cpp -o GameOfLife \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lsfml-graphics -lsfml-window -lsfml-system
