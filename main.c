/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Sammi Li 1271851
  Dated:     09/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial row with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

#define NO_OF_PIECES        12      // number of pieces per colour
#define NO_OF_CELLS         64      // number of cells in the board
#define INDEX_SET           1       // converter from index to desired value
#define BLACK               0       // indicator for player black's turn
#define WHITE               1       // indicator for player white's turn
#define ASCII_COL           65      // convert char into corresponding col
#define FIRST_COL           'A'     // first column
#define LAST_COL            'H'     // last column
#define FIRST_ROW           0       // first row index
#define LAST_ROW            7       // last row index
#define FALSE               0         
#define TRUE                1
#define MOVEMENT            1     	// spaces in a normal turn
#define CAPTURE_MOVEMENT    2       // spaces moved when capturing
#define TOWER_PIECES        0       // starting tower pieces
#define STAGE_1             'A'     // stage 1 indicator
#define STAGE_2             'P'     // stage 2 indicator
#define COL_RESET           '.'     // character to reset the column
#define ERROR               0       // error
#define INITIAL_NODE        0       
#define STAGE2_PLAYS        10      // number of plays in stage 2

/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type

typedef struct tree_node{
  int score;
  int target_row;
  int target_col;
  int source_col;
  int source_row;
  struct tree_node *start;
  struct tree_node **moves;
} tree_node;

void print_board(char board[BOARD_SIZE][BOARD_SIZE]);
int source_in_board(char source_col, int source_row);
int target_in_board(char target_col, int target_row);
int source_empty(char board[BOARD_SIZE][BOARD_SIZE], int source_col,
int source_row);
int target_not_empty(char board[BOARD_SIZE][BOARD_SIZE], int target_col, 
int target_row);
int source_is_opponent(char board[BOARD_SIZE][BOARD_SIZE], int source_col,
int source_row, int player);
int illegal_action(char board[BOARD_SIZE][BOARD_SIZE], int source_col, 
int source_row, int target_col, int target_row, int player);
void check_tower(char board[BOARD_SIZE][BOARD_SIZE], int player, 
int target_row_i, int target_col_i,int *no_of_b, int *no_of_w, int *no_of_B, 
int *no_of_W);
int stage1and2(struct tree_node *start, char board[BOARD_SIZE][BOARD_SIZE], int player, int no_of_B,
int no_of_b, int no_of_w, int no_of_W, int level);
int insert_child(struct tree_node *start, struct tree_node *moves);

