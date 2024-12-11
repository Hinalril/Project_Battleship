
#include "Ship.h"

// Конструктор
Ship::Ship(string name, int size, COORD start, bool is_vertical) : name(name), size(size), is_vertical(is_vertical), hits(0)
{
    // Рассчитываем координаты корабля
    for (int i = 0; i < size; i++)
    {
        status_coordinates.push_back(true);
        if (!is_vertical)
        {
            coordinates.emplace_back(start.X + i, start.Y);
        }
        else
        {
            coordinates.emplace_back(start.X, start.Y + i);
        }
    }
}

// Проверка, потоплен ли корабль
bool Ship::isSunk() const
{
    return hits >= size;
}

// Обработка попадания
bool Ship::takeHit(int x, int y)
{
    int counter = 0;
    for (const auto& coord : coordinates)
    {
        if (coord.first == x && coord.second == y)
        {
            status_coordinates[counter] = false; // false - клетка корабля подбита
            ++hits;
            return true;
        }
        counter++;
    }
    return false;
}

int Ship::statusShip() const
{
    int counter_damage = 0;
    for (int i = 0; i < (status_coordinates.size()); i++)
    {
        if (status_coordinates[i] == false)
        {
            counter_damage++;
        }
    }
    if (counter_damage > 0 && counter_damage < size)
    {
        return 0; // ранен
    }
    else if (counter_damage == size)
    {
        return -1; // убит
    }
    else
    {
        return 1; // без повреждений
    }
}