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
    string name;                         // ������������ �������
    int size;                            // ������ ������� (������)
    vector<COORD> coordinates;           // ������ � ����� ���������, ������ ���������� �������
    vector<bool> status_coordinates;     // 
    bool is_vertical;                    // ����������� ����������� ��� ������������� �� ����� ������ �������?
    int hits;                            // ���������� ��������� �� �������


    // ������ ��������� ������
    const vector<COORD>& getCoordinates() const { return coordinates; }

    Ship(string name, int size, COORD start, bool is_vertical); // ����������� �������
    Ship();
    bool isSunk() const;
    int statusShip() const;
    bool takeHit(int x, int y);
    ~Ship() = default;
};