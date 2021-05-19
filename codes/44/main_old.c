#include <stdio.h>
#include <stdlib.h>
/* havent free*/

int g_pop_counter;
int impossible = 0;
#define bigger(a, b) a > b ? a : b
// leftist_tree
typedef struct LTT_Node
{
    int data;
    int depth;
    struct LTT_Node *left;
    struct LTT_Node *right;
} LTT_Node;
// doubly linked list
typedef struct DLL_node
{
    int data;
    struct DLL_node *next, *prev;
} DLL_node;
typedef struct Rank_arr
{
    int rank;
    int is_head;
    int line_id;
    int parent;

} Rank_arr;
typedef struct target
{
    int data;
    struct target *next;

} Target;
typedef struct Line
{
    LTT_Node *root;
    DLL_node **head;
    Rank_arr *rank_root;
    int closed;

} Line;

typedef struct operation
{
    char op;
    int A;
    int B;
} Operation;
int *table;

LTT_Node *merge_ltt(LTT_Node *A, LTT_Node *B);
void insert(LTT_Node *root, int data);
int find_max(LTT_Node *root);
void del_max(LTT_Node *root);
void push_last(DLL_node **head, int data);
void del_last(DLL_node **head);
void del_first(DLL_node **head);
int find_first(DLL_node **head);
int find_last(DLL_node **head);
DLL_node **merge_dll(DLL_node **A, DLL_node **B);
void push(Line line,Rank_arr set[], int data);
void merge(Line lineA, Line lineB);
int pop(Line line[], int id, Target **target);
int find(Rank_arr set[], int i);
void Union(Rank_arr *A, Rank_arr *B);
void free_line(Line *line, int L);

int main()
{
    int T, N, L, O;
    scanf(" %d", &T);
loop:
    while (T--)
    {
        scanf(" %d %d %d", &N, &O, &L);
        //table
        table = (int *)malloc(sizeof(int) * (N + 1));
        for (size_t i = 0; i < (N + 1); i++)
        {
            table[i] = 0;
        }

        Target *target_head = (Target *)malloc(sizeof(Target));
        target_head->data = 0;
        target_head->next = NULL;

        //
        Operation *operation = (Operation *)malloc(sizeof(Operation) * O);
        Line *line = (Line *)malloc(sizeof(Line) * L);
        for (size_t i = 0; i < L; i++)
        {
            line[i].closed = 0;
            line[i].head = NULL;
            line[i].rank_root = NULL;
            line[i].root = NULL;
        }

        Rank_arr *set = (Rank_arr *)malloc(sizeof(Rank_arr) * (N + 1));
        for (size_t i = 0; i < (N + 1); i++)
        {
            set[i].is_head = 1;
            set[i].line_id = 0;
            set[i].parent = i;
            set[i].rank = 0;
        }

        for (size_t i = 0; i < O; i++)
        {
            char input[20];
            scanf( "%s", input);
            if (input[0] == 'p')
            {
                operation[i].op = 'p';
            }
            else
            {
                operation[i].op = 'm';
            }
            scanf(" %d %d", &(operation[i].A), &(operation[i].B));
        }
        
        int count = 0;
        Target *curr = target_head;
        printf("HAHA\n");
        while (N--)
        {
            printf("N: %d \n", N);
            count++;
            scanf(" %d", &(curr->data));
            Target *tmp = (Target *)malloc(sizeof(Target));
            curr->next = tmp;
            curr = curr->next;
        }
        printf("HAHA\n");
        curr = target_head;
        for (size_t i = 0; i < O; i++)
        {
            int A = operation[i].A;
            int B = operation[i].B;
            if (operation[i].op == 'p')
            {
                //first check target and input
                if (A == curr->data)
                {
                    curr = curr->next;
                    continue;
                }
                else
                {
                    push(line[B], set,A);
                    table[A] = 1;
                    while (table[curr->data])
                    {
                        // pop would make curr --> curr.next
                        if (pop(line, set[find(set, curr->data)].line_id, &curr))
                        {
                            printf("impossible\n");
                            // free(table);
                            // free(operation);
                            // free(target_head);
                            // free_line(line, L);
                            goto loop;
                        }
                    }
                }
            }
            // merge
            else
            {
                merge(line[A], line[B]);
            }
        }
        printf("possible\n");
    }

    return 0;
}
void free_line(Line *line, int L)
{
    for (size_t i = 0; i < L; i++)
    {
        free(line[i].head);
        free(line[i].root);
    }
    free(line);
}
void free_target(Target *target)
{
    Target *curr = target;
    while (curr != NULL)
    {
        free(curr);
        curr = curr->next;
    }
}
LTT_Node *merge_ltt(LTT_Node *A, LTT_Node *B)
{
    /* if A B are Null */
    if (!A || !B)
        return A ? A : B;
    /* Compare the min of both tree */
    if (bigger(A->data, B->data))
    {
        LTT_Node *temp = A;
        A = B;
        B = temp;
    }
    A->right = merge_ltt(A->right, B);
    if (A->left->depth < A->right->depth)
    {
        LTT_Node *temp = A->left;
        A->left = B->right;
        B->right = temp;
    }
    A->depth = A->left->depth + 1;
    return A;
}

