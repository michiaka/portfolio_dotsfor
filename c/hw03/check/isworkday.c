bool isLeapyear(int y)
{
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool isValidDay(int y, int m, int d)
{
    //checking if day is not negative nor bigger than 31
    if(d > 31 || d < 0)
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
        y --;
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
        printf("this date is invalid\n");
        return false;
    }

    //checking the day of the week
    int day_week = whichDay(y, m, d);

    printf("day_week = %d\n", day_week);

    if(day_week < 3 || isHoliday(m, d))
    {
        return false;
    }
    else
        return true;
    
}
