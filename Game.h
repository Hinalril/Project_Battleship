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
	void SetCursor(int x, int y); //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y

public:
	string log[20];
	string player_name_1;
	string player_name_2;

	Game(string name1, string name2);

	int game_enter();		    // ������������� ���� (��������� ������� ����)
	vector<ShipType> calculate_ships(int fieldSize, int& remember_ship_sells);     // ������������� ���� (���������� ��������)
	int info_players(Player player1, Player player2);	    // �������� ���������� �� ������ (��� ������, �����������)
	void cycle_play(Player& player1, Player& player2, bool computer1, bool computer2);		    // ������ �������� ���� (������ �����)
	void process_steps(Player& player1, Player& player2, bool computer, bool only_comp);	    // ��������� ����� �������
	void output_info(Player player1, Player player2);		    // ����� ���������� ������� � ���� ����
	bool check_status(Player player1, Player player2);	    // �������� ��������� ����
	void game_end(Player player1, Player player2);		    // ���������� ����
	void output_finish_info(Player player1, Player player2);  // ����� �������� ����������

};