void insert(LTT_Node *root, int data)
{
    LTT_Node *temp = (LTT_Node *)malloc(sizeof(LTT_Node));
    root = merge_ltt(root, temp);
}

int find_max(LTT_Node *root)
{
    return root->data;
}

void del_max(LTT_Node *root)
{
    LTT_Node *temp = merge_ltt(root->left, root->right);
    free(root);
    root = temp;
}

void push_last(DLL_node **head, int data)
{
    DLL_node *new_node = (DLL_node *)malloc(sizeof(DLL_node));
    new_node->data = data;
    if (!(*head))
    {
        new_node->next = new_node->prev = new_node;
        *head = new_node;
        return;
    }
    DLL_node *last = (*head)->prev;

    new_node->next = *head;
    (*head)->prev = new_node;
    new_node->prev = last;
    last->next = new_node;
    return;
}

void del_last(DLL_node **head)
{
    if (!(*head))
        return;
    DLL_node *last = (*head)->prev;
    /*more than one element*/
    if ((*head) != last->prev)
    {
        (*head)->prev = last->prev;
        last->next = *head;
    }
    else
    {
        *head = NULL;
    }
    free(last);
    return;
}

void del_first(DLL_node **head)
{
    if (!(*head))
        return;
    DLL_node *first = (*head)->next;
    /*more than one element*/
    if ((*head) != first->next)
    {
        (*head)->next = first->next;
        first->prev = *head;
    }
    else
    {
        *head = NULL;
    }
    free(first);
    return;
}

int find_first(DLL_node **head)
{
    if (!(*head))
        return (*head)->next->data;
}
int find_last(DLL_node **head)
{
    if (!(*head))
        return (*head)->prev->data;
}

DLL_node **merge_dll(DLL_node **A, DLL_node **B)
{
    if (!(*A) || !(*B))
        return (*A) ? A : B;
    (*B)->prev->prev = (*A);
    (*A)->next = (*B)->prev;
    (*B)->prev = (*A)->prev;
    (*A)->prev->next = (*B);
    *A = NULL;
    return B;
}
//return the index in rank_arr
int find(Rank_arr set[], int i)
{
    if (set[i].parent != i)
        set[i].parent = find(set, set[i].parent);
    return set[i].parent;
}
// A, B should be the root
void Union(Rank_arr *A, Rank_arr *B)
{
    if (A->rank > B->rank)
    {
        B->parent = A->parent;
        B->line_id = A->line_id;
        B->is_head = 0;
    }
    else if (A->rank < B->rank)
    {
        A->parent = B->parent;
        A->line_id = B->line_id;
        A->is_head = 0;
    }
    else
    {
        B->rank++;
        A->parent = B->parent;
        A->line_id = B->line_id;
        A->is_head = 0;
    }
}
void push(Line line, Rank_arr set[], int data)
{
    table[data] = 1;
    push_last(line.head, data);
    insert(line.root, data);
    // if first element in line
    if (!line.rank_root)
    {
        line.rank_root = &set[data];
    }
    else
    {
        Union(&set[data], line.rank_root);
    }
}
void merge(Line lineA, Line lineB)
{
    lineB.head = merge_dll(lineA.head, lineB.head);
    lineB.root = merge_ltt(lineA.root, lineB.root);
    lineA.root = NULL;
    Union(lineA.rank_root, lineB.rank_root);
    lineA.closed = 1;
}

int pop(Line line[], int id, Target **target)
{
    // no element or closed, return
    if (line[id].closed || (line[id].head && line[id].root))
        return 0;
    /* first check if the max, first and last really exist or not?*/
    if (!line[id].head)
    {
        if (!table[find_first(line[id].head)])
        {
            del_first(line[id].head);
            pop(line, id, &((*target)->next));
        }
        if (!table[find_last(line[id].head)])
        {
            del_last(line[id].head);
            pop(line, id, &((*target)->next));
        }
    }
    if (!line[id].root)
    {
        if (!table[find_max(line[id].root)])
        {
            del_max(line[id].root);
            pop(line, id, &((*target)->next));
        }
    }

    /* check the pop target is valid or not*/
    if (find_first(line[id].head) == (*target)->data)
    {
        g_pop_counter--;
        del_first(line[id].head);
        table[(*target)->data] = 0;
        (*target) = (*target)->next;
        goto possible;
    }

    if (find_last(line[id].head) == (*target)->data)
    {
        g_pop_counter--;
        del_last(line[id].head);
        table[(*target)->data] = 0;
        (*target) = (*target)->next;
        goto possible;
    }
    if (find_max(line[id].root) == (*target)->data)
    {
        g_pop_counter--;
        del_max(line[id].root);
        table[(*target)->data] = 0;
        (*target) = (*target)->next;
        goto possible;
    }
    if (table[(*target)->data])
    {
        impossible = 1;
        return 1;
    }
possible:
    return 0;
}