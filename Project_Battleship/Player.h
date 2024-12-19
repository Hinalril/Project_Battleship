#pragma once

#include "Board.h"
#include "Ship.h"
#include <algorithm>

using namespace std;

struct PlayerInfo
{
	string name;
	bool person;
	string titul_player;
	Board my_ships;
	Board enemy_ships;

	PlayerInfo(string n, bool person, string titul_player, Board my, Board enemy)
		: name(n), person(person), titul_player(titul_player), my_ships(my), enemy_ships(enemy) {} // ������� ��������� ����������� ��� ���������, ����� ����� ���� ���������� ��������� � 4-�� ����������� � ������ �������������

	PlayerInfo()
		: name(""), person(0), titul_player(""), my_ships(), enemy_ships() {} // ���������� �������� ������ �����������
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
	bool in_a_row;
	bool target_mode;   // ���� ��� ���������� // ����� "�����������"
	vector<COORD> target_queue; // ���� ��� ����������
	PlayerResultOfShot result_shot;

	PlayerResultOfStep(bool win_player, bool in_a_row, bool target_mode, PlayerResultOfShot result_shot);
	PlayerResultOfStep();
};

class Player
{
private:
	void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
	void paintFutureShip(int x, int y, ShipType ship_player, bool vertical);
	bool check_zone(int x, int y, bool& horizontal, int& move, bool& move_minus);
	vector<ShipType> CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board);
	void my_stat(PlayerResultOfShot rezult);
public:
	PlayerInfo info;
	PlayerStatistic statistic;
	PlayerResultOfStep result_of_step;
	COORD coords;
	int ship_sells;
	
	Player(string name, Board my_ships, Board enemy_ships, vector<ShipType> ships, int ship_sells, bool person, string titul_player);  // ����������� � �����������
	Player();


	void AutoBoardShipPlacement(string name, Player& another_player);
	void BoardShipPlacement(string name, Player& another_player); // ������������ �������� �� ���� ��� (������)
	void Attack_manual(Player* another_player);
	void Attack_computer(Player* another_player);
	void output_stat(int fieldSize, int y);

	~Player() = default; // ����������
};