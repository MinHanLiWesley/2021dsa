#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
/* havent free*/

int g_pop_counter;
int target_idx = 0;
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
    DLL_node *head;
    Rank_arr *rank_root;
    int closed;
    int line_id;

} Line;

typedef struct operation
{
    char op;
    int A;
    int B;
} Operation;
int *table;

LTT_Node *merge_ltt(LTT_Node *A, LTT_Node *B);
void insert(LTT_Node **root, int data);
int find_max(LTT_Node *root);
void del_max(LTT_Node **root);
void push_last(DLL_node **head, int data);
void del_last(DLL_node **head);
void del_first(DLL_node **head);
int find_first(DLL_node *head);
int find_last(DLL_node *head);
DLL_node *merge_dll(DLL_node *A, DLL_node *B);
void push(Line *line, Rank_arr *set[], int data, int line_id);
void merge(Line *lineA, Line *lineB, int B);
int pop(Line line[], int id, int *target);
int find(Rank_arr *set[], int i);
Rank_arr *Union(Rank_arr *A, Rank_arr *B, int B_id);
int distance(LTT_Node *node);
void swap_children(LTT_Node *node);
LTT_Node *merge_node(LTT_Node *h1, LTT_Node *h2);
void free_heap(LTT_Node **node);
void free_line(Line *line);
int distance(LTT_Node *node)
{
    if (node == NULL)
        return -1;
    return node->depth;
}
void swap_children(LTT_Node *node)
{
    LTT_Node *temp = node->left;
    node->left = node->right;
    node->right = temp;
}
LTT_Node *merge_node(LTT_Node *h1, LTT_Node *h2)
{
    if (h1->left == NULL)
        h1->left = h2;
    else
    {
        h1->right = merge_ltt(h1->right, h2);
        if (h1->left->depth < h1->right->depth)
            swap_children(h1);
        h1->depth = 1 + distance(h1->right);
    }
    return h1;
}

LTT_Node *merge_ltt(LTT_Node *h1, LTT_Node *h2)
{
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;
    if (h1->data > h2->data)
        return merge_node(h1, h2);
    else
        return merge_node(h2, h1);
}
void insert(LTT_Node **root, int data)
{
    LTT_Node *temp = (LTT_Node *)malloc(sizeof(LTT_Node));
    temp->data = data;
    temp->depth = 0;
    temp->left = temp->right = NULL;

    *root = merge_ltt(*root, temp);
}

int find_max(LTT_Node *root)
{
    return root->data;
}

void del_max(LTT_Node **root)
{
    LTT_Node *temp = merge_ltt((*root)->left, (*root)->right);
    free(*root);
    *root = temp;
}

void push_last(DLL_node **head, int data)
{
    DLL_node *new_node = (DLL_node *)malloc(sizeof(DLL_node));
    new_node->data = data;
    if (!(*head))
    {
        (*head) = new_node;
        new_node->next = new_node->prev = new_node;
    }
    else
    {
        DLL_node *last = (*head)->prev;
        last->next = new_node;
        new_node->prev = last;
        (*head)->prev = new_node;
        new_node->next = (*head);
    }

    return;
}

void del_last(DLL_node **head)
{
    if (!(*head))
        return;

    /*more than one element*/
    if ((*head) != (*head)->next)
    {
        DLL_node *last = (*head)->prev;
        last->prev->next = *head;
        (*head)->prev = last->prev;
        free(last);
        last = NULL;
    }
    else
    {
        free(*head);
        *head = NULL;
    }
}

void del_first(DLL_node **head)
{
    if (!(*head))
        return;
    /*more than one element*/

    if ((*head) != (*head)->next)
    {
        DLL_node *first = (*head);
        DLL_node *last = (*head)->prev;
        *(head) = (*head)->next;
        (*head)->prev = last;
        last->next = (*head);

        free(first);
        first = NULL;
    }
    else
    {
        free(*head);
        *head = NULL;
    }
}

int find_first(DLL_node *head)
{
    if (head)
        return head->data;
    else
        return -1;
}
int find_last(DLL_node *head)
{
    if (head)
        return head->prev->data;
    else
        return -1;
}

DLL_node *merge_dll(DLL_node *A, DLL_node *B)
{
    if (!A || !B)
        return A ? A : B;
    A->next = B->prev;
    B->prev->prev = A;
    B->prev = A->prev;
    A->prev->next = B;
    return B;
}
//return the index in rank_arr
int find(Rank_arr *set[], int i)
{
    if ((*set + i)->parent != i)
        (*set + i)->parent = find(set, (*set + i)->parent);
    return (*set + i)->parent;
}
// A, B should be the root
Rank_arr *Union(Rank_arr *A, Rank_arr *B, int B_id)
{

    if (!A || !B)
    {
        return A ? A : B;
    }

    if (A->rank > B->rank)
    {
        B->parent = A->parent;
        B->is_head = 0;
        return A;
    }
    else if (A->rank < B->rank)
    {
        A->parent = B->parent;
        A->is_head = 0;
    }
    else
    {
        B->rank++;
        A->parent = B->parent;
        A->is_head = 0;
    }
    return B;
}

