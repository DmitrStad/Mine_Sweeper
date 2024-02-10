/**
  ******************************************************************************
  * @file    Mine_Sweeper.c
  * @author  Dmitriy Stadnyuk
  * @version V1.0.0
  * @date    18.09.2023
  * @brief   Игра сапёр 
  *
  *			 Управление игрой происходит с помощью мыши
  *
  ******************************************************************************
  */


#include "GameLogic.h"

/**
  * @brief  Основная программа
  * @param  int argc - количество аргументов при запуске приложения из командной строки, char* argv[] - значения этих аргументов
  * @retval None
  */


int main(int argc, char* argv[]) {
    srand(time(NULL));
    setlocale(LC_ALL, "ru");
    bool GameOn = 1;
    bool FirstStep = 0;
    int height = 10, width = 10;
    int x, y;
    Field sweeper(width, height);
    
    sweeper.PrintMatrix();
    
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE); // получаем дескриптор
    INPUT_RECORD InputRecord; // используется для возвращения информации о входных сообщениях в консольном входном буфере
    DWORD Events; // unsigned long
    COORD coord; // для координат X, Y

    /*Запретить выделение консоли*/
    DWORD prev_mode;
    GetConsoleMode(hin, &prev_mode);
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | (prev_mode & ~ENABLE_QUICK_EDIT_MODE));

    SetConsoleMode(hin, ENABLE_MOUSE_INPUT); // разрешаем обработку мыши
    
     

    while (!FirstStep) {
        ReadConsoleInput(hin, &InputRecord, 1, &Events); // считывание
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) // левая кнопка
        {
            coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
            x = coord.X;
            if (x > 1)
                x /= 2; //делим координаты Х на два из-за наличия пробелов при выводе игрового поля
            coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            y = coord.Y;
            sweeper.SetMines(y, x);
            FirstStep = sweeper.OneStep(y, x);
            
        }
    }

    while (GameOn)
    {
        ReadConsoleInput(hin, &InputRecord, 1, &Events);  
        
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) 
        {
            coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
            x = coord.X;
            if (x > 1)
                x /= 2;
            coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            y = coord.Y;
            GameOn = sweeper.OneStep(y, x);
            
        }
        else if (InputRecord.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) // правая кнопка
        {
            coord.X = InputRecord.Event.MouseEvent.dwMousePosition.X;
            x = coord.X;
            if (x > 1)
                x /= 2;
            coord.Y = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            y = coord.Y;
            GameOn=sweeper.MarkCell(y, x);
              
        }

    }

    return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
