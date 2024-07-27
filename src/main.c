#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board(int board[9], int cursor_x, int cursor_y) {
    printf("\e[2J");
    printf("\e[0;0H");
    printf("\e[0m-------------\n");
    
    char* style = (char*)malloc(16);
    for (int i=0; i<3; i++) {
        printf("\e[0m|\n|\n|\e[2A");
        for (int j=0; j<3; j++) {
            memset(style, 0, 16);
            switch (board[i*3 + j]) {
                case 0:
                    sprintf(style, "\e[0m\e[97m");
                    break;
                case 1:
                    sprintf(style, "\e[44m\e[94m");
                    break;
                case 2:
                    sprintf(style, "\e[42m\e[92m");
                    break;
            }
            if (i == cursor_y && j == cursor_x) {
                strcat(style, "\e[7m");
            }
            printf("%s   \e[0m|\e[B\e[4D%s   \e[0m|\e[B\e[4D%s   \e[0m|", style, style, style);
            printf("\e[2A");
        }
        printf("\e[2B\e[0m\n-------------\n");
    }

    printf("\e[14;0H\e[0m");
}

int main() {
    int board[9];
    for (int i=0; i<9; i++) {
        board[i] = i%3;
    }
    
    print_board(board, 2, 0);

    return 0;
}