int
main(int argc, char *argv[]) {
    // YOUR IMPLEMENTATION OF STAGES 0-2
    int no_of_b = NO_OF_PIECES, no_of_w = NO_OF_PIECES,	
	no_of_B = TOWER_PIECES, no_of_W = TOWER_PIECES;
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    printf("#BLACK PIECES: %d\n", no_of_b);
    printf("#WHITE PIECES: %d\n", no_of_w);
    char board[BOARD_SIZE][BOARD_SIZE] = { \
		{CELL_EMPTY, CELL_WPIECE, CELL_EMPTY, CELL_WPIECE, CELL_EMPTY, 
		CELL_WPIECE, CELL_EMPTY, CELL_WPIECE}, /* 1st row_num */ \
      	{CELL_WPIECE, CELL_EMPTY, CELL_WPIECE, CELL_EMPTY, CELL_WPIECE, 
		CELL_EMPTY, CELL_WPIECE, CELL_EMPTY}, /* 2nd row_num */ \
    	{CELL_EMPTY, CELL_WPIECE, CELL_EMPTY, CELL_WPIECE, CELL_EMPTY, 
		CELL_WPIECE, CELL_EMPTY, CELL_WPIECE}, /* 3rd row_num */ \
      	{CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, 
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY}, /* 4th row_num */ \
      	{CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, 
		CELL_EMPTY, CELL_EMPTY, CELL_EMPTY}, /* 5th row_num */ \
      	{CELL_BPIECE, CELL_EMPTY, CELL_BPIECE, CELL_EMPTY, CELL_BPIECE, 
		CELL_EMPTY, CELL_BPIECE, CELL_EMPTY}, /* 6th row_num */ \
      	{CELL_EMPTY, CELL_BPIECE, CELL_EMPTY, CELL_BPIECE, CELL_EMPTY, 
		CELL_BPIECE, CELL_EMPTY, CELL_BPIECE}, /* 7th row_num */ \
      	{CELL_BPIECE, CELL_EMPTY, CELL_BPIECE, CELL_EMPTY, CELL_BPIECE, 
		CELL_EMPTY, CELL_BPIECE, CELL_EMPTY} /* 8th row_num */ 
	};

	print_board(board);

    int source_row, target_row, player=BLACK, no_of_turns=1;
	int board_cost, horizontal, vertical;
    unsigned char source_col, target_col;
    while (scanf("%c%d-%c%d\n", &source_col, &source_row, &target_col, 
	&target_row) == 4){
		int target_row_i = target_row - INDEX_SET;
		int target_col_i = target_col - ASCII_COL;
		int source_row_i = source_row - INDEX_SET;
		int source_col_i = source_col - ASCII_COL;
		if (source_in_board(source_col, source_row_i) == TRUE){
			printf("ERROR: Source cell is outside of the board.\n");
			return EXIT_FAILURE;
		}
		else if (target_in_board(target_col, target_row_i) == TRUE){
			printf("ERROR: Target cell is outside of the board.\n");
			return EXIT_FAILURE;
		}
		else if (source_empty(board, source_col_i, source_row_i) == TRUE){
			printf("ERROR: Source cell is empty.\n");
			return EXIT_FAILURE;
		}
		else if (target_not_empty(board, target_col_i, target_row_i) == TRUE){
			printf("ERROR: Target cell is not empty.\n");
			return EXIT_FAILURE;
		}
		else if (source_is_opponent(board, source_col_i, source_row_i, player) == TRUE){
			printf("ERROR: Source cell holds opponent's piece/tower.\n");
			return EXIT_FAILURE;
		}
		else if (illegal_action(board, source_col_i, source_row_i, target_col_i, target_row_i, player) == TRUE){
			printf("ERROR: Illegal action.\n");
			return EXIT_FAILURE;
		}

		printf("=====================================\n");
		if (player == BLACK){
			printf("BLACK ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col, 
			source_row, target_col, target_row);
			no_of_turns++;
		}
		else {
			printf("WHITE ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col, 
			source_row, target_col, target_row);
			no_of_turns++;
		}
		
		unsigned char source_cell = board[source_row_i][source_col_i];
		horizontal = target_col_i - source_col_i; 
		vertical = target_row - source_row;
		if (fabs(horizontal) == CAPTURE_MOVEMENT && 
			fabs(vertical) == CAPTURE_MOVEMENT){
			if (player == BLACK){
				if (board[source_row_i + (vertical/2)]
				[source_col_i + (horizontal/2)] == CELL_WTOWER){
					no_of_W--;
				}
				else{
					no_of_w--;
				}
			}
			else{
				if (board[source_row_i + (vertical/2)]
				[source_col_i + (horizontal/2)] == CELL_BTOWER){
					no_of_B--;
				}
				else{
					no_of_b--;
				}
			}
			board[target_row_i][target_col_i] = source_cell;
			board[source_row_i][source_col_i] = CELL_EMPTY;
			board[source_row_i + (vertical/2)]
			[source_col_i + (horizontal/2)] = CELL_EMPTY;
		}
		else{
			board[target_row_i][target_col_i] = source_cell;
			board[source_row_i][source_col_i] = CELL_EMPTY;
		}

		if (player == BLACK){
			if (target_row_i == FIRST_ROW){
				board[target_row_i][target_col_i] = CELL_BTOWER;
				no_of_b--;
				no_of_B++;
			}
		}
		else{
			if (target_row_i == LAST_ROW){
				board[target_row_i][target_col_i] = CELL_WTOWER;
				no_of_w--;
				no_of_W++;
			}
		}

		board_cost = no_of_b + 3*no_of_B - no_of_w - 3*no_of_W;
		printf("BOARD COST: %d\n", board_cost);
		print_board(board);
		if (player == BLACK){
			player = WHITE;
		}
		else{
			player = BLACK;
		}
		source_col = COL_RESET;
    };

	unsigned char next_stage = source_col;
	if (next_stage == STAGE_1){
		struct tree_node stage12 = {INITIAL_NODE, INITIAL_NODE, INITIAL_NODE, INITIAL_NODE, INITIAL_NODE};
		struct tree_node *start = &stage12;
		int level=0;
		stage1and2(start, board, player, no_of_B, no_of_b, no_of_w, no_of_W, level);
		int max_score=0, source_row_i=0, source_col_i=0, target_row_i=0, target_col_i=0;
		for (int i=0; start->moves[i]; i++){
			if (start->moves[i]->score > max_score){
				printf("%d",max_score);
				max_score = start->moves[i]->score;
				source_row_i =start->moves[i]->source_col;
				source_col_i = start->moves[i]->source_row;
				target_row_i = start->moves[i]->target_col;
				target_col_i = start->moves[i]->target_row;
			}
		}
		horizontal = target_col_i - source_col_i;
		vertical = target_row_i - source_row_i;
		if (fabs(horizontal) == CAPTURE_MOVEMENT && 
		fabs(vertical) == CAPTURE_MOVEMENT){
			board[target_row_i][target_col_i] = board[source_row_i][source_col_i];
			board[source_row_i][source_col_i] = CELL_EMPTY;
			board[source_row_i + (vertical/2)]
			[source_col_i + (horizontal/2)] = CELL_EMPTY;
		}
		else{
			board[target_row_i][target_col_i] = board[source_row_i][source_col_i];
			board[source_row_i][source_col_i] = CELL_EMPTY;
		}
		if (player == BLACK){
			if (target_row_i == FIRST_ROW){
				board[target_row_i][target_col_i] = CELL_BTOWER;
			}
		}
		else{
			if (target_row_i == LAST_ROW){
				board[target_row_i][target_col_i] = CELL_WTOWER;
			}
		}
		printf("=====================================\n");
		if (player == BLACK){
			printf("*** BLACK ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col_i + ASCII_COL, 
			source_row_i+INDEX_SET, target_col_i+ASCII_COL, target_row_i+INDEX_SET);
			no_of_turns++;
		}
		else {
			printf("*** WHITE ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col_i + ASCII_COL, 
			source_row_i-INDEX_SET, target_col_i+ASCII_COL, target_row_i-INDEX_SET);
			no_of_turns++;
		}
		for (int i=0; start->moves[i]; i++){
			free(start->moves[i]);
		}
		printf("BOARD COST: %d\n", max_score);
		print_board(board);
	}

	if (next_stage == STAGE_2){
		for (int level=0; level<STAGE2_PLAYS; level++){

			struct tree_node stage12 = {INITIAL_NODE, INITIAL_NODE, INITIAL_NODE, INITIAL_NODE, INITIAL_NODE};
			struct tree_node *start = &stage12;
			stage1and2(start, board, player, no_of_B, no_of_b, no_of_w, no_of_W, level);
			int max_score=0, source_row_i=0, source_col_i=0, target_row_i=0, target_col_i=0;
			for (int i=0; start->moves[i]; i++){
				if (start->moves[i]->score > max_score){
					printf("%d",max_score);
					max_score = start->moves[i]->score;
					source_row_i =start->moves[i]->source_col;
					source_col_i = start->moves[i]->source_row;
					target_row_i = start->moves[i]->target_col;
					target_col_i = start->moves[i]->target_row;
				}
			}
			horizontal = target_col_i - source_col_i;
			vertical = target_row_i - source_row_i;
			if (fabs(horizontal) == CAPTURE_MOVEMENT && 
			fabs(vertical) == CAPTURE_MOVEMENT){
				board[target_row_i][target_col_i] = board[source_row_i][source_col_i];
				board[source_row_i][source_col_i] = CELL_EMPTY;
				board[source_row_i + (vertical/2)]
				[source_col_i + (horizontal/2)] = CELL_EMPTY;
			}
			else{
				board[target_row_i][target_col_i] = board[source_row_i][source_col_i];
				board[source_row_i][source_col_i] = CELL_EMPTY;
			}
			if (player == BLACK){
				if (target_row_i == FIRST_ROW){
					board[target_row_i][target_col_i] = CELL_BTOWER;
				}
			}
			else{
				if (target_row_i == LAST_ROW){
					board[target_row_i][target_col_i] = CELL_WTOWER;
				}
			}
			printf("=====================================\n");
			if (player == BLACK){
				printf("*** BLACK ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col_i + ASCII_COL, 
				source_row_i+INDEX_SET, target_col_i+ASCII_COL, target_row_i+INDEX_SET);
				no_of_turns++;
			}
			else {
				printf("*** WHITE ACTION #%d: %c%d-%c%d\n", no_of_turns, source_col_i + ASCII_COL, 
				source_row_i-INDEX_SET, target_col_i+ASCII_COL, target_row_i-INDEX_SET);
				no_of_turns++;
			}
			for (int i=0; start->moves[i]; i++){
				free(start->moves[i]);
			}
			printf("BOARD COST: %d\n", max_score);
			print_board(board);
			if (player == BLACK){
				player = WHITE;
			}
			else{
				player = BLACK;
			}
		}
	}
    return EXIT_SUCCESS;            // exit program with the success code      *
}

