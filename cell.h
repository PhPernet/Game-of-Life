#pragma once
#include <iostream>

class Cell {
private:
    int state;
    int x, y;
    int structureID;
    int age;

public:
    Cell(int initialState = 0)
        : state(initialState), x(0), y(0), structureID(0), age(0) {}
    int getState() const { return state; }
    void setState(int newState) { state = newState; }
    void print() const {
        std::cout << (state ? '#' : ' ');
    }
};