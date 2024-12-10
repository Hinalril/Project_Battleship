
#include "Game.h"

Game::Game(string name1, string name2)
    :player_name_1(name1), player_name_2(name2) {}

// инициализация игры (установка размера поля, количество кораблей)
int Game::game_enter()
{
    int fieldSize;
    string input;

    while (true)
    {
        cout << "Введите размер поля: ";
        getline(cin, input); // Считываем строку целиком

        bool isInt = true; // Проверка строки на целое число
        size_t startIndex = 0;

        if (input.empty())
        {
            isInt = false;
        }
        else if (input[0] == '-')
        {
            if (input.size() == 1)
            {
                isInt = false; // "-" не является числом
            }
            else
            {
                startIndex = 1; // Начинаем проверку с первого символа после "-"
            }
        }

        for (int i = startIndex; i < input.size() && isInt; i++)
        {
            if (input[i] < '0' || input[i] > '9') // Проверяем, является ли символ цифрой
            {
                isInt = false;
            }
        }

        if (isInt)
        {
            fieldSize = stoi(input); // Преобразуем строку в число

            if (fieldSize >= 3)
            {
                break; // Корректное значение введено
            }
            else if (fieldSize < 0)
            {
                cout << "Размер поля должен быть положительным! Попробуйте снова.\n";
            }
            else
            {
                cout << "Выбрано слишком малое поле. Введите значение > 2.\n";
            }
        }
        else
        {
            cout << "Ошибка ввода! Пожалуйста, введите целое число.\n";
        }
    }
    return fieldSize;
}

vector<ShipType> Game::calculate_ships(int fieldSize, int& remember_ship_sells)
{
    int totalCells = fieldSize * fieldSize;        // Общее количество клеток
    int shipCells = static_cast<int>(totalCells * 0.2); // Клетки для кораблей
    remember_ship_sells = shipCells;

    // Инициализация кораблей
    vector<ShipType> ships =
    {
        {"Flagship", 6, 0},    // 6-палубный
        {"Carrier", 5, 0},     // 5-палубный
        {"Battleship", 4, 0},  // 4-палубный
        {"Cruiser", 3, 0},     // 3-палубный
        {"Destroyer", 2, 0},   // 2-палубный
        {"Submarine", 1, 0}    // 1-палубный
    };

    // Основной цикл распределения
    while (shipCells > 0)
    {
        bool added = false;

        // Начинаем распределение с однопалубных
        for (int i = ships.size() - 1; i >= 0; --i)
        {
            // Проверяем, достаточно ли клеток для нового корабля
            if (shipCells >= ships[i].size)
            {
                // Проверяем строгую пропорцию
                if (i == ships.size() - 1 || ships[i].count < ships[i + 1].count - 1)
                {
                    ships[i].count++;          // Увеличиваем количество текущего корабля
                    shipCells -= ships[i].size; // Уменьшаем оставшиеся клетки
                    added = true;
                }
            }
        }

        // Если ни один корабль не был добавлен, выходим из цикла
        if (!added)
        {
            break;
        }
    }

    cout << "Рекомендуемое количество кораблей для поля " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships)
    {
        cout << ship.name << " (размер " << ship.size << "): " << ship.count << "\n";
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
        cout << "Победа 1-го игрока!\n";
    }
    else
    {
        cout << "Победа 2-го игрока!\n";
    }
}

int Game::info_players(Player player1, Player player2)
{
    if ((player1.result_of_step.result_shot.rezult_of_shot == 1 || player1.result_of_step.result_shot.rezult_of_shot == -1) && !player2.result_of_step.in_a_row)
    {
        return 1; // ход 1-го игрока
    }
    if (player2.result_of_step.result_shot.rezult_of_shot == 1 || player2.result_of_step.result_shot.rezult_of_shot == -1)
    {
        return 2; // ход 2-го игрока
    }
    return 0; // чья-то победа, прекращение ходов if (player1.result_of_step.win_player || player2.result_of_step.win_player)
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
    cout << "Информация полей боя для 1-го игрока.\n";
    player1.output_stat(player1.info.enemy_ships.size, 1);
    cout << "\nИнформация полей боя для 2-го игрока.\n";
    player2.output_stat(player2.info.enemy_ships.size, 16);
}

void Game::output_info(Player me, Player enemy)
{
    clearPartOfConsole(0, 15 + me.info.my_ships.size * 2);
    cout << "Информация полей боя для 1-го игрока.\n";
    me.info.my_ships.display(true, me.info.enemy_ships);
    me.output_stat(me.info.enemy_ships.size, 3 + me.info.my_ships.size * 2);
}

void Game::process_steps(Player& me, Player& enemy, bool computer)
{
    if (!computer) // ДЛЯ ИГРОКА
    {
        enemy.result_of_step.result_shot.rezult_of_shot = 1;
        enemy.result_of_step.in_a_row = false;

        output_info(me, enemy);
        me.Attack_manual(&enemy);
        output_info(me, enemy);
        save_log(me, false);
    }
    else // ДЛЯ КОМПЬЮТЕРА
    {
        enemy.result_of_step.result_shot.rezult_of_shot = 1;
        enemy.result_of_step.in_a_row = false;

        me.Attack_computer(&enemy);
        enemy.info.my_ships.display(true, enemy.info.enemy_ships);
        save_log(me, true);
        //Sleep(1000);
    }
}