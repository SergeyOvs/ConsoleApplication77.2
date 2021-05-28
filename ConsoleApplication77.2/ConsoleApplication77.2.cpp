#include <iostream>
#include<random>
#include<stdlib.h>
#include<chrono>
using namespace std;

enum Cell
{
    EMPTY = '~',
    CROSS = 'X',
    ZERO  = '0'
};
enum Progress
{
    IN_PROGRESS,
    WON_HUMAN,
    WON_AI,
    DRAW
};

struct Field
{
    Cell** ppField = nullptr;
    const size_t size = 3;              // Размер поля
    size_t turn = 0;                   // Кто чем ходит 
    Cell Human = EMPTY;
    Cell Ai = EMPTY;
    Progress progress = IN_PROGRESS;
};

struct Coord
{
    size_t y;
    size_t x;
};

// ****************************************************************************************
void ClearScr()
{
    system("cls");                       // Функция очистки экрана
}

int32_t getRandomNum(int32_t min, int32_t max)              // Рандомайзер
{
    const static auto seed = chrono::system_clock::now().time_since_epoch().count();
    static mt19937_64 generator(seed);
    uniform_int_distribution<int32_t> dis(min, max);
    return dis(generator);
}

// *********************************************************************************************
void InitGame(Field& f)
{
    f.ppField = new Cell * [f.size];
    for (size_t y = 0; y < f.size; y++)
    {
        f.ppField[y] = new Cell[f.size];
    }
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            f.ppField[y][x] = EMPTY;
        }
    }


    if (getRandomNum(0, 1000) > 500)
    {
        f.Human = CROSS;
        f.Ai = ZERO;
        f.turn = 0;
    }
    else
    {
        f.Human = ZERO;
        f.Ai = CROSS;
        f.turn = 1;
    }
}






void DeinitGame(Field& f)
{
    for (size_t y = 0; y < f.size; y++)
    {
        delete[] f.ppField[y];
    }
    delete[] f.ppField;
    f.ppField = nullptr;
}

void DrawField(const Field& f)
{
    cout << endl << "     ";
    for (size_t x = 0; x < f.size; x++)
    {
        cout << "  " << x + 1 << "   ";
    }
    cout << endl;
    for (size_t y = 0; y < f.size; y++)
    {
        cout << " " << y + 1 << "  |  ";
        for (size_t x = 0; x < f.size; x++)
        {
            cout << (char)f.ppField[y][x] << "  |  ";
        }
        cout << endl;
    }
    cout << endl << " Human plays: " << (char)f.Human << endl << " Computer plays:  " << (char)f.Ai << endl;
}

Progress isWon(const Field& f)          // Функция определения победителя
                 
{                  // Выигрыш по строкам
    

    for (size_t y = 0; y < f.size; y++)
    {
        if (f.ppField[y][0] == f.ppField[y][1] && f.ppField[y][0] == f.ppField[y][2])
        {
            if (f.ppField[y][0] == f.Human)
            {
                return WON_HUMAN;
            }
            else if (f.ppField[y][0] == f.Ai)
            {
                return WON_AI;
            }
        }

    }
    for (size_t x = 0; x < f.size; x++)
    {
        if (f.ppField[0][x] == f.ppField[1][x] && f.ppField[0][x] == f.ppField[2][x])
        {
            if (f.ppField[0][x] == f.Human)
            {
                return WON_HUMAN;
            }
            else if (f.ppField[0][x] == f.Ai)
            {
                return WON_AI;
            }
        }

    }

    // Диагональ 1*******************************************************************

    if (f.ppField[0][0] == f.ppField[1][1] && f.ppField[0][0] == f.ppField[2][2])
        if (f.ppField[1][1] == f.Human)
        {
            return WON_HUMAN;
        }
        else if (f.ppField[1][1] == f.Ai)
        {
            return WON_AI;
        }
    // Диагональ 2 *******************************************************************

    if (f.ppField[2][0] == f.ppField[1][1] && f.ppField[2][0] == f.ppField[0][2])
        if (f.ppField[1][1] == f.Human)
        {
            return WON_HUMAN;
        }
        else if (f.ppField[1][1] == f.Ai)
        {
            return WON_AI;
        }
    // Ничья

    bool draw = true;
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x] == EMPTY)
            {
                draw = false;
                break;
            }


        }
        if (!draw)
        {
            break;
        }


    }
    if (draw)
    {
        return DRAW;
    }
    return IN_PROGRESS;
}

