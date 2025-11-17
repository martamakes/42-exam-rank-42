#include "permutations.h"

void perm (int *cnt, int n, int pos, char *buf)
{
    if (pos == n)
    {
        buf[n] = '\0';
        puts(buf);
        return;
    }

    for (int i = 0; i < 256; i++)
    {
        if (cnt[i])
        {
            buf[pos] = i;
            cnt[i]--;
            perm(cnt, n, pos + 1, buf);
            cnt[i]++;
        }
    }
}

int main (int ac, char **av)
{
    int cnt [256] = {0};
    int n = 0;

    if (ac != 2 || !av[1][0])
        return 1;
    
    while (av[1][n])
        cnt[(unsigned char)av[1][n++]]++;
    
    char *buf = malloc(n + 1);
    if (!buf)
        return 1;

    perm (cnt, n, 0, buf);
    free (buf);
    return 0;
}
