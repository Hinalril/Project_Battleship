#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <conio.h>
#include "Ship.h"

struct PlayerResultOfShot
{
    int rezult_of_shot;
    int size_of_ship;
    bool ship_dead;
    bool damaged_more_than_one_square;

    PlayerResultOfShot(int rezult_of_shot, int size_of_ship, bool ship_dead, bool damaged_more_than_one_square);
    PlayerResultOfShot();
};

//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };

class Board
{
private:

    void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
    void clearPartOfConsole(int startLine, int endLine);

public:
    int size;                   // Размер игрового поля (size x size)
    vector<vector<char>> grid;  // Игровое поле, представленное в виде матрицы символов
    vector<Ship> ships;         // Вектор размещённых кораблей

    Board(int size);               // Конструктор
    Board();                                        // Конструктор по умолчанию
    void placeShip(const Ship& ship);               // Размещение корабля
    void placeDeadField(const Ship& ship);          // размещение мертвой зоны после убийства корабля
    PlayerResultOfShot processShot(int x, int y);                 // Обработка выстрела
    void display(bool second_battlefield, const Board& second_board);    // Отображение поля (своего и чужого)
    void display(const Board& second_board, string titul_player); // отображение чужого поля
    void display_ship_on_board(const Ship& ship, int x, int y);
    bool can_place_ship(const Ship& ship);          // Функция для проверки, помещается ли корабль на поле

    void setColorWithBackground(int textColor, int backgroundColor);

    ~Board() = default;                             // Деструктор
};