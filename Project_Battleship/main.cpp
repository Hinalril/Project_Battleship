/*
Рекомендации по количеству кораблей в зависимости от размера поля:
Поле 6x6 (36 клеток)
1 четырёхпалубный (Battleship).
1 трёхпалубный (Cruiser).
2 двухпалубных (Destroyer).
3 однопалубных (Submarine).
Итого: 7 кораблей, занимают 20 клеток (≈55% свободного пространства).

Поле 8x8 (64 клетки)
1 четырёхпалубный (Battleship).
2 трёхпалубных (Cruiser).
3 двухпалубных (Destroyer).
4 однопалубных (Submarine).
Итого: 10 кораблей, занимают 25 клеток (≈60% свободного пространства).

Поле 10x10 (100 клеток) — классическое поле
1 четырёхпалубный (Battleship).
2 трёхпалубных (Cruiser).
3 двухпалубных (Destroyer).
4 однопалубных (Submarine).
Итого: 10 кораблей, занимают 25 клеток (25% клеток — оптимально).

Поле 12x12 (144 клетки)
2 четырёхпалубных (Battleship).
3 трёхпалубных (Cruiser).
4 двухпалубных (Destroyer).
6 однопалубных (Submarine).
Итого: 15 кораблей, занимают 40 клеток (≈28% клеток).

Поле 15x15 (225 клеток)
2 четырёхпалубных (Battleship).
4 трёхпалубных (Cruiser).
5 двухпалубных (Destroyer).
7 однопалубных (Submarine).
Итого: 18 кораблей, занимают 55 клеток (≈24% клеток).
*/


#include <iostream>
#include <Windows.h>
#include "Board.h"
#include "Ship.h"

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


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<Ship> ships;

    int fieldSize;
    cout << "Введите размер поля: ";
    cin >> fieldSize;

    Board board(fieldSize);

    // Расчёт кораблей
    vector<ShipType> ships_ = calculateShips(fieldSize);

    cout << "Рекомендуемое количество кораблей для поля " << fieldSize << "x" << fieldSize << ":\n";
    for (const auto& ship : ships_)
    {
        cout << ship.name << " (размер " << ship.size << "): " << ship.count << "\n";
    }

    /*
        Четырёхпалубный — Battleship.
        Трёхпалубный — Cruiser.
        Двухпалубный — Destroyer.
        Однопалубный — Submarine.
    */

    // расположение кораблей
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

            if (board.can_place_ship(new_ship))
            {
                board.placeShip(new_ship);
                ships.push_back(new_ship); // Добавляем корабль в список
            }
            else
            {
                cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                j--; // Повторим попытку размещения для этого корабля
            }

            board.display();
        }
    }

    // Выстрелы
    board.processShot(2, 2); // Попадание
    board.display();
    board.processShot(5, 6); // Промах
    board.display();
    board.processShot(0, 0); // Попадание (1/2)
    board.display();
    board.processShot(0, 1); // Попадание (2/2)
    board.display();
    board.processShot(2, 3); // Попадание (2/3)
    board.display();
    board.processShot(2, 4); // Попадание (3/3)

    // Отображение после выстрелов
    board.display();

    return 1;
}