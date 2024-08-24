#include <stdio.h>
#define MAX 250000
typedef long long int ll;

typedef struct
{
    int start;
    int end;
} Interval;

int printRes(Interval res1, Interval *res2, int distance, int n )
{
    //distance = edge - i + 1;
    //doesnt exist when edge is 0, i >=0, thus, 1 or smaller
    if(distance <= 1)
    {
        printf("Does not exist.\n");
        return -1;
    }
    for(int i = n - 1; i >= 0; i--)
    {
        printf("%d: %d - %d\n", distance, res2[i].start, res2[i].end);
    }
    // printf("%d: %d - %d\n", distance, res1.start, res1.end);
    printf("Answers: %d\n", n);

    return 1;
}

void findedge(ll *array, int index, int *edge, int element)
{
    for(int i = index + 1; i < element; i++)
    {
        // printf("arr[%d] = %d\n", i, array[i]);
        if(array[index] <= array[i])
        {
            *edge = i;
        }
    }
}
int findmaxindex(ll *array, int element)
{
    //find max index for each index with loop
    Interval res2[MAX];
    int edge = 0;
    int lastindex = element - 1;
    int distance = 0, max_distance = 0;
    int extraNum = 0;
    for(int i = 0; i < element; i++)
    {
        // printf("currently this index %d\n",i);
        if(i == lastindex - (max_distance - 1) + 1)
        {
            break;
        }
        findedge(array, i, &edge, element);
        distance = edge - i + 1;
        // printf("%d - %d + 1 = %d\n", edge, i, distance);
        if(distance > max_distance)
        {  
            max_distance = distance;
        }
    }

    
    for(int i = 0; i < element; i++)
    {
        if(i == lastindex - (max_distance - 1) + 1)
        {
            break;
        }
        findedge(array, i, &edge, element);
        distance = edge - i + 1;
        if(distance == max_distance)
        {
            
            res2[extraNum].start = i;
            res2[extraNum].end = edge;                
            extraNum++;
        }
    }
        
    //print the result with number of interval as arg
    if(printRes(res1, res2, max_distance, extraNum) != 1)
    {
        return -1;
    }

    return 1;
}
int readinput(ll *array, int *element)
{
    *element = 0;
    while(scanf("%lld", &array[*element]) == 1)
    {
        // printf("array[%d] = %d\n", *element, array[*element]);
        (*element)++;
    }
    if(!feof(stdin) || *element < 2)
    {
        return -1;
    }
    else
        return 1;
}

int main() 
{
    ll inputarray[MAX];
    int num_ele;
    printf("Values:\n");
    if(readinput(inputarray, &num_ele) != 1)
    {
        printf("Invalid input.\n");
        return -1;
    }
    findmaxindex(inputarray, num_ele);
    
    return 0;
}