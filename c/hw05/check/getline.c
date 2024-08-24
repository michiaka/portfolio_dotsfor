#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct 
{
    int day;
    int year;
    char symbol;
} DATA;

void removeNL(char* str)
{
    //remove the trailing NL
    int l = strlen(str);
    //if it is not empty string, and the last one is \n
    if( l > 0 && str[l-1] == '\n')
    {
        str[l-1] = 0;
    }
}
void anothersscanf(char * inputstr)
{
    char c;
    int l = strlen(inputstr);
    if(l > 0)
    {
        sscanf(inputstr, "%c", &c);
    }

}
int main()
{
    DATA dataArr[3];
    char *buffer;
    size_t bufsize = 32; 
    // size_t characters;
    //dynamically allocate the buffer string
    buffer = (char *)malloc(sizeof(char) * bufsize);

    printf("Enter some strings:\n");
    //getline will automatically realloc buffer
    int cnt = 0;

    while(getline(&buffer, &bufsize, stdin) != EOF)
    {
        // printf("%s", buffer);
        removeNL(buffer);
        // printf("%s", buffer);
        anothersscanf(buffer);
        printf("input after scanf:%s\n", buffer);

        sscanf(buffer, "%d  %d %c", &dataArr[cnt].day, &dataArr[cnt].year, &dataArr[cnt].symbol);
        cnt ++;
    }

    for(int i = 0; i < 3; i ++)
    {
       printf("dataArr[%d] = %d %d %c\n", i, dataArr[i].day, dataArr[i].year, dataArr[i].symbol);
    }
    //Dont forget to memory for buffer
    free(buffer);
    return 0;
}   


