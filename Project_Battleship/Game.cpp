
#include "Game.h"

void playVictorySound()
{
    Beep(294, 1000 / 8);
    Beep(440, 1000 / 4);
    Beep(262 * 2, 1000 / 4);
    Beep(330 * 2, 1000 / 4);
    Beep(415, 1000 / 8);
    Beep(440, 1000);
}

Game::Game(string name1, string name2)
    :player_name_1(name1), player_name_2(name2) {}

Game::Game() : player_name_1(""), player_name_2("") {}

void Game::clearPartOfConsole(int startLine, int endLine)
{
    cout << "\033[" << startLine << ";0H";     // Переместить курсор к начальной строке

    // Очистить каждую строку в указанном диапазоне
    for (int i = startLine; i <= endLine; i++)
    {
        cout << "\033[K"; // Очистить текущую строку
        if (i != endLine)
        {
            cout << "\033[E"; // Перейти на следующую строку
        }
    }
    cout << "\033[" << startLine << ";0H";     // Вернуть курсор к первой очищенной строке
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
            scroll_log(2);
            log[0] = "+-------------------------------------------+\n";
            log[1] = "| " + me.info.name + " Промах.Ход переходит другому игроку! \n";
            displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
        }
        else if (me.result_of_step.result_shot.rezult_of_shot == 1)
        {
            if (me.result_of_step.result_shot.ship_dead == true)
            {
                scroll_log(3);
                log[0] = "+-------------------------------------------+\n";
                log[1] = "| " + me.info.name + " Попадание!Ходите ещё раз.              \n";
                log[2] = "| Корабль уничтожен!                             \n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
            else
            {
                scroll_log(2);
                log[0] = "+-------------------------------------------+\n";
                log[1] = "| " + me.info.name + " Попадание!Ходите ещё раз.              \n";
                displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
            }
        }
        else if (me.result_of_step.result_shot.rezult_of_shot == -1)
        {
            scroll_log(3);
            log[0] = "+-------------------------------------------+\n";
            log[1] = "| " + me.info.name + ", Вы уже стреляли в эту клетку. \n";
            log[2] = "| Стреляйте ещё раз!                    \n";
            displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
        }
    }
    else
    {
        if (me.result_of_step.result_shot.rezult_of_shot == 0)
        {
            scroll_log(2);
            log[0] = "+-------------------------------------------+\n";
            log[1] = "| " + me.info.name + " атакует (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - промах.           \n";
            displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
        }
        else if (me.result_of_step.result_shot.rezult_of_shot == 1)
        {
            scroll_log(2);
            log[0] = "+-------------------------------------------+\n";
            log[1] = "| " + me.info.name + " атакует (" + to_string(me.coords.X) + ", " + to_string(me.coords.Y) + ") - попадание!       \n";
            displayLog(45, me.info.my_ships.size * 2 - 1 + 15 + 3);
        }
    }
}

void Game::SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //Способ перемещения курсора на нужные координаты
}

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
        SetCursor(60, 5);
        cout << "ПОБЕДА ДЛЯ " << player1.info.name << "!              ";
    }
    else
    {
        SetCursor(60, 20);
        cout << "ПОБЕДА ДЛЯ " << player2.info.name << "!              ";
    }
    playVictorySound();
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

void Game::cycle_play(Player& player1, Player& player2, bool person1, bool person2)
{
    bool only_comp = false;
    if (person1 == false && person2 == false)
    {
        only_comp = true;
    }

    while (check_status(player1, player2))
    {
        int turn = info_players(player1, player2);

        switch (turn)
        {
        case 1:
            process_steps(player1, player2, person1, only_comp);
            break;
        case 2:
            process_steps(player2, player1, person2, only_comp);
            break;
        case 0:
            break;
        }
    }

    output_finish_info(player1, player2);
    game_end(player1, player2);
    system("pause");
};

void Game::output_finish_info(Player player1, Player player2)
{
    system("cls");
    cout << "Информация полей боя для игрока "<< player1.info.name << ".\n";
    player1.output_stat(player1.info.enemy_ships.size, 1);
    cout << "Информация полей боя для игрока " << player2.info.name << ".\n";
    player2.output_stat(player2.info.enemy_ships.size, 15);
}

void Game::output_info(Player me, Player enemy)
{
    clearPartOfConsole(0, 15 + me.info.my_ships.size * 2);
    cout << "Информация полей боя для игрока " << me.info.name << ".\n";
    if (me.info.person && enemy.info.person)
    {
        me.info.my_ships.display(me.info.enemy_ships, me.info.titul_player);
    }
    else
    {
        me.info.my_ships.display(true, me.info.enemy_ships);
    }
    me.output_stat(me.info.enemy_ships.size, 3 + me.info.my_ships.size * 2);
}

void Game::process_steps(Player& me, Player& enemy, bool person, bool only_comp)
{
    if (person) // ДЛЯ ИГРОКА
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

        if (only_comp)
        {
            output_info(me, enemy);
        }

        me.Attack_computer(&enemy);
        if (only_comp)
        {
            output_info(me, enemy);
        }
        else
        {
            enemy.info.my_ships.display(true, enemy.info.enemy_ships);
        }

        save_log(me, true);
        Sleep(0);
    }
}