#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"
#include "Player.h"
#include "Game.h"

void SetCursor(int x, int y) //функция для того чтобы устанавливать позицию курсора в консоли по оси Х и Y
{
    COORD myCoords = { x,y }; //инициализация координат
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), myCoords); //Способ перемещения курсора на нужные координаты
}

void setColorWithBackground(int textColor, int backgroundColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = (backgroundColor << 4) | textColor;
    SetConsoleTextAttribute(hConsole, color);
}

//enum Color { Black = 0, DarkBlue = 1, Green = 2, Blue = 3, Red = 4, Purple = 5, Yellow = 6, White = 7, Grey = 8, LightBlue = 9, LightGreen = 10 };
enum Napravlenie { Up = 72, Left = 75, Right = 77, Down = 80, Enter = 13, Tab = 9 };

void setMenuOption(const COORD& coord, int rem_y, int current_y, const string& text)
{
    SetCursor(coord.X, current_y);
    setColorWithBackground((rem_y == current_y) ? 0 : 7, (rem_y == current_y) ? 7 : 0); // rem_y == current_y - если выполнено, то подсвеченный текст, т.е. (0,7)
    cout << text << "\n"; // вывод самого текста
}

int navigateMenu(const vector<string>& options, int start_y,int skip)
{
    COORD menu_coord{ 5, start_y };
    int rem_y = start_y + skip;
    int key;

    do
    {
        system("cls");
        for (int i = 0; i < options.size(); i++) 
        {
            setMenuOption(menu_coord, rem_y, start_y + i, options[i]);  // вывод меню построчно, т.е. построчно вывод vector
        }
        setColorWithBackground(7, 0); // перекрасим в стандартный цвет

        key = _getch(); // ожидаем нажатие клавиши
        if (key == Up && rem_y > start_y + skip) rem_y--; // вверх
        if (key == Down && rem_y < start_y + options.size() - 1) rem_y++; // вниз
    } while (key != Enter);

    return rem_y - start_y;
}

bool yesno(const string& name)
{
    vector<string> options =
    {
        name + ", включить автоматическое расставление?",
        "Да.",
        "Нет, не доверяю компьютеру."
    };
    return (navigateMenu(options, 4, 1) == 1); // Если да, то true. Если нет, то false
}

void game_pusk(bool person1, bool person2)
{
    system("cls");

    string name_1, name_2;
    if (person1)
    {
        cout << "Введите имя первого игрока: ";
        getline(cin, name_1);
    }
    else
    {
        name_1 = "Автобот №1";
    }
    if (person2)
    {
        cout << "Введите имя первого игрока: ";
        getline(cin, name_2);
    }
    else
    {
        name_2 = "Автобот №2";
    }

    Game my_game(name_1, name_2);
    int fieldSize = my_game.game_enter();

    Board board1(fieldSize, false), board2(fieldSize, false);
    int remember_ship_sells;
    vector<ShipType> ships = my_game.calculate_ships(fieldSize, remember_ship_sells);

    Player player1(name_1, board1, board2, ships, remember_ship_sells);
    Player player2(name_2, board2, board1, ships, remember_ship_sells);

    bool automat_1 = true, automat_2 = true;
    if (person1) automat_1 = yesno(name_1);
    if (person2) automat_2 = yesno(name_2);

    if (automat_1) player1.AutoBoardShipPlacement(name_1, player2);
    else player1.BoardShipPlacement(name_1, player2);

    if (automat_2) player2.AutoBoardShipPlacement(name_2, player1);
    else player2.BoardShipPlacement(name_2, player1);

    my_game.cycle_play(player1, player2, person1, person2);
    my_game.game_end(player1, player2);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    vector<string> mainMenu = 
    {
        "Запустить игру.",
        "Об игре.",
        "Выход из игры."
    };

    vector<string> gameModes =
    {
        "Выберете режим игры.",
        "Игрок против игрока.",
        "Игрок против компьютера.",
        "Компьютер против компьютера.",
        "Назад."
    };

    while (true)
    {
        int choice = navigateMenu(mainMenu, 4,0);

        if (choice == 0) // Запустить игру
        { 
            while (true)
            {
                int mode = navigateMenu(gameModes, 4, 1) - 1;
                if (mode == 0) game_pusk(true, true);
                else if (mode == 1) game_pusk(true, false);
                else if (mode == 2) game_pusk(false, false);
                else break; // Назад
            }
        }
        else if (choice == 1) // Об игре
        { 
            system("cls");
            cout << "Об игре.\nАвторы: ...\nДата создания: ...\n";
            system("pause");
        }
        else if (choice == 2)  // Выход из игры
        {
            break;
        }
    }

    return 0;
}


