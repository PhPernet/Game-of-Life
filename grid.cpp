#include <vector>
#include <iostream>
#include "grid.h"
#include "cell.h"

// Constructor with default dimensions 200x100
Grid::Grid(int w, int h) : width(w), height(h), cells(h, std::vector<Cell>(w, Cell(0))) {}

int Grid::getWidth() const {
    return width;
}

int Grid::getHeight() const {
    return height;
}

const std::vector<std::vector<Cell>>& Grid::getCells() const {
    return cells;
}

void Grid::setCell(int x, int y, int value) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        cells[y][x].setState(value);
    }
}

int Grid::evolveCell(int x, int y) const {
    int liveNeighbors = 0;

    // Iterate over the 8 neighbors
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Skip the cell itself
            int nx = x + dx;
            int ny = y + dy;
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                liveNeighbors += cells[ny][nx].getState();
            }
        }
    }

    // Apply the rules of the Game of Life
    if (cells[y][x].getState() == 1) {
        // A live cell survives with 2 or 3 live neighbors
        return (liveNeighbors == 2 || liveNeighbors == 3) ? 1 : 0;
    } else {
        // A dead cell becomes alive with exactly 3 live neighbors
        return (liveNeighbors == 3) ? 1 : 0;
    }
}

int Grid::evolve() {
    std::vector<std::vector<Cell>> newCells = cells;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            newCells[y][x].setState(evolveCell(x, y));
        }
    }

    cells = newCells;
    return 0;
}

void Grid::random_init() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x].setState((rand() % 4 == 0) ? 1 : 0);
        }
    }
}

void Grid::print() const {
    // Clean the console and place the cursor at the top left
    std::cout << "\033[2J\033[H";
    for (const auto& row : cells) {
        for (const auto& cell : row) {
            cell.print();
        }
        std::cout << '\n';
    }
}