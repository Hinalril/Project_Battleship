#pragma once

#include "Board.h"
#include "Ship.h"
#include "Player.h"

using namespace std;


class Game
{
    void clearPartOfConsole(int startLine, int endLine);
    void scroll_log(int lines);
    void displayLog(int x, int y);
    void save_log(Player me, bool computer);
	void SetCursor(int x, int y); //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y

public:
	string log[20];
	string player_name_1;
	string player_name_2;

	Game(string name1, string name2);

	int game_enter();		    // инициализация игры (установка размера поля)
	vector<ShipType> calculate_ships(int fieldSize, int& remember_ship_sells);     // инициализация игры (количество кораблей)
	int info_players(Player player1, Player player2);	    // хранение информации об игрока (кто играет, очередность)
	void cycle_play(Player& player1, Player& player2, bool computer1, bool computer2);		    // запуск игрового цилк (повтор ходов)
	void process_steps(Player& player1, Player& player2, bool computer, bool only_comp);	    // обработка ходов игроков
	void output_info(Player player1, Player player2);		    // вывод информации игрокам о ходе игры
	bool check_status(Player player1, Player player2);	    // проверка состояния игры
	void game_end(Player player1, Player player2);		    // завершение игры
	void output_finish_info(Player player1, Player player2);  // вывод итоговой информации

};