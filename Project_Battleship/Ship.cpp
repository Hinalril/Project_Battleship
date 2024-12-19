
#include "Ship.h"

// �����������
Ship::Ship(string name, int size, COORD start, bool is_vertical) : name(name), size(size), is_vertical(is_vertical), hits(0)
{
    // ������������ ���������� �������
    for (int i = 0; i < size; i++)
    {
        status_coordinates.push_back(true);
        COORD coord;
        if (!is_vertical)
        {
            coord.X = start.X + i;
            coord.Y = start.Y;
        }
        else
        {
            coord.X = start.X;
            coord.Y = start.Y + i;
        }
        coordinates.push_back(coord);
    }
}

Ship::Ship() : name(""), size(0), is_vertical(false), hits(0) {}

// ��������, �������� �� �������
bool Ship::isSunk() const
{
    return hits >= size;
}

// ��������� ���������
bool Ship::takeHit(int x, int y)
{
    int counter = 0;
    for (const auto& coord : coordinates)
    {
        if (coord.X == x && coord.Y == y)
        {
            status_coordinates[counter] = false; // false - ������ ������� �������
            hits++;
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
        return 0; // �����
    }
    else if (counter_damage == size)
    {
        return -1; // ����
    }
    else
    {
        return 1; // ��� �����������
    }
}