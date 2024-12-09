#include "Player.h"

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //���������� ��������� ����
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };
//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };

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


PlayerResultOfStep::PlayerResultOfStep(bool win_player, int result_player, bool first_shot)
    :win_player(win_player), result_player(result_player), first_shot(first_shot){};

PlayerResultOfStep::PlayerResultOfStep()
    :win_player(false), result_player(1), first_shot(true) {};

Player::Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships, int ship_sells)
    : info(name, human, my_ships, enemy_ships), // ������������� info
    statistic(0, 0, 0, 0, ships), // ������������� statistic
    result_of_step(false, 1, true), // ������������� result_of_step
    ship_sells(ship_sells)
{

};

void Player::SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
{
    COORD myCoords = { x,y }; //������������� ���������
    SetConsoleCursorPosition(hStdOut, myCoords); //������ ����������� ������� �� ������ ����������
}

void Player::AutoBoardShipPlacement(string name, Player& another_player)
{
    srand(time(NULL));
    pair<int, int> new_data;
    for (int i = statistic.stat_alive_enemy_ships.size() - 1; i >= 0; i--)
    {
        for (int j = statistic.stat_alive_enemy_ships[i].count; j > 0; j--)
        {
            bool vertical = rand() % 2;
            int x, y;
            x = rand() % info.my_ships.size;
            y = rand() % info.my_ships.size;
            pair<int, int> new_data(x, y);
            Ship new_ship(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].size, new_data, vertical);
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

void Player::BoardShipPlacement(string name, Player& another_player)
{
    int remember_x = 5, remember_y = 3; // ��������� ���������� �������

    // ������������ �������� ��� ������
    for (int i = statistic.stat_alive_enemy_ships.size() - 1; i >= 0; i--)
    {
        for (int j = statistic.stat_alive_enemy_ships[i].count; j > 0; j--)
        {
            bool vertical = false;

            system("cls");
            cout << name << ", ����������� ���� �������.\n";
            info.my_ships.display(false, info.my_ships);
            cout << "����������� " << statistic.stat_alive_enemy_ships[i].name << " (������ " << statistic.stat_alive_enemy_ships[i].size << "):\n";

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
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    break;
                case Right:
                    if (remember_x < 5 + info.my_ships.size * 4 - 4)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_x = remember_x + 4;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    break;
                case Up:
                    if (remember_y > 3)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_y = remember_y - 2;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    break;
                case Down:
                    if (remember_y < 3 + info.my_ships.size * 2 - 2)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        remember_y = remember_y + 2;
                        SetCursor(remember_x, remember_y);
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    break;
                case Tab:
                    if (!vertical)
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        vertical = true;
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    else
                    {
                        SetCursor(0, 1);
                        info.my_ships.display(false, info.my_ships);
                        vertical = false;
                        paintFutureShip(remember_x, remember_y, statistic.stat_alive_enemy_ships[i], vertical);
                    }
                    break;
                case Enter:
                    new_data.first = (remember_x - 5) / 4;
                    new_data.second = (remember_y - 3) / 2;
                    Ship new_ship(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].size, new_data, vertical);
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

void Player::Attack_computer(Player* another_player)
{
    static bool target_mode = false;                  // ����� "�����������"
    static vector<pair<int, int>> target_queue;       // ������� ��������� ��� �����
    int fieldSize = info.enemy_ships.size;            // ������ �������� ����

    random_device rd;                      // �������� ��������� �����
    mt19937 generator(rd());               // ��������� ��������������� �����

    if (!target_queue.empty())
    {
        shuffle(target_queue.begin(), target_queue.end(), generator);
    }

    int x = 0, y = 0;                                 // ���������� ��������
    if (target_mode && !target_queue.empty())
    {
        x = target_queue.back().first;
        y = target_queue.back().second;
        target_queue.pop_back();
    }
    else
    {
        // ��������� ��������� ��������� ��� ��������
        do
        {
            x = rand() % fieldSize + 1; // ���������� �� 1 �� fieldSize
            y = rand() % fieldSize + 1;
        } while (info.enemy_ships.grid[x - 1][y - 1] == 'O' || info.enemy_ships.grid[x - 1][y - 1] == 'X');
    }

    // ��������� �������
    PlayerResultOfShot rezult = (*another_player).info.my_ships.processShot(x, y);
    info.enemy_ships.processShot(x, y);

    my_stat(rezult);

    if (rezult.rezult_of_shot == 1) // ���������
    {
        if (rezult.rezult_ship.second) // ����������� �������
        {
            target_mode = false;      // ���������� ����� �����������
            target_queue.clear();     // ������� �������
        }
        else
        {
            target_mode = true;       // �������� ����� �����������
            //          [x,y-1]
            // [x-1,y]  ������   [x+1,y]
            //          [x,y+1]
            // ��������� �������� ������ � �������

            bool shot_in_line = false;

            // �������� �� 2 ��������� � �����:

            //                          [x,y-1]
            //    [x-2,y]   [x-1,y]=X      X      [x+1,y]
            //                          [x,y+1]

            int step_right = 0;
            int step_left = 0;
            int step_up = 0;
            int step_down = 0;

            while (true)
            {
                if (x - step_left > 1 && info.enemy_ships.grid[x - 2 - step_left][y - 1] == 'X') //
                {
                    step_left++; // ���������� X ����� �� ������� ����� ���������
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                if (x + step_right < fieldSize && info.enemy_ships.grid[x + step_right][y - 1] == 'X')
                {
                    step_right++;
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                if (y - step_up > 1 && info.enemy_ships.grid[x - 1][y - 2 - step_up] == 'X')
                {
                    step_up++; // ���������� X ����� �� ������� ����� ���������
                }
                else
                {
                    break;
                }
            }

            while (true)
            {
                if (y + step_down < fieldSize && info.enemy_ships.grid[x - 1][y + step_down] == 'X')
                {
                    step_up++; // ���������� X ����� �� ������� ����� ���������
                }
                else
                {
                    break;
                }
            }

            if (step_right > 0 || step_left > 0 || step_up > 0 || step_down > 0)
            {
                shot_in_line = true;
            }


            if (shot_in_line)
            {
                target_queue.clear();     // ������� �������
                if (x - step_left - 1 >= 1 && step_up == 0 && step_down == 0)
                {
                    target_queue.emplace_back(x - 1 - step_left, y);
                }
                if (x + step_right + 1 < fieldSize && step_up == 0 && step_down == 0)
                {
                    target_queue.emplace_back(x + 1 + step_right, y);
                }
                if (y - step_up - 1 >= 1 && step_left == 0 && step_right == 0)
                {
                    target_queue.emplace_back(x, y - 1 - step_up);
                }
                if (y + 1 + step_down < fieldSize && step_left == 0 && step_right == 0)
                {
                    target_queue.emplace_back(x, y + 1 + step_down);
                }

            }


            if (!shot_in_line)
            {
                // ���� ���� 2 ��������� � �����:
                if (x > 1 && (info.enemy_ships.grid[x - 2][y - 1] == 'S' || info.enemy_ships.grid[x - 2][y - 1] == ' ')) //
                {
                    target_queue.emplace_back(x - 1, y);
                }
                if (x < fieldSize && (info.enemy_ships.grid[x][y - 1] == 'S' || info.enemy_ships.grid[x][y - 1] == ' '))
                {
                    target_queue.emplace_back(x + 1, y);
                }
                if (y > 1 && (info.enemy_ships.grid[x - 1][y - 2] == 'S' || info.enemy_ships.grid[x - 1][y - 2] == ' '))
                {
                    target_queue.emplace_back(x, y - 1);
                }
                if (y < fieldSize && (info.enemy_ships.grid[x - 1][y] == 'S' || info.enemy_ships.grid[x - 1][y] == ' '))
                {
                    target_queue.emplace_back(x, y + 1);
                }
            }
        }
        result_of_step.first_shot = true; // ������ ������� ��� ������ // ������ �� ������������ ��� ����������

        // ����� ���������� ����������
        cout << "��������� �������: (" << x << ", " << y << ")";
        cout << " � ���������!\n";
        
        result_of_step.result_player = 1;
        return ;
    }
    else if (rezult.rezult_of_shot == 0)
    {
        // ����� ���������� ����������
        cout << "��������� �������: (" << x << ", " << y << ")";
        cout << " � ������.\n";

        result_of_step.result_player = 0;
        return ;
    }
    else
    {
        // ����� ���������� ����������
        cout << "��������� �������: (" << x << ", " << y << ")";
        cout << " � ��� ��������.\n";

        result_of_step.result_player = -1;
        return ;
    }
}

void Player::Attack_manual(int* remember_x, int* remember_y, Player* another_player)
{
    PlayerResultOfShot rezult_shot;
    SetCursor(*remember_x, *remember_y);
    int key;
    do
    {
        key = _getch(); //������� ���������� ����� ������� �������
        switch (key)
        {
        case Left:
            if (*remember_x > 5 + info.my_ships.size * 4 + 6)
            {
                *remember_x = *remember_x - 4;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Right:
            if (*remember_x < 5 + info.my_ships.size * 4 + 6 + info.my_ships.size * 4 - 4)
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
            if (*remember_y < 3 + info.my_ships.size * 2 - 2)
            {
                *remember_y = *remember_y + 2;
                SetCursor(*remember_x, *remember_y);
            }
            break;
        case Enter:
            rezult_shot = (*another_player).info.my_ships.processShot(((*remember_x - 5 - 6 - info.my_ships.size * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);
            info.enemy_ships.processShot(((*remember_x - 5 - 6 - info.my_ships.size * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);
            result_of_step.result_player = rezult_shot.rezult_of_shot;

            my_stat(rezult_shot);

            if (rezult_shot.rezult_of_shot == 1)
            {
                ship_sells--;
                if (ship_sells == 0)
                {
                    system("cls");
                    info.my_ships.display(true, info.enemy_ships);
                    result_of_step.win_player = true;
                    break;
                }
                result_of_step.first_shot = true; // ������ ������� ��� ������
            }

            if (rezult_shot.rezult_of_shot == 0)
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
        statistic.drawned_ships++;
    }
    statistic.stat_alive_enemy_ships = CalcStatShips(statistic.initial_ships, statistic.stat_alive_enemy_ships, statistic.initial_ships, info.enemy_ships);
}

void Player::output_stat(int fieldSize)
{
    info.my_ships.setColorWithBackground(2, 0);

    const int tableWidth = 26;

    // ������� ��� ��������� �������������� �����
    void (*draw_line)(int) = [](int width)
        {
            cout << "+" << string(width - 1, '-') << "+" << endl;
        };

    // ������� ��� ������ ������ � �������
    void (*print_row)(const string&, int, int, const PlayerInfo) = [](const string& label, int value, int width, PlayerInfo info)
        {
            cout << "| ";
            info.my_ships.setColorWithBackground(3, 0);
            cout << "    " << setw(width) << left << label;
            cout << ": " << setw(3) << right << value << " ";
            info.my_ships.setColorWithBackground(2, 0);
            cout << " |" << endl;
        };

    draw_line(tableWidth);
    cout << "|����� ������� ����������:|\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        print_row(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].count, tableWidth / 2, info);
        //cout << right << setw(10) << statistic.stat_alive_enemy_ships[i].name << ": " << statistic.stat_alive_enemy_ships[i].count << ".\n";
    }
    draw_line(tableWidth);

    SetCursor(tableWidth + 3, 3 + fieldSize * 2);
    draw_line(tableWidth);
    SetCursor(tableWidth + 3, 3 + fieldSize * 2 + 1);
    cout << "| ������������ ��������� :|\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        SetCursor(tableWidth + 3, 3 + fieldSize * 2 + i + 2);
        print_row(statistic.stat_dead_enemy_ships[i].name, statistic.stat_dead_enemy_ships[i].count, tableWidth / 2, info);
        //cout << right << setw(10) << statistic.stat_dead_enemy_ships[i].name << ": " << statistic.stat_dead_enemy_ships[i].count << ".\n";
        if (i == statistic.initial_ships.size() - 1)
        {
            SetCursor(tableWidth + 3, 3 + fieldSize * 2 + i + 3);
        }
    }
    draw_line(tableWidth);

    info.my_ships.setColorWithBackground(3, 0);
    cout << "    ���������� ����������� ��������: " << statistic.drawned_ships << ".\n";
    cout << "    ��������� ����������� ���������: " << statistic.processed_shots << ".\n";
    cout << "      ���������� �������� ���������: " << statistic.sucessful_shots << ".\n";
    cout << "         ������� �������� ���������: " << statistic.percent_suc_shots << ".\n";
    info.my_ships.setColorWithBackground(7, 0);
}