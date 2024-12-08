
#include "Board.h"



enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10};


void setColorWithBackground(Color textColor, Color backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = (backgroundColor << 4) | textColor; // ��� ���������� ����� �� 4 ����
    // ��� ��������, ������ ��� � Windows ����� ���� �������� ������� 4 ���� (4�7) ����� �����, � ����� ������ �������� ������� 4 ���� (0�3).
    // �������� | (��������� ���) ���������� ���� backgroundColor � textColor. ��� ��������� "�������" ��� �������� � ���� ����� �����, �� ������� ����������.
    SetConsoleTextAttribute(hConsole, color);
}

// ����������� ������������
Board::Board(int size, bool hide_ships)
    : size(size), grid(size, vector<char>(size, ' ')), hide_ships(hide_ships) {}

Board::Board()
    :size(0), hide_ships(true), grid(0, vector<char>(0, '.')) {}

// ���������� �������
void Board::placeShip(const Ship& ship)
{
    for (const auto& coord : ship.getCoordinates())
    {
        grid[coord.first][coord.second] = 'S';
    }
    ships.push_back(ship);
}

// ��������� ��������
PlayerResultOfShot Board::processShot(int x, int y)
{
    PlayerResultOfShot rezult;
    //rezult.rezult_ship.first(int) � .second(bool) // int - ��������� ���������, bool - ����� �� �������
                                                    // 1 - ���������
                                                    // 0 - ������
                                                    // -1 - ������ ��� ������, ������ ����
    rezult.rezult_ship.second = false;
    x--;
    y--;
    bool found = false;
    if (grid[x][y] == 'S')
    {
        grid[x][y] = 'X'; // ���������
        for (auto& ship : ships)
        {
            found = ship.takeHit(x, y);
            if (found)
            {
                if (ship.isSunk())
                {
                    rezult.rezult_ship.first = ship.getSize();
                    rezult.rezult_ship.second = true;
                    placeDeadField(ship);
                }
                break;
            }
        }
        rezult.rezult_of_shot = 1;
        return rezult;
    }
    else if (grid[x][y] == ' ')
    {
        grid[x][y] = 'O'; // ������
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
    // ������� ��� ������ ���� �������
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

        // ��������� ���� �������
        for (const auto& dir : directions)
        {
            int x_sosed = x + dir.first;
            int y_sosed = y + dir.second;

            // ��������, ��� �� ������� �� ������� ����
            if (x_sosed >= 0 && x_sosed < size && y_sosed >= 0 && y_sosed < size)
            {
                // �������� "������� ����", ���� ��� �� ����� �������
                if (grid[x_sosed][y_sosed] == ' ')
                {
                    grid[x_sosed][y_sosed] = 'O';
                }
            }
        }
    }
}

