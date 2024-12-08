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

struct PlayerStatistic
{
	int drawned_ships;      // ���������� ����������� ��������
	int processed_shots;    // ��������� ����������� ���������
	int sucessful_shots;    // ���������� �������� ���������
	int percent_suc_shots;  // ������� �������� ���������
	vector<ShipType> stat_alive_enemy_ships;     // ���������� ����� �������� ������ ����������
	vector<ShipType> stat_dead_enemy_ships; // ���������� ����������� �������� ����������
	vector<ShipType> initial_ships;    // ���������� �������� �� ������ ���

	PlayerStatistic(int x, int y, int z, int b, vector<ShipType> ships);

	PlayerStatistic(); // ���������� �������� ������ �����������

};

class Player
{
private:


public:
	PlayerInfo info;
	PlayerStatistic statistic;
	
	Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships);  // ����������� � �����������

	void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
	void AutoBoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player);
	void BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player); // ������������ �������� �� ���� ��� (������)
	int Attack_manual(int* remember_x, int* remember_y, const int fieldSize, bool* first_shot, Player* another_player, int* ship_sells, bool* win_player);
	int Attack_computer(bool* first_shot, Player* another_player);
	vector<ShipType> CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board);
	void my_stat(PlayerResultOfShot rezult);
	void paintFutureShip(int x, int y, ShipType ship_player, bool vertical);
	void output_stat(int fieldSize);

	~Player() = default; // ����������

	// �� ������ - ��������
};