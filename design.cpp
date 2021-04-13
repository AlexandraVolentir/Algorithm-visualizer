#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include "algo.h"

Object Block[30];
int i,l,x, y, x1, y1, temp_x, temp_y, tl = 0, tr = 0, bl = 0, br = 0;

bool stop = false;

int MID_X(Object TEMP) {
    return (TEMP.bottom_left + TEMP.top_left) / 2;
}

int MID_Y(Object TEMP) {
    return (TEMP.bottom_right + TEMP.top_right) / 2;
}

void Read_start_stop(int i, int color) {
    //  Gasesc "centrul de greutate" al figurii mele
    int midX = MID_X(Block[i]);
    int midY = MID_Y(Block[i]);
    // Gasesc radiusul x si y pentru a putea desena ovalul
    int L_x = (Block[i].bottom_left - midX) + 11;
    int L_y = (midY - Block[i].top_right) + 6;
    // Desenez figura in sine (in forma de oval), caracteristica pentru START
    fillellipse(midX, midY, L_x, L_y);
    // Centrez cuvantul "START" in bloc si il afisez
    char * a;
    if (Block[i].name == 's') a = "START";
    else a = "STOP";
    int width = textwidth(a);
    int height = textheight(a);
    outtextxy(midX - width / 2, midY - height / 2, a);
}

void Read_input_output(int i, int color) {
    // Pentru o citire mai usoara a punctelor poligonului, atribui valori variabilor x si y
    int x1 = Block[i].top_left;
    int y1 = Block[i].top_right;
    int x2 = Block[i].bottom_left;
    int y2 = Block[i].bottom_right;
    int xx, xy, points[10];
    if (Block[i].name == 'i') {
        // atribui valori punctelor bazei mari a trapezului, care e mai mare ca baza mica
        xx = x2 - 10;
        xy = x1 + 10;

        int p[10] = { x1 - 11, y1, x2 + 11, y1, xx, y2, xy, y2, x1 - 11, y1};
        memcpy(points, p, sizeof(points));
    } else if (Block[i].name == 'o') {
        xx = x2 + 10;
        xy = x1 - 10;
        int p[10] = { x1 + 13, y1, x2 - 13, y1, xx, y2, xy, y2, x1 + 13, y1};
        memcpy(points, p, sizeof(points));
    }

    fillpoly(5, points);
    // Gasesc centul de greutate al figurii
    int midX = MID_X(Block[i]);
    int midY = MID_Y(Block[i]);
    // Atribui pointerului char a valoarea variabilei de input si afisez centrat
    char * a = & Block[i].IOA_name;
    int width = textwidth(a);
    int height = textheight(a);
    outtextxy(midX - width / 2, midY - height / 2, a);

}

void Read_decizie(int i, int color) {

    int x1 = Block[i].top_left;
    int y1 = Block[i].top_right;
    int x2 = Block[i].bottom_left;
    int y2 = Block[i].bottom_right;
    // Gasesc centul de greutate al figurii
    int midX = MID_X(Block[i]);
    int midY = MID_Y(Block[i]);
    int points[8] = {
        midX, y1, x2, y2, x1, y2, midX, y1};
    fillpoly(4, points);
    char * a = & Block[i].decision[0];
    int width = textwidth(a);
    int height = textheight(a);
    outtextxy(midX - width / 2, midY - height / 2 + 7, a);

};

void Read_atribuire(int i, int color) {

    // desenam dreptunghiul
    bar(Block[i].top_left, Block[i].top_right - 1, Block[i].bottom_left, Block[i].bottom_right+1);
    int midX = MID_X(Block[i]);
    int midY = MID_Y(Block[i]);
    char a[100] = "";
    char * IOA_name;
    IOA_name = & Block[i].IOA_name;
    strncat(a, IOA_name, sizeof(char));
    strcat(a, "=");
    strcat(a, Block[i].A_value);
    int width = textwidth(a);
    int height = textheight(a);
    outtextxy(midX - width / 2, midY - height / 2, a);
};

