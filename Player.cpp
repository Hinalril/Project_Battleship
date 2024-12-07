#include "Player.h"

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор активного окна
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };

Player::Player(string name, bool human, Board my_ships, Board enemy_ships, vector<ShipType> ships)
    : info(name, human, my_ships, enemy_ships)
{
    player_ships =
    {
        { "Submarine", 1, ships[5].count },  // 1-палубный
        { "Destroyer", 2, ships[4].count },  // 2-палубный
        { "Cruiser", 3, ships[3].count },    // 3-палубный
        { "Battleship", 4, ships[2].count }, // 4-палубный
        { "Carrier", 5, ships[1].count },   // 5-палубный
        { "Flagship", 6, ships[0].count }    // 6-палубный
    };
};

// Board::Board(int size, bool hide_ships) : size(size), grid(size, vector<char>(size, ' ')), hide_ships(hide_ships) {}

void Player::SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(hStdOut, myCoords); //Способ перемещения курсора на нужные координаты
}

void Player::BoardShipPlacement(vector<ShipType> ships_player, int fieldSize, string name)
{
    int remember_x = 5, remember_y = 3; // начальные координаты курсора

    // расположение кораблей для игрока
    for (int i = ships_player.size() - 1; i >= 0; i--)
    {
        for (int j = ships_player[i].count; j > 0; j--)
        {
            bool vertical = false;

            system("cls");
            cout << name << ", расположите свои корабли.\n";
            info.my_ships.display(false, info.my_ships);
            cout << "Расположите " << ships_player[i].name << " (размер " << ships_player[i].size << "):\n";

            SetCursor(remember_x, remember_y);
            int key;
            do
            {
                key = _getch(); //функция возвращает номер нажатой клавиши
                switch (key)
                {
                case Left:
                    if (remember_x > 5)
                    {
                        remember_x = remember_x - 4;
                        SetCursor(remember_x, remember_y);
                    }
                    break;
                case Right:
                    if (remember_x < 5 + fieldSize * 4 - 4)
                    {
                        remember_x = remember_x + 4;
                        SetCursor(remember_x, remember_y);
                    }
                    break;
                case Up:
                    if (remember_y > 3)
                    {
                        remember_y = remember_y - 2;
                        SetCursor(remember_x, remember_y);
                    }
                    break;
                case Down:
                    if (remember_y < 3 + fieldSize * 2 - 2)
                    {
                        remember_y = remember_y + 2;
                        SetCursor(remember_x, remember_y);
                    }
                    break;
                case Tab:
                    if (!vertical)
                    {
                        vertical = true;
                    }
                    else
                    {
                        vertical = false;
                    }
                    break;
                case Enter:
                    pair<int, int> new_data((remember_x - 5) / 4, (remember_y - 3) / 2);
                    Ship new_ship(ships_player[i].name, ships_player[i].size, new_data, vertical);
                    if (info.my_ships.can_place_ship(new_ship))
                    {
                        info.enemy_ships.placeShip(new_ship);
                        info.my_ships.placeShip(new_ship);
                    }
                    else
                    {
                        system("cls");
                        cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                        j++; // Повторим попытку размещения для этого корабля
                    }
                    break;
                }
            } while (key != Enter);
        }
    }
    system("cls");
    cout << "Размещение кораблей завершено!\n";
    info.my_ships.display(false, info.my_ships);
    Sleep(2000);
}

void Player::Attack_manual(int* remember_x, int* remember_y, const int fieldSize, int* rezult_player, bool* first_shot,  Player* another_player, int* ship_sells, bool* win_player)
{
    SetCursor(*remember_x, *remember_y);
    int key;
    do
    {
        key = _getch(); //функция возвращает номер нажатой клавиши
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
            *rezult_player = (*another_player).info.my_ships.processShot(((*remember_x - 5 - 6 - fieldSize * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);
            info.enemy_ships.processShot(((*remember_x - 5 - 6 - fieldSize * 4) / 4) + 1, ((*remember_y - 3) / 2) + 1);

            if (*rezult_player == 1)
            {
                (*ship_sells)--;
                if (*ship_sells == 0)
                {
                    system("cls");
                    info.my_ships.display(true, info.enemy_ships);
                    *win_player = true;
                    break;
                }
                *first_shot = false; // первый выстрел был сделан
            }

            if (*rezult_player == 0)
            {
                system("cls");
                cout << "Информация полей боя для 1-го игрока.\n";
                info.my_ships.display(true, info.enemy_ships);
                cout << "Промах. Ход переходит 2-му игроку!\n";
                system("pause");
            }
            break;
        }
    } while (key != Enter);
}

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