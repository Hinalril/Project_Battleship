#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "Ship.h"

class Board
{
private:

    vector<Ship> ships;         // Вектор размещённых кораблей
public:
    int size;                   // Размер игрового поля (size x size)
    vector<vector<char>> grid;  // Игровое поле, представленное в виде матрицы символов
    bool hide_ships;

    Board(int size, bool hide_ships);               // Конструктор
    void placeShip(const Ship& ship);               // Размещение корабля
    void placeDeadField(const Ship& ship);          // размещение мертвой зоны после убийства корабля
    int processShot(int x, int y);                 // Обработка выстрела
    void display(bool second_battlefield, const Board& second_board) const;    // Отображение поля
    bool can_place_ship(const Ship& ship);          // Функция для проверки, помещается ли корабль на поле
    ~Board() = default;                             // Деструктор
};