/*

bool yesno(string name)
{
    int key;
    COORD menu_coord_1{ 5,4 };
    int rem_y = menu_coord_1.Y + 1;
    int menu = 0;
    do
    {
        menu_coord_1.Y = 4;
        system("cls");
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << name << ", включить автоматическое расставление?\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Да.\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Нет, не доверяю компьютеру.\n";

        setColorWithBackground(7, 0);
        SetCursor(menu_coord_1.X, rem_y);
        key = _getch(); //функция возвращает номер нажатой клавиши
        switch (key)
        {
        case Up:
            if (rem_y > 5)
            {
                rem_y--;
            }
            break;
        case Down:
            if (rem_y < 6)
            {
                rem_y++;
            }
            break;
        case Enter:
            menu = rem_y - 5;
            switch (menu)
            {
            case 0:
                return true;
                break;
            case 1:
                return false;
                break;
            }
            break;
        }
    } while (key != Enter);
}

void game_pusk(bool person1, bool person2) // false - компьютер, true - человек
{
    system("cls");

    string name_1, name_2;
    if (person1)
    {
        cout << "Введите имя первого игрока: ";
        getline(cin, name_1);
    }
    else
    {
        name_1 = "Автобот №1";
    }

    if (person2)
    {
        cout << "Введите имя первого игрока: ";
        getline(cin, name_2);
    }
    else
    {
        name_2 = "Автобот №2";
    }

    Game my_game(name_1, name_2);

    int fieldSize = my_game.game_enter();

    Board board_first_player_ally(fieldSize, false);
    Board board_second_player_ally(fieldSize, false);

    int remember_ship_sells = 0; // количество ячеек, которые занимают корабли
    vector<ShipType> ships = my_game.calculate_ships(fieldSize, remember_ship_sells); // Расчёт количества кораблей

    Player player1(name_1, board_first_player_ally, board_second_player_ally, ships, remember_ship_sells);
    Player player2(name_2, board_second_player_ally, board_first_player_ally, ships, remember_ship_sells);

    bool automat_1 = true, automat_2 = true;
    if (person1) automat_1 = yesno(name_1);
    if (person2) automat_2 = yesno(name_2);
    
    if (automat_1) player1.AutoBoardShipPlacement(name_1, player2);
    else player1.BoardShipPlacement(name_1, player2);

    if (automat_2) player2.AutoBoardShipPlacement(name_2, player1);
    else player2.BoardShipPlacement(name_2, player1);

    my_game.cycle_play(player1, player2, person1, person2);
    my_game.game_end(player1, player2);
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int key;
    COORD menu_coord_1{ 5,4 };
    int rem_y_1 = menu_coord_1.Y;
    int menu = 0;
    do
    {
        menu_coord_1.Y = 4;
        system("cls");
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Запустить игру.\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Об игре.\n";
        menu_coord_1.Y++;
        SetCursor(menu_coord_1.X, menu_coord_1.Y);
        if (rem_y_1 == menu_coord_1.Y)
        {
            setColorWithBackground(0, 7); // цветной
        }
        else
        {
            setColorWithBackground(7, 0);
        }
        cout << "Выход из игры.\n";

        setColorWithBackground(7, 0);
        SetCursor(menu_coord_1.X, rem_y_1);
        key = _getch(); //функция возвращает номер нажатой клавиши
        switch (key)
        {
        case Up:
            if (rem_y_1 > 4)
            {
                rem_y_1--;
            }
            break;
        case Down:
            if (rem_y_1 < 6)
            {
                rem_y_1++;
            }
            break;
        case Enter:
            menu = rem_y_1 - 4;
            switch (menu)
            {
            case 0:
            {
                // "Запустить игру.\n";
                COORD menu_coord_2{ 5,4 };
                int rem_y_2 = menu_coord_2.Y + 1;
                do
                {
                    menu_coord_2.Y = 4;
                    system("cls");
                    SetCursor(menu_coord_2.X, menu_coord_2.Y);
                    if (rem_y_2 == menu_coord_2.Y)
                    {
                        setColorWithBackground(0, 7); // цветной
                    }
                    else
                    {
                        setColorWithBackground(7, 0);
                    }
                    cout << "Выберете режим игры.\n";
                    menu_coord_2.Y++;
                    SetCursor(menu_coord_2.X, menu_coord_2.Y);
                    if (rem_y_2 == menu_coord_2.Y)
                    {
                        setColorWithBackground(0, 7); // цветной
                    }
                    else
                    {
                        setColorWithBackground(7, 0);
                    }
                    cout << "Игрок против игрока.\n";
                    menu_coord_2.Y++;
                    SetCursor(menu_coord_2.X, menu_coord_2.Y);
                    if (rem_y_2 == menu_coord_2.Y)
                    {
                        setColorWithBackground(0, 7); // цветной
                    }
                    else
                    {
                        setColorWithBackground(7, 0);
                    }
                    cout << "Игрок против компьютера.\n";
                    menu_coord_2.Y++;
                    SetCursor(menu_coord_2.X, menu_coord_2.Y);
                    if (rem_y_2 == menu_coord_2.Y)
                    {
                        setColorWithBackground(0, 7); // цветной
                    }
                    else
                    {
                        setColorWithBackground(7, 0);
                    }
                    cout << "Компьютер против компьютера.\n";
                    menu_coord_2.Y++;
                    SetCursor(menu_coord_2.X, menu_coord_2.Y);
                    if (rem_y_2 == menu_coord_2.Y)
                    {
                        setColorWithBackground(0, 7); // цветной
                    }
                    else
                    {
                        setColorWithBackground(7, 0);
                    }
                    cout << "Назад.\n";

                    setColorWithBackground(7, 0);
                    SetCursor(menu_coord_2.X, rem_y_2);
                    key = _getch(); //функция возвращает номер нажатой клавиши
                    switch (key)
                    {
                    case Up:
                        if (rem_y_2 > 5)
                        {
                            rem_y_2--;
                        }
                        break;
                    case Down:
                        if (rem_y_2 < 8)
                        {
                            rem_y_2++;
                        }
                        break;
                    case Enter:
                    {
                        menu = rem_y_2 - 5;
                        switch (menu)
                        {
                        case 0: // "Игрок против игрока.\n";
                            game_pusk(true, true);
                            break;
                        case 1: // "Игрок против компьютера.\n";
                            game_pusk(true, false);
                            break;
                        case 2: // "Компьютер против компьютера.\n";
                            game_pusk(false, false);
                            break;
                        case 3: // "Назад.\n";
                            key = -1;
                            break;
                        case 4:
                            break;
                        }

                        break;
                    }
                    }

                } while (key != -1);
            }
            break;
            case 1:
            {
                int i = 4;
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Об игре.\n";
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Авторы: ...\n";
                SetCursor(menu_coord_1.X + 25, i++);
                cout << "Дата создания ...\n";
                SetCursor(menu_coord_1.X + 25, i++);
                system("pause");
                break;
            }
            case 2:
                // "Выход из игры.\n";
                key = 0;
                break;
            }
            break;
        }
    } while (key != 0);

    return 0;
}*/