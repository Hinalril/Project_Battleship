
#include <iostream>
#include <Windows.h>
#include "Board.h"
#include "Ship.h"

// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА НАСТЕ ############################### НАЧАЛО №1
struct ShipType
{
    string name;
    int size;
    int count;
};

vector<ShipType> calculateShips(int fieldSize)
{
    int totalCells = fieldSize * fieldSize;        // Общее количество клеток
    int shipCells = static_cast<int>(totalCells * 0.2); // Клетки для кораблей

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
            break;
    }

    return ships;
}
// ############################### ОТДАТЬ ДАННЫЙ КУСОК КОДА НАСТЕ ############################### КОНЕЦ №1


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    //vector<Ship> ships;

    int fieldSize;
    cout << "Введите размер поля: ";
    cin >> fieldSize;

    Board board_first_player_ally(fieldSize, false);
    Board board_first_player_enemy(fieldSize, true);

    Board board_second_player_ally(fieldSize, false);
    Board board_second_player_enemy(fieldSize, true);



    // Расчёт количества кораблей
    vector<ShipType> ships_ = calculateShips(fieldSize);

    cout << "Рекомендуемое количество кораблей для поля " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships_)
    {
        cout << ship.name << " (размер " << ship.size << "): " << ship.count << "\n";
    }

    // расположение кораблей для игрока №1
    for (int i = 0; i < ships_.size(); i++)
    {
        for (int j = 0; j < ships_[i].count; j++)
        {
            int x, y;
            bool vertical;

            cout << "Расположите " << ships_[i].name << " (размер " << ships_[i].size << "):\n";
            cout << "Координаты x, y: ";
            cin >> x >> y;
            x--;
            y--;
            if (ships_[i].name != "Submarine")
            {
                cout << "Расположить вертикально? (0/1): ";
                cin >> vertical;
            }
            else
            {
                vertical = true;
            }

            pair<int, int> new_data(x, y);
            Ship new_ship(ships_[i].name, ships_[i].size, new_data, vertical);

            if (board_first_player_ally.can_place_ship(new_ship))
            {
                board_second_player_enemy.placeShip(new_ship);
                board_first_player_ally.placeShip(new_ship);
                //ships.push_back(new_ship); // Добавляем корабль в список
                board_first_player_ally.display(false, board_first_player_ally);
            }
            else
            {
                board_first_player_ally.display(false, board_first_player_ally);
                cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                j--; // Повторим попытку размещения для этого корабля
            }
        }
    }

    // расположение кораблей для игрока №2
    for (int i = 0; i < ships_.size(); i++)
    {
        for (int j = 0; j < ships_[i].count; j++)
        {
            int x, y;
            bool vertical;

            cout << "Расположите " << ships_[i].name << " (размер " << ships_[i].size << "):\n";
            cout << "Координаты x, y: ";
            cin >> x >> y;
            x--;
            y--;
            if (ships_[i].name != "Submarine")
            {
                cout << "Расположить вертикально? (0/1): ";
                cin >> vertical;
            }
            else
            {
                vertical = true;
            }

            pair<int, int> new_data(x, y);
            Ship new_ship(ships_[i].name, ships_[i].size, new_data, vertical);

            if (board_second_player_ally.can_place_ship(new_ship))
            {
                board_first_player_enemy.placeShip(new_ship);
                board_second_player_ally.placeShip(new_ship);
                //ships.push_back(new_ship); // Добавляем корабль в список
                board_second_player_ally.display(false, board_second_player_ally);
            }
            else
            {
                board_second_player_ally.display(false, board_second_player_ally);
                cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                j--; // Повторим попытку размещения для этого корабля
            }
        }
    }

    // Выстрелы
    // Ходит Игрок №2
    while (true)
    {

        int rezult_first_player = 1;
        int rezult_second_player = 1;

        // ход 1-го игрока
        while (rezult_first_player == 1 || rezult_first_player == -1)
        {
            // вывод информации полей боя для 1-го игрока
            cout << "Информация полей боя для 1-го игрока.\n";
            board_first_player_ally.display(true, board_first_player_enemy);

            int x, y;
            cout << "Координаты x, y: ";
            cin >> x >> y;

            rezult_first_player = board_second_player_ally.processShot(x, y);
            board_first_player_enemy.processShot(x, y);

            if (rezult_first_player == 1)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            else if (rezult_first_player == -1)
            {
                cout << "Вы уже стреляли в эту координату. Ходите заново!\n";
            }
            else
            {
                board_first_player_ally.display(true, board_first_player_enemy);
                cout << "Промах. Ход переходи 2-му игроку!\n";
                system("pause");
            }
        }

        // ход 2-го игрока
        while (rezult_second_player == 1 || rezult_second_player == -1)
        {
            // вывод информации полей боя для 2-го игрока
            cout << "Информация полей боя для 2-го игрока.\n";
            board_second_player_ally.display(true, board_second_player_enemy);

            int x, y;
            cout << "Координаты x, y: ";
            cin >> x >> y;

            rezult_second_player = board_first_player_ally.processShot(x, y);
            board_second_player_enemy.processShot(x, y);

            if (rezult_second_player == 1)
            {
                cout << "Удачное попадание. Ходите ещё раз!\n";
            }
            else if (rezult_second_player == -1)
            {
                cout << "Вы уже стреляли в эту координату.\n";
            }
            else
            {
                board_second_player_ally.display(true, board_second_player_enemy);
                cout << "Промах. Ход переходи 1-му игроку!\n";
                system("pause");
            }
        }
    }


    return 1;
}