int insert_child(struct tree_node *start, struct tree_node *moves) {
  	size_t no_of_child=INITIAL_NODE;
  	if (start->moves != NULL){
		while (start->moves[no_of_child] != NULL) {
	  		no_of_child++;
		}
  	}
  	struct tree_node **new_child = realloc(start->moves, (no_of_child + 2) * sizeof(*new_child));
  	if (new_child == NULL){
	  	return ERROR;
  	}
    start->moves[no_of_child] = NULL;
    start->moves[no_of_child++] = moves;
	start->moves = new_child;
    moves->start = start;
    return 0;
}

void print_board(char board[BOARD_SIZE][BOARD_SIZE]) {
	printf("     A   B   C   D   E   F   G   H\n");
    for (int row_num=0; row_num<BOARD_SIZE; row_num++){
      	printf("   +---+---+---+---+---+---+---+---+\n");
		printf(" %d ", row_num + INDEX_SET);
      	for (int col_num=0; col_num<BOARD_SIZE; col_num++) {
			unsigned char cell = board[row_num][col_num];
			if (col_num == BOARD_SIZE - INDEX_SET){
				printf("| %c |\n", cell);
			}
			else {
				printf("| %c ", cell);
			}
      	}
    }
	printf("   +---+---+---+---+---+---+---+---+\n");
}

