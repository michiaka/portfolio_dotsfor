#include <stdio.h>
#include <string.h>



int main(){
    char input[100];
    printf("Enter:\n");
    int status = 0, i = 0;
    while( (("%c", &input[i])) != 1 ){
        i++;
    }
    int size = strlen(input);
    printf("%d", size);
    // for(i = 0; i < size; i++){
    //     printf("%c ", input[i]);
    // }
    //if it is not the end of the string and stil have something in the buffer then,
    // there were some inputs which couldnt be taken wiht %c
    if(status != EOF){
        printf("invalid\n");
    }

}
