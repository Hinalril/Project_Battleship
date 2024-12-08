
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"
#include "Player.h"


// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА ВАДИМУ ###############################

// функция вычисления количества кораблей
vector<ShipType> calculateShips(int fieldSize, int& remember_ship_sells)
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

    return ships;
}
// ##############################################################################################

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
// ##############################################################################################


// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА ВАДИМУ ###############################
int inputFieldSize()
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
// ##############################################################################################


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА ВАДИМУ ###############################
    int fieldSize = inputFieldSize();
    // ##############################################################################################

    Board board_first_player_ally(fieldSize, false);
    Board board_second_player_ally(fieldSize, false);


    // ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА ВАДИМУ ###############################
    // Расчёт количества кораблей
    int remember_ship_sells = 0; // количество ячеек, которые занимают корабли
    vector<ShipType> ships = calculateShips(fieldSize, remember_ship_sells);

    cout << "Рекомендуемое количество кораблей для поля " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships)
    {
        cout << ship.name << " (размер " << ship.size << "): " << ship.count << "\n";
    }
    Sleep(4000);
    // ##############################################################################################


    Player player1("Игрок 1", true, board_first_player_ally, board_second_player_ally, ships), player2("Игрок 2", true, board_second_player_ally, board_first_player_ally, ships);

    player1.BoardShipPlacement(player1.statistic.stat_alive_enemy_ships, fieldSize, "Игрок 1", player2);
    //player2.AutoBoardShipPlacement(player2.player_ships, fieldSize, "Игрок 2", player1);
    player2.BoardShipPlacement(player2.statistic.stat_alive_enemy_ships, fieldSize, "Игрок 2", player1);


    bool win_first_player = false, win_second_player = false;
    int ship_sells_first_player = remember_ship_sells, ship_sells_second_player = remember_ship_sells;

    int remember_x_first_player = 5 + fieldSize * 4 + 6;
    int remember_y_first_player = 3;

    int remember_x_second_player = 5 + fieldSize * 4 + 6;
    int remember_y_second_player = 3;

    while (!win_first_player && !win_second_player)
    {
        int rezult_first_player = 1; // int - результат попадания
                                     // 1 - попадание
                                     // 0 - промах
                                     // -1 - клетка уже занята, повтор хода
        bool first_shot = true; // отслеживание первого выстрела в ходе игрока (если сделан первый выстрел, значит игрок может сделать 2-ой)

        // ходит игрок №1
        while ((rezult_first_player == 1 || rezult_first_player == -1) && !win_first_player)
        {
            system("cls");
            cout << "Информация полей боя для 1-го игрока.\n";
            player1.info.my_ships.display(true, player1.info.enemy_ships);
            player1.output_stat(fieldSize);

            if (rezult_first_player == 1 && first_shot == false)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            if (rezult_first_player == -1)
            {
                cout << "Вы уже стреляли в эту координату. Ходите заново!\n";
            }

            player1.Attack_manual(&remember_x_first_player, &remember_y_first_player, fieldSize, &rezult_first_player, &first_shot, &player2, &ship_sells_first_player, &win_first_player);
        }

        int rezult_second_player = 1;
        first_shot = true;

        // ходит игрок №2
        while ((rezult_second_player == 1 || rezult_second_player == -1) && !win_second_player && !win_first_player)
        {
            system("cls");
            cout << "Информация полей боя для 2-го игрока.\n";
            player2.info.my_ships.display(true, player2.info.enemy_ships);
            player2.output_stat(fieldSize);

            if (rezult_second_player == 1 && first_shot == false)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            if (rezult_second_player == -1)
            {
                cout << "Вы уже стреляли в эту координату. Ходите заново!\n";
            }

            player2.Attack_manual(&remember_x_second_player, &remember_y_second_player, fieldSize, &rezult_second_player, &first_shot, &player1, &ship_sells_second_player, &win_second_player);
        }
    }

    if (win_first_player)
    {
        cout << "Победа 1-го игрока!\n";
    }
    else
    {
        cout << "Победа 2-го игрока!\n";
    }

    return 0;
}