#pragma once


#include <vector>
#include <iostream>
#include <iomanip>
#include "Ship.h"

class Board
{
private:

    vector<Ship> ships;         // ������ ����������� ��������
public:
    int size;                   // ������ �������� ���� (size x size)
    vector<vector<char>> grid;  // ������� ����, �������������� � ���� ������� ��������

    // �����������
    Board(int size);

    // ���������� �������
    void placeShip(const Ship& ship);

    // ���������� ������� ���� ����� �������� �������
    void placeDeadField(const Ship& ship);

    // ��������� ��������
    bool processShot(int x, int y);

    // ����������� ����
    void display() const;

    // ������� ��� ��������, ���������� �� ������� �� ����
    bool can_place_ship(const Ship& ship);

    // ����������
    ~Board() {}
};

