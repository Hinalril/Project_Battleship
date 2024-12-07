#pragma once

#include "Board.h"
#include "Ship.h"

using namespace std;

struct PlayerInfo
{
	string name;
	bool human;
	Board my_ships;
	Board enemy_ships;

	PlayerInfo(string n, bool h, Board my, Board enemy)
		: name(n), human(h), my_ships(my), enemy_ships(enemy) {} // ������� ��������� ����������� ��� ���������, ����� ����� ���� ���������� ��������� � 4-�� ����������� � ������ �������������

	PlayerInfo()
		: name(""), human(false), my_ships(), enemy_ships() {} // ���������� �������� ������ �����������
};

class Player
{
private:


public:

	PlayerInfo info;
	vector<ShipType> player_ships; // �������������� ����

	Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships);  // ����������� � �����������

	void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
	void BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name); // ������������ �������� �� ���� ��� (������)
	void Attack_manual(int* remember_x, int* remember_y, const int fieldSize, int* rezult_player, bool* first_shot, Player* another_player, int* ship_sells, bool* win_player);
	void output_ships(vector<ShipType> ships, vector<ShipType> ships_player, Board board);


	~Player() = default; // ����������

	/* ���:
	* 1. ��������� �������� �� ���� (��������������) - ��������� ����� ��� ����� �������� ��� � BoardShipPlacement
	* 2. �������� ����� ������������� ������ (��)
	* 3. ������� ���������� ������ (��������, ����� ���������): 1. ���������� ����������� ��������
								*								2. ��������� ����������� ���������
								*								3. ���������� �������� ���������
								*								4. ������� �������� ���������
								*								5. ����� ���������� ��������� �����
	*							

	*/

};