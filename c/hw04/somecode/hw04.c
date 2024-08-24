#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 2000
int main ()
{  
    int *sequence; 
    
    printf("Input sequence:\n");

    /*storing the usr input into a temporary array*/
    char buffer[MAX]; 
    int index = 0;
    sequence = (int*)malloc(sizeof(int)*strlen(buffer));
    if( sequence == NULL ) exit(0);

    while (index < MAX && fgets(buffer, sizeof(buffer), stdin)){
        char *p=buffer;
        char *end;

        while(index < MAX && *p & *p != '\n'){/*while *p has a value exceept new line symbol*/
        index++;
        sequence[index] = strtol(p, &end, 10);
        
        p=end;

        printf("%d", sequence[index]);
    }
    //fgets(buffer,sizeof(buffer),stdin);/*fgets only working with char */
    //int size = sizeof(buffer) / sizeof(buffer[0]);/*counting the number of elements*/
    
    /*creating an array with the size of buffer */


   /*for(int i = 0; i < strlen(buffer); i++)
    {   
        int temp;
        if(isdigit(buffer[i])){
            sscanf(&buffer[i], "%d", &temp);
            sequence[i] = temp;
             printf("%d", sequence[i]);*/ 
    
        
    free(sequence);

    return 0;

}

}

