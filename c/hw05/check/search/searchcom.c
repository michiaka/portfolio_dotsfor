#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <climits>

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
    long int rating;
    char comment[MAX];
} TReview;

typedef struct
{
    TReview *data;
    int capa;
    int length;
    long int sum;
    
} InnerArr;

typedef struct
{
    InnerArr* data; 
    int capa;
    int length;

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

void removeNL(char* str)
{
    int l = strlen(str);
    if(l > 0 || str[l+1] == '\n')
    {
        str[l + 1] = '\0';
    }
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
    if((ret = sscanf(inputstr," + %d-%02d-%02d %d %4096s", &review.curDate.year, &review.curDate.month,
     &review.curDate.day, &review.rating, review.comment)) != 5|| !isValidReview(review, *storedDate))
    {
        // printf("inputstr %s\n", inputstr);
        printf("Invalid input.\n");
        return false;
    }
    //cheking the format 2(extra character at the end)
    char extra = 0;
    if(sscanf(inputstr, "+ %*s %*s %*s %c", &extra) == 1 && extra != '\0') 
    {
        // printf("Invalid input.\n");
        printf("Invalid input.\n");

        return false;
    }
    //add into a dynamically allocated array

    if(isLaterDate(*storedDate,review.curDate))
    {
        //printf("it is later than the previous day.\n");
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
        // printf("reallocation for outer array will happen\n");
        dates->capa += 10;
        // printf("capacity of outer is %d", dates->capa);
        dates->data = (InnerArr *)realloc(dates->data, sizeof(InnerArr) * dates->capa);
        if(dates->data == NULL)
        {
            // printf("reallocation for outer array failed.\n");
            return false;
        }
        //allocating memory for inner array inside of the new created elements of out
        for (int j = *i; j < dates->capa; j++)
        {
            dates->data[*i].capa = 1;
            dates->data[*i].sum = 0;
            dates->data[*i].length = 0;
            //first 1 index for inner
            dates->data[j].data = (TReview *)malloc(sizeof(TReview) * dates->data[*i].capa);
            if(dates->data[j].data == NULL)
            {
                return false;
                // printf("memory for inner array is not initialized.\n");
            }       
        }
    }
    //print("value of j %d\n", *j);
    //print("value of i %d\n", *i);

    (dates->data[*i]).data[*j] = review;
    dates->data[*i].sum += dates->data[*i].data[*j].rating;
    (*j)++;
    //print("this is j: %d\n", *j);
    dates->data[*i].length = *j;

    *storedDate = review.curDate;


    return true;

}

typedef struct
{   
    long int sum;
    size_t startday_index;
    size_t endday_index;
} SumInfo;

typedef struct
{
    SumInfo * data;
    size_t capa;
    size_t length;
} SumArr;

void removePrefix(char * str)
{
    //replacing with space
    while(*str)
    {
        if(*str == ' ')
        {
            str++;
        }
        else
            break;
    }

    // printf("gonna remove %c", *str);

    *str = ' ';
}

//compare function receive two pointers 
int compareNum(SumInfo *a, SumInfo *b)
{
    // 1: sum1>sum2 , -1: sum1 < sum2 , 0: sum1 = sum2 
    return (a->sum > b->sum) - (a->sum < b->sum);
}


SumInfo findclosest(SumArr arr, long int target)
{
    long int min_diff = LONG_MAX;
    long int cur_diff = 0;
    SumInfo min_sum = {0};
    for(size_t i = 0; i < arr.length; i++)
    {
        cur_diff = abs(arr.data[i].sum  -  target);
        if (cur_diff <= min_diff)
        {
            min_diff = cur_diff;
            min_sum = arr.data[i];
        }
    }

    return min_sum;

}
bool search(char *searchcommand, OuterArr dates)
{

    
    //storing the prefix to make a differene in printing at the last
    char prefix;
    sscanf(searchcommand, " %c", &prefix);
    //removing prefix
    // printf("after removing prefix %s", searchcommand);

    removePrefix(searchcommand);
    //input validation
    long int searchvalue = 0;
    // printf("after removing prefix %s", searchcommand);
    if(sscanf(searchcommand, "%ld", &searchvalue) != 1 || searchvalue <= 0)
    {   
        printf("Invalid input.\n");
        return false;
    }
    char extra = 0;
    if(sscanf(searchcommand, "%*s %c", &extra) == 1 && extra != ' ')
    {
        printf("Invalid input.\n");
        return false;
    } 



    //store all the sum of ratings in an array
    SumArr allsum = {0};
    allsum.capa = 1;
    allsum.data = (SumInfo *)malloc(sizeof(SumInfo) * allsum.capa);
    if(allsum.data == NULL)
    {
        return false;
    }
    long int tmp_sum; 
    size_t index = 0;
    // printf("dates.length %d\n", dates.length);
    for(int i = 0; i < dates.length + 1; i++)
    {
        tmp_sum = 0;
        int t = i;
        for(int j = t - 1 ; j >= 0 ; j--)
        {
            tmp_sum += dates.data[j].sum;
            if(index == allsum.capa)
            {
                allsum.capa = allsum.capa * 2 + 10;
                allsum.data = (SumInfo *)realloc(allsum.data, sizeof(SumInfo) * allsum.capa);
                if(allsum.data == NULL)
                {
                    return false;
                }
            }
            allsum.data[index].sum = tmp_sum;
            // printf("allsum[%ld].sum = %ld\n", index, allsum.data[index].sum);
            allsum.data[index].startday_index = j;
            //why t -1 ?
            allsum.data[index].endday_index = t - 1;
            index++;
        }
    }


    //sort them depending on the sum value
    allsum.length = index;
    // qsort(allsum.data, allsum.length, sizeof(SumInfo), (int (*)(const void*, const void*))compareNum);

    // printf("after:\n");
    // for(size_t i = 0; i < allsum.length; i++)
    // {
    //     printf("sum %ld\n", allsum.data[i].sum);
    // }
    SumInfo result;
    // printf("svalue%ld\n", searchvalue);
    result = findclosest(allsum, searchvalue);
    
    // printf("result: sum %ld, sindex %ld, eindex %ld\n", result.sum, result.startday_index, result.endday_index);

    int sday = result.startday_index;
    int eday = result.endday_index;
    //print the result
        
    printf("%d-%02d-%02d - %d-%02d-%02d: %ld\n", dates.data[sday].data[0].curDate.year,
                                        dates.data[sday].data[0].curDate.month, 
                                        dates.data[sday].data[0].curDate.day,
                                        dates.data[eday].data[0].curDate.year,
                                        dates.data[eday].data[0].curDate.month, 
                                        dates.data[eday].data[0].curDate.day,
                                        result.sum);
    if(prefix == '?')
    {

        for(int i = sday; i <= eday; i++)
        {
            int num_rev = dates.data[i].length;
            for(int j = 0; j < num_rev; j++)
            {
                printf("  %d: %s\n", dates.data[i].data[j].rating,dates.data[i].data[j].comment);
            }
        }
                                        
    }
    


    free(allsum.data);



    return true;

}

int main()
{

    printf("Reviews:\n");
    /*allocate memory for input*/
    OuterArr dates;
    dates.length = 0;
    dates.capa = 3;
    //malloc will return the pointer which points to where the memory allocated by mallocs starts at
    dates.data = (InnerArr *)malloc(sizeof(InnerArr) * dates.capa);
    if(dates.data == NULL)
    {
        //print("malloc for outer array failed\n");
    }
    //allocate memory for each element of the OuterArr
    for (int i = 0; i < dates.capa; i++)
    {
        //first 1 index for inner
        dates.data[i].capa = 1;
        dates.data[i].sum = 0;
        dates.data[i].length = 0;
        dates.data[i].data = (TReview *)malloc(sizeof(TReview) * dates.data[i].capa);
        if(dates.data[i].data == NULL)
        {
            //print("memory for inner array is not initialized.\n");
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

    bool datafound = false;
    while(getline(&buffer, &capa, stdin) != EOF)
    {

        removeNL(buffer);

        int ret = prefixchecker(buffer);
        
        if(ret == 1)
        {
            
            if(!addReview(buffer, &dates, &storeddate, &i, &j))
            {
                break;
            }
            else
                datafound = true;
        }
        else if(ret == 0) 
        {

            if(datafound)
                search(buffer, dates);
            else
                printf("Invalid input.\n");

        }
        else 
        {
            printf("Invalid input.\n");
            break;
        }

    }
    free(buffer);

    


    // for(int i = 0; i < dates.length; i++)
    // {
    //     //print("outer[%d] innerarr's length is %d\n", i, dates.data[i].length);
    //     for(int j = 0; j < dates.data[i].length; j++)
    //     {
    //         printf("+ %d-%02d-%02d %d %s\n", dates.data[i].data[j].curDate.year,
    //                                     dates.data[i].data[j].curDate.month, 
    //                                     dates.data[i].data[j].curDate.day,
    //                                     dates.data[i].data[j].rating,
    //                                     dates.data[i].data[j].comment);
    //     }

    //     printf("%ld\n", dates.data[i].sum);
    // }


    for(int i = 0; i < dates.capa; i++)
    {
        free(dates.data[i].data);
    }
        free(dates.data);
    
    return 0;
}






