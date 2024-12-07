
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"

HANDLE hStdOutMain = GetStdHandle(STD_OUTPUT_HANDLE); //Дескриптор активного окна

enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };

// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА НАСТЕ ############################### НАЧАЛО №1
struct ShipType
{
    string name;
    int size;
    int count;
};

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
// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА НАСТЕ ############################### КОНЕЦ №1

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

void SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(hStdOutMain, myCoords); //Способ перемещения курсора на нужные координаты
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int remember_ship_sells = 0; // количество ячеек, которые занимают корабли

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

    Board board_first_player_ally(fieldSize, false);
    Board board_first_player_enemy(fieldSize, true);

    Board board_second_player_ally(fieldSize, false);
    Board board_second_player_enemy(fieldSize, true);

    // Расчёт количества кораблей
    vector<ShipType> ships = calculateShips(fieldSize, remember_ship_sells);
    vector<ShipType> ships_first_player =
    {
        {"Submarine", 1, ships[5].count},  // 1-палубный
        {"Destroyer", 2, ships[4].count},  // 2-палубный
        {"Cruiser", 3, ships[3].count},    // 3-палубный
        {"Battleship", 4, ships[2].count}, // 4-палубный
        {"Carrier", 5, ships[1].count },   // 5-палубный
        {"Flagship", 6, ships[0].count}    // 6-палубный
    };
    vector<ShipType> ships_second_player =
    {
        {"Submarine", 1, ships[5].count},  // 1-палубный
        {"Destroyer", 2, ships[4].count},  // 2-палубный
        {"Cruiser", 3, ships[3].count},    // 3-палубный
        {"Battleship", 4, ships[2].count}, // 4-палубный
        {"Carrier", 5, ships[1].count },   // 5-палубный
        {"Flagship", 6, ships[0].count}    // 6-палубный
    };

    cout << "Рекомендуемое количество кораблей для поля " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships)
    {
        cout << ship.name << " (размер " << ship.size << "): " << ship.count << "\n";
    }
    Sleep(4000);

    int remember_x_first_player_placement = 5;
    int remember_y_first_player_placement = 3;

    // расположение кораблей для игрока №1
    for (int i = 0; i < ships.size(); i++)
    {
        for (int j = 0; j < ships[i].count; j++)
        {
            bool vertical = false;

            system("cls");
            cout << "Игрок 1, расположите свои корабли.\n";
            board_first_player_ally.display(false, board_first_player_ally);
            cout << "Расположите " << ships[i].name << " (размер " << ships[i].size << "):\n";

            SetCursor(remember_x_first_player_placement, remember_y_first_player_placement);
            int key;
            do
            {
                key = _getch(); //функция возвращает номер нажатой клавиши
                switch (key)
                {
                case Left:
                    if (remember_x_first_player_placement > 5)
                    {
                        remember_x_first_player_placement = remember_x_first_player_placement - 4;
                        SetCursor(remember_x_first_player_placement, remember_y_first_player_placement);
                    }
                    break;
                case Right:
                    if (remember_x_first_player_placement < 5 + fieldSize * 4 - 4)
                    {
                        remember_x_first_player_placement = remember_x_first_player_placement + 4;
                        SetCursor(remember_x_first_player_placement, remember_y_first_player_placement);
                    }
                    break;
                case Up:
                    if (remember_y_first_player_placement > 3)
                    {
                        remember_y_first_player_placement = remember_y_first_player_placement - 2;
                        SetCursor(remember_x_first_player_placement, remember_y_first_player_placement);
                    }
                    break;
                case Down:
                    if (remember_y_first_player_placement < 3 + fieldSize * 2 - 2)
                    {
                        remember_y_first_player_placement = remember_y_first_player_placement + 2;
                        SetCursor(remember_x_first_player_placement, remember_y_first_player_placement);
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
                    pair<int, int> new_data((remember_x_first_player_placement - 5) / 4, (remember_y_first_player_placement - 3) / 2);
                    Ship new_ship(ships[i].name, ships[i].size, new_data, vertical);
                    if (board_first_player_ally.can_place_ship(new_ship))
                    {
                        board_second_player_enemy.placeShip(new_ship);
                        board_first_player_ally.placeShip(new_ship);
                    }
                    else
                    {
                        system("cls");
                        cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                        j--; // Повторим попытку размещения для этого корабля
                    }
                    break;
                }
            } while (key != Enter);
        }
    }
    Sleep(2000);

    int remember_x_second_player_placement = 5;
    int remember_y_second_player_placement = 3;

    // расположение кораблей для игрока №2
    for (int i = 0; i < ships.size(); i++)
    {
        for (int j = 0; j < ships[i].count; j++)
        {
            bool vertical = false;

            system("cls");
            cout << "Игрок 2, расположите свои корабли.\n";
            board_second_player_ally.display(false, board_second_player_ally);
            cout << "Расположите " << ships[i].name << " (размер " << ships[i].size << "):\n";

            SetCursor(remember_x_second_player_placement, remember_y_second_player_placement);
            int key;
            do
            {
                key = _getch();//функция возвращает номер нажатой клавиши
                switch (key)
                {
                case Left:
                    if (remember_x_second_player_placement > 5)
                    {
                        remember_x_second_player_placement = remember_x_second_player_placement - 4;
                        SetCursor(remember_x_second_player_placement, remember_y_second_player_placement);
                    }
                    break;
                case Right:
                    if (remember_x_second_player_placement < 5 + fieldSize * 4 - 4)
                    {
                        remember_x_second_player_placement = remember_x_second_player_placement + 4;
                        SetCursor(remember_x_second_player_placement, remember_y_second_player_placement);
                    }
                    break;
                case Up:
                    if (remember_y_second_player_placement > 3)
                    {
                        remember_y_second_player_placement = remember_y_second_player_placement - 2;
                        SetCursor(remember_x_second_player_placement, remember_y_second_player_placement);
                    }
                    break;
                case Down:
                    if (remember_y_second_player_placement < 3 + fieldSize * 2 - 2)
                    {
                        remember_y_second_player_placement = remember_y_second_player_placement + 2;
                        SetCursor(remember_x_second_player_placement, remember_y_second_player_placement);
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
                    pair<int, int> new_data((remember_x_second_player_placement - 5) / 4, (remember_y_second_player_placement - 3) / 2);
                    Ship new_ship(ships[i].name, ships[i].size, new_data, vertical);
                    if (board_second_player_ally.can_place_ship(new_ship))
                    {
                        board_first_player_enemy.placeShip(new_ship);
                        board_second_player_ally.placeShip(new_ship);
                    }
                    else
                    {
                        system("cls");
                        cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                        j--; // Повторим попытку размещения для этого корабля
                    }
                    break;
                }
            } while (key != Enter);
        }
    }
    Sleep(2000);


    bool win_first_player = false, win_second_player = false;
    int ship_sells_first_player = remember_ship_sells, ship_sells_second_player = remember_ship_sells;

    int remember_x_first_player = 5 + fieldSize * 4 + 6;
    int remember_y_first_player = 3;

    int remember_x_second_player = 5 + fieldSize * 4 + 6;
    int remember_y_second_player = 3;

    while (!win_first_player && !win_second_player)
    {
        int rezult_first_player = 1;
        bool first_shot = true;

        // Выстрелы
        // Ходит Игрок №2
        while ((rezult_first_player == 1 || rezult_first_player == -1) && !win_first_player)
        {
            system("cls");
            cout << "Информация полей боя для 1-го игрока.\n";
            board_first_player_ally.display(true, board_first_player_enemy);
            output_ships(ships, ships_first_player, board_second_player_ally);

            if (rezult_first_player == 1 && first_shot == false)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            if (rezult_first_player == -1)
            {
                cout << "Вы уже стреляли в эту координату. Ходите заново!\n";
            }

            SetCursor(remember_x_first_player, remember_y_first_player);
            int key;
            do
            {
                key = _getch();//функция возвращает номер нажатой клавиши
                switch (key)
                {
                case Left:
                    if (remember_x_first_player > 5 + fieldSize * 4 + 6)
                    {
                        remember_x_first_player = remember_x_first_player - 4;
                        SetCursor(remember_x_first_player, remember_y_first_player);
                    }
                    break;
                case Right:
                    if (remember_x_first_player < 5 + fieldSize * 4 + 6 + fieldSize * 4 - 4)
                    {
                        remember_x_first_player = remember_x_first_player + 4;
                        SetCursor(remember_x_first_player, remember_y_first_player);
                    }
                    break;
                case Up:
                    if (remember_y_first_player > 3)
                    {
                        remember_y_first_player = remember_y_first_player - 2;
                        SetCursor(remember_x_first_player, remember_y_first_player);
                    }
                    break;
                case Down:
                    if (remember_y_first_player < 3 + fieldSize * 2 - 2)
                    {
                        remember_y_first_player = remember_y_first_player + 2;
                        SetCursor(remember_x_first_player, remember_y_first_player);
                    }
                    break;
                case Enter:
                    rezult_first_player = board_second_player_ally.processShot(((remember_x_first_player - 5 - 6 - fieldSize * 4) / 4) + 1, ((remember_y_first_player - 3) / 2) + 1);
                    board_first_player_enemy.processShot(((remember_x_first_player - 5 - 6 - fieldSize * 4) / 4) + 1, ((remember_y_first_player - 3) / 2) + 1);

                    if (rezult_first_player == 1)
                    {
                        ship_sells_first_player--;
                        if (ship_sells_first_player == 0)
                        {
                            system("cls");
                            board_first_player_ally.display(true, board_first_player_enemy);
                            win_first_player = true;
                            break;
                        }
                        first_shot = false; // первый выстрел был сделан
                    }

                    if (rezult_first_player == 0)
                    {
                        system("cls");
                        cout << "Информация полей боя для 1-го игрока.\n";
                        board_first_player_ally.display(true, board_first_player_enemy);
                        cout << "Промах. Ход переходит 2-му игроку!\n";
                        system("pause");
                    }
                    break;
                }
            } while (key != Enter);
        }

        int rezult_second_player = 1;
        first_shot = true;

        while ((rezult_second_player == 1 || rezult_second_player == -1) && !win_second_player && !win_first_player)
        {
            system("cls");
            cout << "Информация полей боя для 2-го игрока.\n";
            board_second_player_ally.display(true, board_second_player_enemy);
            output_ships(ships, ships_second_player, board_first_player_ally);

            if (rezult_second_player == 1 && first_shot == false)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            if (rezult_second_player == -1)
            {
                cout << "Вы уже стреляли в эту координату. Ходите заново!\n";
            }

            SetCursor(remember_x_second_player, remember_y_second_player);
            int key;
            do
            {
                key = _getch();//функция возвращает номер нажатой клавиши
                switch (key)
                {
                case Left:
                    if (remember_x_second_player > 5 + fieldSize * 4 + 6)
                    {
                        remember_x_second_player = remember_x_second_player - 4;
                        SetCursor(remember_x_second_player, remember_y_second_player);
                    }
                    break;
                case Right:
                    if (remember_x_second_player < 5 + fieldSize * 4 + 6 + fieldSize * 4 - 4)
                    {
                        remember_x_second_player = remember_x_second_player + 4;
                        SetCursor(remember_x_second_player, remember_y_second_player);
                    }
                    break;
                case Up:
                    if (remember_y_second_player > 3)
                    {
                        remember_y_second_player = remember_y_second_player - 2;
                        SetCursor(remember_x_second_player, remember_y_second_player);
                    }
                    break;
                case Down:
                    if (remember_y_second_player < 3 + fieldSize * 2 - 2)
                    {
                        remember_y_second_player = remember_y_second_player + 2;
                        SetCursor(remember_x_second_player, remember_y_second_player);
                    }
                    break;
                case Enter:
                    rezult_second_player = board_first_player_ally.processShot(((remember_x_second_player - 5 - 6 - fieldSize * 4) / 4) + 1, ((remember_y_second_player - 3) / 2) + 1);
                    board_second_player_enemy.processShot(((remember_x_second_player - 5 - 6 - fieldSize * 4) / 4) + 1, ((remember_y_second_player - 3) / 2) + 1);

                    if (rezult_second_player == 1)
                    {
                        ship_sells_second_player--;
                        if (ship_sells_second_player == 0)
                        {
                            system("cls");
                            board_second_player_ally.display(true, board_second_player_enemy);
                            win_second_player = true;
                            break;
                        }
                        first_shot = false; // первый выстрел был сделан
                    }

                    if (rezult_second_player == 0)
                    {
                        system("cls");
                        cout << "Информация полей боя для 2-го игрока.\n";
                        board_second_player_ally.display(true, board_second_player_enemy);
                        cout << "Промах. Ход переходит 1-му игроку!\n";
                        system("pause");
                    }
                    break;
                }
            } while (key != Enter);
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