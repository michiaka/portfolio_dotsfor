#include<stdio.h>

int main()
{
    int a,b;
    if(scanf("+%c%d %d", &a, &b) != 2)
    {
        printf("invalid.\n");
    }

    printf("int a = %d int b = %d\n", a, b);

    return 0;
}