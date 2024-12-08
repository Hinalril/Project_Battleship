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
    pair<int, bool> rezult_ship;

};

class Board
{
private:

    void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y



public:
    int size;                   // Размер игрового поля (size x size)
    vector<vector<char>> grid;  // Игровое поле, представленное в виде матрицы символов
    bool hide_ships;
    vector<Ship> ships;         // Вектор размещённых кораблей

    Board(int size, bool hide_ships);               // Конструктор
    Board();                                        // Конструктор по умолчанию
    void placeShip(const Ship& ship);               // Размещение корабля
    void placeDeadField(const Ship& ship);          // размещение мертвой зоны после убийства корабля
    PlayerResultOfShot processShot(int x, int y);                 // Обработка выстрела
    void display(bool second_battlefield, const Board& second_board) const;    // Отображение поля
    void display_ship_on_board(const Ship& ship, int x, int y);
    bool can_place_ship(const Ship& ship);          // Функция для проверки, помещается ли корабль на поле

    ~Board() = default;                             // Деструктор
};