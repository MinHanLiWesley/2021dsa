#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _DEBUG 0
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
struct maxheap
{
    int n;
    int *data;
};

#define maxheap_root(mh) ((mh)->data[0])

static inline int
_maxheap_findmax(const int *arr, int k)
{
    int idx = 0;
    int t = arr[0];
    for (int i = 0; i < k; ++i)
    {
        if (t < arr[i])
        {
            t = arr[i];
            idx = i;
        }
    }
    return idx;
}

static inline void
_maxheap_swap(int **arr, int i, int j)
{
    int t = (*arr)[i];
    (*arr)[i] = (*arr)[j];
    (*arr)[j] = t;
}

struct maxheap *
maxheap_new(const int *arr, int l, int k)
{
    int n = k;
    int idx = 0;
    struct maxheap *mh = (struct maxheap *)malloc(sizeof(*mh) + sizeof(int) * n);
    mh->n = n;
    mh->data = (int *)malloc(sizeof(int) * k);

    memmove(mh->data, (void *)&arr[l], sizeof(int) * n);
    idx = _maxheap_findmax(mh->data, n);
    _maxheap_swap(&mh->data, 0, idx);
    return mh;
}

void maxheap_free(struct maxheap **mh)
{
    if (mh)
    {
        free(*mh);
        *mh = 0;
    }
}

void maxheap_insert(struct maxheap *mh, int small)
{
    int idx;
    mh->data[0] = small;
    idx = _maxheap_findmax(mh->data, mh->n);
    _maxheap_swap(&mh->data, 0, idx);
}

/* select K-smallest elements in an array */
int heapksmallest(const int *arr, int l, int r, int k)
{
    struct maxheap *mh = maxheap_new(arr, l, k);
    for (int i = k + l; i <= r; ++i)
    {
        if (arr[i] < maxheap_root(mh))
        {
            maxheap_insert(mh, arr[i]);
        }
    }
    int a = mh->data[0];
    free(mh->data);
    free(mh);
    mh = NULL;
    return a;
}

int partition(int *nums, int head, int tail)
{
    int smallerIdx = head;
    for (int i = head; i < tail; i++)
    {
        /* pivot = nums[tail] */
        if (nums[i] < nums[tail])
        {
            swap(&(nums[i]), &(nums[smallerIdx]));
            smallerIdx++;
        }
    }
    swap(&(nums[tail]), &(nums[smallerIdx]));
    return smallerIdx;
}
int randomPartition(int *nums, int l, int r)
{
    int n = r - l + 1;
    int pivot = rand() % n;
    swap(&nums[l + pivot], &nums[r]);
    return partition(nums, l, r);
}
int kth_smallest_qsort(int *nums, int left, int right, int head, int tail, int k, int limit)
{
    if (head > tail)
        return -1;
    if (limit == 0)
    {
        // if (_DEBUG)
        // printf("heap!!\n");
        int a = heapksmallest(nums, head, tail, k);
        swap(&(nums[tail]), &(nums[a]));
        // return a;
    }

    //pivot means the sorted position in nums, all left element is smaller than
    //pivot and right is larger
    int pivot = randomPartition(nums, head, tail);
    limit--;
    if ((pivot + 1) == k + left)
        return nums[pivot];
    else if ((pivot + 1) < k + left)
        return kth_smallest_qsort(nums, left, right, pivot + 1, tail, k, limit);
    else
        return kth_smallest_qsort(nums, left, right, head, pivot - 1, k, limit);
}
unsigned int Log2n(unsigned int n)
{
    return (n > 1) ? 1 + Log2n(n / 2) : 0;
}
int findKthSmallest(int *nums, int l, int r, int k)
{
    return kth_smallest_qsort(nums, l, r, l, r, k, Log2n(r - l) * 5);
}

void insert(int **nums, int index, int data, int *size)
{
    if (index != *size)
        memmove((void *)&(*nums)[index + 1], (void *)&(*nums)[index], (*size - index) * sizeof(int));
    (*nums)[index] = data;
    (*size)++;
}
void delete (int **nums, int index, int *size)
{
    memmove((void *)&(*nums)[index], (void *)&(*nums)[index + 1], (*size - index) * sizeof(int));
    (*size)--;
}

void reverse(int **nums, int left, int right)
{
    for (size_t i = 0; i < (right - left + 1) / 2; i++)
    {
        swap(&(*nums)[i + left], &(*nums)[right - i]);
    }
}
void print(int *nums, int size)
{
    printf("{");
    for (size_t i = 0; i < size; i++)
    {
        if (i == size - 1)
            printf("%d}\n", nums[i]);
        else
            printf("%d,", nums[i]);
    }
}
int main()
{
    int n, q;
    scanf("%d %d ", &n, &q);
    // n = 90000;
    // q = 10;
    // int list[100000];
    // int tmp[100000];
    int *list = (int *)malloc(sizeof(int) * 100000);
    int *tmp = (int *)malloc(sizeof(int) * 100000);
    // for (size_t i = 0; i < n/1000; i+=1)
    // {
    //     for (size_t j = 0; j < 1000; j++)
    //     {
    //         list[i*1000+j] = i;
    //     }
        
        
    // }

    for (size_t i = 0; i < n; i++)
    {
        int t;
        scanf("%d", &t);
        list[i] = t;
    }
    // int list[] = {-10, 1, 4, -3, -5};
    int k, l, r;
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
            memmove((void *)tmp, (void *)&list[l], (r-l+1) * sizeof(int));
            // print(tmp, r-l+1);
            int a = findKthSmallest(tmp, 0, r-l, k);
            // int a = select_ksmallest(tmp, l, r, k);

            if (_DEBUG)
                print(list, n);
            printf("%d\n", a);
        }
        else if (input[0] == 'R')
        {
            scanf("%d %d", &l, &r);
            l--;
            r--;
            reverse(&list, l, r);
            if (_DEBUG)
                print(list, n);
        }
        else if (input[0] == 'I')
        {
            scanf("%d %d", &l, &r);
            l--;
            insert(&list, l, r, &n);
            if (_DEBUG)
                print(list, n);
        }
        else if (input[0] == 'D')
        {
            scanf("%d", &k);
            k--;
            delete (&list, k, &n);
            if (_DEBUG)
                print(list, n);
        }

        count++;
    }

    // system("pause");
    return 0;
}