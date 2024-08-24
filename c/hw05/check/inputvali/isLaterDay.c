#include <stdio.h>
#include <stdbool.h>
constexpr int MAX = 4096;

typedef struct
{
    int year;
    int month;
    int day;
} DATE;

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
    return (m >= 1  && m <= 12 && isValidDay(y, m, d));
}

int isLaterDate(DATE predate, DATE date)
{
    int y1 = predate.year, y2 = date.year;
    int m1 = predate.month, m2 = date.month;
    int d1 = predate.day, d2 = date.day;

    if(y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && d1 > d2))
    {
        return -1;
    }
    
    return 1;
}

int main()
{   
    DATE date, predate;               
    predate.year = 2002;
    predate.month = 1;
    predate.day = 8;


    printf("Enter the day:\n");
    scanf("%d %d %d", &date.year, &date.month, &date.day);

    if(isLaterDate(predate, date) != 1) 
    {
        printf("THE INPUT DATE IS NOT EQUAL OR LATER THAN PREVIOUS\n");
        
    }
    
    if(!isValidDate(date.year, date.month, date.day))
    {
        printf("this is not a valid date.\n");

    }


    return 0;
}