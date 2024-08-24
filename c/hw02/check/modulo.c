#include <stdio.h>
#include <assert.h>

void modulo(int num1, int num2, int *result)
{
    *result = ((num1 % num2) + num2) % num2;
}

int main()
{
    int a,b,res;
    a = 3;
    b = 2;
    modulo(a, b, &res);
    printf("solusion is %d", res);
    // assert(res == 1);
    
    a = -3;
    b = 4;
    modulo(a, b, &res);
    printf("solusion is %d", res);
    // assert(res == 1);

    return 0;
}