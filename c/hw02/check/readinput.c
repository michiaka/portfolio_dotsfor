#include <stdio.h>
#include <math.h>
#include <string.h>

typedef struct 
{
    int prefix;
    // int rest;
    // int unit;
    char first;
    char last;
} MG;

//infinite loop since while loop keep reading the same charavter
int Readinput(MG* message)
{
    char* buffer = NULL;
    size_t size = 100;
    char symbol;
    int index = 0, status, sum = 0;
    while (getline(&buffer, &size, stdin) != -1)
    {   
        const int len = strlen(buffer);
        message[index].last = buffer[len -1];

        int i = 0;
        do 
        {     
            status = sscanf(buffer,"%c", &symbol);
            printf("%c", symbol);
            if(i == 0)
            {
                message[index].first = symbol;
            }
            // if(symbol == '|')
            // {
            //   c  break;
            // }
            if(status != 1 || !((symbol >= 'a' && symbol <= 'z') || symbol == '|') )
            { //means empty or non-alphabetic nor | 
                printf("Invalid input.\n");
                free(buffer);
                return 1;
            }
            sum += converter(symbol);
            i ++;
            buffer ++;
        }while(symbol != '|');// take sscanf inside of the while loop bracket and when it gets out of the loop not satisfying 
        // a-z, then it checks if the input is | if so, it is okay, but if not then it is invalid
        //storing the sum of prefix
        message[index].prefix = sum;
        //reading after |

        index ++;
    }

    free(buffer);

    return 0;
}


int main(){

    //array of message
    MG message[10];
    
    
    Readinput(message);



    

    return 0;
}