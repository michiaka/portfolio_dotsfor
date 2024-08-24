#include <stdio.h>
#include <math.h>

int main(){
    char symbol;
    scanf("%c", &symbol);
    int expo, num;
    if(symbol == '|')
    {
        num = 0;
    }   
    else
    {
        expo = symbol - 'a';
        num = pow(2, expo);
    }

    printf("%d\n", num);

    return 0;
}