void Read_Blocks() {
    for (int i = 0; i < l; i++) {
        if (Block[i].name == 's' || Block[i].name == 't') Read_start_stop(i, 11);
        else if (Block[i].name == 'i' || Block[i].name == 'o') Read_input_output(i, 11);
        else if (Block[i].name == 'd') Read_decizie(i, 11);
        else if (Block[i].name == 'c') Read_atribuire(i, 11);
    }
}
/*int x, y, x1, y1, temp_x, temp_y, tl = 0, tr = 0, bl = 0, br = 0;*/
POINT pt_cursorPos;
void Move_The_Blocks()
{
      bool flag = false;
      POINT cursorPosition;
      GetCursorPos(&cursorPosition);
      ScreenToClient(GetForegroundWindow(),&cursorPosition);
      x = cursorPosition.x;
      y = cursorPosition.y;
      for (int j = 0; j < l; j++)
      {
                if (x >= (Block[j].top_left) && x <= (Block[j].bottom_left) && y >= (Block[j].top_right) && y <= (Block[j].bottom_right)) {
                    i = j;
                    flag = true;
                    break;
                }
      }
      if (GetAsyncKeyState(VK_LBUTTON))
      {
          GetCursorPos(&pt_cursorPos);
          ScreenToClient(GetForegroundWindow(),&pt_cursorPos);
          x1 = cursorPosition.x;
          y1 = cursorPosition.y;
          temp_x = x - x1;
          temp_y = y - y1;
          tl = Block[i].top_left;
          tr = Block[i].top_right;
          bl = Block[i].bottom_left;
          br = Block[i].bottom_right;

          while (GetAsyncKeyState(VK_LBUTTON))
            {
                if (x1 >= (Block[i].top_left) && x1 <= (Block[i].bottom_left) && y1 >= (Block[i].top_right) && y1 <= (Block[i].bottom_right)){
                    DrawLinksBetweenThem(BLACK);
                    setfillstyle(SOLID_FILL, BLACK);
                    delay(20);
                    switch (Block[i].name) {
                        case 's':
                        case 't':
                            fillellipse(MID_X(Block[i]), MID_Y(Block[i]), 41, 16);
                            break;
                        case 'i':
                            bar(Block[i].top_left-11, Block[i].top_right-5, Block[i].bottom_left+11, Block[i].bottom_right+9);
                        case 'o':
                            bar(Block[i].top_left-10, Block[i].top_right-5, Block[i].bottom_left+10, Block[i].bottom_right+5);
                        case 'c':
                            bar(Block[i].top_left, Block[i].top_right-5, Block[i].bottom_left, Block[i].bottom_right+5);
                            break;
                        case 'd':
                            bar(Block[i].top_left-9, Block[i].top_right-15, Block[i].bottom_left+9, Block[i].bottom_right+15);
                            break;
                    }
                    setfillstyle(SOLID_FILL, LIGHTCYAN);
                    GetCursorPos(&pt_cursorPos);
                    ScreenToClient(GetForegroundWindow(),&pt_cursorPos);
                    x1 = pt_cursorPos.x;
                    y1 = pt_cursorPos.y;
                    temp_x = x - x1;
                    temp_y = y - y1;
                    Block[i].top_left = tl - temp_x;
                    Block[i].top_right = tr - temp_y;
                    Block[i].bottom_left = bl - temp_x;
                    Block[i].bottom_right = br - temp_y;
                    setcolor(BLACK);
                    int color = WHITE;
                    switch (Block[i].name) {
                    case 's':
                    case 't':
                        Read_start_stop(i, color);
                        break;
                    case 'i':
                        Read_input_output(i, color);
                        break;
                    case 'o':
                        Read_input_output(i, color);
                        break;
                    case 'c':
                        Read_atribuire(i, color);
                        break;
                    case 'd':
                        Read_decizie(i, color);
                        break;
                    }
                    AssignSmallCircles(); //Florin
                    DrawLinksBetweenThem(YELLOW); //Florin
                    delay(20);
                    setcolor(BLACK);
            } else break;}
             Read_Blocks();
      }
}

void Window() {
    int x = getmaxx();
    int y = getmaxy();
    setcolor(WHITE);
    line(x * 2 / 3 + 50, 0, x * 2 / 3 + 50, y);
    line(0, y * 5 / 7, x * 2 / 3 + 50, y * 5 / 7);
}

void InitWindow() { // Functia principala - care afiseaza fereastra si blocurile
    initwindow(900, 600, "Interschem - vizualizator de scheme logice");
    cleardevice();

    Window();

    settextstyle(8, HORIZ_DIR, 1);
    setfillstyle(SOLID_FILL, LIGHTCYAN);
    setbkcolor(LIGHTCYAN);
    setcolor(BLACK);
    Read_Blocks();
    AssignSmallCircles(); //Florin
    DrawLinksBetweenThem(YELLOW); //Florin

    setbkcolor(LIGHTCYAN);
    setcolor(BLACK);
    Generate(0);
    do
    {
        Move_The_Blocks();
    } while(1);

    getch();

    closegraph();
}




