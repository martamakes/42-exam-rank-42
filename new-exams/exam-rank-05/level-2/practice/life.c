#include "life.h"

void	free_board(char **board, int height)
{
	int		array = 0;
	while (array < height)
		free(board[array++]);
	free(board);
}

void	print_board(char **board, int width, int height)
{
	int		array = 0;
	int		index = 0;
	while (array < height)
	{
		index = 0;
		while (index < width)
			putchar(board[array][index++]);
		putchar('\n');
		array++;
	}
}

char	**board_creation(int width, int height)
{
	char	**empty_board;
	int		index = 0;
	int		array = 0;

	empty_board = (char **)calloc(height + 1, sizeof(char *));
	while (array < height)
	{
		empty_board[array] = (char *)calloc(width + 1, sizeof(char));
		index = 0;
		while (index < width)
			empty_board[array][index++] = ' ';
		empty_board[array][index] = '\0';
		array++;
	}
	return	(empty_board);
}

char	**add_livingcells(int width, int height)
{
	char	buf;
	int		index = 0;
	int		array = 0;
	bool	drawing_status = false;
	char	**empty_board = board_creation(width, height);

	//use read function and store the value we got into buf, thus '&'
	//	return (> 0) -->Number of bytes successfully read
	//	return (0)   -->End of File (EOF) was reached.
	//	return (-1)  -->An error occurred(errno)
	while (read(0, &buf, 1) > 0)
	{
		if (buf == 'x')
			drawing_status = !drawing_status;
		else if (buf == 'w' && (array - 1 >= 0))
			array--;
		else if (buf == 'a' && (index - 1 >= 0))
			index --;
		else if (buf == 's' && (array + 1 < height))
			array++;
		else if (buf == 'd' && (index + 1 < width))
			index++;

		if (drawing_status == true)
			empty_board[array][index] = '0';
	}
	return (empty_board);
}

char	**update_board(char **new_board, int width, int height)
{
	char	**updated_board = board_creation(width, height);
	int		index = 0;
	int		array = 0;
	int		left = 0;
	int		right = 0;
	int		cell_counter = 0;

	while (array < height)
	{
		index = 0;
		while (index < width)
		{
			left = index - 1;
			if (left < 0)
				left = 0;
			right = index + 1;
			if (right >= width)
			{
				right = width - 1;
				if (width < 0)
					right++;
			}

			cell_counter = 0;
			if (array - 1 >= 0)
			{
				if (left != index && new_board[array - 1][left] == '0')
					cell_counter++;
				if (new_board[array - 1][index] == '0')
					cell_counter++;
				if (right != index && new_board[array - 1][right] == '0')
					cell_counter++;
			}
			if (array + 1 <= height - 1)
			{
				if (left != index && new_board[array + 1][left] == '0')
					cell_counter++;
				if (new_board[array + 1][index] == '0')
					cell_counter++;
				if (right != index && new_board[array + 1][right] == '0')
					cell_counter++;
			}
			if (left != index && new_board[array][left] == '0')
					cell_counter++;
			if (right != index && new_board[array][right] == '0')
					cell_counter++;
			if (((cell_counter == 2 || cell_counter == 3) && new_board[array][index] == '0') ||
					(cell_counter == 3 && new_board[array][index] == ' '))
				updated_board[array][index] = '0';
			index++;
		}
		array++;
	}
	free_board(new_board, height);
	return (updated_board);
}

void	game_of_life(int width, int height, int iteration)
{
	char	**empty_board;
	char	**new_board;

	new_board = add_livingcells(width, height);
	while (iteration--)
		new_board = update_board(new_board, width, height);

	print_board(new_board, width, height);
	free_board(new_board, height);
}
