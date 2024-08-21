import random

MINE = "⚑"
MARK = "⚐"
NOT_REVEALED = "*"
CELL_WIDTH = 3


class Cell:
    is_mine: bool = False
    is_revealed: bool = False
    num_mines: int = 0
    is_marked: bool = False


class Operations:
    REVEAL = "reveal"
    MARK = "mark"
    UNMARK = "unmark"
    OPEN_ALL = "open_all"


class Minesweeper:
    def __init__(self, rows, cols, mines):
        self.rows = rows
        self.cols = cols
        self.mines = mines
        if self.mines >= self.rows * self.cols:
            raise ValueError(f"Too many mines: {self.mines} > {self.rows * self.cols} - 1")
        self.first_move = True
        self.field = [[Cell() for _ in range(self.cols)] for _ in range(self.rows)]
        self.not_revealed = self.rows * self.cols - self.mines

    def show_field(self, reveal_mines=False, reveal_all=False):
        print(" " * (CELL_WIDTH + 1) + "".join(f"{n:{CELL_WIDTH}}" for n in range(self.cols)))
        print(" " * (CELL_WIDTH + 1) + f"{'-':>{CELL_WIDTH}}" * self.cols)
        for row in range(self.rows):
            s = f"{row:{CELL_WIDTH}}:"
            for col in range(self.cols):
                cell = self.field[row][col]
                if cell.is_mine and (reveal_mines or reveal_all):
                    s += f"{MINE:>{CELL_WIDTH}}"
                elif cell.is_revealed or reveal_all:
                    s += f"{cell.num_mines:>{CELL_WIDTH}}"
                elif cell.is_marked and not (reveal_mines or reveal_all):
                    s += f"{MARK:>{CELL_WIDTH}}"
                else:
                    s += f"{NOT_REVEALED:>{CELL_WIDTH}}"
            print(s)

    def populate_field(self, start_row, start_col):
        possible_mines = [(row, col) for row in range(self.rows) for col in range(self.cols)]
        possible_mines.remove((start_row, start_col))
        mines = random.sample(possible_mines, k=self.mines)
        for row, col in mines:
            self.field[row][col].is_mine = True
            for i in range(row - 1, row + 2):
                for j in range(col - 1, col + 2):
                    if 0 <= i < self.rows and 0 <= j < self.cols:
                        if not self.field[i][j].is_mine:
                            self.field[i][j].num_mines += 1

    def reveal(self, row, col):
        if self.field[row][col].is_revealed or self.field[row][col].is_marked:
            return 0

        self.field[row][col].is_revealed = True
        revealed = 1
        if self.field[row][col].num_mines == 0:
            for i in range(row - 1, row + 2):
                for j in range(col - 1, col + 2):
                    if 0 <= i < self.rows and 0 <= j < self.cols:
                        revealed += self.reveal(i, j)
        return revealed

    def move(self, row, col, operation=Operations.REVEAL):
        if self.first_move:
            self.populate_field(row, col)
            self.first_move = False
        if operation == Operations.REVEAL:
            if not self.field[row][col].is_revealed and not self.field[row][col].is_marked:
                if self.field[row][col].is_mine:
                    return -1
                self.not_revealed -= self.reveal(row, col)
        elif operation == Operations.MARK:
            if not self.field[row][col].is_revealed and not self.field[row][col].is_marked:
                self.field[row][col].is_marked = True
        elif operation == Operations.UNMARK:
            if not self.field[row][col].is_revealed and self.field[row][col].is_marked:
                self.field[row][col].is_marked = False
        elif operation == Operations.OPEN_ALL:
            for i in range(row - 1, row + 2):
                for j in range(col - 1, col + 2):
                    if 0 <= i < self.rows and 0 <= j < self.cols:
                        self.not_revealed -= self.reveal(i, j)
        else:
            print("Invalid operation")
        if self.not_revealed == 0:
            return 1
        return 0


def read_input(n_rows, n_cols):
    while True:
        s = input("Enter ROW and COL separated by SPACE: ")
        if len(s.split(" ")) == 2:
            row, col = s.split(" ")
            cmd = Operations.REVEAL
        elif len(s.split(" ")) == 3:
            cmd, row, col = s.split(" ")
            if cmd == "M":
                cmd = Operations.MARK
            elif cmd == "U":
                cmd = Operations.UNMARK
            elif cmd == "A":
                cmd = Operations.OPEN_ALL
            else:
                print("Invalid operation.")
                continue
        else:
            print("Invalid input.")
            continue
        try:
            row = int(row)
            col = int(col)
            if not (0 <= row < n_rows and 0 <= col < n_cols):
                print("Invalid input.")
                continue
        except Exception:
            print("Invalid input.")
            continue
        return cmd, row, col


def play():
    s = input("Enter number of rows, cols and mines, separated by space: ")
    rows, cols, mines = s.split(" ")
    rows = int(rows)
    cols = int(cols)
    mines = int(mines)
    minesweeper = Minesweeper(rows, cols, mines)
    minesweeper.show_field()
    print(
        "To open a cell enter ROW and COL separated by SPACE. "
        "To open all neighbors enter A ROW COL"
        "To mark cell as mine enter M ROW COL. "
        "To unmark cell enter U ROW COL."
    )
    while minesweeper.not_revealed > 0:
        cmd, row, col = read_input(minesweeper.rows, minesweeper.cols)
        res = minesweeper.move(row=row, col=col, operation=cmd)
        if res == -1:
            print()
            minesweeper.show_field(reveal_mines=True)
            print("LOSER!")
            return
        elif res == 1:
            print()
            minesweeper.show_field(reveal_all=True)
            print("WINNER!")
            return
        else:
            minesweeper.show_field()


if __name__ == "__main__":
    play()