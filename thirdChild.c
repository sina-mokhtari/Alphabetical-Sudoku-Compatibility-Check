#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    printf("3rd Child file | pid : %d\n", getpid());
    int status;
    _exit(0);
    return 0;
}
