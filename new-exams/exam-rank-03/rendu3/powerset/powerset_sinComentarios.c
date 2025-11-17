#include <stdio.h>
#include <stdlib.h>

// Función para imprimir un subset
void print_subset(int *subset, int size)
{
    if (size == 0)
    {
        printf("\n");
        return;
    }
    
    int i = 0;
    while (i < size)
    {
        if (i == size - 1)
            printf("%d", subset[i]);
        else
            printf("%d ", subset[i]);
        i++;
    }
    printf("\n");
}

void backtrack(int *set, int *subset, int set_size, int subset_size, 
               int index, int current_sum, int target, int *found)
{
    if (index == set_size)
    {
        if (current_sum == target)
        {
            print_subset(subset, subset_size);
            *found = 1;
        }
        return;
    }
    
    backtrack(set, subset, set_size, subset_size, index + 1, 
                current_sum, target, found);
    
    subset[subset_size] = set[index];           
    backtrack(set, subset, set_size,            
                subset_size + 1,                
                index + 1,                      
                current_sum + set[index],       
                target, found);
    

}

int check_args(int argc, char **argv)
{
   int i = 1;
    while (i < argc)
    {
        int j = 0;
        
        if (argv[i][j] == '+' || argv[i][j] == '-')
            j++;
        
       
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return 0; 
            j++;
        }
        i++;
    }
    return 1; 
}

int main(int argc, char **argv)
{
    if (argc == 1 || (argc >= 2 && argv[1][0] == '0' && argv[1][1] == '\0'))
    {
        printf("\n");
        return 0;
    }

    if (!check_args(argc, argv))
        return 1;

    int target = atoi(argv[1]);
    int set_size = argc - 2;  
    
    if (set_size <= 0)
        return 1;

 
    int *set = malloc(set_size * sizeof(int));
    int *subset = malloc(set_size * sizeof(int));
    
    if (!set || !subset)
    {
        free(set);
        free(subset);
        return 1;
    }

   
    int i = 0;
    int j = 2;
    while (j < argc)
    {
        set[i] = atoi(argv[j]);
        i++;
        j++;
    }

    int found = 0;
    
    backtrack(set, subset, set_size, 0, 0, 0, target, &found);
    

    if (!found)
        printf("\n");

    free(set);
    free(subset);
    return 0;
}

