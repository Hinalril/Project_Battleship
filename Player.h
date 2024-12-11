#pragma once

#include "Board.h"
#include "Ship.h"
#include <algorithm>
#include <random>

using namespace std;

struct PlayerInfo
{
	string name;
	Board my_ships;
	Board enemy_ships;

	PlayerInfo(string n, Board my, Board enemy)
		: name(n), my_ships(my), enemy_ships(enemy) {} // вручную определим конструктор для структуры, чтобы можно было определять структуру с 4-мя аргументами в списке инициализации

	PlayerInfo()
		: name(""), my_ships(), enemy_ships() {} // компилятор требовал данный конструктор
};

struct PlayerStatistic
{
	int drawned_ships;      // количество потопленных кораблей
	int processed_shots;    // количесво совершенных выстрелов
	int sucessful_shots;    // количество успешных попаданий
	int percent_suc_shots;  // процент успешных попаданий
	vector<ShipType> stat_alive_enemy_ships;     // количество живых кораблей игрока противника
	vector<ShipType> stat_dead_enemy_ships; // количество потопленных кораблей противника
	vector<ShipType> initial_ships;    // количество кораблей до начала боя

	PlayerStatistic(int x, int y, int z, int b, vector<ShipType> ships);

	PlayerStatistic(); // компилятор требовал данный конструктор

};

struct PlayerResultOfStep
{
	bool win_player;
	bool in_a_row;
	PlayerResultOfShot result_shot;

	PlayerResultOfStep(bool win_player, bool in_a_row, PlayerResultOfShot result_shot);
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
	COORD coords;
	int ship_sells;
	
	Player(string name, Board my_ships, Board enemy_ships, vector<ShipType> ships, int ship_sells);  // Конструктор с параметрами

	void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
	void AutoBoardShipPlacement(string name, Player& another_player);
	void BoardShipPlacement(string name, Player& another_player); // расположение кораблей по поле боя (ручное)
	void Attack_manual(Player* another_player);
	void Attack_computer(Player* another_player);
	vector<ShipType> CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board);
	void my_stat(PlayerResultOfShot rezult);
	void paintFutureShip(int x, int y, ShipType ship_player, bool vertical);
	void output_stat(int fieldSize, int y);
	bool check_zone(int x, int y, bool& horizontal, int& move, bool& move_minus);

	~Player() = default; // Деструктор
};