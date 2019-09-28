#include "solve.h"

int solve(void) {
    extern int board[9][9], *next_fill, *first_guess;
    int status, guess_made = FALSE;
    status = initial_check();
    if (status == CONTRADICTION)
        return CONTRADICTION;
    while (TRUE) {
        status = infer_values_with_certainty();
        if (status == SUCCESS)
            return SUCCESS;
        else if (status == CONTRADICTION) {
            if (!guess_made)
                return CONTRADICTION;
            trace_back_to_latest_guess();
            while (can_increment_latest_guess() == FALSE) {
                if (latest_guess_is_first_guess())
                    return CONTRADICTION;
                trace_back_to_previous_guess();
            }
            increment_latest_guess();
        }
        else if (status == MAKE_GUESS) {
            make_guess();
            if (guess_made == FALSE) {
                guess_made = TRUE;
                first_guess = next_fill - 1;
            }
        }
    }
}

/* checks for contradictions in the starting configuration of the board
 e.g. two 1s in the same row */
static int initial_check(void) {
    extern int board[9][9];
    int row_num = 0, col_num = 0, current_val, possibilities[10];
    while (row_num * 9 + col_num < 81) {
        if (board[row_num][col_num] != 0) {
            current_val = board[row_num][col_num];
            board[row_num][col_num] = 0;
            get_possibilities(possibilities, row_num, col_num);
            if (possibilities[current_val] == 0)
                return CONTRADICTION;
            board[row_num][col_num] = current_val;
        }
        move_to_next_cell(&row_num, &col_num);
    }
    return SUCCESS;
}

static void move_to_next_cell(int *row_num_address, int *col_num_address) {
    if (*col_num_address < 8) {
        (*col_num_address)++;
    }
    else {
        *col_num_address = 0;
        (*row_num_address)++;
    }
}

/* places 1 at address (possiblities + i) if cell specified by row_num, col_num
 may take on the value i, else 0 */
static void get_possibilities(int *possibilities, int row_num, int col_num) {
    extern int board[9][9];
    int i, j, box_row_num, box_col_num;
    // initialize possibilities
    for (i = 1; i < 10; i++){
        possibilities[i] = 1;
    }
    // eliminate possibilities by going across row & down column
    for (i = 0; i < 9; i++) {
        possibilities[board[row_num][i]] = 0;
        possibilities[board[i][col_num]] = 0;
    }
    // eliminate possibilities by looking in current 3x3 box
    box_row_num = (row_num / 3) * 3;
    box_col_num = (col_num / 3) * 3;
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++)
        possibilities[board[box_row_num+i][box_col_num+j]] = 0;
    }
}

/* starting at top-left cell (0, 0), cycles through the cells (in left-to-right, downwards fashion)
 inferring their values with certainty wherever possible;
 - if a cell's value is inferred, the process restarts at (0, 0)
 - if a cell cannot take ANY values, CONTRADICTION is returned
 - if the process finishes with no empty cells, SUCCESS is returned
 - if the processes finished with empty cells and no contradiction, MAKE_GUESS is returned */
static int infer_values_with_certainty(void) {
    extern int board[9][9];
    extern int *next_fill;
    // row & column of cell currently under consideration
    int row_num = 0, col_num = 0;
    // for i = 1, ..., 9, possibilities[i] is 1 if i is a valid choice for the current cell, else 0
    int possibilities[10];
    // counter variables
    int i;
    // number of possible values for current cell
    int num_possibilities;
    // inferred value of current cell
    int inferred_value = 0;
    // board complete flag
    int board_complete = TRUE;
    while (row_num * 9 + col_num < 81) {
        if (board[row_num][col_num] == 0) {
            board_complete = FALSE;
            get_possibilities(possibilities, row_num, col_num);
            // count the number of valid possibilities for current cell
            num_possibilities = 0;
            for (i = 1; i < 10; i++) {
                if (possibilities[i]) {
                    inferred_value = i;
                    num_possibilities++;
                }
            }
            if (num_possibilities == 0)
                return CONTRADICTION;
            else if (num_possibilities == 1) {
                board[row_num][col_num] = inferred_value;
                *next_fill++ = row_num * 9 + col_num;
                board_complete = TRUE;
                row_num = 0; col_num = 0;
            }
            else {
                move_to_next_cell(&row_num, &col_num);
            }
        }
        else {
            move_to_next_cell(&row_num, &col_num);
        }
    }
    if (board_complete)
        return SUCCESS;
    else
        return MAKE_GUESS;
}

/* makes a guess at the first empty cell; the guess made is the lowest possible value
 that that cell can take, based on the values of the non-empty cells */
static void make_guess(void) {
    extern int board[9][9];
    extern int *next_fill;
    int row_num = 0, col_num = 0;
    int possibilities[10];
    for (; row_num * 9 + col_num < 81; move_to_next_cell(&row_num, &col_num)) {
        if (board[row_num][col_num] == 0) {
            get_possibilities(possibilities, row_num, col_num);
            for (int i = 1; i < 10; i++) {
                if (possibilities[i]) {
                    board[row_num][col_num] = i;
                    *next_fill++ = GUESS_MARKER;
                    *next_fill++ = row_num * 9 + col_num;
                    return;
                }
            }
        }
    }
}

/* undoes the board back to its state when the latest guess was made */
static void trace_back_to_latest_guess(void) {
    extern int board[9][9];
    extern int *next_fill;
    int row_num, col_num;
    while ((next_fill[-2] != GUESS_MARKER)) {
        row_num = next_fill[-1]/9;
        col_num = next_fill[-1] - 9 * row_num;
        board[row_num][col_num] = 0;
        next_fill--;
    }
}

/* checks if value of the last cell guessed can be increased */
static int can_increment_latest_guess(void) {
    extern int board[9][9];
    extern int *next_fill;
    int row_num = next_fill[-1]/9;
    int col_num = next_fill[-1] - 9 * row_num;
    int possibilities[10];
    get_possibilities(possibilities, row_num, col_num);
    for (int i = board[row_num][col_num] + 1; i < 10; i++){
        if (possibilities[i])
            return TRUE;
    }
    return FALSE;
}

/* increments the value of the last cell guessed to the next highest possible value */
static void increment_latest_guess(void) {
    extern int board[9][9];
    extern int *next_fill;
    int row_num = next_fill[-1]/9;
    int col_num = next_fill[-1] - 9 * row_num;
    int possibilities[10];
    get_possibilities(possibilities, row_num, col_num);
    for (int i = board[row_num][col_num] + 1; i < 10; i++){
        if (possibilities[i]) {
            board[row_num][col_num] = i;
            return;
        }
    }
}

/* checks if the cell whose value was last guessed is the first cell whose value was guessed */
static int latest_guess_is_first_guess(void) {
    extern int *next_fill;
    extern int *first_guess;
    return (next_fill - 1 == first_guess);
}

/* undoes the board back to its state when the second-last guess was made */
static void trace_back_to_previous_guess(void) {
    extern int board[9][9];
    extern int *next_fill;
    next_fill--;
    int row_num = *next_fill/9;
    int col_num = *next_fill - 9 * row_num;
    board[row_num][col_num] = 0;
    next_fill--;
    trace_back_to_latest_guess();
}

