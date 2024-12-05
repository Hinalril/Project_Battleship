
#include "Board.h"

// Определение конструктора
Board::Board(int size, bool hide_ships) : size(size), grid(size, vector<char>(size, ' ')), hide_ships(hide_ships) {}

// Размещение корабля
void Board::placeShip(const Ship& ship)
{
    for (const auto& coord : ship.getCoordinates())
    {
        grid[coord.first][coord.second] = 'S';
    }
    ships.push_back(ship);
}


// Обработка выстрела
bool Board::processShot(int x, int y)
{
    x--;
    y--;
    bool found = false;
    if (grid[x][y] == 'S')
    {
        grid[x][y] = 'X'; // Попадание
        for (auto& ship : ships)
        {
            found = ship.takeHit(x, y);
            if (found)
            {
                if (ship.isSunk())
                {
                    placeDeadField(ship);
                }
                break;
            }
        }
        return true;
    }
    else if (grid[x][y] == ' ')
    {
        grid[x][y] = 'O'; // Промах
    }
    return false;
}

void Board::placeDeadField(const Ship& ship)
{
    // Векторы для обхода всех соседей
    const vector<pair<int, int>> directions =
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (const auto& coord : ship.getCoordinates())
    {
        int x = coord.first;
        int y = coord.second;

        // Проверяем всех соседей
        for (const auto& dir : directions)
        {
            int x_sosed = x + dir.first;
            int y_sosed = y + dir.second;

            // Убедимся, что не выходим за пределы поля
            if (x_sosed >= 0 && x_sosed < size && y_sosed >= 0 && y_sosed < size)
            {
                // Помечаем "мертвую зону", если там не часть корабля
                if (grid[x_sosed][y_sosed] == ' ')
                {
                    grid[x_sosed][y_sosed] = 'O';
                }
            }
        }
    }
}

// Отображение поля
void Board::display() const
{

    // Печать верхней границы таблицы
    cout << "    ";  // Отступ перед номерами столбцов
    for (int i = 0; i < size; ++i)
    {
        cout << setw(2) << i+1 << "  ";  // Печать номеров столбцов
    }
    cout << "\n";

    // Печать горизонтальной линии после заголовков столбцов
    cout << "   +";
    for (int i = 0; i < size; ++i)
    {
        cout << "---+";  // Печать горизонтальной линии для каждой ячейки
    }
    cout << "\n";

    // Печать строк с границами
    for (int i = 0; i < size; ++i)
    {
            cout << setw(2) << i+1 << " |";  // Печать номера строки и вертикальной границы

        // Печать содержимого ячеек с вертикальными границами
        for (int j = 0; j < size; ++j)
        {
            if (!hide_ships)
            {
                cout << " " << grid[j][i] << " |";
            }
            else
            {
                if (grid[j][i] != 'S')
                {
                    cout << " " << grid[j][i] << " |";
                }
                else
                {
                    cout << "  " << " |";
                }
            }
        }

        // Печать завершающей вертикальной границы строки
        cout << "\n";

        // Печать горизонтальной линии после каждой строки
        cout << "   +";
        for (int j = 0; j < size; ++j)
        {
            cout << "---+";  // Печать горизонтальной линии для каждой ячейки
        }
        cout << "\n";
    }
}

// Функция для проверки, помещается ли корабль на поле
bool Board::can_place_ship(const Ship& ship)
{
    int x = ship.coordinates[0].first;
    int y = ship.coordinates[0].second;

    if (ship.is_vertical)
    {
        // Проверка по вертикали
        if (y + ship.size - 1 >= size)
        {
            return false; // Если корабль выходит за пределы поля
        }
        else
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x][y + i] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка слева от вертикали
        if (x - 1 >= 0)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x - 1][y + i] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка справа от вертикали
        if (x + 1 < size)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + 1][y + i] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка верха над вертикалями
        if (x - 1 >= 0 && y - 1 >= 0)
        {
            if (grid[x - 1][y - 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (y - 1 >= 0)
        {
            if (grid[x][y - 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x + 1 < size && y - 1 >= 0)
        {
            if (grid[x + 1][y - 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }

        // проверка низа над вертикалями
        if (x - 1 >= 0 && y + ship.size < size)
        {
            if (grid[x - 1][y + ship.size] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (y + ship.size + 1 < size)
        {
            if (grid[x][y + ship.size] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x + 1 < size && y + ship.size < size)
        {
            if (grid[x + 1][y + ship.size] != ' ')
            {
                return false; // Если клетка занята
            }
        }
    }
    else
    {
        // Проверка по горизонтали
        if (x + ship.size - 1 >= size)
        {
            return false; // Если корабль выходит за пределы поля
        }
        else
        {
            for (int i = 0; i < ship.size; ++i)
            {
                if (grid[x + i][y] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка сверху от горизонтали
        if (y - 1 >= 0)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + i][y - 1] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка сверху от горизонтали
        if (y + 1 < size)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + i][y + 1] != ' ')
                {
                    return false; // Если клетка занята
                }
            }
        }

        // проверка слева над горизонталями
        if (x - 1 >= 0 && y - 1 >= 0)
        {
            if (grid[x - 1][y - 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x - 1 >= 0)
        {
            if (grid[x - 1][y] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x - 1 >= 0 && y + 1 < size)
        {
            if (grid[x - 1][y + 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }

        // проверка справа над горизонталями
        if (x + ship.size < size && y - 1 >= 0)
        {
            if (grid[x + ship.size][y - 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x + ship.size < size)
        {
            if (grid[x + ship.size][y] != ' ')
            {
                return false; // Если клетка занята
            }
        }
        if (x + ship.size < size && y + 1 < size)
        {
            if (grid[x + ship.size][y + 1] != ' ')
            {
                return false; // Если клетка занята
            }
        }
    }

    return true;
}




// добавить вывод, который будето выводить 2 доски текущего игрока: мои корабли и статус поля боя соперника

// всего будет 4 доски: игрок 1 (его корабли), игрок 1 (корабли соперника, которые он поразил)
//                      игрок 2 (его корабли), игрок 2 (корабли соперника, которые он поразил)