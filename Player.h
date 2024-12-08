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
		: name(n), human(h), my_ships(my), enemy_ships(enemy) {} // вручную определим конструктор для структуры, чтобы можно было определять структуру с 4-мя аргументами в списке инициализации

	PlayerInfo()
		: name(""), human(false), my_ships(), enemy_ships() {} // компилятор требовал данный конструктор
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

class Player
{
private:


public:
	PlayerInfo info;
	PlayerStatistic statistic;
	
	Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships);  // Конструктор с параметрами

	void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
	void AutoBoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player);
	void BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player); // расположение кораблей по поле боя (ручное)
	int Attack_manual(int* remember_x, int* remember_y, const int fieldSize, bool* first_shot, Player* another_player, int* ship_sells, bool* win_player);
	int Attack_computer(bool* first_shot, Player* another_player);
	vector<ShipType> CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board);
	void my_stat(PlayerResultOfShot rezult);
	void paintFutureShip(int x, int y, ShipType ship_player, bool vertical);
	void output_stat(int fieldSize);

	~Player() = default; // Деструктор

	// ИИ глупый - доделать
};