﻿/*
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
#include <fstream>
#include <vector>
#include <algorithm>
#include "Board.h"
#include "Ship.h"

void print_pic(string file) { // рисует картинки из файлов
    ifstream fin(file);
    string temp;
    while (!fin.eof()) {
        fin >> temp;
        cout << temp << endl;
    }
    fin.close();
}

template<class T> // Функция ввода пользователем корректных данных
T require(string text, string error, bool (*pred)(T)) {
    T ans;
    while (true) {
        cout << text << endl;
        cout << ">> ";
        cin >> ans;
        if (pred(ans)) return ans;
        cout << error << endl;
    }
}

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

void game_pvp(Board boards[], vector<Ship> ships[], int fieldSize) { // сама игра в морской бой
    
    /* Выстрелы
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
    */

}

void game_pvc(Board boards[], vector<Ship> ships[], int fieldSize) { // сама игра в морской бой
    /* Выстрелы
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
    */
}

void fill_board(Board &board, int fieldSize, vector<Ship> &ships, string user_name) {

    // Расчёт кораблей
    vector<ShipType> ships_ = calculateShips(fieldSize);

    /*
        Четырёхпалубный — Battleship.
        Трёхпалубный — Cruiser.
        Двухпалубный — Destroyer.
        Однопалубный — Submarine.
    */

    // расположение кораблей
    for (int i = 0; i < ships_.size(); i++)
    {
        while (ships_[i].count != 0)
        {
            cout << user_name << " расставляет корабли!" << endl << endl; // кто выставляет

            cout << "Осталось расставить кораблей:" << endl; // сколько осталось
            for (int k = 0; k < ships_.size(); k++)
            {
                cout << ships_[k].name << " (размер " << ships_[k].size << "): " << ships_[k].count << "\n";
            }
            cout << endl;

            board.display(); // дисплей
            cout << endl;

            int x, y;
            bool vertical;

            // Просим у пользователя координаты
            cout << "Расположите " << ships_[i].name << " (размер " << ships_[i].size << "):\n";
            x = require<int>("Координата x: ", "Невозможная координата!",
                [](int a) {return a >= 1 && a <= 15; });
            y = require<int>("Координата y: ", "Невозможная координата!",
                [](int a) {return a >= 1 && a <= 15; });
            if (x > fieldSize || y > fieldSize) continue;

            x--;
            y--;
            if (ships_[i].name != "Submarine")
            {
                vertical = require<bool>("Расположить вертикально? (0/1): ",
                    "Неправильные данные!",
                    [](bool a) {return a == 1 || a == 0; });
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

                ships_[i].count--;
            }
            else
            {
                cout << "Невозможно разместить корабль в данной позиции. Попробуйте снова.\n";
                // Повторим попытку размещения для этого корабля
            }

            system("cls");
        }
    }

}

void make_game() { // создание игры, ввод параметров, расстановка короблей

    int fieldSize;
    string mode;

    fieldSize = require<int>("Введите размер поля: ", "Невозможный размер поля",
        [](int a) {return a >= 6 && a <= 15; }); // просьба ввести размер поля

    mode = require<string>("Выберете режим(pvp/pvc): ", "Такого режима нет",
        [](string a) {return a == "pvp" || a == "pvc"; }); // просьба ввести режим игры

    system("cls");

    vector<Ship> ships[2];
    Board boards[2] = { fieldSize, fieldSize };

    if (mode == "pvp") { // РЕЖИМ ИГРОК ПРОТИВ ИГРОКА
        for (int i = 0; i < 2; i++) {
            fill_board(boards[i], fieldSize, ships[i], "Игрок " + to_string(i + 1));
        }
        game_pvp(boards, ships, fieldSize);
    }
    else { // РЕЖИМ ИГРОК ПРОТИВ КОМПЬЮТЕРА
        fill_board(boards[0], fieldSize, ships[0], "Игрок");
        
        /*
        
        Задача для Насти!!! Написать здесь чтобы компьютер сам расставил корабли на
        поле board[1] и записал их в ships[1].

        */
        
        game_pvc(boards, ships, fieldSize);
    }
}

void load_game() { // загрузка игр

}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    // Варианты команд предусмотренные программой
    string play[] = { "начать игру", "Начать игру", "начать", "Начать", "новая игра", 
        "Новая игра", "играть", "Играть", "начать", "Начать", "play",
        "new game", "Play", "New game", "Start", "start" };
    string continu[] = { "Загрузить игру", "Продолжить игру", "загрузить игру", "продолжить игру",
        "загрузить", "Загрузить", "Продолжить", "продолжить", "continue", "Continue" };
    string exit[] = { "Выйти", "выйти", "выход", "Выход", "Выйти из игры", "выйти из игры",
        "exit", "Exit" };

    string answer; // Ответ пользователя

    while (true) { // Цикл главно меню

        print_pic("images/main_menu.txt"); // Красивая картинка меню
        cout << endl << "          Начать игру" << endl;
        cout << "          Загрузить игру" << endl;
        cout << "          Выйти" << endl;
        cout << endl << ">> "; // выбор между вариантами
        getline(cin, answer);
        system("cls");

        if (find(play, play + 16, answer) != (play + 16)) { // Игрок решил начать игру
            make_game();
        }
        else if (find(continu, continu + 10, answer) != (continu + 10)) { // Игрок решил продолжить игру

        }
        else if (find(exit, exit + 8, answer) != (exit + 8)) { // Игрок решил выйти
            break;
        }
    }

}