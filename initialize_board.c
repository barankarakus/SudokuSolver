#include <stdio.h>
#include <stdlib.h>

static char *allocate_array_for_input(void) {
    char *user_input = (char *) malloc(100);
    for (int i = 0; i < 100; i++) {
        user_input[i] = 0;
    }
    return user_input;
}

void initialize_board(void) {
    extern int board[9][9];
    int row_num, col_num;
    char character;
    char *user_input;
    printf("Please enter the sudoku board to be solved.\n");
    printf("The following characters will be interpreted as an empty value: 0, -, /, _.\n");
    printf("Each of the digits 1-9 will be interpreted as itself.\n");
    printf("All other characters will be ignored.\n");
    printf("If less than 9 of the cells of a row are specified, the unspecified cells are assumed empty.\n");
    printf("\n");
    for (row_num = 0; row_num < 9; row_num++) {
        for (col_num = 0; col_num < 9; col_num++)
            board[row_num][col_num] = 0;
        printf("Enter the numbers in row %d.\n", row_num + 1);
        user_input = allocate_array_for_input();
        fgets(user_input, 99, stdin);
        col_num = 0;
        for (int i = 0; (i < 100 && col_num < 9 && user_input[i] != '\n'); i++) {
            character = user_input[i];
            switch (character) {
                case '0': case '-': case '/': case '_':
                    col_num++;
                    break;
                case '1':
                    board[row_num][col_num++] = 1;
                    break;
                case '2':
                    board[row_num][col_num++] = 2;
                    break;
                case '3':
                    board[row_num][col_num++] = 3;
                    break;
                case '4':
                    board[row_num][col_num++] = 4;
                    break;
                case '5':
                    board[row_num][col_num++]= 5;
                    break;
                case '6':
                    board[row_num][col_num++] = 6;
                    break;
                case '7':
                    board[row_num][col_num++] = 7;
                    break;
                case '8':
                    board[row_num][col_num++] = 8;
                    break;
                case '9':
                    board[row_num][col_num++] = 9;
                    break;
            }
        }
        free(user_input);
    }
}