// ����������� ����
void Board::display(bool second_battlefield, const Board& second_board) const
{
    // ������ ������� ������� �������
    cout << "    ";  // ������ ����� �������� ��������
    for (int i = 0; i < size; ++i)
    {
        cout << setw(2) << i + 1 << "  ";  // ������ ������� ��������
    }
    if (second_battlefield)
    {
        cout << "  ";
        // ������ ������� ������� �������
        cout << "    ";  // ������ ����� �������� ��������
        for (int i = 0; i < size; ++i)
        {
            cout << setw(2) << i + 1 << "  ";  // ������ ������� ��������
        }
    }
    cout << "\n";


    // ������ ����� � ���������
    for (int i = 0; i < size; ++i)
    {
        // ������ �������������� ����� ����� ���������� ��������
        cout << "   +";
        for (int i = 0; i < size; ++i)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }
        if (second_battlefield)
        {
            cout << "  ";
            // ������ �������������� ����� ����� ���������� ��������
            cout << "   +";
            for (int i = 0; i < size; ++i)
            {
                cout << "---+";  // ������ �������������� ����� ��� ������ ������
            }
        }
        cout << "\n";

        // ������ ����� � ���������

        cout << setw(2) << i + 1 << " |";  // ������ ������ ������ � ������������ �������

        // ������ ����������� ����� � ������������� ���������
        for (int j = 0; j < size; ++j)
        {
            if (grid[j][i] == 'S')
            {
                setColorWithBackground(Red, Red);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(White, Black);
                cout << "|";
            }
            else if (grid[j][i] == 'O')
            {
                setColorWithBackground(Grey, Grey);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(White, Black);
                cout << "|";
            }
            else if (grid[j][i] == 'X')
            {
                setColorWithBackground(Purple, Purple);
                cout << " " << grid[j][i] << " ";
                setColorWithBackground(White, Black);
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
            cout << setw(2) << i + 1 << " |";  // ������ ������ ������ � ������������ �������

            // ������ ����������� ����� � ������������� ���������
            for (int j = 0; j < size; ++j)
            {
                if (second_board.grid[j][i] != 'S')
                {
                    if (second_board.grid[j][i] == 'O')
                    {
                        setColorWithBackground(Grey, Grey);
                        cout << " " << second_board.grid[j][i] << " ";
                        setColorWithBackground(White, Black);
                        cout << "|";
                    }
                    else if (second_board.grid[j][i] == 'X')
                    {
                        setColorWithBackground(Purple, Purple);
                        cout << " " << second_board.grid[j][i] << " ";
                        setColorWithBackground(White, Black);
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

    // ������ �������������� ����� ����� ������ ������
    cout << "   +";
    for (int j = 0; j < size; ++j)
    {
        cout << "---+";  // ������ �������������� ����� ��� ������ ������
    }
    if (second_battlefield)
    {
        cout << "  ";
        cout << "   +";
        for (int j = 0; j < size; ++j)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }
    }
    cout << "\n";
}

// ������� ��� ��������, ���������� �� ������� �� ����
bool Board::can_place_ship(const Ship& ship)
{
    int x = ship.coordinates[0].first;
    int y = ship.coordinates[0].second;

    if (ship.is_vertical)
    {
        // �������� �� ���������
        if (y + ship.size - 1 >= size)
        {
            return false; // ���� ������� ������� �� ������� ����
        }
        else
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x][y + i] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ����� �� ���������
        if (x - 1 >= 0)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x - 1][y + i] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ������ �� ���������
        if (x + 1 < size)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + 1][y + i] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ����� ��� �����������
        if (x - 1 >= 0 && y - 1 >= 0)
        {
            if (grid[x - 1][y - 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (y - 1 >= 0)
        {
            if (grid[x][y - 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x + 1 < size && y - 1 >= 0)
        {
            if (grid[x + 1][y - 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }

        // �������� ���� ��� �����������
        if (x - 1 >= 0 && y + ship.size < size)
        {
            if (grid[x - 1][y + ship.size] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (y + ship.size + 1 < size)
        {
            if (grid[x][y + ship.size] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x + 1 < size && y + ship.size < size)
        {
            if (grid[x + 1][y + ship.size] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
    }
    else
    {
        // �������� �� �����������
        if (x + ship.size - 1 >= size)
        {
            return false; // ���� ������� ������� �� ������� ����
        }
        else
        {
            for (int i = 0; i < ship.size; ++i)
            {
                if (grid[x + i][y] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ������ �� �����������
        if (y - 1 >= 0)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + i][y - 1] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ������ �� �����������
        if (y + 1 < size)
        {
            for (int i = 0; i < ship.size; i++)
            {
                if (grid[x + i][y + 1] != ' ')
                {
                    return false; // ���� ������ ������
                }
            }
        }

        // �������� ����� ��� �������������
        if (x - 1 >= 0 && y - 1 >= 0)
        {
            if (grid[x - 1][y - 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x - 1 >= 0)
        {
            if (grid[x - 1][y] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x - 1 >= 0 && y + 1 < size)
        {
            if (grid[x - 1][y + 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }

        // �������� ������ ��� �������������
        if (x + ship.size < size && y - 1 >= 0)
        {
            if (grid[x + ship.size][y - 1] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x + ship.size < size)
        {
            if (grid[x + ship.size][y] != ' ')
            {
                return false; // ���� ������ ������
            }
        }
        if (x + ship.size < size && y + 1 < size)
        {
            if (grid[x + ship.size][y + 1] != ' ')
            {
                return false; // ���� ������ ������
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
        if (ship.is_vertical == false) // ������ ������� �������������
        {
            setColorWithBackground(Red, Red);
            cout << "   ";
            setColorWithBackground(White, Black);
            cout << "|";
        }
        else                           // ������ ������� �����������
        {
            setColorWithBackground(Red, Red);
            cout << "   ";
            y = y + 2;
            SetCursor(x, y);
            setColorWithBackground(White, Black);
        }
    }
    SetCursor(rem_x, rem_y);
}

void Board::SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
{
    COORD myCoords = { x,y }; //������������� ���������
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //������ ����������� ������� �� ������ ����������
}