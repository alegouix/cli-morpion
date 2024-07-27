#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_board(int board[9], int cursor_x, int cursor_y) {
    printf("\e[2J"); /* clear screen */
    printf("\e[0;0H"); /* place cursor in the top left of the screen */
    printf("\e(0"); /* change the character set to draw nice lines */
    /*
     * | is x
     * - is q
     * l, w, k, t, n, u, m, v and j are for corners and intersections
    */

    printf("\e[0mlqqqwqqqwqqqk\n");
    
    char* style = (char*)malloc(16);
    for (int i=0; i<3; i++) {
        printf("\e[0mx\nx\nx\e[2A");
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
            printf("%s   \e[0mx\e[B\e[4D%s   \e[0mx", style, style, style);
            printf("\e[1A");
        }
        if (i != 2) {
            printf("\e[1B\e[0m\ntqqqnqqqnqqqu\n");
        }
    }
    printf("\e[1B\e[0m\nmqqqvqqqvqqqj\n");

    printf("\e[14;0H\e[0m"); /* put cursor out of the board and reset style */
    printf("\e(B"); /* change character set back */
}

int main() {
    int board[9];
    for (int i=0; i<9; i++) {
        board[i] = i%3;
    }
    
    print_board(board, 2, 0);
    printf("\e(B");

    return 0;
}
