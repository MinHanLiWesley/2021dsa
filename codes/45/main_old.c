#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#define INTSIZE sizeof(int)
int total_repeat = 0;
void swapptr(int *arr[], int i, int j);
int get_unsorted_idx(int Arr[], int *ptr);
void merge_sort_p(int *P[], int start, int end);
void merge_p(int *P[], int start, int mid, int end);
void merge_sep_qr(float qr[], int *orr, int start, int mid, int end, int Q);
void merge_sort_qr(float *qr, int *orr, int start, int end, int Q);
void merge_whole_qr(float qr  [], int start, int end);
int merge_inverrsion_arr(float qr[], float temp[], int start, int mid, int end);
int count_inversion(float qr[], float temp[], int start, int end);
int n, *p, *q, *r;
int main()
{
    generator.init();
    int t = generator.getT();
    while (t--)
    {
        generator.getData(&n, &p, &q, &r);
        for (size_t i = 0; i < n; i++)
        {
            if (q[i] > r[i])
            {
                int temp = q[i];
                q[i] = r[i];
                r[i] = temp;
            }
        }

        int **p_ptr = malloc(sizeof(int *) * n);
        for (size_t i = 0; i < n; i++)
        {
            p_ptr[i] = (p+i);
        }
        
        float *qr = (float *)malloc(sizeof(float) * 2 * n);
        merge_sort_p(p_ptr, 0, n - 1);
        merge_sort_qr(qr, q, 0, (n - 1)/2, 0);
        merge_sort_qr(qr, r, (n-1)/2 + 1, n - 1, 0.5);
        merge_whole_qr(qr, 0, 2 * n - 1);
        float *tmp = (float *)malloc(sizeof(float) * 2 * n);
        int ans = count_inversion(qr, tmp, 0, 2 * n - 1);
        printf("ans = %d\n", ans);
    }
    system("pause");
    return 0;
}
//TODO
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
    return (ptr - &Arr[0]) / INTSIZE;
}
void merge_sort_p(int *P[], int start, int end)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
        merge_sort_p(P, start, mid);
        merge_sort_p(P, mid + 1, end);
        merge_p(P, start, mid, end);
    }
}
void merge_p(int *P[], int start, int mid, int end)
{
    // create a temp array
    int *temp[end - start + 1];

    // crawlers for both intervals and for temp
    int i = start, j = mid + 1, k = 0;

    // traverse both arrays and in each iteration add smaller of both elements in temp
    while (i <= mid && j <= end)
    {
        int i_idx = get_unsorted_idx(p, P[i]);
        int j_idx = get_unsorted_idx(p, P[j]);
        if (*(P[i]) < *(P[j]))
        {
            //check if rq is repeated
            //smaller one would be left
            //only check left triangle's r and right triangle's q

            if (r[i_idx] == q[j_idx])
                total_repeat += 1;
            temp[k] = P[i];
            k += 1;
            i += 1;
        }
        else if (*(P[i]) == *(P[j]))
        {
            //check which one is in left
            // if i is right, swap
            if (!(q[i_idx] <= q[j_idx] && q[i_idx] < r[j_idx]))
                swapptr(P, i, j);
            // since other contidion would be found in intersection,
            // just leave them alone
            if (r[i_idx] <= q[j_idx])
                total_repeat += 1;
            temp[k] = P[i];
            k += 1;
            i += 1;
        }
        //*(P[i]) > *(P[j])
        else
        {
            if (r[i_idx] == q[j_idx])
                total_repeat += 1;
            temp[k] = P[j];
            k += 1;
            j += 1;
        }
    }

    // add elements left in the first interval
    while (i <= mid)
    {
        temp[k] = P[i];
        k += 1;
        i += 1;
    }

    // add elements left in the second interval
    while (j <= end)
    {
        temp[k] = P[j];
        k += 1;
        j += 1;
    }
    // copy temp to original interval
    memmove(&P[start], (void *)&temp[0], sizeof(int *) * (end - start + 1));
    // copy temp to original interval
}
void merge_sep_qr(float qr[], int *orr, int start, int mid, int end, int Q)
{

    // create a temp array
    float temp[end - start + 1];

    // crawlers for both intervals and for temp
    int i = start, j = mid + 1, k = 0;

    // traverse both arrays and in each iteration add smaller of both elements in temp
    while (i <= mid && j <= end)
    {
        if (orr[i] < orr[j])
        {
            temp[k] = i + Q;
            k += 1;
            i += 1;
        }
        else if (orr[i] == orr[j])
        {
            if (p[i] <= p[j])
            {
                temp[k] = i + Q;
                k += 1;
                i += 1;
            }
            else
            {
                temp[k] = j + Q;
                k += 1;
                j += 1;
            }
        }
        else
        {
            temp[k] = j + Q;
            k += 1;
            j += 1;
        }
    }

    // add elements left in the first interval
    while (i <= mid)
    {
        temp[k] = i + Q;
        k += 1;
        i += 1;
    }

    // add elements left in the second interval
    while (j <= end)
    {
        temp[k] = j + Q;
        k += 1;
        j += 1;
    }

    // copy temp to original interval
    memmove(&qr[start], (void *)&temp[0], sizeof(float) * (end - start + 1));
}
void merge_whole_qr(float qr[], int start, int end)
{

    // create a temp array
    float temp[end - start + 1];
    int mid = (start + end) / 2;
    // crawlers for both intervals and for temp
    int i = start, j = mid, k = 0;

    // traverse both arrays and in each iteration add smaller of both elements in temp
    while (i <= mid && j <= end)
    {

        if (r[(int)qr[i]] <= q[(int)(qr[j] - 0.5)])
        {
            temp[k] = qr[i];
            k += 1;
            i += 1;
        }
        else
        {
            temp[k] = qr[j];
            k += 1;
            j += 1;
        }
    }

    // add elements left in the first interval
    while (i <= mid)
    {
        temp[k] = qr[i];
        k += 1;
        i += 1;
    }

    // add elements left in the second interval
    while (j <= end)
    {
        temp[k] = qr[j];
        k += 1;
        j += 1;
    }

    // copy temp to original interval
    memmove(&qr[start], (void *)&temp[0], sizeof(float) * (end - start + 1));
}
void merge_sort_qr(float *qr, int *orr, int start, int end, int Q)
{
    if (start < end)
    {
        int mid = (start + end) / 2;
        merge_sort_qr(qr, orr, start, mid, Q);
        merge_sort_qr(qr, orr, mid + 1, end, Q);
        merge_sep_qr(qr, orr, start, mid, end, Q);
    }
}
int merge_inverrsion_arr(float qr[], float temp[], int start, int mid, int end)
{
    int i, j, k;
    int inv_count = 0;
    i = start;
    j = mid;
    k = end;
    while ((i <= mid - 1) && (j <= end))
    {
        if (qr[i] < qr[j])
            temp[k++] = qr[i++];
        else
        {
            temp[k++] = qr[j++];
            inv_count += (mid - i);
        }
    }

    while (i <= mid - 1)
        temp[k++] = qr[i++];

    while (j <= end)
        temp[k++] = qr[j++];

    memmove(&qr[start], (void *)&temp[0], sizeof(float) * (end - start + 1));

    return inv_count;
}
int count_inversion(float qr[], float temp[], int start, int end)
{
    int mid, inv_count = 0;
    if (end > start)
    {
        mid = (start + end) / 2;
        inv_count += count_inversion(qr, temp, start, mid);
        inv_count += count_inversion(qr, temp, mid + 1, end);
        inv_count += merge_inverrsion_arr(qr, temp, start, mid + 1, end);
    }
    return inv_count;
}