int source_in_board(char source_col, int source_row){
	int error_bool = FALSE;
	/*1. source code is outside of the board. */
	if (FIRST_COL>source_col || LAST_COL<source_col || FIRST_ROW>source_row ||
	LAST_ROW<source_row){
		error_bool = TRUE;
	}
	return error_bool;
}

int target_in_board(char target_col, int target_row){
	int error_bool = FALSE;
	/* 2. target cell is outside of the board. */
	if (FIRST_COL>target_col || LAST_COL<target_col || FIRST_ROW>target_row ||
	LAST_ROW<target_row){
		error_bool = TRUE;
	}
	return error_bool;
}

int source_empty(char board[BOARD_SIZE][BOARD_SIZE], int source_col, 
int source_row){
	int error_bool = FALSE;
	/* 3. source code is empty. */
	unsigned char source_cell = board[source_row][source_col];
	if (source_cell == CELL_EMPTY){
;		error_bool = TRUE;
	}
	return error_bool;
}

int target_not_empty(char board[BOARD_SIZE][BOARD_SIZE], int target_col, 
int target_row){
	int error_bool = FALSE;
	/* 4. target cell is not empty. */
	unsigned char target_cell = board[target_row][target_col];
	if (target_cell != CELL_EMPTY){
		error_bool = TRUE;
	}
	return error_bool;
}

