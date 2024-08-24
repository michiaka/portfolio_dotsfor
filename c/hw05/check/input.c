#include <stdio.h>
#include <stdlib.h>
#include <stbool.h>
constexpr int MAX = 4096;

typedef struct
{
    int year;
    int month;
    int day;
} DATE;

typedef struct 
{
    DATE curDate;
    DATE preDate;
    int rating;
    char comment[MAX];
} TReview;

typedef struct
{
    TReview *data;
    size_t capa;
    size_t length;
} InnerArr;

typedef struct
{
    InnerArr* data; 
    size_t capa;
    size_t length;
} OuterArr;



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

bool isLaterDate(DATE predate, DATE date)
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
    return (isValidDate(rev.curDate) && isLaterDate(rev.curDate, rev.preDate)) && isValidRate(rev.rating);
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
    // printf("input: + %d-%d-%d %d %s\nprevious date: %d-%d-%d\n", review.curDate.year, review.curDate.month,
    //  review.curDate.day, review.rating, review.comment, review.preDate.year, review.preDate.month, review.preDate.day);

    //add into a dynamically allocated array

    

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


int readinput()
{
    /*allocate memory for input*/
    OuterArr dates;
    dates.length = 0
    dates.capa = 1;
    //malloc will return the pointer which points to where the memory allocated by mallocs starts at
    dates.data = (InnerArr *)malloc(sizeof(InnerArr) * dates.capa);
    //allocate memory for each element of the OuterArr
    int inner_capa = 1;
    for (int i = 0; i < length; i++)
    {
        //first 1 index for inner
        dates.data[i].data = (Review *)malloc(sizeof(Review) * inner_capa);
    }

    int cur_dates = 0, cur_reviews = 0;
    /*reading input*/
    char *buffer = (char *)malloc(sizeof(char)* bufsize);
    size_t bufsize = 32;
    //initializing the preDate
    DATE storeddate;
    storeddate.year = 0;
    storeddate.month = 0;
    storeddate.day = 0;

    
    while(getline(&buffer, &bufsize, stdin) != EOF)
    {
        removeNL(buffer);
        //do the different operation for each 
        int ret = prefixchecker(buffer);
        
        if(ret == 1)
        {
            if(addReview(buffer, &cur_dates, &cur_reviews, &storeddate) != 1)
            {
                free(buffer);
                return -1;
            }
        }
        else if(ret == 0)
        {
            if(isValidSearch(buffer) != 1)
            {
                free(buffer);
                return -1;
            }
            search(buffer);
        }        
        else
        {
            printf("no prefix or different prefix\n");
            free(buffer);
            return -1;
        }
    }

    free(buffer);


    return 1;
}

int main()
{
 
    if(readinput() != 1)
    {
        //program stops because of invalid input
        return -1;
    }


}