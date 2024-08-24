#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

int main()
{
    int y1, m1, d1,
                     y2,  m2,  d2 ;
    printf("Enter with this format y1/m1/d1, y2/m2/d1:\n");
    scanf("%d/%d/%d, %d/%d/%d",&y1,&m1,&d1,&y2,&m2,&d2);
//condition where you wanted to execute on 
    for(; y1 <= y2 ; y1 ++)
    {
        for(; m1 <= 12 ; m1 ++)
        { 
            for(; d1 <= 31; d1 ++)
            {
                if(y1 == y2 && m1 == m2 && d1 == d2)
                {  
                    printf("current day: %d/%d/%d\n", y1, m1, d1);
                    break; 
                }
                printf("current day: %d/%d/%d\n", y1, m1, d1);
            }
            if(y1 == y2 && m1 == m2 && d1 == d2)
            { 
                break; 
            }
            d1 = 1;
        }
        if(y1 == y2 && m1 == m2 && d1 == d2)
        { 
            break; 
        }
        m1 = 1;
    }

    return 0;
}