int source_is_opponent(char board[BOARD_SIZE][BOARD_SIZE], int source_col, 
int source_row, int player){
	int error_bool = FALSE;
	/* 5. source cell holds opponent's piece/tower.*/
	unsigned char source_cell = board[source_row][source_col];
	if (player == BLACK){
		if (source_cell == CELL_WPIECE || source_cell == CELL_WTOWER){
			error_bool = TRUE;
		}
	}
	else {
		if (source_cell == CELL_BPIECE || source_cell == CELL_BTOWER){
			error_bool = TRUE;
		}
	}
	return error_bool;
} 

int illegal_action(char board[BOARD_SIZE][BOARD_SIZE], int source_col, 
int source_row, int target_col, int target_row, int player){
	int error_bool = FALSE;
	bool backwards_movement;
	/* 6. Illegal action. */
	unsigned char source_cell = board[source_row][source_col];
	int horizontal = target_col - source_col; 
	int vertical = target_row - source_row; 
	if (player == BLACK){
		backwards_movement = (vertical > 0);
	}
	else{
		backwards_movement = (vertical < 0);
	}
	if (source_cell == CELL_BTOWER || source_cell == CELL_WTOWER){
		if (fabs(horizontal) == CAPTURE_MOVEMENT && 
			fabs(vertical) == CAPTURE_MOVEMENT){
			/* check if the move captured a piece */
			unsigned char capture_cell = board[source_row + (vertical/2)]
			[source_col + (horizontal/2)];
			if (player == BLACK){
				if (capture_cell != CELL_WTOWER && 
				capture_cell != CELL_WPIECE){
					error_bool = TRUE;
				}
			}
			else {
				if (capture_cell != CELL_BTOWER && 
				capture_cell != CELL_BPIECE){
					error_bool = TRUE;
				}
			}
		}
		else if(fabs(horizontal) != MOVEMENT || fabs(vertical) != MOVEMENT){
			error_bool = TRUE;
		}
	}
	else{
		if (backwards_movement){
			error_bool = TRUE;
		}
		else if (fabs(horizontal) == CAPTURE_MOVEMENT && 
			fabs(vertical) == CAPTURE_MOVEMENT){
			/* check if the move captured a piece */
			unsigned char capture_cell = board[source_row + (vertical/2)]
			[source_col + (horizontal/2)];
			if (player == BLACK){
				if (capture_cell != CELL_WTOWER && 
				capture_cell != CELL_WPIECE){
					error_bool = TRUE;
				}
			}
			else {
				if (capture_cell != CELL_BTOWER && 
				capture_cell != CELL_BPIECE){
					error_bool = TRUE;
				}
			}
		}
		else if(fabs(horizontal) != MOVEMENT || fabs(vertical) != MOVEMENT){
			error_bool = TRUE;
		}
	}
	return error_bool;
}

