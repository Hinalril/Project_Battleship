#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"
#include "Player.h"
#include "Game.h"

void SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //Способ перемещения курсора на нужные координаты
}

void setColorWithBackground(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = (backgroundColor << 4) | textColor; // Фон сдвигается влево на 4 бита
    // Это делается, потому что в Windows цвета фона занимают старшие 4 бита (4–7) байта цвета, а цвета текста занимают младшие 4 бита (0–3).
    // Оператор | (побитовое ИЛИ) объединяет биты backgroundColor и textColor. Это позволяет "смешать" два значения в одно целое число, не потеряв информации.
    SetConsoleTextAttribute(hConsole, color);
}

//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int key;
    COORD menu_coord_1{ 5,4 };
    int rem_y_1 = menu_coord_1.Y;
    int menu = 0;
    do
    {
        menu_coord_1.Y = 4;
        system("cls");
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Запустить игру.\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Об игре.\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Выход из игры.\n";

        setColorWithBackground(7, 0);
        SetCursor(menu_coord_1.X, rem_y_1);
        key = _getch(); //функция возвращает номер нажатой клавиши
        switch (key)
        {
        case Up:
            if (rem_y_1 > 3)
            {
                rem_y_1--;
            }
            break;
        case Down:
            if (rem_y_1 < 6)
            {
                rem_y_1++;
            }
            break;
        case Enter:
            menu = rem_y_1 - 4;
            switch (menu)
            {
            case 0:
            {
                // "Запустить игру.\n";

            }
            break;
            case 1:
            {
                // "Об игре.\n";
                int i = 4;
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Об игре.\n";
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Авторы: ...\n";
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Дата создания ...\n";
                SetCursor(menu_coord_1.X + 25, i++);
                system("pause");
                break;
            }
            case 2: // ++
                // "Выход из игры.\n";
                key = 0;
                break;
            }
            break;
        }
    } while (key != 0);


    //int menu;
    while (true)
    {
        /*
        cout << "Хотите начать игру?"
        cin >> menu;
        switch (menu)
        {
        case 1:
            break;
        case 2:
            break;
        }*/
        Game my_game("Игрок 1", "Игрок 2");
        int fieldSize = my_game.game_enter();

        Board board_first_player_ally(fieldSize, false);
        Board board_second_player_ally(fieldSize, false);

        int remember_ship_sells = 0; // количество ячеек, которые занимают корабли
        vector<ShipType> ships = my_game.calculate_ships(fieldSize, remember_ship_sells); // Расчёт количества кораблей

        Player player1("Игрок 1", true, board_first_player_ally, board_second_player_ally, ships, remember_ship_sells);
        Player player2("Игрок 2", true, board_second_player_ally, board_first_player_ally, ships, remember_ship_sells);

        //player1.BoardShipPlacement("Игрок 1", player2);
        player1.AutoBoardShipPlacement("Игрок 1", player2);
        player2.AutoBoardShipPlacement("Игрок 2", player1);
        //player2.BoardShipPlacement("Игрок 2", player1);

        bool computer = true;

        my_game.cycle_play(player1, player2, computer);
        my_game.game_end(player1, player2);


    }
    return 0;
}