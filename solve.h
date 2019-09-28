#include <stdlib.h>
#define SUCCESS 1
#define CONTRADICTION -1
#define MAKE_GUESS 0
#define GUESS_MARKER -1
#define TRUE 1
#define FALSE 0

static int initial_check(void);
static void move_to_next_cell(int *row_num_address, int *col_num_address);
static void get_possibilities(int *possibilities, int row_num, int col_num);
static int infer_values_with_certainty(void);
static void make_guess(void);
static void trace_back_to_latest_guess(void);
static void increment_latest_guess(void);
static int can_increment_latest_guess(void);
static int latest_guess_is_first_guess(void);
static void trace_back_to_previous_guess(void);
