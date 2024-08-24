#include <stdio.h>
#include <stdlib.h>

int* getClosest(int val1, int val2, int target, int* count);

// Returns all elements closest to target in arr[]
int* findClosest(int arr[], int n, int target, int* count)
{
    // Allocate memory to store the result
    int* result = (int*)malloc(n * sizeof(int));
    *count = 0;

    // Corner cases
    // left-side case
    if (target <= arr[0])
    {
        result[(*count)++] = arr[0];
        return result;
    }
    // right-side case
    if (target >= arr[n - 1])
    {
        result[(*count)++] = arr[n - 1];
        return result;
    }

    // Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j)
    {
        mid = (i + j) / 2;

        if (arr[mid] == target)
        {
            result[(*count)++] = arr[mid];
            return result;
        }

        /* If target is less than array element,
            then search in left */
        if (target < arr[mid])
        {

            // If target is greater than previous
            // to mid, add closest of two
            if (mid > 0 && target > arr[mid - 1])
            {
                int* closestValues = getClosest(arr[mid - 1], arr[mid], target, count);
                for (int k = 0; k < *count; ++k)
                {
                    result[k] = closestValues[k];
                }
                free(closestValues);
            }
            j = mid;
        }
        /* Repeat for left half */

        // If target is greater than mid
        else
        {
            if (mid < n - 1 && target < arr[mid + 1])
            {
                int* closestValues = getClosest(arr[mid], arr[mid + 1], target, count);
                for (int k = 0; k < *count; ++k)
                {
                    result[k] = closestValues[k];
                }
                free(closestValues);
            }
            // update i
            i = mid + 1;
        }
    }

    // Only single element left after search
    result[(*count)++] = arr[mid];
    return result;
}

// Method to compare which one is more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
int* getClosest(int val1, int val2, int target, int* count)
{
    // Allocate memory to store the result
    int* result = (int*)malloc(2 * sizeof(int));
    *count = 0;

    if (target - val1 == val2 - target)
    {
        result[(*count)++] = val1;
        result[(*count)++] = val2;
    }
    else if (target - val1 < val2 - target)
    {
        result[(*count)++] = val1;
    }
    else
    {
        result[(*count)++] = val2;
    }

    return result;
}

// Driver code
int main()
{
    int arr[] = {1, 2, 4, 5, 6, 6, 8, 8, 9};
    int n = sizeof(arr) / sizeof(arr[0]);
    int target = 8;
    int count;

    int* closestValues = findClosest(arr, n, target, &count);

    printf("Closest values to %d are: ", target);
    for (int i = 0; i < count; ++i)
    {
        printf("%d ", closestValues[i]);
    }

    // Free allocated memory
    free(closestValues);

    return 0;
}