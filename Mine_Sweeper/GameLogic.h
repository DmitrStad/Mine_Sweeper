#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <windows.h>


#undef min
#undef max

struct Point {
public:
    Point(int x, int y) { this->x = x; this->y = y; }
    int Get_X() const { return x; }
    int Get_Y() const { return y; }
private:
    int x, y;
};

class Field
{
public:
    Field(int width, int height);
    void SetMines(int i, int j);
    void Count_Mines(int x, int y);
    bool OneStep(int x, int y);
    void PrintMatrix();
    bool MarkCell(int x, int y);
    void PrintDeadScreen(int x, int y);

private:
    int height;
    int width;
    std::vector<std::vector<bool>> field;
    std::vector<std::vector<bool>> is_cell_visible;
    std::vector<std::vector<int>> neighbours;
    std::vector<std::vector<bool>> mark;
    std::vector<Point> points;
    int counter;
    int mines;
};

void clearConsoleScreen();
