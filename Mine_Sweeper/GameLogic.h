#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <random>
#include <windows.h>


#undef min
#undef max

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
    int counter;
    int mines;
};

void clearConsoleScreen();
