#include <stdio.h>
#include <stdlib.h>

int n;
int *board;

int is_safe(int row, int col) {
    int i;
    for (i = 0; i < col; i++) {
        if (board[i] == row || 
            board[i] - i == row - col || 
            board[i] + i == row + col)
            return 0;
    }
    return 1;
}

void solve(int col) {
    int row;
    if (col == n) {
        for (row = 0; row < n; row++) {
            printf("%d", board[row]);
            if (row < n - 1) printf(" ");
        }
        printf("\n");
        return;
    }
    
    for (row = 0; row < n; row++) {
        if (is_safe(row, col)) {
            board[col] = row;
            solve(col + 1);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    
    n = atoi(argv[1]);
    if (n <= 0) return 1;
    
    board = malloc(n * sizeof(int));
    if (!board) return 1;
    
    solve(0);
    
    free(board);
    return 0;
}
