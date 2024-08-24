#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct 
{
    int prefix;
    int rest;
    int unit;
    // char first;
    // char last;
} MG;

int converter(char symbol)
{
    int expo, num;
    if(symbol == '|')
    {
        return 0;
    }
    else
    {
        expo = symbol - 'a';
        num = pow(2, expo);
        return num;
    }
}

int readrest(char * c, int* rest_sum)
{
    rest_sum = 0;
    while(scanf("%c", c) == 1)
    {
        if(*c == '|')
        {
            return 0;
        }
        if(*c == '\n')
        {
            
            
            return 1;
        }
        rest_sum += converter(*c);
    }

    return 0;
}

//function to get the input line by line 
int ReadLine(MG* all_message, char* buffer)
{
    char c;
    int sum, num_line = 0, rest_sum = 0;
    bool separator = false;
    while(sscanf(buffer, "%c", &c) == 1)
    {
        if(c == '|')
        {   
            separator = true;
            all_message[num_line].prefix = sum;
            if(!readrest(&c, &rest_sum,&num_line))
            {
                break;
            }
            all_message[num_line].rest = rest_sum;
            all_message[num_line].unit = sum + rest_sum;
        }

         
        
        sum += converter(c);
    }
    if(!feof(stdin) || num_line < 1 || !separator ){        
        printf("Invalid input.\n");
        return 1;
    }

    return 0;
}

int ReadLineLoop(MG* all_message)
{

    char* buffer = NULL;
    size_t size = 1000;
    while(getline(&buffer, &size, stdin) != EOF)
    {
        //function to read each line
        if(!ReadLine(all_message, buffer))
            return 0;
    }
    return 1;
}


int main()
{
    MG all_message[100];
    if(!ReadLineLoop(all_message))
    {
        return 1;
    }
    printf("%d\n", all_message[0].prefix);
    return 0;

}