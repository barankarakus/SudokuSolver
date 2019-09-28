#include "main.h"

int board[9][9];  // stores 9x9 sudoku board
int cells_filled[162];  /* each time an empty cell's value is filled, that cell's number
(row_num * 9 + col_num) is added to this array; if the cell's value
was guessed, -1 is added to the array, followed by the cell's number */
int *next_fill = cells_filled;  // points to next position in cells_filled
int *first_guess;  // points to the number of the first cell whose value was guessed

int main() {
    initialize_board();
    printf("Input:\n");
    print_board();
    int status = solve();
    if (status == SUCCESS) {
        printf("\nSolved successfully:\n");
        print_board();
    }
    else if (status == CONTRADICTION) {
        printf("\nUnable to solve; the input leads to a contradiction.\n");
    }
    return 0;
}

