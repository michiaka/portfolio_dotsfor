#include <stdio.h>
#include <stdlib.h>
// #include <stdbool.h>


int prefixchecker(char * inputstr)
{

    char prefix;

    //we consider [space]prefix as valid
    sscanf(inputstr, " %c", &prefix);
    if(prefix == '+')
        return 1;
    else if(prefix == '?'|| prefix == '#')
        return 0;

    return -1;
}

int main()
{

    /*reading input*/
    size_t bufsize = 32;
    char *buffer = (char *)malloc(sizeof(char)* bufsize);
    
    while(getline(&buffer, &bufsize, stdin) != EOF)
    {

        int ret = prefixchecker(buffer);
        
        if(ret == 1)
        {
            printf("+ an a prefix\n"); 
        }
        else if(ret == 0) 
        {
            printf("? or # as prefix\n");
        }
        else 
        {
            printf("no prefix or different prefix\n");
        }
        //dont freebuffer here
    }

    free(buffer);

    return 1;
}
