#include "GameLogic.h"

/**
  * @brief  Конструктор структуры Field
  * @param  int width - ширина игрового поля
  * @param  int height - высота игрового поля
  * @retrival None
  */

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

/**
  * @brief  Функция, помещающая мины на игровое поле
  * @param  int i - координата мины по Ox
  * @param  int j - координата мины по Oy
  * @retrival None
  */

void Field::SetMines(int i, int j) {

    std::default_random_engine randomEngine(time(0)); 
    std::uniform_int_distribution<int> distribution(0, height - 1); 
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

/**
  * @brief  Счётчик мин на поле
  * @param  int x - координата клетки по Ox
  * @param  int y - координата клетки по Oy
  * @retrival None
  */

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

/**
  * @brief  Рачсчёт игроового поля после хода игрока
  * @param  int x - координата клетки по Ox
  * @param  int y - координата клетки по Oy
  * @retrival bool - 0 конец игры, 1 - игра продолжается
  */

bool Field::OneStep(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    if (field[x][y]) {
        PrintDeadScreen(x, y);
        std::cout << " Game Over" << "\n";
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

/**
  * @brief  Функция отрисовки игрового поля
  * @param  None
  * @retrival None
  */

void Field::PrintMatrix() {
    clearConsoleScreen();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            if (is_cell_visible[i][j] && !mark[i][j]) {
                SetConsoleTextAttribute(hConsole, 3);
                std::cout << neighbours[i][j] << " ";
            }

            else if (mark[i][j]) {
                SetConsoleTextAttribute(hConsole, 4);
                std::cout << "? ";
            }

            else if (is_cell_visible[i][j] && field[i][j]) {
                SetConsoleTextAttribute(hConsole, 5);
                std::cout << "M ";
            }

            else {
                SetConsoleTextAttribute(hConsole, 7);
                std::cout << "* ";
            }
        }
        std::cout << "\n";
    }
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "Mines Found " << counter << " of " << mines << " total\n";
}

/**
  * @brief  Пометка отжельно взятой клетки, в зависимости от её значения 
  * @param  int x - координата клетки по Ox
  * @param  int y - координата клетки по Oy
  * @retrival bool - 0 число расрытых мин равняется общему числу мин и игрок выигрываеи, 1 - игра продолжается
  */

bool Field::MarkCell(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 7);
    if (mark[x][y])
        mark[x][y] = 0;
    else
        mark[x][y] = 1;
    if (field[x][y] && mark[x][y]) {
        Point point(x, y);
        points.push_back(point);
        for (int i = 0; i < points.size() - 1; i++) {
            if (points[i].Get_X() == x && points[i].Get_Y() == y) {
                counter--;
                break;
            }
        }
        counter++;
    }

    if (counter == mines) {
        PrintMatrix();
        std::cout << "You won!";
        return 0;
    }
    PrintMatrix();
    return 1;
}

/**
  * @brief  Отрисовка игрового поля при окончании игры
  * @param  int x - координата клетки по Ox
  * @param  int y - координата клетки по Oy
  * @retrival None
  */

void Field::PrintDeadScreen(int x, int y) {
    clearConsoleScreen();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {

            if (is_cell_visible[i][j] && !mark[i][j]) {
                SetConsoleTextAttribute(hConsole, 3);
                std::cout << neighbours[i][j] << " ";
            }

            else if (mark[i][j]) {
                SetConsoleTextAttribute(hConsole, 4);
                std::cout << "? ";
            }

            else if (x == i && y == j && field[i][j]) {
                SetConsoleTextAttribute(hConsole, 5);
                std::cout << "M ";
            }

            else {
                SetConsoleTextAttribute(hConsole, 7);
                std::cout << "* ";
            }
        }
        std::cout << "\n";
    }
    SetConsoleTextAttribute(hConsole, 7);
    std::cout << "Mines Found " << counter << " of " << mines << " total\n";
}

**
  * @brief  Очистка игрового поля
  * @param  None
  * @retrival None
  */

void clearConsoleScreen() { 
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
}
