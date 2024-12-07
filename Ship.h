#pragma once

#include <vector>
#include <string>

using namespace std;

class Ship
{
public:
    string name;                         // наименование корабля
    int size;                            // длинна корабля (размер)
    vector<pair<int, int>> coordinates;  // вектор с парой координат, откуда начинается корабль
    vector<bool> status_coordinates;     // 
    bool is_vertical;                    // расположить вертикально или горизонтально от точки начала корабля?
    int hits;                            // количество попаданий по кораблю


    // Методы получения данных
    int getSize() const { return size; }
    const vector<pair<int, int>>& getCoordinates() const { return coordinates; }
    bool getDirection() const { return is_vertical; }


    Ship(string name, int size, pair<int, int> start, bool is_vertical); // конструктор корабля
    bool isSunk() const;
    int statusShip() const;
    bool takeHit(int x, int y);
    ~Ship() = default;
};