#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*Xor linked list*/
#ifdef DEBUG
#define debug 1
#else
#define debug 0
#endif
typedef struct Car
{
    int label;
    struct Car *nxp;
} Car;
typedef struct Rail
{
    Car *head;
    Car *tail;
    int size;
    int railnum;
} Rail;
Car * xor (Car * a, Car *b);
Rail *Rail_init(int r);
void delete_linked();
void print_list();
void reverse(Rail *rail);
void enter(Rail **rail_arr, int r, int label); // insert label in the tail of list
void leave(Rail **rail_arr, int r);            // leave the last member pf the list
// or maybe change the flag in head node
void migrate(Rail **rail_arr, int a, int b); // pop members in r1 in sequence and enter them to r2
void print_list(Rail **rail_arr, int k);
void print_judge_ans(Rail **rail_arr, int k);
/*
strategies:
    1. initiate an array with k size -> contain head node of every list
    2. enter: find the head node in array and insert
    3. leave : find the head node in arrat and delete
    4. migrate: 
*/
int main()
{
    int k, n;
    scanf("%d %d\n", &k, &n);
    Rail *rail_arr[k];
    for (size_t i = 0; i < k; i++)
    {
        rail_arr[i] = Rail_init(i);
    }
    int count = 0;
    char input[20];
    while(count != n){
        int a, b;
        scanf("%s", input);
        if(input[0]=='e'){
        scanf("%d %d", &a,&b);
            enter(rail_arr, a, b);
        }
        else if(input[0]=='l'){
            scanf("%d", &a);
            leave(rail_arr, a);
        }
        else if(input[0]=='m'){
            scanf("%d %d", &a, &b);
            migrate(rail_arr, a, b);
        }
        count++;
    }


    
    print_judge_ans(rail_arr, k);
    // print_list(rail_arr, k);
    // system("pause");
    return 0;
}
Rail *Rail_init(int r)
{
    Rail *rail = (Rail *)malloc(sizeof(Rail));
    rail->head = NULL;
    rail->tail = NULL;
    rail->size = 0;
    rail->railnum = r;
    return rail;
}

//Performs the xor operation of two pointers
Car * xor (Car * a, Car *b) {
    return (Car *)((uintptr_t)a ^ (uintptr_t)b);
}



