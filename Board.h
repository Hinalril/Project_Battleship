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

    void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y



public:
    int size;                   // ������ �������� ���� (size x size)
    vector<vector<char>> grid;  // ������� ����, �������������� � ���� ������� ��������
    bool hide_ships;
    vector<Ship> ships;         // ������ ����������� ��������

    Board(int size, bool hide_ships);               // �����������
    Board();                                        // ����������� �� ���������
    void placeShip(const Ship& ship);               // ���������� �������
    void placeDeadField(const Ship& ship);          // ���������� ������� ���� ����� �������� �������
    PlayerResultOfShot processShot(int x, int y);                 // ��������� ��������
    void display(bool second_battlefield, const Board& second_board) const;    // ����������� ����
    void display_ship_on_board(const Ship& ship, int x, int y);
    bool can_place_ship(const Ship& ship);          // ������� ��� ��������, ���������� �� ������� �� ����

    ~Board() = default;                             // ����������
};