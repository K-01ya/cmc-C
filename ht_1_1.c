#include<stdio.h>

void sort(int arr[], int left, int right, int n)
{
        int mid, i = right, t;
        while (left < right){
                mid = (left + right) / 2;
                if (arr[mid] == n){
                        left = mid;
                        break;
                }
                else if (arr[mid] < n)
                        left = mid + 1;
                else
                        right = mid;
        }

        for (int j = left; j < i; j++){
                t = arr[j];
                arr[j] = arr[i];
                arr[i] = t;
        }

}

int main()
{
        int n, i = 0, arr[1024];
        printf("Enter the number (<= 1024) \n");
        scanf("%d", &n);
        printf("Enter %d integers \n", n);

        for (i = 0; i < n; i++){
                scanf("%d", &arr[i]);
                sort(arr, 0, i, arr[i]);
        }

        printf("  \nInteger in ascending order: \n");

        for (int j = 0; j < i; j++){
                printf("%d ", arr[j]);
        }
        printf("\n");
        return 0;
}

