#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Note to students. This is is kind of in a draft state, so I make no guarantees this is perfect or optimal.
 * However, it seems to be working pretty well!
 */

/* These are constants to define the initial maze setup. */

#define EMPTY -1
#define WALL -2
#define START -3

/* maze_data_t allows us to think of the maze (mostly) as an abstract data type. */

typedef struct _maze_data {
    int **maze;
    int rows;
    int cols;
} maze_data_t;


/* These inline functions ensure safe handling of the underlying array. */
/* assertions are used to ensure no unsafe access is performed, thereby eliminating segmentation faults. */

/* note that I do not expect students to know about inlining */

#define INLINE static inline

INLINE void maze_data_init(maze_data_t* maze_data) {
    maze_data->rows = 0;
    maze_data->cols = 0;
    maze_data->maze = NULL;
}


INLINE int mazedata_get(maze_data_t* maze_data, int i, int j) {
    assert(i < maze_data->rows);
    assert(j < maze_data->cols);
    return maze_data->maze[i][j];
}

INLINE void mazedata_set(maze_data_t* maze_data, int i, int j, int value) {
    assert(i < maze_data->rows);
    assert(j < maze_data->cols);
    maze_data->maze[i][j] = value;
}

INLINE void mazedata_allocate_maze(maze_data_t* maze_data)
{
    maze_data->maze = (int **)malloc(maze_data->rows * sizeof(int*));
    for (int row=0; row < maze_data->rows; row++)
        maze_data->maze[row] = (int*)malloc(maze_data->cols * sizeof(int));
}

INLINE void mazedata_free_maze(maze_data_t* maze_data)
{
    for (int row=0; row < maze_data->rows; row++)
        free(maze_data->maze[row]);
    free(maze_data->maze);
}

/* Read the input file to find number of rows & columns for dynamic allocation of the maze. */

void maze_preprocess(char *filename, int *max_lines, int *max_line_length)
{
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t chars_read, line_max;

    *max_lines = 0;
    *max_line_length = 0;

    stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    /* use getline(), which is recommended for security reasons over gets() and variants. */
    while ((chars_read = getline(&line, &len, stream)) != -1) {
        if (chars_read > *max_line_length)
            *max_line_length = chars_read-1; /* don't count newline */
        //fwrite(line, chars_read, 1, stdout);
        (*max_lines)++;
    }

    free(line);
    fclose(stream);
}

/* dynamically allocate the maze data structure and construct the 2d array representation */
/* maze_data must be allocated with expected number of rows & columns */

void maze_load(maze_data_t* maze_data, char* filename) {
    FILE *stream;
    char *line = NULL;
    size_t len = 0;
    ssize_t chars_read, line_max;
    int row=0;

    stream = fopen(filename, "r");

    while (row < maze_data->rows && (chars_read = getline(&line, &len, stream)) != -1) {
        for (int col=0; col < maze_data->cols; col++) {
            if (col >= chars_read) continue;
            switch (line[col]) {
            case 'b':
                mazedata_set(maze_data, row, col, WALL);
                break;
            case 's':
                mazedata_set(maze_data, row, col, START);
                break;
            default:
                mazedata_set(maze_data, row, col, EMPTY);
                break;
            }
        }
        row++;
    }
}

/* this is a basic maze printing routine to make sure the maze representation matches the original input */

void maze_print(maze_data_t* maze_data) {
    for (int row=0; row < maze_data->rows; row++) {
        printf("|");
        for (int col=0; col < maze_data->cols; col++) {
            switch(mazedata_get(maze_data, row, col)) {
            case WALL:
                printf("b");
                break;
            case START:
                printf("s");
                break;
            case EMPTY:
                printf(" ");
                break;
            default:
                printf("*");
            }
        }
        printf("|\n");
    }
}

/* this shows the maze representation plus the numbers stored in the array. The numbers greater than 0
 * correspond to distances */

