
#include "Board.h"

void Board::setColorWithBackground(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = (backgroundColor << 4) | textColor; // Фон сдвигается влево на 4 бита
    // Это делается, потому что в Windows цвета фона занимают старшие 4 бита (4–7) байта цвета, а цвета текста занимают младшие 4 бита (0–3).
    // Оператор | (побитовое ИЛИ) объединяет биты backgroundColor и textColor. Это позволяет "смешать" два значения в одно целое число, не потеряв информации.
    SetConsoleTextAttribute(hConsole, color);
}

PlayerResultOfShot::PlayerResultOfShot(int rezult_of_shot, int size_of_ship, bool ship_dead, bool damage_more_one_square)
    :rezult_of_shot(rezult_of_shot), size_of_ship(size_of_ship), ship_dead(ship_dead), damage_more_one_square(damage_more_one_square){};

PlayerResultOfShot::PlayerResultOfShot()
    :rezult_of_shot(0), size_of_ship(0), ship_dead(false), damage_more_one_square(false) {};

// Определение конструктора
Board::Board(int size, bool hide_ships)
    : size(size), grid(size, vector<char>(size, ' ')), hide_ships(hide_ships) {}

Board::Board()
    :size(0), hide_ships(true), grid(0, vector<char>(0, '.')) {}

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
PlayerResultOfShot Board::processShot(int x, int y)
{
    PlayerResultOfShot rezult(0,0,false, false);
    rezult.ship_dead = 0;

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
                if (ship.hits >= 2)
                {
                    rezult.damage_more_one_square = true;
                }
                else
                {
                    rezult.damage_more_one_square = false;
                }

                if (ship.isSunk())
                {
                    rezult.size_of_ship = ship.getSize();
                    rezult.ship_dead = true;
                    placeDeadField(ship);
                }
                else
                {
                    rezult.ship_dead = false;
                }
                break;
            }
        }
        rezult.rezult_of_shot = 1;
        return rezult;
    }
    else if (grid[x][y] == ' ')
    {
        grid[x][y] = 'O'; // Промах
        rezult.rezult_of_shot = 0;
        return rezult;
    }
    else if (grid[x][y] == 'O' || grid[x][y] == 'X')
    {
        rezult.rezult_of_shot = -1;
        return rezult;
    }
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
void Board::display(bool second_battlefield, const Board& second_board)
{
    SetCursor(0, 1);
    // Печать верхней границы таблицы
    cout << "    ";  // Отступ перед номерами столбцов
    for (int i = 0; i < size; ++i)
    {
        cout << setw(2) << i + 1 << "  ";  // Печать номеров столбцов
    }
    if (second_battlefield)
    {
        cout << "  ";
        // Печать верхней границы таблицы
        cout << "    ";  // Отступ перед номерами столбцов
        for (int i = 0; i < size; ++i)
        {
            cout << setw(2) << i + 1 << "  ";  // Печать номеров столбцов
        }
    }
    cout << "\n";


    // Печать строк с границами
    for (int i = 0; i < size; ++i)
    {
        // Печать горизонтальной линии после заголовков столбцов
        cout << "   +";
        for (int i = 0; i < size; ++i)
        {
            cout << "---+";  // Печать горизонтальной линии для каждой ячейки
        }
        if (second_battlefield)
        {
            cout << "  ";
            // Печать горизонтальной линии после заголовков столбцов
            cout << "   +";
            for (int i = 0; i < size; ++i)
            {
                cout << "---+";  // Печать горизонтальной линии для каждой ячейки
            }
        }
        cout << "\n";

        // Печать строк с границами

        cout << setw(2) << i + 1 << " |";  // Печать номера строки и вертикальной границы

        // Печать содержимого ячеек с вертикальными границами
        for (int j = 0; j < size; ++j)
        {
            if (grid[j][i] == 'S')
            {
                setColorWithBackground(4, 4);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else if (grid[j][i] == 'O')
            {
                setColorWithBackground(8, 8);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else if (grid[j][i] == 'X')
            {
                setColorWithBackground(5, 5);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else
            {
                cout << " " << grid[j][i] << " |";
            }

        }
        if (second_battlefield)
        {
            cout << "  ";
            cout << setw(2) << i + 1 << " |";  // Печать номера строки и вертикальной границы

            // Печать содержимого ячеек с вертикальными границами
            for (int j = 0; j < size; ++j)
            {
                if (second_board.grid[j][i] != 'S')
                {
                    if (second_board.grid[j][i] == 'O')
                    {
                        setColorWithBackground(8, 8);
                        cout << " " << second_board.grid[j][i] << " ";
                        setColorWithBackground(7, 0);
                        cout << "|";
                    }
                    else if (second_board.grid[j][i] == 'X')
                    {
                        setColorWithBackground(5, 5);
                        cout << " " << second_board.grid[j][i] << " ";
                        setColorWithBackground(7, 0);
                        cout << "|";
                    }
                    else
                    {
                        cout << " " << second_board.grid[j][i] << " |";
                    }
                }
                else
                {
                    cout << "  " << " |";
                }

            }
        }
        cout << "\n";
    }

    // Печать горизонтальной линии после каждой строки
    cout << "   +";
    for (int j = 0; j < size; ++j)
    {
        cout << "---+";  // Печать горизонтальной линии для каждой ячейки
    }
    if (second_battlefield)
    {
        cout << "  ";
        cout << "   +";
        for (int j = 0; j < size; ++j)
        {
            cout << "---+";  // Печать горизонтальной линии для каждой ячейки
        }
    }
    cout << "\n";
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

void Board::display_ship_on_board(const Ship& ship, int x, int y)
{
    int rem_x = x, rem_y = y;
    x--;
    SetCursor(x, y);
    for (int i = 0; i < ship.size; i++)
    {
        if (ship.is_vertical == false) // рисуем корабль горизонтально
        {
            setColorWithBackground(4, 4);
            cout << "   ";
            setColorWithBackground(7, 0);
            cout << "|";
        }
        else                           // рисуем корабль вертикально
        {
            setColorWithBackground(4, 4);
            cout << "   ";
            y = y + 2;
            SetCursor(x, y);
            setColorWithBackground(7, 0);
        }
    }
    SetCursor(rem_x, rem_y);
}

void Board::SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //Способ перемещения курсора на нужные координаты
}