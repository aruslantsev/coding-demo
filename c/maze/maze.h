#include "solve.c"
#include "show.c"
#include "generate.c"


enum directions {UP = 0, RIGHT, DOWN, LEFT};

const int movements[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

const char cursors[5] = "^>V<X";

enum statuses {SUCCESS = 0, PROGRESS};
