#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
//I changed my code from using scanf to getchar to acept \n 
//, increasing the numline in the readrest
//but it doesnt increment
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

int readrest(int* rest_sum, FILE* fp, int* num_line)
{
    char c = '|';
    *rest_sum = 0;
    while((c = getchar()) != EOF)
    {
        if( !(c >= 'a' && c <= 'z'))
        {  
            if(c == '|')
            {
                return 0;
            }
            else if(c == '\n')
            {
                (*num_line) ++;
                return 1;
            }
        
        }

        *rest_sum += converter(c);
        
    }
    return 1;//nothing to read anymore 
}

//function to get the input line by line 
int ReadLineLoop(MG* all_message)
{
    char c;
    int sum = 0, num_line = -1, rest_sum = 0;
    while((c = getchar()) != EOF)
    {
        
        if( !(c >= 'a' && c <= 'z'))
        {   
            
            if(c == '|')
            {   
                
                if(!readrest(&rest_sum, stdin, &num_line))
                {
                    break;//detecting another |
                }
                all_message[num_line].prefix = sum;
                printf("prefix[%d] : %d\n", num_line, sum);
                all_message[num_line].rest = rest_sum;
                printf("rest[%d] : %d\n", num_line, rest_sum);
                all_message[num_line].unit = sum + rest_sum;
                printf("sum[%d] : %d\n", num_line, sum + rest_sum);
                sum = 0;
                
            }
            else if(c == '\n')
            {
               break;
            }else
            {
                printf("Invalid input.\n");
                return 0;
            }
        }   
    
        sum += converter(c);
    }
    if(!feof(stdin))
    {
        printf("Invalid input.\n");
        return 0;
    }

    return 1;//success

}


int main()
{
    MG all_message[100];
    if(!ReadLineLoop(all_message))
    {
        return 1;
    }
    // printf("%d\n", all_message[0].prefix);
    // printf("%d\n", all_message[0].prefix);
    // printf("%d\n", all_message[0].rest);
    // printf("%d\n", all_message[1].prefix);
    // printf("%d\n", all_message[1].rest);
    // printf("%d\n", all_message[2].prefix);
    // printf("%d\n", all_message[2].rest);


    return 0;

}