#include "Player.h"

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //���������� ��������� ����
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };

PlayerStatistic::PlayerStatistic(int drawned_ships, int processed_shots, int sucessful_shots, int percent_suc_shots, vector<ShipType> ships)
    : drawned_ships(drawned_ships), processed_shots(processed_shots), sucessful_shots(sucessful_shots), percent_suc_shots(percent_suc_shots) // ������� ��������� ����������� ��� ���������, ����� ����� ���� ���������� ��������� � 4-�� ����������� � ������ �������������
{
    stat_dead_enemy_ships =
    {
        { "Submarine", 1, 0 },  // 1-��������
        { "Destroyer", 2, 0 },  // 2-��������
        { "Cruiser", 3, 0 },    // 3-��������
        { "Battleship", 4, 0}, // 4-��������
        { "Carrier", 5, 0 },   // 5-��������
        { "Flagship", 6, 0 }    // 6-��������
    };
    stat_alive_enemy_ships =
    {
        { "Submarine", 1, ships[5].count },  // 1-��������
        { "Destroyer", 2, ships[4].count },  // 2-��������
        { "Cruiser", 3, ships[3].count },    // 3-��������
        { "Battleship", 4, ships[2].count }, // 4-��������
        { "Carrier", 5, ships[1].count },   // 5-��������
        { "Flagship", 6, ships[0].count }    // 6-��������
    };
    initial_ships = 
    {
        { "Submarine", 1, ships[5].count },  // 1-��������
        { "Destroyer", 2, ships[4].count },  // 2-��������
        { "Cruiser", 3, ships[3].count },    // 3-��������
        { "Battleship", 4, ships[2].count }, // 4-��������
        { "Carrier", 5, ships[1].count },   // 5-��������
        { "Flagship", 6, ships[0].count }    // 6-��������
    };
}

PlayerStatistic::PlayerStatistic()
: drawned_ships(0), processed_shots(0), sucessful_shots(0), percent_suc_shots(0) {} // ���������� �������� ������ �����������

Player::Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships)
    : info(name, human, my_ships, enemy_ships), // ������������� info
    statistic(0, 0, 0, 0, ships) // ������������� statistic
{

};

void Player::SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
{
    COORD myCoords = { x,y }; //������������� ���������
    SetConsoleCursorPosition(hStdOut, myCoords); //������ ����������� ������� �� ������ ����������
}

void Player::AutoBoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player)
{
    srand(time(NULL));
    pair<int, int> new_data;
    for (int i = ships_player.size() - 1; i >= 0; i--)
    {
        for (int j = ships_player[i].count; j > 0; j--)
        {
            bool vertical = rand() % 2;
            int x, y;
            x = rand() % fieldSize;
            y = rand() % fieldSize;
            pair<int, int> new_data(x, y);
            Ship new_ship(ships_player[i].name, ships_player[i].size, new_data, vertical);
            if (info.my_ships.can_place_ship(new_ship))
            {
                another_player.info.enemy_ships.placeShip(new_ship);
                info.my_ships.placeShip(new_ship);

                /* ��� ��� ����� : ����� ������ �� �������
                system("cls");
                info.my_ships.display(false, info.my_ships);
                */
            }
            else
            {
                j++; // �������� ������� ���������� ��� ����� �������
            }
        }
    }
}

void Player::BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name, Player& another_player)
{
    int remember_x = 5, remember_y = 3; // ��������� ���������� �������

    // ������������ �������� ��� ������
    for (int i = ships_player.size() - 1; i >= 0; i--)
    {
        for (int j = ships_player[i].count; j > 0; j--)
        {
            bool vertical = false;

            system("cls");
            cout << name << ", ����������� ���� �������.\n";
            info.my_ships.display(false, info.my_ships);
            cout << "����������� " << ships_player[i].name << " (������ " << ships_player[i].size << "):\n";

            int key;
            pair<int, int> new_data;
            do
            {
                SetCursor(remember_x, remember_y);
                key = _getch(); //������� ���������� ����� ������� �������
                switch (key)
                {
                case Left:
                    if (remember_x > 5)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_x = remember_x - 4;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    break;
                case Right:
                    if (remember_x < 5 + fieldSize * 4 - 4)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_x = remember_x + 4;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    break;
                case Up:
                    if (remember_y > 3)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_y = remember_y - 2;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    break;
                case Down:
                    if (remember_y < 3 + fieldSize * 2 - 2)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_y = remember_y + 2;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    break;
                case Tab:
                    if (!vertical)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        vertical = true;
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    else
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        vertical = false;
                        paintFutureShip(remember_x, remember_y, ships_player[i], vertical);
                    }
                    break;
                case Enter:
                    new_data.first = (remember_x - 5) / 4;
                    new_data.second = (remember_y - 3) / 2;
                    Ship new_ship(ships_player[i].name, ships_player[i].size, new_data, vertical);
                    if (info.my_ships.can_place_ship(new_ship))
                    {
                        another_player.info.enemy_ships.placeShip(new_ship);
                        info.my_ships.placeShip(new_ship);
                    }
                    else
                    {
                        system("cls");
                        cout << "���������� ���������� ������� � ������ �������. ���������� �����.\n";
                        j++; // �������� ������� ���������� ��� ����� �������
                    }
                    break;
                }
            } while (key != Enter);
        }
    }
    system("cls");
    cout << "���������� �������� ���������!\n";
    info.my_ships.display(false, info.my_ships);
    Sleep(2000);
}

