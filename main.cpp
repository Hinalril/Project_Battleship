
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"
#include "Player.h"
#include "Game.h"

// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА НАСТЕ ###############################
// вывод живых кораблей
void output_ships(vector<ShipType> ships, vector<ShipType> ships_player, Board board)
{
    int number_board_ships = 0;
    for (int i = 0; i < ships.size(); i++)
    {
        for (int j = 0; j < ships[i].count; j++)
        {
            int counter = 0;
            for (int k = 0; k < board.ships[number_board_ships].size; k++)
            {
                if (board.ships[number_board_ships].status_coordinates[k] == false) // false - клетка корабля подбита
                {
                    counter++;
                }
            }
            if (counter == board.ships[number_board_ships].size)
            {
                ships_player[board.ships[number_board_ships].size - 1].count--;
            }
            number_board_ships++;
        }
    }
    cout << "Живых кораблей противника:\n";
    for (int i = 0; i < ships_player.size(); i++)
    {
        cout << ships_player[i].name << ": " << ships_player[i].count << ".\n";
    }
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int menu;
    while (true)
    {
        /*
        cout << "Хотите начать игру?"
        cin >> menu;
        switch (menu)
        {
        case 1:
            break;
        case 2:
            break;
        }*/
        Game my_game("Игрок 1", "Игрок 2");
        int fieldSize = my_game.game_enter();

        Board board_first_player_ally(fieldSize, false);
        Board board_second_player_ally(fieldSize, false);

        int remember_ship_sells = 0; // количество ячеек, которые занимают корабли
        vector<ShipType> ships = my_game.calculate_ships(fieldSize, remember_ship_sells); // Расчёт количества кораблей

        Player player1("Игрок 1", true, board_first_player_ally, board_second_player_ally, ships, remember_ship_sells);
        Player player2("Игрок 2", true, board_second_player_ally, board_first_player_ally, ships, remember_ship_sells);

        //player1.BoardShipPlacement("Игрок 1", player2);
        player1.AutoBoardShipPlacement("Игрок 1", player2);
        player2.AutoBoardShipPlacement("Игрок 2", player1);
        //player2.BoardShipPlacement("Игрок 2", player1);

        bool computer = true;

        my_game.cycle_play(player1, player2, computer);
        my_game.game_end(player1, player2);


    }
    return 0;
}