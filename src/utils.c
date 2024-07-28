#include <termios.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "utils.h"

int min(int a, int b) {
    if (a>b) {
        return b;
    }
    return a;
}

int max (int a, int b) {
    if (a>b) {
        return a;
    }
    return b;
}

struct termios original_termios;

void reset_input_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void set_noncanonical() {
    struct termios tattr;

    if (!isatty(STDIN_FILENO)) {
        printf("error : stdin is not a tty\n");
        exit(-1);
    }

    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(reset_input_mode);

    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO);
    tattr.c_cc[VMIN] = 0;
    tattr.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

int get_input() {
    char buf[3];
    while (1) {
        while (read(STDIN_FILENO, buf, 3) == 0);
        if (buf[0] == 'q') {
            return -1; /* -1 to quit */
        } else if (buf[0] == '\n') {
            return 5;
        } else if (buf[0] == '\e') {
            if (buf[1] == '[') {
                switch (buf[2]) {
                    case 'A':
                        return 1;
                        break;
                    case 'B':
                        return 2;
                        break;
                    case 'C':
                        return 3;
                        break;
                    case 'D':
                        return 4;
                        break;
                    default:
                        break;
                }
            }
        }
        memset(buf, 0, 3);
    }

    return 0;
}
