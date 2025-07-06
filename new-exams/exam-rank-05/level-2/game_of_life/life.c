#include "life.h"

//Create a function to create a board

//Check the arguments and if its correct, store them as size_t value
//1.1) check the iteration, if its >= to the value, stop the game.

//2) create a function, clone the board only by its width and height. Make it empty board.

// --Working on getting new cells--
//3) figure out the new location of the new generation of cells, store their locations.
//4) figure out the next iteration where the cells are gonna be ded. Store their locations.

//5) free the old board and replace it with the new one
//Repeat


/*
pen no move outside board and stays still if a command ask for it.
pen do nothing in case of invalid command.
*/
void	print_board(char **new_board, int height)
{
	int		array = 0;
	while (array < height)
	{
		printf("%s\n", new_board[array]);
		array++;
	}
}

void free_board(char **board, int height)
{
    for (int a = 0; a < height; a++)
        free(board[a]);
    free(board);
}

char**	living_cells_start(char **new_board, int width, int height)
{
	char	c;
	int		index = 0;
	int		array = 0;
	bool	start_drawing = false;

	while (read(STDIN_FILENO, &c, 1) > 0)
	{
		// Skip newline characters
		if (c == '\n')
			continue;
		if (new_board[array] == NULL)
			break;

		if (c == 'x')
			start_drawing = !start_drawing;
		else if (c == 'w' && array > 0)
				array--;
		else if (c == 'a' && index > 0)
				index--;
		else if (c == 's' && (array + 1 < height))
				array++;
		else if (c == 'd' && (index + 1 < width))
				index++;
		if (start_drawing == true && new_board[array] && index < width)
			new_board[array][index] = '0';
	}
	return (new_board);
}

char**	board_creation(int width, int height)
{
	char	**new_board;
	int		index;
	int		array;

	new_board = (char **)calloc(height + 1, sizeof(char *));//allocate size for each array
	array = 0;

	while (array < height)
	{
		//allocate size for each index inside array
		new_board[array] = (char *)calloc(width + 1, sizeof(char));// +1 for '\0' if needed
		index = 0;
		while (index < width)
		{
			new_board[array][index] = ' ';
			index++;
		}
		new_board[array][index] = '\0';
		array++;
    }
	return (new_board);
	//or assign new function to find living cells
}

char**	next_gen(char** new_board, int width, int height)
{
	int		row = 0;
	int		col = 0;
	int		cell_counter = 0;
	int		left = 0;
	int		right = 0;
	char	**empty_board = board_creation(width, height);

	while (row < height)
	{
		col = 0;
		while (col < width)
		{
			cell_counter = 0;
			if (new_board[row][col] == ' ')
			{
				//check 8 neighbours
				left = col - 1;
				right = col + 1;
				if (left < 0)
					left = 0;
				if (right > width)
					right = height;
				if ((row - 1) >= 0)
				{
					//check the top left, mid and right side
					if (new_board[row - 1][col] == '0')
						cell_counter++;
					if (left != col && new_board[row - 1][left] == '0')
						cell_counter++;
					if (right != col && new_board[row - 1][right] == '0')
						cell_counter++;
				}
				if ((row + 1) < height)
				{
					if (new_board[row + 1][col] == '0')
						cell_counter++;
					if (left != col && new_board[row + 1][left] == '0')
						cell_counter++;
					if (right != col && new_board[row + 1][right] == '0')
						cell_counter++;
				}
				if (left != col && new_board[row][left] == '0')
					cell_counter++;
				if (right != col && new_board[row][right] == '0')
					cell_counter++;
			}
			if (cell_counter == 3)
			{
				//printf("[CELL COUNTER]--> %i\n", cell_counter);
				empty_board[row][col] = 'n';
			}
			col++;
		}
		row++;
	}
	//print_board(empty_board, height);
	return (empty_board);
}

char**	liveboard(char** new_board, int width, int height)
{
	int		row = 0;
	int		col = 0;
	int		cell_counter = 0;
	int		left = 0;
	int		right = 0;
	char	**empty_board = board_creation(width, height);

	while (row < height)
	{
		col = 0;
		while (col < width)
		{
			cell_counter = 0;
			if (new_board[row][col] == '0')
			{
				//check 8 neighbours
				left = col - 1;
				right = col + 1;
				if (left < 0)
					left = 0;
				if (right > width)
					right = height;
				if ((row - 1) >= 0)
				{
					//check the top left, mid and right side
					if (new_board[row - 1][col] == '0')
						cell_counter++;
					if (left != col && new_board[row - 1][left] == '0')
						cell_counter++;
					if (right != col && new_board[row - 1][right] == '0')
						cell_counter++;
				}
				if ((row + 1) < height)
				{
					if (new_board[row + 1][col] == '0')
						cell_counter++;
					if (left != col && new_board[row + 1][left] == '0')
						cell_counter++;
					if (right != col && new_board[row + 1][right] == '0')
						cell_counter++;
				}
				if (left != col && new_board[row][left] == '0')
					cell_counter++;
				if (right != col && new_board[row][right] == '0')
					cell_counter++;
			}
			if (cell_counter == 2 || cell_counter == 3)
			{
				//printf("[CELL COUNTER]--> %i\n", cell_counter);
				empty_board[row][col] = 'c';
			}
			col++;
		}
		row++;
	}
	//print_board(empty_board, height);
	return (empty_board);
}

char**	combineboard(char **nextgen_board, char **livecell_board, int width, int height)
{
	int		array = 0;
	int		index = 0;
	char	**updated_board = board_creation(width, height);

	while (array < height)
	{
		index = 0;
		while (index < width)
		{
			if (nextgen_board[array][index] == 'n' || livecell_board[array][index] == 'c')
				updated_board[array][index] = '0';
			index++;
		}
		array++;
	}
	return (updated_board);
}

char**	living_cells_continue(char** new_board, int width, int height)
{
	char	**nextgen_board;
	char	**livecell_board;
	char	**updated_board;

	//check the cells, potential new generation cells marked with 'n'.
	nextgen_board = next_gen(new_board, width, height);

	//check which cells are gonna live to next gen, marked in 'c'
	livecell_board = liveboard(new_board, width, height);

	//clear the board and replace those with 'c' with '0'
	updated_board = combineboard(nextgen_board, livecell_board, width, height);
	
	//update board by adding 'n' and replace them as 0


	free_board(nextgen_board, height);
	free_board(livecell_board, height);
	free_board(new_board, height);
	return (updated_board);
}



void	game_of_life_start(char	**argv)
{
	int		width = 0;
	int		height = 0;
	int		max_iterations = 0;
	char	**empty_board;
	char	**board;

	width = atoi(argv[1]);
	height = atoi(argv[2]);
	max_iterations = atoi(argv[3]);

	empty_board = board_creation(width, height);
	board = living_cells_start(empty_board, width, height);
	if (!board)
		return ;

	while (max_iterations--)
		board = living_cells_continue(board, width, height);

	print_board(board, height);
	free_board(board, height);
}