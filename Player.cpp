#include "Player.h"

HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор активного окна
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };
const int pause_duration = 0;
const int beep_destroy = 200;

void playHitSound()
{
    Beep(300, 200);
}

void playDestroySound()
{
    Beep(400, 300);
    Beep(500, 300);
    Beep(600, 300);
}


//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };

PlayerStatistic::PlayerStatistic(int drawned_ships, int processed_shots, int sucessful_shots, int percent_suc_shots, vector<ShipType> ships)
    : drawned_ships(drawned_ships), processed_shots(processed_shots), sucessful_shots(sucessful_shots), percent_suc_shots(percent_suc_shots) // вручную определим конструктор для структуры, чтобы можно было определять структуру с 4-мя аргументами в списке инициализации
{
    stat_dead_enemy_ships =
    {
        { "Submarine", 1, 0 },  // 1-палубный
        { "Destroyer", 2, 0 },  // 2-палубный
        { "Cruiser", 3, 0 },    // 3-палубный
        { "Battleship", 4, 0}, // 4-палубный
        { "Carrier", 5, 0 },   // 5-палубный
        { "Flagship", 6, 0 }    // 6-палубный
    };
    stat_alive_enemy_ships =
    {
        { "Submarine", 1, ships[5].count },  // 1-палубный
        { "Destroyer", 2, ships[4].count },  // 2-палубный
        { "Cruiser", 3, ships[3].count },    // 3-палубный
        { "Battleship", 4, ships[2].count }, // 4-палубный
        { "Carrier", 5, ships[1].count },   // 5-палубный
        { "Flagship", 6, ships[0].count }    // 6-палубный
    };
    initial_ships = 
    {
        { "Submarine", 1, ships[5].count },  // 1-палубный
        { "Destroyer", 2, ships[4].count },  // 2-палубный
        { "Cruiser", 3, ships[3].count },    // 3-палубный
        { "Battleship", 4, ships[2].count }, // 4-палубный
        { "Carrier", 5, ships[1].count },   // 5-палубный
        { "Flagship", 6, ships[0].count }    // 6-палубный
    };
}

PlayerStatistic::PlayerStatistic()
: drawned_ships(0), processed_shots(0), sucessful_shots(0), percent_suc_shots(0) {} // компилятор требовал данный конструктор


PlayerResultOfStep::PlayerResultOfStep(bool win_player, bool in_a_row, bool target_mode, PlayerResultOfShot result_shot)
    :win_player(win_player), in_a_row(in_a_row),result_shot(result_shot), target_mode(target_mode){};

PlayerResultOfStep::PlayerResultOfStep()
    :win_player(false), in_a_row(false), target_mode(false), result_shot(PlayerResultOfShot(0,0,0,0)){};

Player::Player(string name, Board my_ships, Board enemy_ships, vector<ShipType> ships, int ship_sells, bool person, string titul_player)
    : info(name, person, titul_player, my_ships, enemy_ships), // Инициализация info
    statistic(0, 0, 0, 0, ships), // Инициализация statistic
    result_of_step(false, false, false, PlayerResultOfShot(1, 0, true, false)), // Инициализация result_of_step
    ship_sells(ship_sells)
{
    coords.X = 5 + info.enemy_ships.size * 4 + 6;
    coords.Y = 3;
};

Player::Player()
    : info("", false, "", Board(), Board()),  // Инициализация info
    statistic(0, 0, 0, 0, vector<ShipType>()), // Инициализация statistic
    result_of_step(false, false, false, PlayerResultOfShot(0, 0, false, false)), // Инициализация result_of_step
    ship_sells(0)
{
    coords.X = 0; // Установка начальных координат
    coords.Y = 0;
}

void Player::SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(hStdOut, myCoords); //Способ перемещения курсора на нужные координаты
}

void Player::AutoBoardShipPlacement(string name, Player& another_player)
{
    int sum = 0;
    for (int i = 0; i < info.name.size(); i++)
    {
        sum += info.name[i];
    }
    srand(time(NULL));
    for (int i = statistic.stat_alive_enemy_ships.size() - 1; i >= 0; i--)
    {
        for (int j = statistic.stat_alive_enemy_ships[i].count; j > 0; j--)
        {
            bool vertical = rand() % 2;
            int x, y;
            x = rand() % info.my_ships.size;
            y = rand() % info.my_ships.size;
            COORD new_data = { x, y };
            Ship new_ship(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].size, new_data, vertical);
            if (info.my_ships.can_place_ship(new_ship))
            {
                another_player.info.enemy_ships.placeShip(new_ship);
                info.my_ships.placeShip(new_ship);
            }
            else
            {
                j++; // Повторим попытку размещения для этого корабля
            }
        }
    }
}

