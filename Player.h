#pragma once

#include "Board.h"
#include "Ship.h"
#include <algorithm>
#include <random>

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

struct PlayerResultOfStep
{
	bool win_player;
	int result_player;
	bool first_shot;

	PlayerResultOfStep(bool win_player, int result_player, bool first_shot);
	PlayerResultOfStep();
};

struct RandGenerator
{
	int operator()(int n)
	{
		return rand() % n;
	}
};

class Player
{
private:


public:
	PlayerInfo info;
	PlayerStatistic statistic;
	PlayerResultOfStep result_of_step;
	int ship_sells;
	
	Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships, int ship_sells);  // ����������� � �����������

	void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
	void AutoBoardShipPlacement(string name, Player& another_player);
	void BoardShipPlacement(string name, Player& another_player); // ������������ �������� �� ���� ��� (������)
	void Attack_manual(int* remember_x, int* remember_y, Player* another_player);
	void Attack_computer(Player* another_player);
	vector<ShipType> CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board);
	void my_stat(PlayerResultOfShot rezult);
	void paintFutureShip(int x, int y, ShipType ship_player, bool vertical);
	void output_stat(int fieldSize);
	bool check_zone(int x, int y, bool& horizontal, int& move, bool& move_minus);

	~Player() = default; // ����������

	// �� ������ - ��������
};