#pragma once

#include <vector>
#include <string>
#include <windows.h>

using namespace std;

struct ShipType
{
    string name;
    int size;
    int count;
};

class Ship
{
public:
    string name;                         // наименование корабля
    int size;                            // длинна корабля (размер)
    vector<COORD> coordinates;           // вектор с парой координат, откуда начинается корабль
    vector<bool> status_coordinates;     // 
    bool is_vertical;                    // расположить вертикально или горизонтально от точки начала корабля?
    int hits;                            // количество попаданий по кораблю


    // Методы получения данных
    const vector<COORD>& getCoordinates() const { return coordinates; }

    Ship(string name, int size, COORD start, bool is_vertical); // конструктор корабля
    Ship();
    bool isSunk() const;
    int statusShip() const;
    bool takeHit(int x, int y);
    ~Ship() = default;
};