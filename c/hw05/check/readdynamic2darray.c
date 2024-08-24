#include <stdio.h>
#include <stdlib.h>



int main() {
    char **outer;  // dinamically allocated array of pointers
    int outer_capa = 3;
    outer = (char **) malloc(outer_capa * sizeof(int *));

    char *inner;
    int inner_capa = 3;
    inner = (char *) malloc(inner_capa * sizeof(int));

    int outer_used = 0;  // Number of rows in the matrix
    int inner_used = 0;
    char c = 'a';
    while(scanf("%c", &c))
    {   
        //input validation 
        //should be here

        //store the pointer in outer
        *outer = inner;
        while(c  = 'a' || c == '!')
        {

            if( c == '!')
            {
                break;
            }
            
            //resize the inner array
   
            //store the value in inner array
            *inner = c; 
            //increment inner_used
            inner_used++;
        }

        //resize the outer array
        
        // //make the pointer move by innerused*sizeof(int)
        inner += inner_used * sizeof(int);
        //increment outer_used
        outer_used++;
        
    }

    //print the value of the array of the pointers
    for(int i = 0; i < outer_used; i++)
    {
        printf("%dth index of outer array\n", i);
        for(int j = 0; j < inner_used; j++)
        {
            printf("")
        }
    }

    free(outer);
    free(inner);

    return 0;
}
