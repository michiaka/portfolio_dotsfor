#include <stdio.h>
#include <stdlib.h>

int main(){
 
    int n = 0;
    int status; char c;

  while((status = scanf("%c", &c)) == 1)
{
    n++;
 }
    if(status != EOF || n == 0){
        printf("Invalid input.\n");
        return EXIT_FAILURE;
    }

}

 
