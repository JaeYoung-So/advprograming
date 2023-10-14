#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>

#define DIALOG_DURATION_SEC 4

void gotoxy(int row, int col) {
    COORD pos = { col, row };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printxy(char ch, int row, int col) {
    gotoxy(row, col);
    printf("%c", ch);
}

void drawInitScreen() {
    printf("***********************\n");
    printf("*    0                *\n");
    printf("*                     *\n");
    printf("*  3                  *\n");
    printf("*            2        *\n");
    printf("*                     *\n");
    printf("*     1          4    *\n");
    printf("***********************\n");
}

void showPauseDialog() {
    int countdown = DIALOG_DURATION_SEC;

    while (countdown >= 0) {
        system("cls");
        drawInitScreen();

        int centerRow = 3;
        int centerCol = 7;

        gotoxy(centerRow, centerCol - 2);
        printf("Paused");
        gotoxy(centerRow + 1, centerCol - 2); 
        printf("Time: %d", countdown);

        if (_kbhit()) {
            char key = _getch();
            if (key == ' ') {
                break;
            }
        }

        Sleep(1000);
        countdown--;
    }
}

int main() {
    int gamePaused = 0;  

    while (1) {
        system("cls");
        drawInitScreen();

        if (!gamePaused) {
            printf("Press space to pause...");
        }
        else {
            printf("Press space to pause...");
        }

        while (1) {
            if (_kbhit()) {
                char key = _getch();
                if (key == ' ') {
                    if (!gamePaused) {
                        gamePaused = 1;  
                    }
                    else {
                        break; 
                    }
                }
            }
        }

        if (gamePaused) {
            showPauseDialog();  
        }
    }

    return 0;
}