void Player::Attack_manual(int* remember_x, int* remember_y, const int fieldSize, int* rezult_player, bool* first_shot,  Player* another_player, int* ship_sells, bool* win_player)
{
    SetCursor(*remember_x, *remember_y);
    int key;
    do
    {
        key = _getch(); //������� ���������� ����� ������� �������
        switch (key)
        {
        case Left:
            if (*remember_x > 5 + fieldSize * 4 + 6)
            {
                *remember_x = *remember_x - 4;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Right:
            if (*remember_x < 5 + fieldSize * 4 + 6 + fieldSize * 4 - 4)
            {
                *remember_x = *remember_x + 4;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Up:
            if (*remember_y > 3)
            {
                *remember_y = *remember_y - 2;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Down:
            if (*remember_y < 3 + fieldSize * 2 - 2)
            {
                *remember_y = *remember_y + 2;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Enter:
            PlayerResultOfShot rezult;
            rezult = (*another_player).info.my_ships.processShot(((*remember_x - 5 - 6 - fieldSize * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);
            *rezult_player = rezult.rezult_of_shot;
            info.enemy_ships.processShot(((*remember_x - 5 - 6 - fieldSize * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);

            my_stat(rezult);

            if (rezult.rezult_of_shot == 1)
            {
                (*ship_sells)--;
                if (*ship_sells == 0)
                {
                    system("cls");
                    info.my_ships.display(true, info.enemy_ships);
                    *win_player = true;
                    break;
                }
                *first_shot = false; // ������ ������� ��� ������
            }

            if (rezult.rezult_of_shot == 0)
            {
                system("cls");
                cout << "���������� ����� ��� ��� % ������.\n";
                info.my_ships.display(true, info.enemy_ships);
                cout << "������. ��� ��������� ������� ������!\n";
                system("pause");
            }
            break;
        }
    } while (key != Enter);
}

// ����� ����� ��������
vector<ShipType> Player::CalcStatShips(vector<ShipType> ships, vector<ShipType> ships_player, vector<ShipType> initial_ships, Board board)
{
    ships_player = initial_ships;
    int number_board_ships = 0;
    for (int i = 0; i < ships.size(); i++)
    {
        for (int j = 0; j < ships[i].count; j++)
        {
            int counter = 0;
            for (int k = 0; k < board.ships[number_board_ships].size; k++)
            {
                if (board.ships[number_board_ships].status_coordinates[k] == false) // false - ������ ������� �������
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

    return ships_player;
}

void Player::paintFutureShip(int x, int y, ShipType ship_player, bool vertical)
{
    pair<int, int> new_data((x - 5) / 4, (y - 3) / 2);
    Ship new_ship(ship_player.name, ship_player.size, new_data, vertical);
    if (info.my_ships.can_place_ship(new_ship))
    {
        info.my_ships.display_ship_on_board(new_ship, x, y);
    }
}

void Player::my_stat(PlayerResultOfShot rezult)
{
    if (rezult.rezult_of_shot == 1) // ���� ��� �������� ���������
    {
        statistic.processed_shots++;
        statistic.sucessful_shots++;
        float suc_shot = statistic.sucessful_shots;
        statistic.percent_suc_shots = static_cast <int>(100 * (suc_shot / statistic.processed_shots));
    }
    else if (rezult.rezult_of_shot == 0) // ���� ��� ������
    {
        statistic.processed_shots++;
        float suc_shot = statistic.sucessful_shots;
        statistic.percent_suc_shots = static_cast <int>(100 * (suc_shot / statistic.processed_shots));
    }

    if (rezult.rezult_ship.second == true)
    {
        statistic.stat_dead_enemy_ships[rezult.rezult_ship.first - 1].count++;
    }
    statistic.stat_alive_enemy_ships = CalcStatShips(statistic.initial_ships, statistic.stat_alive_enemy_ships, statistic.initial_ships, info.enemy_ships);
}

void Player::output_stat(int fieldSize)
{
    cout << "����� �������� ����������:\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        cout << statistic.stat_alive_enemy_ships[i].name << ": " << statistic.stat_alive_enemy_ships[i].count << ".\n";
    }

    cout << "������������ �������� ����������:\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        cout << statistic.stat_dead_enemy_ships[i].name << ": " << statistic.stat_dead_enemy_ships[i].count << ".\n";
    }

    cout << "���������� ����������� ��������: " << statistic.drawned_ships << ".\n";
    cout << "��������� ����������� ���������: " << statistic.processed_shots << ".\n";
    cout << "���������� �������� ���������: " << statistic.sucessful_shots << ".\n";
    cout << "������� �������� ���������: " << statistic.percent_suc_shots << ".\n";
}