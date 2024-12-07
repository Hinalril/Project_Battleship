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

class Player
{
private:


public:

	PlayerInfo info;
	vector<ShipType> player_ships; // дополнительное поле

	Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships);  // Конструктор с параметрами

	void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
	void BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name); // расположение кораблей по поле боя (ручное)
	void Attack_manual(int* remember_x, int* remember_y, const int fieldSize, int* rezult_player, bool* first_shot, Player* another_player, int* ship_sells, bool* win_player);
	void output_ships(vector<ShipType> ships, vector<ShipType> ships_player, Board board);


	~Player() = default; // Деструктор

	/* нет:
	* 1. рамещение кораблей на поле (автоматическое) - отдельный метод или после внедрить код в BoardShipPlacement
	* 2. алгоритм атаки компьютерного игрока (ИИ)
	* 3. ведение статистики игрока (например, через структуру): 1. количество потопленных кораблей
								*								2. количесво совершенных выстрелов
								*								3. количество успешных попаданий
								*								4. процент успешных попаданий
								*								5. общее количество сделанных ходов
	*							

	*/

};