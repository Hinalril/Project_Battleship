#pragma once

#include <vector>
#include <string>

using namespace std;

class Ship
{
public:
    string name;                         // ������������ �������
    int size;                            // ������ ������� (������)
    vector<pair<int, int>> coordinates;  // ������ � ����� ���������, ������ ���������� �������
    vector<bool> status_coordinates;     // 
    bool is_vertical;                    // ����������� ����������� ��� ������������� �� ����� ������ �������?
    int hits;                            // ���������� ��������� �� �������


    // ������ ��������� ������
    int getSize() const { return size; }
    const vector<pair<int, int>>& getCoordinates() const { return coordinates; }
    bool getDirection() const { return is_vertical; }


    Ship(string name, int size, pair<int, int> start, bool is_vertical); // ����������� �������
    bool isSunk() const;
    int statusShip() const;
    bool takeHit(int x, int y);
    ~Ship() = default;
};