#pragma once
#include <vector>
#include <iostream>
#include "cell.h"

class Grid {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;

public:
    Grid(int w = 200, int h = 100);
    int getWidth() const;
    int getHeight() const;
    const std::vector<std::vector<Cell>>& getCells() const;
    void setCell(int x, int y, int value);
    int evolveCell(int x, int y) const;
    int evolve();
    void random_init();
    void print() const;
};
