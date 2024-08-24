#include <stdio.h>
#include <stdlib.h>
#include <string.h>
constexpr int MAX = 4096;

typedef struct 
{
    DATE curDate;
    // DATE preDate;
    int rating;
    char comment[MAX];
} TReview;


bool isLeapyear(int y)
{
    return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
}

bool isValidDay(DATE date)
{
    int y = date.year;
    int m = date.month;
    int d = date.day;
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

bool isValidDate(DATE date)
{
    int m = date.month;
    return (m >= 1  && m <= 12 && isValidDay(date));
}

bool isSameLaterDate(DATE predate, DATE date)
{
    int y1 = predate.year, y2 = date.year;
    int m1 = predate.month, m2 = date.month;
    int d1 = predate.day, d2 = date.day;

    if(y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && d1 > d2))
    {
        return false;
    }

    return true;   
}

bool isValidRate(int rating)
{
    if(rating < 1)
        return false;
    else
        return true;
}

bool isValidReview(TReview rev)
{
    return (isValidDate(rev.curDate) && isSameLaterDate(rev.curDate, rev.preDate)) && isValidRate(rev.rating);
}

int addReview(char *inputstr, DATE *storedDate)
{
    TReview review;
    review.preDate = *storedDate;
    int ret;
    //checking the format 1
    if((ret = sscanf(inputstr," + %d-%02d-%02d %d %s", &review.curDate.year, &review.curDate.month,
     &review.curDate.day, &review.rating, review.comment)) != 5 || !isValidReview(review))
    {
        // printf("Invalid input./n");
        return -1;
    }
    //cheking the format 2(extra character at the end)
    char extra = 0;
    if(sscanf(inputstr, "+ %*s %*s %*s %c", &extra) == 1 && extra != '\0') 
    {
        printf("there is extra characters.\n");
        return -1;
    }
    *storedDate = review.curDate;
    printf("input: + %d-%d-%d %d %s\nprevious date: %d-%d-%d\n", review.curDate.year, review.curDate.month,
     review.curDate.day, review.rating, review.comment, review.preDate.year, review.preDate.month, review.preDate.day);

    return 1;

}

void removeNL(char* str)
{
    int l = strlen(str);
    if(l > 0 || str[l-1] == '\n')
    {
        str[l - 1] = '\0';
    }
}

int main()
{
    /*reading input*/
    size_t bufsize = 32;
    char *buffer = (char *)malloc(sizeof(char)* bufsize);
    
    getline(&buffer, &bufsize, stdin);

    removeNL(buffer);
    DATE storedDate;
    storedDate.year = 0;
    storedDate.month = 0;
    storedDate.day = 0;

    if(addReview(buffer, &storedDate) != 1)
    {
        printf("Invalid input.\n");
        return -1;
    }

    printf("preDate for the next:%d-%d-%d", storedDate.year, storedDate.month, storedDate.day);
    
    free(buffer);


    return 1;

}





