
#include "Board.h"

void Board::setColorWithBackground(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = (backgroundColor << 4) | textColor; // ��� ���������� ����� �� 4 ����
    // ��� ��������, ������ ��� � Windows ����� ���� �������� ������� 4 ���� (4�7) ����� �����, � ����� ������ �������� ������� 4 ���� (0�3).
    // �������� | (��������� ���) ���������� ���� backgroundColor � textColor. ��� ��������� "�������" ��� �������� � ���� ����� �����, �� ������� ����������.
    SetConsoleTextAttribute(hConsole, color);
}

PlayerResultOfShot::PlayerResultOfShot(int rezult_of_shot, int size_of_ship, bool ship_dead, bool damaged_more_than_one_square)
    :rezult_of_shot(rezult_of_shot), size_of_ship(size_of_ship), ship_dead(ship_dead), damaged_more_than_one_square(damaged_more_than_one_square){};

PlayerResultOfShot::PlayerResultOfShot()
    :rezult_of_shot(0), size_of_ship(0), ship_dead(false), damaged_more_than_one_square(false) {};

// ����������� ������������
Board::Board(int size)
    : size(size), grid(size, vector<char>(size, ' ')) {}

Board::Board()
    :size(0), grid(0, vector<char>(0, '.')) {}

void Board::clearPartOfConsole(int startLine, int endLine)
{
    cout << "\033[" << startLine << ";0H";     // ����������� ������ � ��������� ������

    // �������� ������ ������ � ��������� ���������
    for (int i = startLine; i <= endLine; i++)
    {
        cout << "\033[K"; // �������� ������� ������
        if (i != endLine)
        {
            cout << "\033[E"; // ������� �� ��������� ������
        }
    }
    cout << "\033[" << startLine << ";0H";     // ������� ������ � ������ ��������� ������
}

// ���������� �������
void Board::placeShip(const Ship& ship)
{
    for (const auto& coord : ship.getCoordinates())
    {
        grid[coord.X][coord.Y] = 'S';
    }
    ships.push_back(ship);
}

