#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

int sandbox(void (*f)(void), unsigned int timeout, bool verbose);

void nice_func(void)   { return; }                          // exit 0  → nice
void bad_exit(void)    { _exit(42); }                       // exit 42 → bad
void bad_signal(void)  { int *p = NULL; *p = 1; }          // segfault → bad
void bad_timeout(void) { while (1) ; }                      // loop    → bad (timeout)

int main(void)
{
    printf("nice:    %d\n", sandbox(nice_func,   2, true));
    printf("exit42:  %d\n", sandbox(bad_exit,    2, true));
    printf("segfault:%d\n", sandbox(bad_signal,  2, true));
    printf("timeout: %d\n", sandbox(bad_timeout, 2, true));
    return 0;
}
