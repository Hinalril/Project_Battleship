#pragma once

#include "Board.h"
#include "Ship.h"
#include "Player.h"

using namespace std;


class Game
{
	void clearPartOfConsole(int startLine, int endLine)
	{
		// Переместить курсор к начальной строке
		cout << "\033[" << startLine << ";0H";

		// Очистить каждую строку в указанном диапазоне
		for (int i = startLine; i <= endLine; ++i) {
			cout << "\033[K"; // Очистить текущую строку
			if (i != endLine) {
				cout << "\033[E"; // Перейти на следующую строку
			}
		}

		// Вернуть курсор к первой очищенной строке
		cout << "\033[" << startLine << ";0H";
	}

	void scroll_log(int lines)
	{
		for (int i = 19; i >= lines; i--)
		{
			log[i] = log[i - lines];
		}
	};

	void displayLog(int x, int y)
	{
		for (int i = 0; i < 20; i++)
		{
			SetCursor(x, y + i);
			cout << log[i];
		}
	};

    void save_log(Player me, bool computer)
    {
        if (!computer)
        {
            if (me.result_of_step.result_shot.rezult_of_shot == 0)
            {
                scroll_log(3);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| Промах. Ход переходит другому игроку! |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == 1)
            {
                if (me.result_of_step.result_shot.ship_dead == true)
                {
                    scroll_log(4);
                    log[0] = "+---------------------------------------+\n";
                    log[1] = "| Попадание! Ходите ещё раз.            |\n";
                    log[2] = "| Корабль уничтожен!                    |\n";
                    log[3] = "+---------------------------------------+\n";
                    displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
                }
                else
                {
                    scroll_log(3);
                    log[0] = "+---------------------------------------+\n";
                    log[1] = "| Попадание! Ходите ещё раз.            |\n";
                    log[2] = "+---------------------------------------+\n";
                    displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
                }
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == -1)
            {
                scroll_log(4);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| Вы уже стреляли в эту клетку.         |\n";
                log[2] = "| Стреляйте ещё раз!                    |\n";
                log[3] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
        }
        else
        {
            if (me.result_of_step.result_shot.rezult_of_shot == 0)
            {
                scroll_log(3);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| Компьютер атакует (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - промах.    |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == 1)
            {
                scroll_log(3);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| Компьютер атакует (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - попадание! |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
        }
    }


	void SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
	{
		COORD myCoords = { x,y }; //инициализация координат
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //Способ перемещения курсора на нужные координаты
	}

public:

	string log[20];
	string player_name_1;
	string player_name_2;

	Game(string name1, string name2);

	int game_enter();		    // инициализация игры (установка размера поля)
	vector<ShipType> calculate_ships(int fieldSize, int& remember_ship_sells);     // инициализация игры (количество кораблей)
	int info_players(Player player1, Player player2);	    // хранение информации об игрока (кто играет, очередность)
	void cycle_play(Player& player1, Player& player2, bool computer);		    // запуск игрового цилк (повтор ходов)
	void process_steps(Player& player1, Player& player2, bool computer);	    // обработка ходов игроков
	void output_info(Player player1, Player player2);		    // вывод информации игрокам о ходе игры
	bool check_status(Player player1, Player player2);	    // проверка состояния игры
	void game_end(Player player1, Player player2);		    // завершение игры
	void output_finish_info(Player player1, Player player2);  // вывод итоговой информации

};