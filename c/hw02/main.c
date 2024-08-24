#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct 
{
    long int prefix;
    long int rest;
    long int unit;

} MG;

// long int modulo_arr[100];

long int converter(char symbol)
{
    long int expo, num;
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

int readrest(long int* rest_sum, FILE* fp, long int* num_line)
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
            else
                return 0;
        
        }

        *rest_sum += converter(c);
        
    }
    return 1;//nothing to read anymore 
}

//function to get the input line by line 
int ReadLineLoop(MG* all_message, long int* num_mg)
{
    char c;
    long int sum = 0, num_line = -1, rest_sum = 0;
    while((c = getchar()) != EOF)
    {
        
        if( !(c >= 'a' && c <= 'z'))
        {   
            
            if(c == '|')
            {   
                
                if(!readrest(&rest_sum, stdin, &num_line))
                {
                    break;//detecting non accepted value
                }
                all_message[num_line].prefix = sum;
                // printf("prefix[%d] : %d\n", num_line, sum);
                all_message[num_line].rest = rest_sum;
                // printf("rest[%d] : %d\n", num_line, rest_sum);
                if(sum + rest_sum < 1)
                {
                    printf("Invalid input.\n");
                    return 0;
                }
                all_message[num_line].unit = sum + rest_sum;
                // printf("sum[%d] : %d\n", num_line, sum + rest_sum);
                sum = 0;
                
            }
            else if(c == '\n')
            {
               break;
            }
            else
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
    *num_mg = num_line + 1;
    return 1;//success

}


//0) check if it is only one
int checkmessage(long int num_mg)//returns 1 when it is only 2 successfully read mg
{
    if(num_mg < 2)
        return 0;

    return 1;
}
//-> arg: array of MG
//separate into some functions (with steps)

// solve gcd(unit i unit j)
long int gcdcaluculator(long int a, long int b)//where a = unit1, b = unit2
{
    if(b == 0)
        return a;
    else
        return gcdcaluculator(b, a % b);
}
//-> needs the c in the congruence
long int solveC(long int prefix1, long int prefix2)
{
    long int c = prefix2 - prefix1;
    // printf("prefix1: %d\n", c);

    return c;
}
//1-a) check if it has solution(= checkifsolvavble)
long int single_checkifsolvavble(long int c, long int gcd)
{
    if(c % gcd != 0)
        return 0;
    else
        return 1;
}

//2) if it has then find bezouz coeffient by eea
long int extendedGCD(long int a, long int b, long int *bez1, long int *bez2) {
    long int x1 = 1, y1 = 0; // Initialize x1 and y1 as the initial values

    if (a == 0) {
        *bez1 = 0;
        *bez2 = 1;
        return b;
    }

    int x2 = 0, y2 = 1; // Initialize x2 and y2 as the initial values of the next iteration

    while (a > 0) {
        long int q = b / a;

        long int temp = a;
        a = b % a;
        b = temp;

        temp = x1;
        x1 = x2 - q * x1;
        x2 = temp;

        temp = y1;
        y1 = y2 - q * y1;
        y2 = temp;
    }

    *bez1 = x2;
    *bez2 = y2;

    return b;
}

//3) multipy bezouz coeffience for message[0].unit by prefixj - prefixi/gcd (unit i unit j)
void solution(long int* num_loop, long int c, long int bez1, long int gcd)
{
    *num_loop = bez1 * (c / gcd); 

}
/*
 * validating solution by using modulo
 * @return solution in long int
 */
int validsolution(long int* num_loop, long int moduli)
{
    // while(!( 0 <= *num_loop && *num_loop < moduli))
    // {
    //     if( 0 > *num_loop)
    //     {
    //         *num_loop += moduli;
    //     }
    //     else
    //     {
    //         *num_loop -= moduli;
    //     }
    // }
    //if positive 
    *num_loop = ((*num_loop % moduli) + moduli) % moduli;
    return 1;
}

long int findsolution(MG* all_message, long int unit1, long int unit2, long int prefix1, long int rest, long int c, long int depth, long int num_mg, long int* x)
{
        long int i = depth + 1;
        long int bez1 = 0, bez2 = 0;
        long int gcd = gcdcaluculator(unit1, unit2);
        
        if(!single_checkifsolvavble(c, gcd))
        {
            return 0;
        }

        extendedGCD(unit1, unit2, &bez1, &bez2);

        long int num_loop;
        solution(&num_loop, c, bez1, gcd);
        long int moduli = unit2 / gcd;
        validsolution(&num_loop, moduli);
        *x = prefix1 + unit1*num_loop;
        *x = *x % (unit1*unit2 / gcd);

        // if(rest == 0 && all_message[1].rest == 0)
        // {
        //     *x = 0;
        // }

        while(num_mg > i)
        {
            // findsolution(all_message, length, all_message[i].unit, all_message[i].prefix, 0, 0, depth + 1, num_mg, x);
            if(*x == 0)
            {
                unit1 = unit1*unit2 / gcd;
            }
            else
            {
                unit1 = *x;
            }
            unit2 = all_message[i].unit;
            // prefix1 = all_message[i].prefix;
            rest = 0;
            c = 0,
            bez1 = 0, bez2 = 0;
            gcd = gcdcaluculator(unit1, unit2);                        
            
            if(!single_checkifsolvavble(c, gcd))
            {
                return 0;
            }

            extendedGCD(unit1, unit2, &bez1, &bez2);
            solution(&num_loop, c, bez1, gcd);
            moduli = unit2 / gcd;
            
            validsolution(&num_loop, moduli);
            if(num_loop == 0)
            {
                num_loop = 1;
            }
            // printf("numloop%ld\n", num_loop);
            *x = unit1*num_loop;  
            // printf("gcd:%ld, unit1*unit2:%ld\n", gcd, unit1*unit2);
            
            // *x = *x % (unit1*unit2 / gcd);
            i ++;
            // depth ++;
        }

        return 1;

}



int main()
{
    MG all_message[10000];
    long int num_mg = 0;
    
    printf("Messages:\n");
    if(!ReadLineLoop(all_message, &num_mg))
    {
        return 0;
    }

    if(!checkmessage(num_mg))
    {
        printf("Invalid input.\n");
        return 0;
    }

    long int unit1 = all_message[0].unit, 
        unit2 = all_message[1].unit,
        prefix1 = all_message[0].prefix,
        rest = all_message[0].rest,
        c = solveC(all_message[0].prefix, all_message[1].prefix),
        depth = 1;

    long int x = 0;

    if(!findsolution(all_message, unit1, unit2, prefix1, rest,c, depth, num_mg, &x))
    {
       printf("Never synchronizes.\n");
       return 0;
    }

    int restzero = 0;
    for (long int  i = 0; i < num_mg; i++)
    {
        if(all_message[i].rest != 0)
            restzero = 1;
    }

    if(restzero == 0)
    {
        printf("Synchronized in: 0\n");
    }
    else
    {
        printf("Synchronized in: %ld\n", x);
    }
    return 0;    
}