void push(Line *line, Rank_arr *set[], int data, int line_id)
{
    table[data] = 1;
    push_last(&(line->head), data);
    insert(&(line->root), data);
    (*set + data)->line_id = line_id;
    Rank_arr *temp = (*set + data);
    // assert(temp->parent = data);
    // assert(temp->rank == 0);
    // if first element in line
    if (!line->rank_root)
    {
        temp->line_id = line_id;
        line->rank_root = temp;
    }
    else
    {
        line->rank_root = Union(temp, line->rank_root, line_id);
    }
}
void merge(Line *lineA, Line *lineB, int B)
{
    if (lineA->head || lineB->head)
    {
        lineB->head = merge_dll(lineA->head, lineB->head);
        lineA->head = NULL;
    }
    if (lineA->root || lineB->root)
    {
        lineB->root = merge_ltt(lineA->root, lineB->root);
        lineA->root = NULL;
    }
    if (lineA->rank_root || lineB->rank_root)
    {
        lineB->rank_root = Union((lineA->rank_root), (lineB->rank_root), B);
        lineB->rank_root->line_id = B;
        lineA->rank_root = NULL;
    }
}

int pop(Line line[], int id, int *target)
{
    // no element or closed, return
    // printf("%d\n", target[target_idx]);
    // if (line[id].closed || (!line[id].head && !line[id].root))
    //     return 0;
    /* first check if the max, first and last really exist or not?*/

    while (!table[find_first(line[id].head)] && line[id].head)
    {
        del_first(&(line[id].head));
    }
    while (!table[find_last(line[id].head)] && line[id].head)
    {
        del_last(&(line[id].head));
    }

    while (!table[find_max(line[id].root)] && line[id].root)
    {
        del_max(&(line[id].root));
    }

    /* check the pop target is valid or not*/
    if (find_first(line[id].head) == target[target_idx])
    {
        // printf("f:%d -->", (*target)->data);
        g_pop_counter--;
        del_first(&(line[id].head));
        table[target[target_idx]] = 0;
        target_idx++;
        goto possible;
    }

    if (find_last(line[id].head) == target[target_idx])
    {
        // printf("l:%d -->", (*target)->data);
        g_pop_counter--;
        del_last(&(line[id].head));
        table[target[target_idx]] = 0;
        target_idx++;
        goto possible;
    }

    if (find_max(line[id].root) == target[target_idx])
    {
        // printf("m:%d -->", (*target)->data);
        g_pop_counter--;
        del_max(&(line[id].root));
        table[target[target_idx]] = 0;
        target_idx++;
        goto possible;
    }
    if (table[target[target_idx]])
    {
        impossible = 1;
        return 1;
    }
possible:
    if (!line[id].head || !line[id].root)
        free_line(&line[id]);
    return 0;
}

void free_heap(LTT_Node **node)
{
    if (*node)
    {
        free_heap(&(*node)->left);
        free_heap(&(*node)->right);
        free((*node));
        (*node) = NULL;
    }
}
void free_list(DLL_node **head)
{
    if (*head)
    {
        DLL_node *tmp, *curr;
        curr = (*head)->next;
        while (curr != *head)
        {
            tmp = curr->next;
            free(tmp);
            curr = tmp;
        }
        *head = NULL;
    }
    // printf("All nodes are deleted successfully.\n");
}

void free_line(Line *line)
{
    if (line->head)
        free_list(&(line->head));
    if (line->root)
        free_heap(&(line->root));
}

int main()
{
    int T, N, L, O;
    scanf(" %d", &T);
loop:
    while (T--)
    {
        target_idx = 0;
        scanf(" %d %d %d", &N, &O, &L);
        g_pop_counter = N;
        //table
        table = (int *)malloc(sizeof(int) * (N + 1));
        int *target = (int *)malloc(sizeof(int) * N);
        for (size_t i = 0; i < (N + 1); i++)
        {
            table[i] = 0;
        }

        //
        Operation *operation = (Operation *)malloc(sizeof(Operation) * O);
        Line *line = (Line *)malloc(sizeof(Line) * L);
        for (size_t i = 0; i < L; i++)
        {
            line[i].head = NULL;
            line[i].rank_root = NULL;
            line[i].root = NULL;
        }

        Rank_arr *set = (Rank_arr *)malloc(sizeof(Rank_arr) * (N + 1));
        for (size_t i = 1; i < (N + 1); i++)
        {
            set[i].is_head = 1;
            set[i].line_id = 0;
            set[i].parent = i;
            set[i].rank = 0;
        }

        for (size_t i = 0; i < O; i++)
        {
            char input[20];
            scanf("%s", input);
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

        for (int i = 0; i < N; i++)
        {
            scanf(" %d", &target[i]);
        }
        for (size_t i = 0; i < O; i++)
        {
            int A = operation[i].A;
            int B = operation[i].B;
            if (operation[i].op == 'p')
            {
                //first check target and input
                if (A == target[target_idx])
                {
                    // printf("s:%d -->", curr->data);
                    target_idx++;
                    g_pop_counter--;
                }
                else
                {
                    push(&line[B], &set, A, B);
                    table[A] = 1;
                }

                while (table[target[target_idx]])
                {
                    if (g_pop_counter < 0)
                    {
                        printf("error!\n");
                        return 1;
                    }

                    if (target_idx == N)
                        break;
                    // pop would make curr --> curr.next
                    int line_id = set[find(&set, target[target_idx])].line_id;
                    if (pop(line, line_id, target))
                    {
                        printf("impossible\n");
                        free(table);
                        table = NULL;
                        for (int i = L - 1; i >= 0; i--)
                        {
                            free_line(&line[i]);
                        }
                        free(line);
                        line = NULL;
                        free(operation);
                        operation = NULL;
                        free(set);
                        set = NULL;
                        free(target);
                        target = NULL;

                        goto loop;
                    }
                }
            }
            // merge
            else
            {
                merge(&line[A], &line[B], B);
            }
        }
        printf("possible\n");
        for (int i = L - 1; i >= 0; i--)
        {
            free_line(&line[i]);
        }
        free(line);
        line = NULL;
        free(table);
        table = NULL;
        free(operation);
        free(set);
        set = NULL;
        free(target);
        target = NULL;
    }

    return 0;
}
