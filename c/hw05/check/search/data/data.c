#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

constexpr int MAX = 4097;

typedef struct
{
    int year;
    int month;
    int day;
} DATE;

typedef struct 
{
    DATE curDate;
    // DATE preDate;
    int rating;
    char comment[MAX];
} TReview;

typedef struct
{
    TReview *data;
    int capa;
    int length;
} InnerArr;

typedef struct
{
    InnerArr* data; 
    int capa;
    int length;
} OuterArr;



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

bool isValidReview(TReview rev, DATE storedDate)
{
    if(!isValidDate(rev.curDate))
    {
        printf("1\n");
    }
    if(!isValidRate(rev.rating))
    {
        printf("2\n");
    }
    if(!isSameLaterDate(rev.curDate, storedDate))
    {
        printf("3\n");
    }
    return (isValidDate(rev.curDate) && isSameLaterDate(storedDate,rev.curDate)) && isValidRate(rev.rating);
}

bool isLaterDate(DATE predate, DATE date)
{
    int y1 = predate.year, y2 = date.year;
    int m1 = predate.month, m2 = date.month;
    int d1 = predate.day, d2 = date.day;

    if(y1 > y2 || (y1 == y2 && m1 > m2) || (y1 == y2 && m1 == m2 && d1 >= d2))
    {
        return false;
    }

    return true;   
}

bool addReview(char *inputstr, OuterArr *dates, DATE *storedDate, int *i, int *j)
{
    TReview review;
    int ret;
    //checking the format 1, considering the nullterminator 
    if((ret = sscanf(inputstr," + %d-%02d-%02d %d %4097s", &review.curDate.year, &review.curDate.month,
     &review.curDate.day, &review.rating, review.comment)) != 5|| !isValidReview(review, *storedDate))
    {
        printf("inputstr %s\n", inputstr);
        printf("return value %d.Invalid input.\n", ret);
        return false;
    }
    //cheking the format 2(extra character at the end)
    char extra = 0;
    if(sscanf(inputstr, "+ %*s %*s %*s %c", &extra) == 1 && extra != '\0') 
    {
        printf("there is extra characters.\n");
        return false;
    }
    //add into a dynamically allocated array

    if(isLaterDate(*storedDate,review.curDate))
    {
        printf("it is later than the previous day.\n");
        (*i) ++;
        dates->length = *i + 1;
        *j = 0;
    }

    if(*j >= (dates->data[*i]).capa)
    {
        (dates->data[*i]).capa += 10;
        dates->data[*i].data = (TReview *)realloc(dates->data[*i].data, sizeof(TReview) * (dates->data[*i].capa));

    }
    if(*i >= dates->capa)
    {
        printf("reallocation for outer array will happen\n");
        dates->capa += 10;
        printf("capacity of outer is %d", dates->capa);
        dates->data = (InnerArr *)realloc(dates->data, sizeof(InnerArr) * dates->capa);
        if(dates->data == NULL)
        {
            printf("reallocation for outer array failed.\n");
            return false;
        }
        //allocating memory for inner array inside of the new created elements of out
        for (int j = *i; j < dates->capa; j++)
        {
            dates->data[*i].capa = 1;
            //first 1 index for inner
            dates->data[j].data = (TReview *)malloc(sizeof(TReview) * dates->data[*i].capa);
            if(dates->data[j].data == NULL)
            {
                printf("memory for inner array is not initialized.\n");
            }       
        }
    }
    printf("value of j %d\n", *j);
    printf("value of i %d\n", *i);

    (dates->data[*i]).data[*j] = review;
    (*j)++;
    printf("this is j: %d\n", *j);
    dates->data[*i].length = *j;
    *storedDate = review.curDate;


    return true;

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
    /*allocate memory for input*/
    OuterArr dates;
    dates.length = 0;
    dates.capa = 10;
    //malloc will return the pointer which points to where the memory allocated by mallocs starts at
    dates.data = (InnerArr *)malloc(sizeof(InnerArr) * dates.capa);
    if(dates.data == NULL)
    {
        printf("malloc for outer array failed\n");
    }
    //allocate memory for each element of the OuterArr
    for (int i = 0; i < dates.capa; i++)
    {
        //first 1 index for inner
        dates.data[i].capa = 1;
        dates.data[i].data = (TReview *)malloc(sizeof(TReview) * dates.data[i].capa);
        if(dates.data[i].data == NULL)
        {
            printf("memory for inner array is not initialized.\n");
        }        
    }

    size_t capa = 32;
    char *buffer = nullptr;
    //initializing the preDate
    DATE storeddate;
    storeddate.year = 0;
    storeddate.month = 0;
    storeddate.day = 0;
    //index to store 
    int i = -1, j = 0;

    while(getline(&buffer, &capa, stdin) != EOF)
    {
        removeNL(buffer);

        if(!addReview(buffer, &dates, &storeddate, &i, &j))
        {
            break;
        }
    }
    free(buffer);

    

    for(int i = 0; i < dates.length; i++)
    {
        printf("outer[%d] innerarr's length is %d\n", i, dates.data[i].length);
        for(int j = 0; j < dates.data[i].length; j++)
        {
            printf("+ %d-%02d-%02d %d %s\n", dates.data[i].data[j].curDate.year,
                                        dates.data[i].data[j].curDate.month, 
                                        dates.data[i].data[j].curDate.day,
                                        dates.data[i].data[j].rating,
                                        dates.data[i].data[j].comment);
        }
    }


    for(int i = 0; i < dates.capa; i++)
    {
        free(dates.data[i].data);
    }
        free(dates.data);
    
    return 0;
}