int stage1and2(struct tree_node *start, char board[BOARD_SIZE][BOARD_SIZE], int player, int no_of_B,
int no_of_b, int no_of_w, int no_of_W, int level){
	/* step 1: construct the tree of all reachable board configurations of the requested depth starting from the current configuration */
	/* step 2: compute the costs of all the leaf boards in the tree (level 3) */
	/* step 3: porpagate leaf coats toward the root of the tree by alternating min and max decisions */
	int cur_b, cur_B, cur_w, cur_W, score;
	if (level == 3){
		return 1;
	}
	if (player == BLACK){
		for (int row=0; row<BOARD_SIZE; row++){
			for (int col=0; col<BOARD_SIZE; col++){
				if (board[row][col] == CELL_BPIECE){
					int ver = CAPTURE_MOVEMENT;
					for (int hor = CAPTURE_MOVEMENT, i=0; i!=2; hor*=-1){
						cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
						cur_W = no_of_W;
						int target_row_i = row - ver;
						char target_col = col + hor + ASCII_COL;
						int target_col_i = col + hor;
						if (target_in_board(target_col, target_row_i) != TRUE &&
						target_not_empty(board, target_col_i, target_row_i) != TRUE &&
						illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
							if (target_row_i == FIRST_ROW){
								cur_b--;
								cur_B++;
							}
							if (board[row - (ver/2)][col + (hor/2)] == CELL_WTOWER){
								cur_W--;
							}
							else if (board[row - (ver/2)][col + (hor/2)] == CELL_WPIECE){
								cur_w--;
							}
							score = cur_b + 3*cur_B - cur_w - 3*cur_W;
							tree_node moves = {score, target_col_i, target_row_i, row, col};
							insert_child(start, &moves);
						}
						i++;
					}
					int cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
					cur_W = no_of_W;
					ver = MOVEMENT;
					for (int hor = MOVEMENT, i=0; i!=2; hor*=-1){
						cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
						cur_W = no_of_W;
						int target_row_i = row - ver;
						char target_col = col + hor + ASCII_COL;
						int target_col_i = col + hor;
						if (target_in_board(target_col, target_row_i) != TRUE &&
						target_not_empty(board, target_col_i, target_row_i) != TRUE &&
						illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
							if (target_row_i == FIRST_ROW){
								cur_b--;
								cur_B++;
							}
							score = cur_b + 3*cur_B - cur_w - 3*cur_W;
							tree_node moves = {score, target_col_i, target_row_i, row, col};
							insert_child(start, &moves);
						}
						i++;
					}
				}
				else if (board[row][col] == CELL_BTOWER){
					for (int ver = CAPTURE_MOVEMENT, j=0; j!=2; ver*=-1){
						for (int hor = CAPTURE_MOVEMENT, i=0; i!=2; hor*=-1){
							cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
							cur_W = no_of_W;
							int target_row_i = row - ver;
							char target_col = col + hor + ASCII_COL;
							int target_col_i = col + hor;
							if (target_in_board(target_col, target_row_i) != TRUE &&
							target_not_empty(board, target_col_i, target_row_i) != TRUE &&
							illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
								if (board[row + (ver/2)][col + (hor/2)] == CELL_WTOWER){
									cur_W--;
								}
								else if (board[row + (ver/2)][col + (hor/2)] == CELL_WPIECE){
									cur_w--;
								}
								int score = cur_b + 3*cur_B - cur_w - 3*cur_W;
								tree_node moves = {score, target_col_i, target_row_i, row, col};
								insert_child(start, &moves);
							}
							i++;
						}
						j++;
					}
					int cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w, cur_W = no_of_W;
					for (int ver = MOVEMENT, j=0; j!=2; ver*=-1){
						for (int hor = MOVEMENT, i=0; i!=2; hor*=-1){
							cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
							cur_W = no_of_W;
							int target_row_i = row - ver;
							char target_col = col + hor + ASCII_COL;
							int target_col_i = col + hor;
							if (target_in_board(target_col, target_row_i) != TRUE &&
							target_not_empty(board, target_col_i, target_row_i) != TRUE &&
							illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
								if (target_row_i == FIRST_ROW){
									cur_b--;
									cur_B++;
								}
								score = cur_b + 3*cur_B - cur_w - 3*cur_W;
								tree_node moves = {score, target_col_i, target_row_i, row, col};
								insert_child(start, &moves);
							}
							i++;
						}
						j++;
					}
				}
			}
		}
	}
	else {
		for (int row=0; row<BOARD_SIZE; row++){
			for (int col=0; col<BOARD_SIZE; col++){
				if (board[row][col] == CELL_WPIECE){
					int ver = CAPTURE_MOVEMENT;
					for (int hor = CAPTURE_MOVEMENT, i=0; i!=2; hor*=-1){
						cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
						cur_W = no_of_W;
						int target_row_i = row + ver;
						char target_col = col + hor + ASCII_COL;
						int target_col_i = col + hor;
						if (target_in_board(target_col, target_row_i) != TRUE &&
						target_not_empty(board, target_col_i, target_row_i) != TRUE &&
						illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
							if (target_row_i == FIRST_ROW){
								cur_w--;
								cur_W++;
							}
							if (board[row - (ver/2)][col + (hor/2)] == CELL_WTOWER){
								cur_B--;
							}
							else if (board[row - (ver/2)][col + (hor/2)] == CELL_WPIECE){
								cur_b--;
							}
							score = cur_b + 3*cur_B - cur_w - 3*cur_W;
							tree_node moves = {score, target_col_i, target_row_i, row, col};
							insert_child(start, &moves);
						}
						i++;
					}
					ver = MOVEMENT;
					for (int hor = MOVEMENT, i=0; i!=2; hor*=-1){
						cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
						cur_W = no_of_W;
						int target_row_i = row + ver;
						char target_col = col + hor + ASCII_COL;
						int target_col_i = col + hor;
						if (target_in_board(target_col, target_row_i) != TRUE &&
						target_not_empty(board, target_col_i, target_row_i) != TRUE &&
						illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
							if (target_row_i == FIRST_ROW){
								cur_w--;
								cur_W++;
							}
							score = cur_b + 3*cur_B - cur_w - 3*cur_W;
							tree_node moves = {score, target_col_i, target_row_i, row, col};
							insert_child(start, &moves);
						}
						i++;
					}
				}
				else if (board[row][col] == CELL_WTOWER){
					for (int ver = CAPTURE_MOVEMENT, j=0; j!=2; ver*=-1){
						for (int hor = CAPTURE_MOVEMENT, i=0; i!=2; hor*=-1){
							cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
							cur_W = no_of_W;
							int target_row_i = row + ver;
							char target_col = col + hor + ASCII_COL;
							int target_col_i = col + hor;
							if (target_in_board(target_col, target_row_i) != TRUE &&
							target_not_empty(board, target_col_i, target_row_i) != TRUE &&
							illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
								if (board[row + (ver/2)][col + (hor/2)] == CELL_WTOWER){
									cur_W--;
								}
								else if (board[row + (ver/2)][col + (hor/2)] == CELL_WPIECE){
									cur_w--;
								}
								score = cur_b + 3*cur_B - cur_w - 3*cur_W;
								tree_node moves = {score, target_col_i, target_row_i, row, col};
								insert_child(start, &moves);
							}
							i++;
						}
						j++;
					}
					int cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w, cur_W = no_of_W;
					for (int ver = MOVEMENT, j=0; j!=2; ver*=-1){
						for (int hor = MOVEMENT, i=0; i!=2; hor*=-1){
							cur_b = no_of_b, cur_B = no_of_B, cur_w = no_of_w,
							cur_W = no_of_W;
							int target_row_i = row + ver;
							char target_col = col + hor + ASCII_COL;
							int target_col_i = col + hor;
							if (target_in_board(target_col, target_row_i) != TRUE &&
							target_not_empty(board, target_col_i, target_row_i) != TRUE &&
							illegal_action(board, col, row, target_col_i, target_row_i, player) != TRUE){
								if (target_row_i == FIRST_ROW){
									cur_w--;
									cur_W++;
								}
								score = cur_b + 3*cur_B - cur_w - 3*cur_W;
								tree_node moves = {score, target_col_i, target_row_i, row, col};
								insert_child(start, &moves);
							}
							i++;
						}
						j++;
					}
				}
			}
		}
	}
	return 0;
}
/* THE END -------------------------------------------------------------------*/
/* algorithms are fun :) */