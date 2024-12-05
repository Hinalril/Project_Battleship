#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include "Ship.h"

class Board
{
private:

    vector<Ship> ships;         // Вектор размещённых кораблей
public:
    int size;                   // Размер игрового поля (size x size)
    vector<vector<char>> grid;  // Игровое поле, представленное в виде матрицы символов
    bool hide_ships;

    // Конструктор
    Board(int size, bool hide_ships);

    // Размещение корабля
    void placeShip(const Ship& ship);

    // размещение мертвой зоны после убийства корабля
    void placeDeadField(const Ship& ship);

    // Обработка выстрела
    bool processShot(int x, int y);

    // Отображение поля
    void display() const;

    // Функция для проверки, помещается ли корабль на поле
    bool can_place_ship(const Ship& ship);

    // Деструктор
    ~Board() {}
};