void enter(Rail **rail_arr, int r, int label)
{
    // int success = 0;
    Car *ptr = (Car *)malloc(sizeof(Car));
    ptr->label = label;
    Car **head = &rail_arr[r]->head;
    Car **tail = &rail_arr[r]->tail;
    if (*head == NULL)
    {
        // success = 1;
        ptr->nxp = NULL;
        *head = *tail = ptr;
    }
    else
    {
        // success = 1;
        ptr->nxp = xor(*tail, NULL);
        (*tail)->nxp = xor(ptr, xor((*tail)->nxp, NULL));
        *tail = ptr;
    }
    rail_arr[r]->size++;
    // if (success)
    // {
    //     rail_arr[r]->size++;
    //     if (debug)
    //         printf("Enter car %d to rail %d successfully.\n", label, r);
    // }
    // else
    // {
    //     if (debug)
    //         printf("ERROR in Entering car %d to rail %d.\n", label, r);
    // }
    return;
}
void leave(Rail **rail_arr, int r)
{
    //int label;
    //int success = 0;
    Car *ptr;
    Car **head = &rail_arr[r]->head;
    Car **tail = &rail_arr[r]->tail;
    if (*head == NULL)
    {
        //printf("Empty!!\n");
        return;
    }
    else
    {
        ptr = *tail;
        // label = ptr->label;
        Car *prev = xor(ptr->nxp, NULL);
        if (prev == NULL)
            *head = NULL;
        else
            prev->nxp = xor(ptr, xor(prev->nxp, NULL));
        *tail = prev;
        free(ptr);
        ptr = NULL;
        // success = 1;
    }
    rail_arr[r]->size--;
    // if (success)
    // {
    //     rail_arr[r]->size--;
    //     if (debug)
    //         printf("Delete car %d from rail %d successfully.\n", label, r);
    // }
    // else
    // {
    //     if (debug)
    //         printf("ERROR in deleting car %d from rail %d.\n", label, r);
    // }
}
void reverse(Rail *rail)
{
    // a(head) <-> b <-> c <-> d <-> e <-> f(tail)
    // f(head) <-> e <-> d <-> c <-> b <-> a(tail)
    Car *tmp_head = rail->head;
    Car *tmp_head_npx = tmp_head->nxp;
    Car *tmp_tail = rail->tail;
    Car *tmp_tail_npx = tmp_tail->nxp;
    Car  *c, *d;
    c = xor(xor(tmp_head->nxp, NULL)->nxp, tmp_head);
    d = xor(xor(tmp_tail->nxp, NULL)->nxp, tmp_tail);

    //swap head and tail
    rail->head = tmp_tail;
    rail->head->nxp = tmp_tail_npx;
    rail->tail = tmp_head;
    rail->tail->nxp = tmp_head_npx;
    //update the neighbors of tail and head
    //tail
    xor(tmp_tail->nxp, NULL)->nxp = xor(d,rail->head);
    //head
    xor(tmp_head->nxp, NULL)->nxp = xor(c, rail->tail);


}
void reverse_old(Rail *rail)
{
    // a(head) <-> b <-> c <-> d <-> e <-> f(tail)
    // f(head) <-> e <-> d <-> c <-> b <-> a(tail)
    Car *tmp_head = rail->head;
    Car *tmp_head_npx = tmp_head->nxp;
    Car *tmp_tail = rail->tail;
    Car *tmp_tail_npx = tmp_tail->nxp;
    Car *b, *c, *d, *e;
    b = xor(tmp_head->nxp, NULL);
    c = xor(b->nxp, tmp_head);
    e = xor(tmp_tail->nxp, NULL);
    d = xor(e->nxp, tmp_tail);
    //swap head and tail
    rail->head = tmp_tail;
    rail->head->nxp = tmp_tail_npx;
    rail->tail = tmp_head;
    rail->tail->nxp = tmp_head_npx;
    //update the neighbors of tail and head
    //tail
    e->nxp = xor(d,rail->head);
    //head
    b->nxp = xor(c, rail->tail);

}
void migrate(Rail **rail_arr, int a, int b)
{
    Rail *ra = rail_arr[a];
    Rail *rb = rail_arr[b];
    // first reverse ra
    if (ra->size ==0){
        return;
    }
    if (ra->size > 1)
    {
        reverse(ra);
    }
    if (debug)
        print_list(rail_arr, 5);
    if (rb->size == 0)
    {
        rb->head = ra->head;
    }
    else
    {
        Car *ct = xor(rb->tail->nxp, NULL);
        rb->tail->nxp = xor(ra->head, xor(rb->tail->nxp, NULL));
        ra->head->nxp = xor(rb->tail, xor(ra->head->nxp, NULL));
    }
    //concatenate ra->head and rb->tail

    //update information in ra and tb
    rb->tail = ra->tail;
    rb->size += ra->size;
    ra->head = ra->tail = NULL;
    ra->size = 0;
}
void print_list(Rail **rail_arr, int k)
{
    for (size_t i = 0; i < k; i++)
    {
        printf("Rail %d:\n", i);
        Car *curr = rail_arr[i]->head;
        Car *prev = NULL, *next;

        printf("List elements are : ");
        while (NULL != curr)
        {
            printf("%d ", curr->label);
            next = xor(prev, curr->nxp);
            prev = curr;
            curr = next;
        }

        printf("\n\n");
    }
}

void print_judge_ans(Rail **rail_arr, int k)
{
    for (size_t i = 0; i < k; i++)
    {
        Car *curr = rail_arr[i]->head;
        Car *prev = NULL, *next;
        while (NULL != curr)
        {
            printf("%d ", curr->label);
            next = xor(prev, curr->nxp);
            prev = curr;
            curr = next;
        }
        printf("\n");
    }
}

    // enter(rail_arr, 4, 1);
    // enter(rail_arr, 4, 2);
    // enter(rail_arr, 4, 3);
    // enter(rail_arr, 4, 4);
    // enter(rail_arr, 4, 5);
    // print_list(rail_arr, k);
    // reverse(rail_arr[4]);
    // print_list(rail_arr, k);
    // enter(rail_arr, 4, 2788);
    // leave(rail_arr, 4);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 4, 3);
    // print_list(rail_arr, k);
    // enter(rail_arr, 2, 132);
    // enter(rail_arr, 0, 2800);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 2, 3);
    // print_list(rail_arr, k);
    // enter(rail_arr, 1, 3116);
    // enter(rail_arr, 3, 8714);
    // enter(rail_arr, 0, 267);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 0, 1);
    // print_list(rail_arr, k);
    // enter(rail_arr, 4, 4055);
    // migrate(rail_arr, 3, 0);
    // enter(rail_arr, 2, 2567);
    // migrate(rail_arr, 1, 4);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 1, 2);
    // print_list(rail_arr, k);
    // enter(rail_arr, 4, 72);
    // leave(rail_arr, 1);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 0, 4);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 4, 1);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 2, 0);
    // enter(rail_arr, 1, 1230);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 3, 2);
    // print_list(rail_arr, k);
    // migrate(rail_arr, 0, 4);
    // print_list(rail_arr, k);
    // leave(rail_arr, 3);
    // enter(rail_arr, 2, 8548);
    // enter(rail_arr, 0, 4983);
    // leave(rail_arr, 4);
    // migrate(rail_arr, 3, 2);
    // enter(rail_arr, 3, 3731);
    // enter(rail_arr, 1, 9536);