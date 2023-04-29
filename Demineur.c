//
// Created by Piquionne Andy on 30/04/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 9
#define COLS 9
#define MINES 10

void initialize_board(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = ' ';
        }
    }
}

void place_mines(char board[ROWS][COLS], int mines) {
    int placed_mines = 0;
    while (placed_mines < mines) {
        int row = rand() % ROWS;
        int col = rand() % COLS;

        if (board[row][col] != '*') {
            board[row][col] = '*';
            placed_mines++;
        }
    }
}

int count_adjacent_mines(char board[ROWS][COLS], int row, int col) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && board[newRow][newCol] == '*') {
                count++;
            }
        }
    }
    return count;
}

void display_board(char board[ROWS][COLS], bool reveal_single_mine) {
    int mines_revealed = 0;

    printf("  ");
    for (int i = 0; i < COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("%d ", i);
        for (int j = 0; j < COLS; j++) {
            if (reveal_single_mine && board[i][j] == '*' && mines_revealed < 1) {
                printf("* ");
                mines_revealed++;
            } else if (board[i][j] == '*') {
                printf("  ");
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}





bool in_bounds(int row, int col) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS;
}

void reveal(char board[ROWS][COLS], char revealed[ROWS][COLS], int row, int col) {
    if (!in_bounds(row, col) || revealed[row][col]) {
        return;
    }

    revealed[row][col] = true;

    if (board[row][col] == '*') {
        return;
    }

    int adjacent_mines = count_adjacent_mines(board, row, col);
    if (adjacent_mines > 0) {
        board[row][col] = '0' + adjacent_mines;
    } else {
        board[row][col] = ' ';
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                reveal(board, revealed, row + i, col + j);
            }

        }
    }
}

bool is_victory(char board[ROWS][COLS], char revealed[ROWS][COLS], int mines) {
    int unrevealed_cells = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (!revealed[i][j]) {
                unrevealed_cells++;
            }
        }
    }
    return unrevealed_cells == mines;
}

int main() {
    srand(time(NULL));

    char board[ROWS][COLS];
    char revealed[ROWS][COLS] = {false};

    initialize_board(board);
    place_mines(board, MINES);

    bool game_over = false;
    bool victory = false;

    while (!game_over) {
        system("cls");
        display_board(board, true); // Passer 'true' pour révéler une seule mine
        printf("Il y a %d mines sur la grille.\n", MINES);
        int row, col;
        char action;

        printf("Enter row, col and action (R for reveal, F for flag): ");
        scanf("%d %d %c", &row, &col, &action);

        if (in_bounds(row, col)) {
            if (action == 'R' || action == 'r') {
                if (board[row][col] == '*') {
                    game_over = true;
                } else {
                    reveal(board, revealed, row, col);
                    if (is_victory(board, revealed, MINES)) {
                        game_over = true;
                        victory = true;
                    }
                }
            } else if (action == 'F' || action == 'f') {
                board[row][col] = (board[row][col] == 'F' || board[row][col] == 'f') ? ' ' : 'F';
            }
        }
    }

    system("cls");
    display_board(board, false);

    if (victory) {
        printf("Congratulations! You won!\n");
    } else {
        printf("Game over! You hit a mine.\n");
    }

    return 0;
}

