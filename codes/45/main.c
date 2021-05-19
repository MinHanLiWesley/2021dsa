#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

void merge_sort_p(int *p, int *q, int *r, int *temp, int start, int end);
void merge_p(int *p, int *q, int *r, int *temp, int start, int mid, int end);
long long int merge_qr(int *q, int *r, int *temp, int start, int mid, int end);
long long int merge_sort_qr(int *q, int *r, int *temp, int start, int end);

void println(int *arr, int size);
int main()
{
    generator.init();
    int t = generator.getT();
    while (t--)
    {
        int n, *p, *q, *r, *q_cpy, *r_cpy;

        generator.getData(&n, &p, &q, &r);
        int *temp = (int *)malloc(sizeof(int) * 3 * n);
        q_cpy = (int *)malloc(sizeof(int) * n);
        r_cpy = (int *)malloc(sizeof(int) * n);
        for (size_t i = 0; i < n; i++)
        {
            if (q[i] > r[i])
            {
                int tmp = q[i];
                q[i] = r[i];
                r[i] = tmp;
            }
        }
        //sort p

        merge_sort_p(p, q, r, temp, 0, n - 1);

        memmove((void *)q_cpy, (void *)q, sizeof(int) * n);
        memmove((void *)r_cpy, (void *)r, sizeof(int) * n);
        long long int ans = merge_sort_qr(q, r, temp, 0, n - 1);
        // printf("first ans: %lld\n", ans);
        int head, tail, i = 0, found = 0;
        long long int CN_2 = 0;
        long long int repeat = 0;
        while (i < n)
        {
            if (i == n - 1)
            {
                if (found)
                {
                    tail = i;
                    CN_2 = (tail - head + 1) * (tail - head) / 2;
                    repeat += (CN_2 - merge_sort_qr(q_cpy, r_cpy, temp, head, tail));
                    // printf("first: %lld\n", repeat);
                }
            }
            else
            {
                if (p[i] == p[i + 1] && !found)
                {
                    head = i;
                    found = 1;
                }
                else if (p[i] != p[i + 1] && found)
                {
                    tail = i;
                    CN_2 = (tail - head + 1) * (tail - head) / 2;
                    repeat += (CN_2 - merge_sort_qr(q_cpy, r_cpy, temp, head, tail));
                    // printf("first: %lld\n", repeat);
                    head = 0;
                    tail = 0;
                    found = 0;
                }
            }
            i++;
        }

        printf("%lld\n", ans + repeat);
    }
    return 0;
}
//TODO

void println(int *arr, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%d, ", arr[i]);
    }
    printf("\n");
}
//check if this is working
void swapptr(int *arr[], int i, int j)
{
    int tmp;
    tmp = *(arr[i]);
    *(arr[i]) = *(arr[j]);
    *(arr[j]) = tmp;
}

int get_unsorted_idx(int Arr[], int *ptr)
{
    return (ptr - &Arr[0]) / sizeof(int);
}
void merge_sort_p(int *p, int *q, int *r, int *temp, int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
        merge_sort_p(p, q, r, temp, start, mid);
        merge_sort_p(p, q, r, temp, mid + 1, end);
        merge_p(p, q, r, temp, start, mid, end);
    }
}
void merge_p(int *p, int *q, int *r, int *temp, int start, int mid, int end)
{
    // crawlers for both intervals and for temp
    int i = start, j = mid + 1, k = 0;

    // traverse both arrays and in each iteration add smaller of both elements in temp
    while (i <= mid && j <= end)
    {
        if (p[i] <= p[j])
        {
            temp[k++] = p[i];
            temp[k++] = q[i];
            temp[k++] = r[i++];
        }
        //*(P[i]) > *(P[j])
        else
        {
            temp[k++] = p[j];
            temp[k++] = q[j];
            temp[k++] = r[j++];
        }
    }

    // add elements left in the first interval
    while (i <= mid)
    {
        temp[k++] = p[i];
        temp[k++] = q[i];
        temp[k++] = r[i++];
    }

    // add elements left in the second interval
    while (j <= end)
    {
        temp[k++] = p[j];
        temp[k++] = q[j];
        temp[k++] = r[j++];
    }
    for (k = start; k <= end; k++)
    {
        p[k] = temp[3 * (k - start)];
        q[k] = temp[3 * (k - start) + 1];
        r[k] = temp[3 * (k - start) + 2];
    }
}
long long int merge_qr(int *q, int *r, int *temp, int start, int mid, int end)
{
    int i = start, j = mid + 1, k = 0;
    long long int inv_count = 0;
    long long int total_inv_count = 0;
    while (i <= mid && j <= end)
    {
        if (r[i] < q[j])
        {
            i++;
            total_inv_count += inv_count;
        }
        else
        {
            j++;
            inv_count++;
        }
    }
    while (i <= mid)
    {
        i++;
        total_inv_count += inv_count;
    }
    // sort q
    i = start;
    j = mid + 1;
    k = 0;
    while (i <= mid && j <= end)
    {
        if (q[i] <= q[j])
        {
            temp[k] = q[i];
            k++;
            i++;
        }

        else
        {
            temp[k] = q[j];
            k++;
            j++;
        }
    }
    while (i <= mid)
        temp[k++] = q[i++];

    while (j <= end)
        temp[k++] = q[j++];
    memmove((void *)&q[start], (void *)&temp[0], sizeof(int) * (end - start + 1));
    // sort r
    i = start, j = mid + 1, k = 0;
    while (i <= mid && j <= end)
    {
        if (r[i] <= r[j])
            temp[k++] = r[i++];
        else
            temp[k++] = r[j++];
    }
    while (i <= mid)
        temp[k++] = r[i++];

    while (j <= end)
        temp[k++] = r[j++];

    memmove((void *)&r[start], (void *)&temp[0], sizeof(int) * (end - start + 1));
    return total_inv_count;
    // return 0;
}

long long int merge_sort_qr(int *q, int *r, int *temp, int start, int end)
{
    int mid = 0;
    long long int inv_count = 0;
    if (end > start)
    {
        mid = (start + end) / 2;
        inv_count += merge_sort_qr(q, r, temp, start, mid);
        inv_count += merge_sort_qr(q, r, temp, mid + 1, end);
        inv_count += merge_qr(q, r, temp, start, mid, end);
    }
    return inv_count;
}
