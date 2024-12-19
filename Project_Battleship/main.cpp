#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Board.h"
#include "Ship.h"
#include "Player.h"
#include "Game.h"

void playMenuSwitchSound()
{
    Beep(750, 80); // Короткий высокий звук
}

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

int navigateMenu(const vector<string>& options, int start_y, int skip)
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
        if (key == Up && rem_y > start_y + skip) { playMenuSwitchSound(); rem_y--; }// вверх
        if (key == Down && rem_y < start_y + options.size() - 1) { playMenuSwitchSound(); rem_y++; }// вниз
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
    if (person1) { cout << "Введите имя первого игрока: "; getline(cin, name_1); }
    else { name_1 = "Автобот №1"; }
    if (person2) { cout << "Введите имя второго игрока: "; getline(cin, name_2); }
    else { name_2 = "Автобот №2"; }

    Game my_game(name_1, name_2);
    int fieldSize = my_game.game_enter();

    Board board1(fieldSize), board2(fieldSize);
    int remember_ship_sells;
    vector<ShipType> ships = my_game.calculate_ships(fieldSize, remember_ship_sells);

    Player player1(name_1, board1, board2, ships, remember_ship_sells, person1, "Player 1");
    Player player2(name_2, board2, board1, ships, remember_ship_sells, person2, "Player 2");

    if (person1 && person2) { player1.coords.X = 7; }

    bool automat_1 = true, automat_2 = true;
    if (person1) automat_1 = yesno(name_1);
    if (person2) automat_2 = yesno(name_2);

    if (automat_1) player1.AutoBoardShipPlacement(name_1, player2);
    else player1.BoardShipPlacement(name_1, player2);

    Sleep(1000); // нужно, чтобы srand(time(NULL)) не выдавало те же случайные числа
    if (automat_2) player2.AutoBoardShipPlacement(name_2, player1);
    else player2.BoardShipPlacement(name_2, player1);

    my_game.cycle_play(player1, player2, person1, person2);
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
        int choice = navigateMenu(mainMenu, 4, 0);

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
            cout << "Об игре. Приложение - игра морской бой. Реализована в рамках лабораторной работы №7 как проектная работа.\n";
            cout << "В игре представлены разные режимы игры:\n1. игрок против игрока\n2. игрок против компьютера\n3. компьютер против компьютера\n\n";
            cout << "Авторы: группа ИМ23-02Б\n";
            cout << "1. Ефремов Илья Валентинович\n";
            cout << "2. Медведев Вадим Дмитриевич\n";
            cout << "3. Литвинова Анастасия Романовна\n\n";
            cout << "Дата создания: 28.11.24 - 16.12.24\n";
            system("pause");
        }
        else if (choice == 2)  // Выход из игры
        {
            break;
        }
    }

    return 0;
}