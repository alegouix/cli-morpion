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

/* used to save terminal settings for reset_input_mode */
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

    /* save the terminal settings and register reset_input_mode to be called at process termination */
    tcgetattr(STDIN_FILENO, &original_termios);
    atexit(reset_input_mode);

    tcgetattr(STDIN_FILENO, &tattr);
    /* put terminal in noncanonical mode and disable echoing the input */
    tattr.c_lflag &= ~(ICANON|ECHO);
    /* set parameters to make read behave well */
    tattr.c_cc[VMIN] = 0;
    tattr.c_cc[VTIME] = 0;
    /* apply changes */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
}

int get_input() {
    char buf[3];
    while (1) {
        /* wait for input */
        while (read(STDIN_FILENO, buf, 3) == 0);
        if (buf[0] == 'q') { /* quit if q is pressed */
            return -1; /* -1 to quit */
        } else if (buf[0] == '\n') { /* return if enter is pressed */
            return 5;
        } else if (buf[0] == '\e') { /* check for escape sequence */
            if (buf[1] == '[') {
                switch (buf[2]) { /* arrow keys are \e[A throug \e[D */
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
        /* reset buffer after each read */
        memset(buf, 0, 3);
    }

    return 0;
}