void Player::BoardShipPlacement(string name, Player& another_player)
{
    int remember_x = 5, remember_y = 3; // начальные координаты курсора

    // расположение кораблей для игрока
    for (int i = statistic.stat_alive_enemy_ships.size() - 1; i >= 0; i--)
    {
        for (int j = statistic.stat_alive_enemy_ships[i].count; j > 0; j--)
        {
            bool vertical = false;

            system("cls");
            cout << name << ", расположите свои корабли.\n";
            info.my_ships.display(false, info.my_ships);
            cout << "Расположите " << statistic.stat_alive_enemy_ships[i].name << " (размер " << statistic.stat_alive_enemy_ships[i].size << "):\n";

            int key;
            COORD new_data;
            do
            {
                SetCursor(remember_x, remember_y);
                key = _getch(); //функция возвращает номер нажатой клавиши
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
                    new_data.X = (remember_x - 5) / 4;
                    new_data.Y = (remember_y - 3) / 2;
                    Ship new_ship(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].size, new_data, vertical);
                    if (info.my_ships.can_place_ship(new_ship))
                    {
                        another_player.info.enemy_ships.placeShip(new_ship);
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
    Sleep(pause_duration);
}


bool Player::check_zone(int x, int y, bool& horizontal, int& move, bool& move_minus)
{
    int min_size_ship = 1;
    for (int i = 0; i < statistic.stat_alive_enemy_ships.size(); i++)
    {
        if (statistic.stat_alive_enemy_ships[i].count == 0)
        {
            min_size_ship++;
        }
        else
        {
            break;
        }
    }
    if (min_size_ship == 1)
    {
        return false;
    }

    const int point = 1;
    int right = 0, left = 0, up = 0, down = 0;
    
    int (*checkDirection)(PlayerInfo, int, int, int, int) = [](PlayerInfo info, int x, int y, int dx, int dy)
        {
            int counter = 0;
            while (true)
            {
                int newX = x - 1 + dx * (counter + 1);
                int newY = y - 1 + dy * (counter + 1);

                if (newX >= 0 && newX < info.enemy_ships.size && newY >= 0 && newY < info.enemy_ships.size)
                {
                    if (info.enemy_ships.grid[newX][newY] != 'O' && info.enemy_ships.grid[newX][newY] != 'X')
                    {
                        counter++;
                    }
                    else
                    {
                        return counter;
                    }
                }
                else
                {
                    return counter;
                }
            }
        };

    left = checkDirection(info, x, y, -1, 0);
    right = checkDirection(info, x, y, 1, 0);
    up = checkDirection(info, x, y, 0, -1);
    down = checkDirection(info, x, y, 0, 1);

    int in_a_row = point + right + left;
    int in_a_column = point + up + down;

    if (in_a_row >= min_size_ship || in_a_column >= min_size_ship)
    {
        if (in_a_row >= min_size_ship && in_a_column < min_size_ship)
        {
            if (left < min_size_ship)
            {
                move = min_size_ship - 1;
                int i = 0;
                while (true)
                {
                    i++;
                    if (move >= in_a_row / 2 && move < min_size_ship)
                    {
                        move = min_size_ship - 1 - i;
                    }
                    else
                    {
                        break;
                    }
                }
                move_minus = false;
            }
            if (right < min_size_ship)
            {
                move = min_size_ship - 1;
                int i = 0;
                while (true)
                {
                    i++;
                    if (move >= in_a_row / 2 && move < min_size_ship)
                    {
                        move = min_size_ship - 1 - i;
                    }
                    else
                    {
                        break;
                    }
                }
                move_minus = true;
            }
            horizontal = true;
        }
        else if (in_a_column >= min_size_ship)
        {
            if (up < min_size_ship)
            {
                move = min_size_ship - 1;
                int i = 0;
                while (true)
                {
                    i++;
                    if (move >= in_a_row / 2 && move < min_size_ship)
                    {
                        move = min_size_ship - 1 - i;
                    }
                    else
                    {
                        break;
                    }
                }
                move_minus = false;
            }
            if (down < min_size_ship)
            {
                move = min_size_ship - 1;
                int i = 0;
                while (true)
                {
                    i++;
                    if (move >= in_a_row / 2 && move < min_size_ship)
                    {
                        move = min_size_ship - 1 - i;
                    }
                    else
                    {
                        break;
                    }
                }
                move_minus = true;
            }
            horizontal = false;
        }

        return false;
    }
    else
    {
        return true;
    }
}

void Player::Attack_computer(Player* another_player)
{
    int fieldSize = info.enemy_ships.size;            // Размер игрового поля

    if (!result_of_step.target_queue.empty())
    {
        random_shuffle(result_of_step.target_queue.begin(), result_of_step.target_queue.end());
    }

    int x = 0, y = 0;                                 // Координаты выстрела
    if (result_of_step.target_mode && !result_of_step.target_queue.empty())
    {
        x = result_of_step.target_queue.back().X;
        y = result_of_step.target_queue.back().Y;
        result_of_step.target_queue.pop_back();
    }
    else
    {
        bool repeat = true;
        bool horizontal = true;
        int move = 0;
        bool move_minus = true;
        while (repeat)
        {
            // Генерация случайных координат для выстрела
            do
            {
                x = rand() % fieldSize + 1; // Координаты от 1 до fieldSize
                y = rand() % fieldSize + 1;
            } while (info.enemy_ships.grid[x - 1][y - 1] == 'O' || info.enemy_ships.grid[x - 1][y - 1] == 'X');

            repeat = check_zone(x, y, horizontal, move, move_minus);
            if (move != 0)
            {
                if (move_minus)
                {
                    if (horizontal)
                    {
                        x = x - move;
                    }
                    else
                    {
                        y = y - move;
                    }
                }
                else
                {
                    if (horizontal)
                    {
                        x = x + move;
                    }
                    else
                    {
                        y = y + move;
                    }
                }
            }

        }
    }
    coords.X = x;
    coords.Y = y;

    result_of_step.result_shot = (*another_player).info.my_ships.processShot(x, y); // Выполняем выстрел
    info.enemy_ships.processShot(x, y);

    my_stat(result_of_step.result_shot);

    if (result_of_step.result_shot.rezult_of_shot == 1) // Попадание
    {
        playHitSound();
        ship_sells--;
        if (ship_sells == 0)
        {
            result_of_step.win_player = true;
        }

        if (result_of_step.result_shot.damaged_more_than_one_square == true && result_of_step.result_shot.ship_dead == false)
        {
            result_of_step.target_queue.clear();
        }
        if (result_of_step.result_shot.ship_dead) // Уничтожение корабля
        {
            playDestroySound();
            result_of_step.target_mode = false;      // Сбрасываем режим уничтожения
            result_of_step.target_queue.clear();     // Очищаем очередь
        }
        else
        {
            result_of_step.target_mode = true;       // Включаем режим уничтожения
            //          [x,y-1]
            // [x-1,y]  клетка   [x+1,y]
            //          [x,y+1]
            // Добавляем соседние клетки в очередь

            bool shot_in_line = false;

            // проверка на 2 попадания в линию:

            //                          [x,y-1]
            //    [x-2,y]   [x-1,y]=X      X      [x+1,y]
            //                          [x,y+1]

            int (*countSteps)(int, int, int, int, int, PlayerInfo) = [](int x, int y, int fieldSize, int step, int direction, PlayerInfo info)
                {
                    while (true)
                    {
                        if (direction == 0 && x - step > 1 && info.enemy_ships.grid[x - 2 - step][y - 1] == 'X') // Влево
                            step++;
                        else if (direction == 1 && x + step < fieldSize && info.enemy_ships.grid[x + step][y - 1] == 'X') // Вправо
                            step++;
                        else if (direction == 2 && y - step > 1 && info.enemy_ships.grid[x - 1][y - 2 - step] == 'X') // Вверх
                            step++;
                        else if (direction == 3 && y + step < fieldSize && info.enemy_ships.grid[x - 1][y + step] == 'X') // Вниз
                            step++;
                        else
                            break;
                    }
                    return step;
                };

            int step_left = countSteps(x, y, fieldSize, 0, 0, info);  // 0 - влево
            int step_right = countSteps(x, y, fieldSize, 0, 1, info); // 1 - вправо
            int step_up = countSteps(x, y, fieldSize, 0, 2, info);    // 2 - вверх
            int step_down = countSteps(x, y, fieldSize, 0, 3, info);  // 3 - вниз

            if (step_right > 0 || step_left > 0 || step_up > 0 || step_down > 0)
            {
                shot_in_line = true;
            }


            if (shot_in_line)
            {
                result_of_step.target_queue.clear();     // Очищаем очередь
                if (x - step_left - 1 >= 1 && step_up == 0 && step_down == 0)
                {
                    if (info.enemy_ships.grid[x - 2 - step_left][y - 1] == 'S' || info.enemy_ships.grid[x - 2 - step_left][y - 1] == ' ')
                    {
                        COORD new_coord = { x - 1 - step_left, y };
                        result_of_step.target_queue.push_back(new_coord);
                    }
                }
                if (x + step_right + 1 < fieldSize && step_up == 0 && step_down == 0)
                {
                    if (info.enemy_ships.grid[x + step_right][y - 1] == 'S' || info.enemy_ships.grid[x + step_right][y - 1] == ' ')
                    {
                        COORD new_coord = { x + 1 + step_right, y };
                        result_of_step.target_queue.push_back(new_coord);
                    }
                }
                if (y - step_up - 1 >= 1 && step_left == 0 && step_right == 0)
                {
                    if (info.enemy_ships.grid[x - 1][y - 2 - step_up] == 'S' || info.enemy_ships.grid[x - 1][y - 2 - step_up] == ' ')
                    {
                        COORD new_coord = { x, y - 1 - step_up };
                        result_of_step.target_queue.push_back(new_coord);
                    }
                }
                if (y + 1 + step_down < fieldSize && step_left == 0 && step_right == 0)
                {
                    if (info.enemy_ships.grid[x - 1][y + step_down] == 'S' || info.enemy_ships.grid[x - 1][y + step_down] == ' ')
                    {
                        COORD new_coord = { x, y + 1 + step_down };
                        result_of_step.target_queue.push_back(new_coord);
                    }
                }

            }

            if (!shot_in_line) // если нету 2 попаданий в линию:
            {
                if (x > 1 && (info.enemy_ships.grid[x - 2][y - 1] == 'S' || info.enemy_ships.grid[x - 2][y - 1] == ' ')) //
                {
                    COORD new_coord = { x - 1, y };
                    result_of_step.target_queue.push_back(new_coord);
                }
                if (x < fieldSize && (info.enemy_ships.grid[x][y - 1] == 'S' || info.enemy_ships.grid[x][y - 1] == ' '))
                {
                    COORD new_coord = { x + 1, y };
                    result_of_step.target_queue.push_back(new_coord);
                }
                if (y > 1 && (info.enemy_ships.grid[x - 1][y - 2] == 'S' || info.enemy_ships.grid[x - 1][y - 2] == ' '))
                {
                    COORD new_coord = { x, y - 1 };
                    result_of_step.target_queue.push_back(new_coord);
                }
                if (y < fieldSize && (info.enemy_ships.grid[x - 1][y] == 'S' || info.enemy_ships.grid[x - 1][y] == ' '))
                {
                    COORD new_coord = { x, y + 1 };
                    result_of_step.target_queue.push_back(new_coord);
                }
            }
        }
        result_of_step.in_a_row = true; // попал, первый выстрел in a row
        
        return ;
    }
    else if (result_of_step.result_shot.rezult_of_shot == 0)
    {
        result_of_step.in_a_row = false; // прерывание череды выстрелов
        return ;
    }
    else
    {
        result_of_step.in_a_row = false; // прерывание череды выстрелов
        return ;
    }
}

void Player::Attack_manual(Player* another_player)
{
    int right_edge, left_edge, formula;
    if (info.titul_player == "Player 1" && info.person && another_player->info.person)
    {
        left_edge = 5 + 4;
        right_edge = info.my_ships.size * 4 + 2;
        formula = left_edge - 4;
    }
    else
    {
        left_edge = 5 + info.my_ships.size * 4 + 6;
        right_edge = 5 + info.my_ships.size * 4 + 6 + info.my_ships.size * 4 - 4;
        formula = left_edge;
    }

    SetCursor(coords.X, coords.Y);
    int key;
    do
    {
        key = _getch(); //функция возвращает номер нажатой клавиши
        switch (key)
        {
        case Left:
            if (coords.X > left_edge)
            {
                coords.X = coords.X - 4;
                SetCursor(coords.X, coords.Y);
            }
            break;
        case Right:
            if (coords.X < right_edge)
            {
                coords.X = coords.X + 4;
                SetCursor(coords.X, coords.Y);
            }
            break;
        case Up:
            if (coords.Y > 3)
            {
                coords.Y = coords.Y - 2;
                SetCursor(coords.X, coords.Y);
            }
            break;
        case Down:
            if (coords.Y < 3 + info.my_ships.size * 2 - 2)
            {
                coords.Y = coords.Y + 2;
                SetCursor(coords.X, coords.Y);
            }
            break;
        case Enter:
            result_of_step.result_shot = (*another_player).info.my_ships.processShot(((coords.X - formula) / 4) + 1, ((coords.Y - 3) / 2) + 1);
            info.enemy_ships.processShot(((coords.X - formula) / 4) + 1, ((coords.Y - 3) / 2) + 1);

            if (info.person && another_player->info.person)
            {
                info.my_ships.display(info.enemy_ships, info.titul_player);
            }
            else
            {
                info.my_ships.display(true, info.enemy_ships);
            }

            my_stat(result_of_step.result_shot);

            if (result_of_step.result_shot.ship_dead) // Уничтожение корабля
            {
                playDestroySound();
            }

            if (result_of_step.result_shot.rezult_of_shot == 1)
            {
                playHitSound();
                ship_sells--;
                if (ship_sells == 0)
                {
                    result_of_step.win_player = true;
                    break;
                }
                result_of_step.in_a_row = true; // попадание
            }
            else if (result_of_step.result_shot.rezult_of_shot == 0)
            {

                Sleep(pause_duration);
                result_of_step.in_a_row = false; // промах
            }
            else if (result_of_step.result_shot.rezult_of_shot == -1)
            {

                Sleep(pause_duration);
                result_of_step.in_a_row = true; // промах, но нужно продолжить бить
            }
            break;
        }
    } while (key != Enter);
}

// вывод живых кораблей
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

    return ships_player;
}

void Player::paintFutureShip(int x, int y, ShipType ship_player, bool vertical)
{
    COORD new_data = { (x - 5) / 4, (y - 3) / 2 };
    Ship new_ship(ship_player.name, ship_player.size, new_data, vertical);
    if (info.my_ships.can_place_ship(new_ship))
    {
        info.my_ships.display_ship_on_board(new_ship, x, y);
    }
}

void Player::my_stat(PlayerResultOfShot rezult)
{
    if (rezult.rezult_of_shot == 1) // если это успешное попадание
    {
        statistic.processed_shots++;
        statistic.sucessful_shots++;
        float suc_shot = statistic.sucessful_shots;
        statistic.percent_suc_shots = static_cast <int>(100 * (suc_shot / statistic.processed_shots));
    }
    else if (rezult.rezult_of_shot == 0) // если это промах
    {
        statistic.processed_shots++;
        float suc_shot = statistic.sucessful_shots;
        statistic.percent_suc_shots = static_cast <int>(100 * (suc_shot / statistic.processed_shots));
    }

    if (rezult.ship_dead == true)
    {
        statistic.stat_dead_enemy_ships[rezult.size_of_ship - 1].count++;
        statistic.drawned_ships++;
        statistic.stat_alive_enemy_ships = CalcStatShips(statistic.initial_ships, statistic.stat_alive_enemy_ships, statistic.initial_ships, info.enemy_ships);
    }
    
}

void Player::output_stat(int fieldSize,int y)
{
    const int tabel_width = 26;

    info.my_ships.setColorWithBackground(2, 0);

    // Функция для рисования горизонтальной линии
    void (*draw_line)(int) = [](int width)
        {
            cout << "+" << string(width - 1, '-') << "+" << endl;
        };

    // Функция для вывода строки с данными
    void (*print_row)(const string&, int, int, const PlayerInfo) = [](const string& label, int value, int width, PlayerInfo info)
        {
            cout << "| ";
            info.my_ships.setColorWithBackground(3, 0);
            cout << "    " << setw(width) << left << label;
            cout << ": " << setw(3) << right << value << " ";
            info.my_ships.setColorWithBackground(2, 0);
            cout << " |" << endl;
        };

    draw_line(tabel_width);
    cout << "|Живые корабли противника:|\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        print_row(statistic.stat_alive_enemy_ships[i].name, statistic.stat_alive_enemy_ships[i].count, tabel_width / 2, info);
    }
    draw_line(tabel_width);

    SetCursor(tabel_width + 3, y);
    draw_line(tabel_width);
    SetCursor(tabel_width + 3, y + 1);
    cout << "| Уничтоженный противник :|\n";
    for (int i = 0; i < statistic.initial_ships.size(); i++)
    {
        SetCursor(tabel_width + 3, y + i + 2);
        print_row(statistic.stat_dead_enemy_ships[i].name, statistic.stat_dead_enemy_ships[i].count, tabel_width / 2, info);
        if (i == statistic.initial_ships.size() - 1)
        {
            SetCursor(tabel_width + 3, y + i + 3);
        }
    }
    draw_line(tabel_width);

    info.my_ships.setColorWithBackground(3, 0);
    cout << "    Количество потопленных кораблей: " << statistic.drawned_ships << ".\n";
    cout << "    Количесво совершенных выстрелов: " << statistic.processed_shots << ".\n";
    cout << "      Количество успешных попаданий: " << statistic.sucessful_shots << ".\n";
    cout << "         Процент успешных попаданий: " << statistic.percent_suc_shots << "%.\n";
    info.my_ships.setColorWithBackground(7, 0);
}