#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct
{
  int m_TotalDays;
  int m_WorkDays;
} TResult;
#endif /* __PROGTEST__ */

bool isLeapyear(int y)
{
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool isValidDay(int y, int m, int d)
{
    //checking if day is not negative nor bigger than 31
    if(d > 31 || d < 1)
    {
        return false;
    }
    //checking if day is contained in the month
    if(m == 4 || m == 6 || m == 9 || m == 11)
    {
        if(d > 30)
            return false;
    }
    //for feburuary, check if it is leap year
    else if(m == 2)
    {
        if(isLeapyear(y))
        {
            if(d > 29)
                return false;
        }
        else
        {
            if(d > 28)
                return false;
        }
    }
    
    return true;
}

bool isValidDate(int y, int m, int d)
{
    return (y >= 2000 && m >= 1  && m <= 12 && isValidDay(y, m, d));
}

int whichDay(int y, int m, int d)
{
    if(m < 3)
    {  
        y--;
        m += 12;
    }
    int k = y % 100;
    int j = y / 100;

    int day_Week = (d + 13*(m + 1)/5 + k + k/4 + j/4 + 5*j) % 7;
    
    return day_Week;
}

bool isHoliday(int m, int d)
{
    return ((m == 1 && d == 1) || (m == 5 && d == 1) || (m == 5 && d == 8) ||
                (m == 7 && d == 5) || (m == 7 && d == 6) || (m == 9 && d == 28) ||
                    (m == 10 && d == 28) || (m == 11 && d == 17) || (m == 12 && d == 24) ||
                        (m == 12 && d == 25) || (m == 12 && d == 26));
}

bool isWorkDay ( int y, int m, int d )
{
    if(!isValidDate(y, m, d))
    {
        // printf("this date is invalid\n");
return false;
    }

    //checking the day of the week
    int day_week = whichDay(y, m, d);

    // printf("day_week = %d\n", day_week);

    if(day_week < 2 || isHoliday(m, d))
    {
        // printf("%d/%d/%d is not workday for being holiday.\n", y, m, d);
        return false;
    }
    else
        return true;
    
}

//function for checking which year in interval covers march 1

//function for checking if the year containing march 1 is leap year
//-> arg : the array of the year contaiing march 1 to avoid the numebr of time this funciton is called
//-> variable : num_loopyear(counted by increment) 

TResult countDays ( int y1, int m1, int d1,
                    int y2, int m2, int d2 )
{
    //initializing the struct
    TResult solution;
    solution.m_TotalDays = 0;
    solution.m_WorkDays = 0;
    //check if the parameters are invalid
    if(!isValidDate(y1, m1, d1) || !isValidDate(y2, m2, d2) || 
            y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && d1 > d2))
    {
        solution.m_TotalDays = -1;
        solution.m_WorkDays = -1;

        return solution;
    }
    //iterate the interval
    //the last iteration 
    //when y1 == y2, m1 == m2, d1 == d2
    //the stop condition
    // (y1 == y2 && m1 == m2 && d1 > d2) || (y1 == y2 && m1 > m2 && d2 < d1) || (y1 > y2 && m1 < m2 && d1 < d2)
    //nagation of the stop condition

    int num_wday = 0, num_day = 0;
    for(; y1 <= y2 ; y1 ++)
    {
        for(; m1 <= 12 ; m1 ++)
        { 
            for(; d1 <= 31; d1 ++)
            {
                if(y1 == y2 && m1 == m2 && d1 == d2)
                {  
                    if(isWorkDay(y1, m1, d1)){ num_wday++; }
                    if(isValidDate(y1, m1, d1)){ num_day++; }
                    break; 
                }
                if(isWorkDay(y1, m1, d1)){ num_wday++; }
                if(isValidDate(y1, m1, d1)){ num_day++; }
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
    
    // printf("this is the number of the working days %d\n", num_wday);
    // printf("this is the number of the working days %d\n", num_day);

    solution.m_TotalDays = num_day;
    solution.m_WorkDays = num_wday;

    return solution;
}

#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
  TResult r;

  assert ( isWorkDay ( 2023, 10, 10 ) );

  assert ( ! isWorkDay ( 2023, 11, 11 ) );

  assert ( ! isWorkDay ( 2023, 11, 17 ) );

  assert ( ! isWorkDay ( 2023, 11, 31 ) );

  assert ( ! isWorkDay ( 2023,  2, 29 ) );

  assert ( ! isWorkDay ( 2004,  2, 29 ) );

  assert ( isWorkDay ( 2008,  2, 29 ) );

  assert ( ! isWorkDay ( 2001,  2, 29 ) );

  assert ( ! isWorkDay ( 1996,  1,  2 ) );

//-----------------------------------------


 
 //------------------------------------------


  r = countDays ( 2023, 11,  1,
                  2023, 11, 30 );
  assert ( r . m_TotalDays == 30 );
  assert ( r . m_WorkDays == 21 );

  r = countDays ( 2023, 11,  1,
                  2023, 11, 17 );
  assert ( r . m_TotalDays == 17 );
  assert ( r . m_WorkDays == 12 );

  r = countDays ( 2023, 11,  1,
                  2023, 11,  1 );
  assert ( r . m_TotalDays == 1 );
  assert ( r . m_WorkDays == 1 );

  r = countDays ( 2023, 11, 17,
                  2023, 11, 17 );
  assert ( r . m_TotalDays == 1 );
  assert ( r . m_WorkDays == 0 );

  r = countDays ( 2023,  1,  1,
                  2023, 12, 31 );
  assert ( r . m_TotalDays == 365 );
  assert ( r . m_WorkDays == 252 );

  r = countDays ( 2024,  1,  1,
                  2024, 12, 31 );
  assert ( r . m_TotalDays == 366 );
  assert ( r . m_WorkDays == 254 );

  r = countDays ( 2000,  1,  1,
                  2023, 12, 31 );
  assert ( r . m_TotalDays == 8766 );
  assert ( r . m_WorkDays == 6072 );

  r = countDays ( 2001,  2,  3,
                  2023,  7, 18 );
  assert ( r . m_TotalDays == 8201 );
  assert ( r . m_WorkDays == 5682 );

  r = countDays ( 2021,  3, 31,
                  2023, 11, 12 );
  assert ( r . m_TotalDays == 957 );
  assert ( r . m_WorkDays == 666 );

  r = countDays ( 2001,  1,  1,
                  2000,  1,  1 );
  assert ( r . m_TotalDays == -1 );
  assert ( r . m_WorkDays == -1 );

  r = countDays ( 2001,  1,  1,
                  2023,  2, 29 );
  assert ( r . m_TotalDays == -1 );
  assert ( r . m_WorkDays == -1 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
