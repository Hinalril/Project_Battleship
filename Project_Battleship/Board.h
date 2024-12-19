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
    int size_of_ship;
    bool ship_dead;
    bool damaged_more_than_one_square;

    PlayerResultOfShot(int rezult_of_shot, int size_of_ship, bool ship_dead, bool damaged_more_than_one_square);
    PlayerResultOfShot();
};

//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };

class Board
{
private:

    void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
    void clearPartOfConsole(int startLine, int endLine);

public:
    int size;                   // ������ �������� ���� (size x size)
    vector<vector<char>> grid;  // ������� ����, �������������� � ���� ������� ��������
    vector<Ship> ships;         // ������ ����������� ��������

    Board(int size);               // �����������
    Board();                                        // ����������� �� ���������
    void placeShip(const Ship& ship);               // ���������� �������
    void placeDeadField(const Ship& ship);          // ���������� ������� ���� ����� �������� �������
    PlayerResultOfShot processShot(int x, int y);                 // ��������� ��������
    void display(bool second_battlefield, const Board& second_board);    // ����������� ���� (������ � ������)
    void display(const Board& second_board, string titul_player); // ����������� ������ ����
    void display_ship_on_board(const Ship& ship, int x, int y);
    bool can_place_ship(const Ship& ship);          // ������� ��� ��������, ���������� �� ������� �� ����

    void setColorWithBackground(int textColor, int backgroundColor);

    ~Board() = default;                             // ����������
};