#include <stdio.h>
//linear search 
//-> traversing every element in the arry
//binary search 
//-> at each iteration the search space is divided in half
#define COLUM_MAX 200

void iterate(int arr[][COLUM_MAX], int rows, int colums)
{
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < colums; j++)
        {
            printf("array[%d][%d] = %d\n", i, j, arr[i][j]);
        }
    }
}


int main ()
{
    //to assign values to the elements
    //-> when declaration or index by index
    int arr1[3][COLUM_MAX] = {
     {1, 2, 3},
     {4, 5, 6},
     {7, 8, 9}
    };

    int arr2[3][COLUM_MAX] = {
        {1,2,4,5},
        {3,3,5,6},
        {2,4,5,6},
    };
    //you can actually access the non-allocated index but it is undefined
    // printf("arr[0][1]:%d\n", arr1[0][0]);
    // printf("arr[0][2]:%d\n", arr1[0][1]);
    // printf("arr[0][3]:%d\n", arr1[0][2]);
    // printf("arr[1][0]:%d\n", arr1[1][0]);

    iterate(arr1, 3, 3);
    iterate(arr2, 3, 4);
    return 0;
}