// ��������� ��������
PlayerResultOfShot Board::processShot(int x, int y)
{
    PlayerResultOfShot rezult(0,0,false, false);
    rezult.ship_dead = 0;

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
                if (ship.hits >= 2)
                {
                    rezult.damaged_more_than_one_square = true;
                }
                else
                {
                    rezult.damaged_more_than_one_square = false;
                }

                if (ship.isSunk())
                {
                    rezult.size_of_ship = ship.size;
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
    const vector<COORD> directions =
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        { 0, -1},          { 0, 1},
        { 1, -1}, { 1, 0}, { 1, 1}
    };

    for (const auto& coord : ship.getCoordinates())
    {
        int x = coord.X;
        int y = coord.Y;

        // ��������� ���� �������
        for (const auto& dir : directions)
        {
            int x_sosed = x + dir.X;
            int y_sosed = y + dir.Y;

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
void Board::display(bool second_battlefield, const Board& second_board)
{
    SetCursor(0, 1);
    // ������ ������� ������� �������
    cout << "    ";  // ������ ����� �������� ��������
    for (int i = 0; i < size; i++)
    {
        cout << setw(2) << i + 1 << "  ";  // ������ ������� ��������
    }
    if (second_battlefield)
    {
        cout << "  ";
        // ������ ������� ������� �������
        cout << "    ";  // ������ ����� �������� ��������
        for (int i = 0; i < size; i++)
        {
            cout << setw(2) << i + 1 << "  ";  // ������ ������� ��������
        }
    }
    cout << "\n";


    // ������ ����� � ���������
    for (int i = 0; i < size; i++)
    {
        // ������ �������������� ����� ����� ���������� ��������
        cout << "   +";
        for (int i = 0; i < size; i++)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }
        if (second_battlefield)
        {
            cout << "  ";
            // ������ �������������� ����� ����� ���������� ��������
            cout << "   +";
            for (int i = 0; i < size; i++)
            {
                cout << "---+";  // ������ �������������� ����� ��� ������ ������
            }
        }
        cout << "\n";

        // ������ ����� � ���������

        cout << setw(2) << i + 1 << " |";  // ������ ������ ������ � ������������ �������

        // ������ ����������� ����� � ������������� ���������
        for (int j = 0; j < size; j++)
        {
            if (grid[j][i] == 'S')
            {
                setColorWithBackground(4, 4);
                cout << "   ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else if (grid[j][i] == 'O')
            {
                setColorWithBackground(8, 8);
                cout << "   ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else if (grid[j][i] == 'X')
            {
                setColorWithBackground(5, 5);
                cout << "   ";
                setColorWithBackground(7, 0);
                cout << "|";
            }
            else
            {
                cout << "   |";
            }

        }
        if (second_battlefield)
        {
            cout << "  ";
            cout << setw(2) << i + 1 << " |";  // ������ ������ ������ � ������������ �������

            // ������ ����������� ����� � ������������� ���������
            for (int j = 0; j < size; j++)
            {
                if (second_board.grid[j][i] != 'S')
                {
                    if (second_board.grid[j][i] == 'O')
                    {
                        setColorWithBackground(8, 8);
                        cout << "   ";
                        setColorWithBackground(7, 0);
                        cout << "|";
                    }
                    else if (second_board.grid[j][i] == 'X')
                    {
                        setColorWithBackground(5, 5);
                        cout << "   ";
                        setColorWithBackground(7, 0);
                        cout << "|";
                    }
                    else
                    {
                        cout << "   |";
                    }
                }
                else
                {
                    cout << "   |";
                }

            }
        }
        cout << "\n";
    }

    // ������ �������������� ����� ����� ������ ������
    cout << "   +";
    for (int j = 0; j < size; j++)
    {
        cout << "---+";  // ������ �������������� ����� ��� ������ ������
    }
    if (second_battlefield)
    {
        cout << "  ";
        cout << "   +";
        for (int j = 0; j < size; j++)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }
    }
    cout << "\n";
}


void Board::display(const Board& second_board, string titul_player)
{
    clearPartOfConsole(1, 2 + second_board.size * 2);
    int push;
    if (titul_player == "Player 2")
    {
        push = second_board.size * 4 + 4;
    }
    else
    {
        push = 0;
    }
    SetCursor(push, 1);

    cout << "  ";
    // ������ ������� ������� �������
    cout << "    ";  // ������ ����� �������� ��������
    for (int i = 0; i < size; i++)
    {
        cout << setw(2) << i + 1 << "  ";  // ������ ������� ��������
    }
    cout << "\n";


    // ������ ����� � ���������
    for (int i = 0; i < size; i++)
    {
        SetCursor(push, 2+ i * 2);
        cout << "  ";
        // ������ �������������� ����� ����� ���������� ��������
        cout << "   +";
        for (int i = 0; i < size; i++)
        {
            cout << "---+";  // ������ �������������� ����� ��� ������ ������
        }

        cout << "\n";

        // ������ ����� � ���������


        SetCursor(push, 2 + (i * 2) + 1);
        cout << "  ";
        cout << setw(2) << i + 1 << " |";  // ������ ������ ������ � ������������ �������

        // ������ ����������� ����� � ������������� ���������
        for (int j = 0; j < size; j++)
        {
            if (second_board.grid[j][i] != 'S')
            {
                if (second_board.grid[j][i] == 'O')
                {
                    setColorWithBackground(8, 8);
                    cout << "   ";
                    setColorWithBackground(7, 0);
                    cout << "|";
                }
                else if (second_board.grid[j][i] == 'X')
                {
                    setColorWithBackground(5, 5);
                    cout << "   ";
                    setColorWithBackground(7, 0);
                    cout << "|";
                }
                else
                {
                    cout << "   |";
                }
            }
            else
            {
                cout << "   |";
            }

        }

        cout << "\n";
    }

    SetCursor(push, 2 + second_board.size*2);
    cout << "  ";
    cout << "   +";
    for (int j = 0; j < size; ++j)
    {
        cout << "---+";  // ������ �������������� ����� ��� ������ ������
    }

    cout << "\n";
}

// ������� ��� ��������, ���������� �� ������� �� ����
bool Board::can_place_ship(const Ship& ship)
{
    int x = ship.coordinates[0].X;
    int y = ship.coordinates[0].Y;

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
            for (int i = 0; i < ship.size; i++)
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
            setColorWithBackground(4, 4);
            cout << "   ";
            setColorWithBackground(7, 0);
            cout << "|";
        }
        else                           // ������ ������� �����������
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

void Board::SetCursor(int x, int y) //������� ��� ���� ����� ������������� ������� ������� � ������� �� ��� � � Y
{
    COORD myCoords = { x,y }; //������������� ���������
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //������ ����������� ������� �� ������ ����������
}