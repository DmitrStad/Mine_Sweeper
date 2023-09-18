#include "GameLogic.h"

Field::Field(int width, int height) {
    this->height = height;
    this->width = width;
    counter = 0;
    mines = rand() % ((width * height) / 10) + 5;
    field.resize(width, std::vector<bool>(height, false));
    is_cell_visible.resize(width, std::vector<bool>(height, false));
    neighbours.resize(width, std::vector<int>(height, 0));
    mark.resize(width, std::vector<bool>(height, false));
}

void Field::SetMines(int i, int j) {

    std::default_random_engine randomEngine(time(0)); // Генератор случайных чисел
    std::uniform_int_distribution<int> distribution(0, height - 1); // Равномерное распределение от 0 до N-1
    int tmp = mines;
    while (tmp > 0) {
        int x = distribution(randomEngine);
        int y = distribution(randomEngine);
        if (!field[x][y] && x != i && y != j) {
            field[x][y] = 1;
            tmp--;
        }
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Count_Mines(i, j);
        }
    }
}

void Field::Count_Mines(int x, int y) {
    for (unsigned int i = std::max(0, x - 1); i <= std::min(width - 1, x + 1); i++) {
        for (unsigned int j = std::max(0, y - 1); j <= std::min(height - 1, y + 1); j++) {
            if (i == x && j == y)
                continue;
            else if (field[i][j] == 1 && i >= 0 && i < height && j >= 0 && j < width)
                neighbours[x][y]++;
        }
    }
}

bool Field::OneStep(int x, int y) {
    if (field[x][y]) {
        PrintMatrix();
        std::cout << "Мина в точке " << x << " " << y << "\nИгра Окончена" << "\n";
        return 0;
    }

    else if (neighbours[x][y] == 0) {
        for (int i = std::max(0, x - 1); i <= std::min(width - 1, x + 1); i++) {
            for (int j = std::max(0, y - 1); j <= std::min(height - 1, y + 1); j++) {
                if (!field[i][j])
                    is_cell_visible[i][j] = 1;
                if (neighbours[i][j] == 0 && !field[i][j]) {
                    for (unsigned int n = std::max(0, i - 1); n <= std::min(width - 1, i + 1); n++) {
                        for (unsigned int m = std::max(0, j - 1); m <= std::min(height - 1, j + 1); m++) {
                            if (!field[n][m])
                                is_cell_visible[n][m] = 1;
                        }
                    }
                }
            }
        }
    }
    else
        is_cell_visible[x][y] = 1;

    PrintMatrix();

    return 1;
}

void Field::PrintMatrix() {
    clearConsoleScreen();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            if (is_cell_visible[i][j]) {
                std::cout << neighbours[i][j] << " ";
            }

            else if (mark[i][j])
                std::cout << "? ";

            else
                std::cout << "* ";
        }
        std::cout << "\n";
    }

    std::cout << "Мин найдено " << counter << " из " << mines << "\n";
}

bool Field::MarkCell(int x, int y) {
    mark[x][y] = 1;
    if (field[x][y])
        counter++;

    if (counter == mines) {
        PrintMatrix();
        std::cout << "Победа!";
        return 0;
    }
    PrintMatrix();
    return 1;
}

void clearConsoleScreen() { //функция очистки консоли
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
}