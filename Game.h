#pragma once

#include "Board.h"
#include "Ship.h"
#include "Player.h"

using namespace std;


class Game
{
	void clearPartOfConsole(int startLine, int endLine)
	{
		// ����������� ������ � ��������� ������
		cout << "\033[" << startLine << ";0H";

		// �������� ������ ������ � ��������� ���������
		for (int i = startLine; i <= endLine; ++i) {
			cout << "\033[K"; // �������� ������� ������
			if (i != endLine) {
				cout << "\033[E"; // ������� �� ��������� ������
			}
		}

		// ������� ������ � ������ ��������� ������
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
                log[1] = "| ������. ��� ��������� ������� ������! |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == 1)
            {
                if (me.result_of_step.result_shot.ship_dead == true)
                {
                    scroll_log(4);
                    log[0] = "+---------------------------------------+\n";
                    log[1] = "| ���������! ������ ��� ���.            |\n";
                    log[2] = "| ������� ���������!                    |\n";
                    log[3] = "+---------------------------------------+\n";
                    displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
                }
                else
                {
                    scroll_log(3);
                    log[0] = "+---------------------------------------+\n";
                    log[1] = "| ���������! ������ ��� ���.            |\n";
                    log[2] = "+---------------------------------------+\n";
                    displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
                }
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == -1)
            {
                scroll_log(4);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| �� ��� �������� � ��� ������.         |\n";
                log[2] = "| ��������� ��� ���!                    |\n";
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
                log[1] = "| ��������� ������� (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - ������.    |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
            else if (me.result_of_step.result_shot.rezult_of_shot == 1)
            {
                scroll_log(3);
                log[0] = "+---------------------------------------+\n";
                log[1] = "| ��������� ������� (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - ���������! |\n";
                log[2] = "+---------------------------------------+\n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
        }
    }


	void SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
	{
		COORD myCoords = { x,y }; //������������� ���������
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //������ ����������� ������� �� ������ ����������
	}

public:

	string log[20];
	string player_name_1;
	string player_name_2;

	Game(string name1, string name2);

	int game_enter();		    // ������������� ���� (��������� ������� ����)
	vector<ShipType> calculate_ships(int fieldSize, int& remember_ship_sells);     // ������������� ���� (���������� ��������)
	int info_players(Player player1, Player player2);	    // �������� ���������� �� ������ (��� ������, �����������)
	void cycle_play(Player& player1, Player& player2, bool computer);		    // ������ �������� ���� (������ �����)
	void process_steps(Player& player1, Player& player2, bool computer);	    // ��������� ����� �������
	void output_info(Player player1, Player player2);		    // ����� ���������� ������� � ���� ����
	bool check_status(Player player1, Player player2);	    // �������� ��������� ����
	void game_end(Player player1, Player player2);		    // ���������� ����
	void output_finish_info(Player player1, Player player2);  // ����� �������� ����������

};