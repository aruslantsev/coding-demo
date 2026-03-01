#include "terminal.h"

static void disableRawMode() {
    extern struct screenConfig config;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &config.orig_termios) == -1)
        die("tcsetattr");
}


void enableRawMode() {
    extern struct screenConfig config;
    if (tcgetattr(STDIN_FILENO, &config.orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = config.orig_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


void die(const char *s) {
    clearScreen();
    write(STDOUT_FILENO, "\x1b[?25h", 6);
    perror(s);
    exit(EXIT_FAILURE);
}


void clearScreen(void) {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}


void hideCursor(void) {
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

void showCursor(void) {
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}
