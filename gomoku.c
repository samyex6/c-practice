/**
 * Gomoku
 * @author    Δοδύο
 * @version   0.1
 * @changelog 
 *      - V0.1: Basic implementation, dumb move picking
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX(n, m) ((n) > (m) ? (n) : (m))
#define MIN(n, m) ((n) < (m) ? (n) : (m))

#define MIN_DIMENSION  8

#define PLAYER_AI      0
#define PLAYER_ME      1

#define STATE_NONE    -1
#define STATE_DRAW     2

#define DIR_VERTICAL   0
#define DIR_HORIZONTAL 1
#define DIR_DIAGONAL   2
#define DIR_DIAGONALR  3

typedef struct Board {
    int   dimension;
    int   piece_count;
    int   is_end;
    char* config;
} Board;

typedef struct Cell {
    int row;
    int col;
} Cell;


int  checkWinning (Board* board, Cell* last_move, int player);
int  isValidMove (Board* board, Cell* move);
char getCell (Board* board, int row, int col);
void initBoard (Board* board, int n);
void setCell (Board* board, int row, int col, char symbol);
void printBoard (Board* board);
Cell pickMove (Board* board, char symbol);


// Say if a board is like this
// 3 - - - 1 - - - 4
// - 3 - - 1 - - 4 -
// - - 3 - 1 - 4 - -
// - - - 3 1 4 - - -
// 2 2 2 2 O 2 2 2 2
// - - - 4 1 3 - - -
// - - 4 - 1 - 3 - -
// - 4 - - 1 - - 3 -
// 4 - - - 1 - - - 3
// then we'll have 4 directions to check with, the indexes are
// 1. [R-4, C  ]~[R+4, C  ]
// 2. [R  , C-4]~[R  , C+4]
// 3. [R-4, C-4]~[R+4, C+4]
// 4. [R+4, C-4]~[R-4, C+4]
int checkWinning (Board* board, Cell* last_move, int player) {
    int  col, row, start, end, dir, idx, idx2, start_col, start_row, end_col, end_row;
    int  row_ls        = MAX(last_move->row - 4, 0                   );
    int  col_ls        = MAX(last_move->col - 4, 0                   );
    int  row_rs        = MIN(last_move->row + 4, board->dimension - 1);
    int  col_rs        = MIN(last_move->col + 4, board->dimension - 1);
    char label         = getCell(board, last_move->row, last_move->col);
    int  options[4][3] = {
        {row_ls, row_rs, last_move->col},
        {col_ls, col_rs, last_move->row},
        {row_ls, row_rs,  1            },
        {row_rs, row_ls, -1            }
    };

    // checking vertical & horizontal, general processing
    for (dir = DIR_VERTICAL; dir < DIR_HORIZONTAL; dir++) {
        // starting point selection, from top to bottom, left to right, since we are looping for the 
        // next 4 elements from the starting point, this means the selecting process can be terminated
        // after reaching the center.
        for (start = options[dir][0], idx2 = options[dir][2]; options[dir][1] - start >= 4; start++) {
            int interrupted = 0;
            for (idx = start, end = idx + 4; idx <= end; idx++) {
                char val = dir == DIR_VERTICAL ? getCell(board, idx, idx2) : getCell(board, idx2, idx);
                if (val != label) {
                    interrupted = 1;
                    break;
                }
            }
            if (!interrupted) {
                return player;
            }
        }
    }

    // checking diagonal, almost the same implementation with vertical & horizontal
    for (dir = DIR_DIAGONAL; dir <= DIR_DIAGONALR; dir++) {
        start_col = col_ls;
        start_row = options[dir][0];
        for (; last_move->col - start_col >= 4; start_col++, start_row += options[dir][2]) {
            int interrupted = 0;
            row     = start_row;
            col     = start_col;
            end_row = end + 4;
            end_col = col + 4;
            for (; row <= end_row && col <= end_col; col++, row += options[dir][2]) {
                if (getCell(board, row, col) != label) {
                    interrupted = 1;
                    break;
                }
            }
            if (!interrupted) {
                return player;
            }
        }
    }
    

    return board->piece_count == board->dimension * board->dimension ? STATE_DRAW : STATE_NONE;
}

int isValidMove (Board* board, Cell* move) {
    return move->row >= 0 && move->row < board->dimension &&
           move->col >= 0 && move->col < board->dimension &&
           getCell(board, move->row, move->col) == '-';
}

char getCell (Board* board, int row, int col) {
    return board->config[row * board->dimension + col];
}

void initBoard (Board* board, int n) {
    board->config      = (char*)  malloc(sizeof(char*) * n * n);
    board->dimension   = n;
    board->is_end      = 0;
    board->piece_count = 0;
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            setCell(board, i, j, '-');
        }
    }
    board->piece_count = 0;
}

void setCell (Board* board, int row, int col, char symbol) {
    board->config[row * board->dimension + col] = symbol;
    board->piece_count++;
}

void printBoard (Board* board) {
    int col, row, n = board->dimension;
    printf("  ");
    for (col = 0; col < n; col++) {
        printf(" %2d", col);
    }
    printf("\n");
    for (row = 0; row < n; row++) {
        printf("%2d", row);
        for (col = 0; col < n; col++) {
            printf(" %2c", getCell(board, row, col));
        }
        printf("\n");
    }
}

// right now it's just a dumb randomizer, actuell AI awaiting
Cell pickMove (Board* board, char symbol) {
    Cell decision;
    while (1) {
        decision.row = rand() % board->dimension;
        decision.col = rand() % board->dimension;
        if (getCell(board, decision.row, decision.col) == '-') {
            break;
        }
    }
    return decision;
}



int main () {

    while (1) {

        srand(time(NULL));

        printf("Please specify a Gomoku dimension (the number must be >= %d):\n", MIN_DIMENSION);

        int n;
        scanf("%d", &n);

        // dealing with small numbers
        if (n < MIN_DIMENSION) {
            printf("Number too small, automatically set to %d.\n", MIN_DIMENSION);
            n = MAX(n, MIN_DIMENSION);
        }

        // generating board
        printf("Initializing board...\n");
        Board board;
        initBoard(&board, n);
        printBoard(&board);

        // randomizing player, 0=AI, 1=you
        int player = rand() % 2;
        printf("player=%d\n", player);

        const char* lang[3][5] = {
            {"AI" , "'s", "O" , "AI has won"  , "boo"},
            {"you", "r" , "X" , "You have won", "what an amazing (my) move (ass)"},
            {NULL , NULL, NULL, "It's a draw" , NULL}
        };
        while (!board.is_end) {
            printf("Now is %s%s turn\n", lang[player][0], lang[player][1]);

            // making a move, is it's AI, pass the decision right to the conputer
            Cell decision;
            switch (player) {
                case PLAYER_AI:
                    decision = pickMove(&board, lang[player][2][0]);
                    break;
                case PLAYER_ME:
                    //decision = pickMove(&board, lang[player][2][0]);
                    //break;
                    while (1) {
                        printf("Please pick a move in \"row col\" format: ");
                        scanf("%d %d", &decision.row, &decision.col);
                        if (isValidMove(&board, &decision)) {
                            break;
                        }
                        printf("Invalid move! Please repick.\n");
                    }
                    break;
            }

            // has made the decision and now printing out the board
            printf("%s picked %d %d, %s!\n", lang[player][0], decision.row, decision.col, lang[player][4]);
            setCell(&board, decision.row, decision.col, lang[player][2][0]);
            printBoard(&board);

            int after_state = checkWinning(&board, &decision, player);
            if (after_state != STATE_NONE) {
                board.is_end = 1;
                printf("Nice game! %s!\n", lang[after_state][3]);
            } else {
                player ^= 1; // swap player
            }
        }
    }

    return 0;
}