Coord GetHumanCoord(const Field& f)
{
    Coord c{ 0 };

    cout << endl;

    do {
        cout << " Enter coord X( 1..3)";
        cin >> c.x;
        cout << " Enter coord Y( 1..3)";
        cin >> c.y;

    } while (c.x > 3 || c.y > 3 || f.ppField[c.y - 1][c.x - 1] != EMPTY);

    c.x--;
    c.y--;
    return c;

}

Coord GetAICoord(Field& f)           // Функция ИИ
{
    Coord c{ 0 };

    // Может ли выиграть AI одним ходом ********************************************
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
            if (f.ppField[y][x]==EMPTY)
           {
               f.ppField[y][x] = f.Ai;
                if (isWon(f)==WON_AI)
                {
                    f.ppField[y][x]=EMPTY;
                    return {y,x};
                }
               f.ppField[y][x]= EMPTY;
           }

        }
    }
    // Помешать выиграть человеку  ************************************************
    for (size_t y = 0; y < f.size; y++)
    {
        for (size_t x = 0; x < f.size; x++)
        {
             if (f.ppField[y][x] == EMPTY)
            {
                f.ppField[y][x] = f.Human;
                if (isWon(f) == WON_HUMAN)
                {
                    f.ppField[y][x]= EMPTY;
                    return { y,x };
                }
                f.ppField[y][x]= EMPTY;
            }

        }
    }
    // Центр

    if (f.ppField[1][1] == EMPTY)
    {
        return{ 1, 1 };
    }

    Coord buf[4];
    size_t  num = 0;

    if (f.ppField[0][0] == EMPTY)
    {
        buf[num] = { 0,0 };
        num++;
    }
    if (f.ppField[2][2] == EMPTY)
    {
        buf[num] = { 2,2 };
        num++;
    }
    if (f.ppField[0][2] == EMPTY)
    {
        buf[num] = { 0,2 };
        num++;
    }
    if (f.ppField[2][0] == EMPTY)
    {
        buf[num] = { 2,0 };
        num++;
    }
    if (num != 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }

    num = 0;

    if (f.ppField[0][1] == EMPTY)
    {
        buf[num] = { 0, 1 };
        num++;
    }
    if (f.ppField[2][1] == EMPTY)
    {
        buf[num] = { 2, 1 };
        num++;
    }
    if (f.ppField[1][0] == EMPTY)
    {
        buf[num] = { 1, 0 };
        num++;
    }
    if (f.ppField[1][2] == EMPTY)
    {
        buf[num] = { 1, 2 };
        num++;
    }
    if (num != 0)
    {
        const size_t index = getRandomNum(0, 1000) % num;
        return buf[index];
    }
    return c;
}


int main()
{
    Field f;
    InitGame(f);

    ClearScr();
    DrawField(f);

    do {
        if (f.turn % 2 == 0)
        {
            Coord c = GetHumanCoord(f);
            f.ppField[c.y][c.x] = f.Human;
        }
        else
        {
            Coord c = GetAICoord(f);
            f.ppField[c.y][c.x] = f.Ai;
        }
        f.turn++;
        ClearScr();
        DrawField(f);

        f.progress = isWon(f);

    } while (f.progress == IN_PROGRESS);



    if (f.progress == WON_HUMAN)
    {
        cout << " Human is WON! Congrats!" << endl;
    }
    else if (f.progress == WON_AI)
    {
        cout << " AI is WON! Congrats!" << endl;
    }
    else if (f.progress == DRAW)
    {
        cout << "DRAW =(" << endl;
    }

    DeinitGame(f);
    return 0;
}