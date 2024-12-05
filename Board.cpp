
#include "Board.h"

// ����������� ������������
Board::Board(int size, bool hide_ships) : size(size), grid(size, vector<char>(size, ' ')), hide_ships(hide_ships) {}

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
bool Board::processShot(int x, int y)
{
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
                    placeDeadField(ship);
                }
                break;
            }
        }
        return true;
    }
    else if (grid[x][y] == ' ')
    {
        grid[x][y] = 'O'; // ������
    }
    return false;
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
void Board::display() const
{

    // ������ ������� ������� �������
    cout << "    ";  // ������ ����� �������� ��������
    for (int i = 0; i < size; ++i)
    {
        cout << setw(2) << i+1 << "  ";  // ������ ������� ��������
    }
    cout << "\n";

    // ������ �������������� ����� ����� ���������� ��������
    cout << "   +";
    for (int i = 0; i < size; ++i)
    {
        cout << "---+";  // ������ �������������� ����� ��� ������ ������
    }
    cout << "\n";

    // ������ ����� � ���������
    for (int i = 0; i < size; ++i)
    {
            cout << setw(2) << i+1 << " |";  // ������ ������ ������ � ������������ �������

        // ������ ����������� ����� � ������������� ���������
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

        // ������ ����������� ������������ ������� ������
        cout << "\n";

        // ������ �������������� ����� ����� ������ ������
        cout << "   +";
        for (int j = 0; j < size; ++j)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }
        cout << "\n";
    }
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




// �������� �����, ������� ������ �������� 2 ����� �������� ������: ��� ������� � ������ ���� ��� ���������

// ����� ����� 4 �����: ����� 1 (��� �������), ����� 1 (������� ���������, ������� �� �������)
//                      ����� 2 (��� �������), ����� 2 (������� ���������, ������� �� �������)