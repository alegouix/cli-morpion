#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "utils.h"

/*
 * prints the board to the screen
 * cursor_x and cursor_y are used to highlight the cursors position and can be outside the board to highlight nothing
 * if turn is >= 0, it represents the turn number
 *  if it is -1, it means the game ended in a tie
 *  -2 means player 1 won
 *  -3 means player 2 won
*/
void print_board(int board[9], int cursor_x, int cursor_y, int turn) {
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
            printf("%s   \e[0mx\e[B\e[4D%s   \e[0mx", style, style);
            printf("\e[1A");
        }
        if (i != 2) {
            printf("\e[1B\e[0m\ntqqqnqqqnqqqu\n");
        }
    }
    printf("\e[1B\e[0m\nmqqqvqqqvqqqj\n");

    printf("\e[11;0H\e[0m");
    printf("\e(B"); /* change character set back */
    if (turn == -1) {
        printf("Égalité\n");
    } else if (turn == -2) {
        printf("Victoire du joueur \e[94m\e[1mbleu\e[0m !\n");
    } else if (turn == -3) {
        printf("Victoire du joueur \e[92m\e[1mvert\e[0m !\n");
    } else if (turn%2 == 0) {
        printf("C'est au tour du joueur \e[94m\e[1mbleu\n");
    } else {
        printf("C'est au tour du joueur \e[92m\e[1mvert\n");
    }

    printf("\e[12;0H\e[0m"); /* put cursor out of the board and reset style */
}

/*
 * checks if a player won
 * returns 0 if no player won, the winners number (1 or 2) otherwise
*/
int win(int board[9]) {
    for (int i=0; i<3; i++) {
        /* check rows */
        if (board[i*3] != 0 && board[i*3] == board[i*3+1] && board[i*3] == board[i*3+2]) {
            return board[i*3];
        }

        /* check columns */
        if (board[i] != 0 && board[i] == board[i+3] && board[i] == board[i+6]) {
            return board[i];
        }
    }
    /* check first diagonal */
    if (board[0] != 0 && board[0] == board[4] && board[0] == board[8]) {
        return board[0];
    }

    /* check second diagonal */
    if (board[2] != 0 && board[2] == board[4] && board[2] == board[6]) {
        return board[2];
    }

    return 0;
}

void gameloop() {
    /* initialize board */
    int board[9];
    for (int i=0; i<9; i++) {
        board[i] = 0;
    }

    int cursor_x = 1, cursor_y = 1;
    int input;
    for (int turn=0; turn<9; turn++) {
        int ok = 1;
        do {
            print_board(board, cursor_x, cursor_y, turn);
            input=get_input();
    
            if (input == -1) {
                printf("bye\n");
                return;
            }
            switch (input) {
                case 1:
                    cursor_y = max(cursor_y-1, 0);
                    break;
                case 2:
                    cursor_y = min(2, cursor_y + 1);
                    break;
                case 3:
                    cursor_x = min(2, cursor_x + 1);
                    break;
                case 4:
                    cursor_x = max(cursor_x-1, 0);
                    break;
                case 5:
                    ok = board[cursor_x + cursor_y*3];
                    break;
            }
        } while (ok != 0);

        board[cursor_x + cursor_y*3] = turn%2 + 1;

        int v = win(board);
        if (v == 1) {
            print_board(board, -1, -1, -2);
            return;
        } else if (v == 2) {
            print_board(board, -1, -1, -3);
            return;
        }
    }

    print_board(board, -1, -1, -1);
}

int main() {
    set_noncanonical();

    /* enable alternative screen buffer and hide the cursor */
    printf("\e[?1049h\e[?25l");

    /* check terminal size */
    struct winsize w;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &w);

    if (w.ws_row < 13 || w.ws_col < 28) {
        printf("terminal too small. Min size : 28*13\n");
        return 1;
    }

    gameloop();
    sleep(1);

    /* disable alternative screen buffer and show the cursor */
    printf("\e[?1049l\e[?25h");

    return 0;
}
