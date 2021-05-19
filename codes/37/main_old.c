#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int *nums, int head, int tail)
{
    int smallerIdx = head;
    for (int i = head; i < tail; i++) {
        /* pivot = nums[tail] */
        if (nums[i] < nums[tail]) {
            swap(&(nums[i]), &(nums[smallerIdx]));
            smallerIdx++;
        }
    }
    swap(&(nums[tail]), &(nums[smallerIdx]));
    return smallerIdx;
}

int kth_smallest_qsort(int *nums, int left,int head, int tail, int k)
{
    if (head > tail)
        return -1;

    /* pivot means the sorted position in nums, all left element is smaller than
     * pivot and right is larger */
    int pivot = partition(nums, head, tail);

    if ((pivot + 1) == k+left)
        return nums[pivot];
    else if ((pivot + 1) < k+left)
        return kth_smallest_qsort(nums, left,pivot + 1, tail, k);
    else
        return kth_smallest_qsort(nums, left,head, pivot - 1, k);
}

int findKthSmallest(int *nums, int l,int r, int k)
{
    return kth_smallest_qsort(nums, l,l, r, k);
}


int main()
{
    int n, q;
    scanf("%d %d ", &n, &q);
    int *list = (int *)malloc(sizeof(int) * n);
    for (size_t i = 0; i < n; i++)
    {
        int t;
        scanf("%d", &t);
        list[i] = t;
    }
    // int list[] = {-10, 1, 4, -3, -5};
    int k = 1;

    int l = 3, r = 4;
    // int a = findKthSmallest(list, l, r, k);
    // printf("%d\n", a);
    int count = 0;
    while (count < q)
    {
        char input[30];
        scanf("%s", input);
        if (input[0] == 'Q')
        {
            scanf("%d %d %d", &l, &r, &k);
            l--;
            r--;
            
            int a = findKthSmallest(list, l, r, k);
            printf("%d\n", a);
        }

        count++;
    }

    // system("pause");
    return 0;
}
