
#include "Game.h"

Game::Game(string name1, string name2)
    :player_name_1(name1), player_name_2(name2) {}

void Game::clearPartOfConsole(int startLine, int endLine)
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

void Game::scroll_log(int lines)
{
    for (int i = 19; i >= lines; i--)
    {
        log[i] = log[i - lines];
    }
};

void Game::displayLog(int x, int y)
{
    for (int i = 0; i < 20; i++)
    {
        SetCursor(x, y + i);
        cout << log[i];
    }
};

void Game::save_log(Player me, bool computer)
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

void Game::SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
{
    COORD myCoords = { x,y }; //������������� ���������
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //������ ����������� ������� �� ������ ����������
}

// ������������� ���� (��������� ������� ����, ���������� ��������)
int Game::game_enter()
{
    int fieldSize;
    string input;

    while (true)
    {
        cout << "������� ������ ����: ";
        getline(cin, input); // ��������� ������ �������

        bool isInt = true; // �������� ������ �� ����� �����
        size_t startIndex = 0;

        if (input.empty())
        {
            isInt = false;
        }
        else if (input[0] == '-')
        {
            if (input.size() == 1)
            {
                isInt = false; // "-" �� �������� ������
            }
            else
            {
                startIndex = 1; // �������� �������� � ������� ������� ����� "-"
            }
        }

        for (int i = startIndex; i < input.size() && isInt; i++)
        {
            if (input[i] < '0' || input[i] > '9') // ���������, �������� �� ������ ������
            {
                isInt = false;
            }
        }

        if (isInt)
        {
            fieldSize = stoi(input); // ����������� ������ � �����

            if (fieldSize >= 3)
            {
                break; // ���������� �������� �������
            }
            else if (fieldSize < 0)
            {
                cout << "������ ���� ������ ���� �������������! ���������� �����.\n";
            }
            else
            {
                cout << "������� ������� ����� ����. ������� �������� > 2.\n";
            }
        }
        else
        {
            cout << "������ �����! ����������, ������� ����� �����.\n";
        }
    }
    return fieldSize;
}

vector<ShipType> Game::calculate_ships(int fieldSize, int& remember_ship_sells)
{
    int totalCells = fieldSize * fieldSize;        // ����� ���������� ������
    int shipCells = static_cast<int>(totalCells * 0.2); // ������ ��� ��������
    remember_ship_sells = shipCells;

    // ������������� ��������
    vector<ShipType> ships =
    {
        {"Flagship", 6, 0},    // 6-��������
        {"Carrier", 5, 0},     // 5-��������
        {"Battleship", 4, 0},  // 4-��������
        {"Cruiser", 3, 0},     // 3-��������
        {"Destroyer", 2, 0},   // 2-��������
        {"Submarine", 1, 0}    // 1-��������
    };

    // �������� ���� �������������
    while (shipCells > 0)
    {
        bool added = false;

        // �������� ������������� � ������������
        for (int i = ships.size() - 1; i >= 0; --i)
        {
            // ���������, ���������� �� ������ ��� ������ �������
            if (shipCells >= ships[i].size)
            {
                // ��������� ������� ���������
                if (i == ships.size() - 1 || ships[i].count < ships[i + 1].count - 1)
                {
                    ships[i].count++;          // ����������� ���������� �������� �������
                    shipCells -= ships[i].size; // ��������� ���������� ������
                    added = true;
                }
            }
        }

        // ���� �� ���� ������� �� ��� ��������, ������� �� �����
        if (!added)
        {
            break;
        }
    }

    cout << "������������� ���������� �������� ��� ���� " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships)
    {
        cout << ship.name << " (������ " << ship.size << "): " << ship.count << "\n";
    }
    Sleep(4000);

    return ships;
}

bool Game::check_status(Player player1, Player player2)
{
    if (player1.result_of_step.win_player || player2.result_of_step.win_player)
    {
        return false;
    }
    return true;
}

void Game::game_end(Player player1, Player player2)
{
    if (player1.result_of_step.win_player)
    {
        cout << "������ 1-�� ������!\n";
    }
    else
    {
        cout << "������ 2-�� ������!\n";
    }
}

int Game::info_players(Player player1, Player player2)
{
    if ((player1.result_of_step.result_shot.rezult_of_shot == 1 || player1.result_of_step.result_shot.rezult_of_shot == -1) && !player2.result_of_step.in_a_row)
    {
        return 1; // ��� 1-�� ������
    }
    if (player2.result_of_step.result_shot.rezult_of_shot == 1 || player2.result_of_step.result_shot.rezult_of_shot == -1)
    {
        return 2; // ��� 2-�� ������
    }
    return 0; // ���-�� ������, ����������� ����� if (player1.result_of_step.win_player || player2.result_of_step.win_player)
}

void Game::cycle_play(Player& player1, Player& player2, bool computer)
{
    while (check_status(player1, player2))
    {
        int turn = info_players(player1, player2);

        switch (turn)
        {
        case 1:
            process_steps(player1, player2, false);
            break;
        case 2:
            process_steps(player2, player1, computer);
            break;
        case 0:
            break;
        }
    }

    output_finish_info(player1, player2);
    system("pause");
};

void Game::output_finish_info(Player player1, Player player2)
{
    system("cls");
    cout << "���������� ����� ��� ��� 1-�� ������.\n";
    player1.output_stat(player1.info.enemy_ships.size, 1);
    cout << "\n���������� ����� ��� ��� 2-�� ������.\n";
    player2.output_stat(player2.info.enemy_ships.size, 16);
}

void Game::output_info(Player me, Player enemy)
{
    clearPartOfConsole(0, 15 + me.info.my_ships.size * 2);
    cout << "���������� ����� ��� ��� 1-�� ������.\n";
    me.info.my_ships.display(true, me.info.enemy_ships);
    me.output_stat(me.info.enemy_ships.size, 3 + me.info.my_ships.size * 2);
}

void Game::process_steps(Player& me, Player& enemy, bool computer)
{
    if (!computer) // ��� ������
    {
        enemy.result_of_step.result_shot.rezult_of_shot = 1;
        enemy.result_of_step.in_a_row = false;

        output_info(me, enemy);
        me.Attack_manual(&enemy);
        output_info(me, enemy);
        save_log(me, false);
    }
    else // ��� ����������
    {
        enemy.result_of_step.result_shot.rezult_of_shot = 1;
        enemy.result_of_step.in_a_row = false;

        me.Attack_computer(&enemy);
        enemy.info.my_ships.display(true, enemy.info.enemy_ships);
        save_log(me, true);
        //Sleep(1000);
    }
}