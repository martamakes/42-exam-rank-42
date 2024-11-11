#include <stdio.h>
#include <stdlib.h>

int *ft_range(int start, int end);

void print_range(int *range, int start, int end)
{
    int size = (start < end) ? (end - start + 1) : (start - end + 1);
    for (int i = 0; i < size; i++)
    {
        printf("%d ", range[i]);
    }
    printf("\n");
}

int main()
{
    int *range;

    range = ft_range(1, 3);
    printf("Range (1, 3): ");
    print_range(range, 1, 3);
    free(range);

    range = ft_range(-1, 2);
    printf("Range (-1, 2): ");
    print_range(range, -1, 2);
    free(range);

    range = ft_range(0, 0);
    printf("Range (0, 0): ");
    print_range(range, 0, 0);
    free(range);

    range = ft_range(0, -3);
    printf("Range (0, -3): ");
    print_range(range, 0, -3);
    free(range);

    return 0;
}