void maze_print_distances(maze_data_t* maze_data) {
    for (int row=0; row < maze_data->rows; row++) {
        for (int col=0; col < maze_data->cols; col++) {
            printf("%3d/", mazedata_get(maze_data, row, col));
            switch(mazedata_get(maze_data, row, col)) {
            case WALL:
                printf("b");
                break;
            case START:
                printf("s");
                break;
            case EMPTY:
                printf("?");
                break;
            default:
                printf("*");
            }
        }
        printf("\n");
    }
}

/* find the starting character in the maze and set the row/column variables passed in.
 * A boolean found/not found is returned */

int find_start(maze_data_t* maze_data, int* start_row, int* start_col) {
    for (int row=0; row < maze_data->rows; row++) {
        for (int col=0; col < maze_data->cols; col++) {
            if (mazedata_get(maze_data, row, col) == START) {
                *start_row = row;
                *start_col = col;
                return 1;
            }
        }
    }
    return 0;
}


/* boolean function to return whether trial_row, trial_col is in the bounds of the supplied maze
 * or not */

int in_bounds(maze_data_t* maze_data, int trial_row, int trial_col) {
    if (trial_row < 0 || trial_row >= maze_data->rows)
        return 0;
    if (trial_col < 0 || trial_col >= maze_data->cols)
        return 0;
    return 1;
}

/* Internal (recursive) function to solve the maze and set all distances reachable from the start position.
 * The code is smart enough to handle any exits (or even none), because getting to an exit does not
 * terminate the code. Having no more places to visit is the condition for termination. So if a start
 * character is by itself in a "box", this means it would be 0 and every other location remains infinite
 * distance.
 * The backtracking is baked into the recursion.
 * The i/j loop magic iterates the NEWS positions in the maze.
 */

void maze_solve_recursive(maze_data_t *maze_data, int current_row, int current_col, int current_distance) {
    int this_cell_distance = mazedata_get(maze_data, current_row, current_col);
    if (this_cell_distance == EMPTY || current_distance < this_cell_distance)
       mazedata_set(maze_data, current_row, current_col, current_distance);

    for (int i=-1; i < 2; i++ ) {
        for (int j=-1; j < 2; j++) {
            if (i == j || i + j == 0) /* restrict to NEWS neighbors */
                continue;
            int neighbor_row = current_row + i;
            int neighbor_col = current_col + j;
            if (in_bounds(maze_data, neighbor_row, neighbor_col)) {
                /* We only visit empty neighbors (i.e. not a wall). */
                if (mazedata_get(maze_data, neighbor_row,neighbor_col) == EMPTY) {
                    /* distance increases by one relative to current row/col */
                    maze_solve_recursive(maze_data, neighbor_row, neighbor_col, current_distance+1);
                }
            }
        }
    }
}

/* this is the top-level driver to start the maze solver. Among other things, it makes sure there
 * is a starting coordinate in the maze and then starts the recursive solver with distance 0 as the
 * initial (base) case.
 */

void maze_solve(maze_data_t* maze_data) {
    int start_row, start_col;
    if (find_start(maze_data, &start_row, &start_col)) {
        maze_solve_recursive(maze_data, start_row, start_col, 0 /* distance */);
    } else {
        fprintf(stderr, "At least one start 's' cell must be provided.\n");
    }
}

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("usage: maze filename(s)\n");
        exit(EXIT_FAILURE);
    }

    for (int arg=1; arg < argc; arg++) {
        maze_data_t maze_data;
        printf("%s:\n", argv[arg]);
        printf("> preprocessing\n");
        maze_preprocess(argv[arg], &maze_data.rows, &maze_data.cols);
        printf("> allocating\n");
        mazedata_allocate_maze(&maze_data);
        printf("> loading %d x %d maze\n", maze_data.rows, maze_data.cols);
        maze_load(&maze_data, argv[arg]);
        printf("> solving\n");
        maze_solve(&maze_data);
        maze_print_distances(&maze_data);
        mazedata_free_maze(&maze_data);
    }
}


