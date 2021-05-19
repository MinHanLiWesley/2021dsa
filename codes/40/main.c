#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max_length (int)1e6
/*Ciucular doubly linked list implementation*/
typedef struct Node
{
    struct Node *prev;
    struct Node *next;
    int isoperator; //true = 1
    double operand;
    char operator;
    int size;
} Node;
Node *List_init();
void insert_operand(Node *head, double operrand);
void insert_operator(Node *head, char operator);
void delete_linked();
void print_list();

/*Stack implementation*/
typedef struct Stack
{
    char top;
    double top_double;
    int is_operator;
    int capacity;
    int size;
    char *operator;
    double *operand;
} Stack;
Stack *Stack_init();
int StackOverFlow(Stack *S);
int Stack_empty(Stack *S);
void enlarge(Stack *S);
char pop(Stack *S);
double pop_double(Stack *S);
void push(Stack *S, char data);
void push_double(Stack *S, double data);
void print_top_double(Stack *S);
void print_top(Stack *S);
/*
stack consists of three properties and two main functions:
1. size
2. top
3. base
4. capacity
5. operator => for infix to postfix
6. data => for evaluate
func:
a. pop()
b. push()
since dynamic..
enlarge()

some auxilary function:
1. StackOverFlow()
2. Stack_empty()
*/

/*Calculator function*/
void in2pos(Node *Postfix, Stack *S, char *Input, int Input_size);
double pos_evaluate(Node *Postfix, Stack *S, int Input_size);
void freeList( Node *head)
{
    Node *current = head;

    while (current != head)
    {
        Node *next = current->next;
        free(current);
        current = next;
    }
}
int main()
{
    while(1){
        char *input = (char *)malloc(sizeof(char) * max_length);
    int templen = 0;
    int count = 0;
    char buffer[20000];
    int eof = 1;
    int a = 0;
    while ((a = fscanf(stdin, "%20000[^\n]%n", buffer, &templen)) == 1)
    {
        eof = 0;
        memcpy(input + count, buffer, templen);
        count += templen;
    }
    if(eof){
        //printf("EOF!\n");
      return 0;
    }
        
    memset(input + count, '\0', 1);

    Node *Postfix = List_init();
    // insert_operand(Postfix,(double)9);
    // insert_operand(Postfix,(double)10);
    // insert_operand(Postfix,(double)11);
    // print_list(Postfix);
    Stack *S = Stack_init();
    int Input_size = strlen(input);
    in2pos(Postfix, S, input, Input_size);
    // print_list(Postfix);

    S->is_operator = 0;
    // insert_operand(head, (_ld)9);
    // insert_operator(head,'+');
    // printll(head);
    double result = pos_evaluate(Postfix, S, S->size);
    printf("%.15f\n", result);
    // printf("Result: %.15f\n", result);
    free(input);
    free(S->operator);
    free(S->operand);
    free(S);
    freeList(Postfix);
    //free(Postfix);
    //fflush(stdin);
    scanf("%*c");
    }
    

    // system("pause");
    return 0;
}


Stack *Stack_init()
{
    Stack *S = (Stack *)malloc(sizeof(Stack));
    S->top = '#';
    S->top_double = -1;
    S->is_operator = 1;
    S->capacity = 2000;
    S->size = 0;
    S->operator=(char *) malloc(S->capacity * sizeof(char));
    S->operand = (double *)malloc(S->capacity * sizeof(double));

    S->operator[0] = S->top;
    S->operand[0] = S->top_double;

    return S;
};

int StackOverFlow(Stack *S)
{
    return (S->capacity - 1 == S->size);
}

int Stack_empty(Stack *S)
{
    if (S->is_operator)
        return (S->operator[S->size] == '#');
    else
        return (S->operand[S->size] == -1);
}

void enlarge(Stack *S)
{
    int new_capacity = S->capacity * 2;
    S->capacity = new_capacity;
    S->operator=realloc(S->operator, new_capacity * sizeof(char));
    // printf("Enlarged!\n");
}

char pop(Stack *S)
{
    char pop_operator = S->operator[S->size];
    // printf("pop %c out of stack\n", pop_operator);
    S->top = S->operator[--S->size];
    // printf("top: %c \n", S->top);

    return pop_operator;
}
double pop_double(Stack *S)
{
    double pop_operand = S->operand[S->size];
    // printf("pop %f out of stack\n", pop_operand);
    S->top_double = S->operand[--S->size];
    // printf("top: %f \n", S->top_double);

    return pop_operand;
}
void print_top_double(Stack *S)
{
    // printf("top_operand: %f\n", S->top_double);
}
void print_top(Stack *S)
{
    // printf("top_operator: %c\n", S->top);
}

void push(Stack *S, char data)
{
    // printf("size: %d\n", S->size);
    S->operator[++S->size] = data;
    // printf("size: %d\n", S->size);
    S->top = S->operator[S->size];
    // printf("push %c into stack\n", S->top);
    if (StackOverFlow(S))
    {
        enlarge(S);
    }
}
/*linked list implementation*/
void push_double(Stack *S, double data)
{
    // printf("size: %d\n", S->size);
    S->operand[++S->size] = data;
    // printf("size: %d\n", S->size);
    S->top_double = S->operand[S->size];
    // printf("push %f into stack\n", S->top_double);
    if (StackOverFlow(S))
    {
        enlarge(S);
    }
}
Node *List_init()
{
    Node *head = (Node *)malloc(sizeof(Node));
    head->next = head;
    head->prev = head;
    head->size = 0;
    return head;
}

/*insert_operrand(head,head->list,operrand)*/
void insert_operand(Node *head, double operand)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->isoperator = 0;
    new->operand = operand;

    Node *temp = head->prev;
    head->prev = new;
    new->next = head;
    new->prev = temp;
    temp->next = new;
    head->size++;
    // printf("\nOperand Added!\n");
}

void insert_operator(Node *head, char operator)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->isoperator = 1;
    new->operator= operator;

    Node *temp = head->prev;
    head->prev = new;
    new->next = head;
    new->prev = temp;
    temp->next = new;
    head->size++;
    // printf("\nOperator Added!\n");
}

int preced(char ch)
{
    if (ch == '+' || ch == '-')
    {
        return 1; //Precedence of + or - is 1
    }
    else if (ch == '*' || ch == '/')
    {
        return 2; //Precedence of * or / is 2
    }
    else
    {
        return 0;
    }
}
void print_list(Node *head)
{
    if (head == NULL)
    {
        // printf("\nList is Empty!\n");
        return;
    }
    else
    {
        Node *temp = head->next;
        printf("\nHead ->");
        while (temp != head)
        {
            if (temp->isoperator)
                printf(" %c ->", temp->operator);
            else
                printf(" %f ->", temp->operand);
            temp = temp->next;
        }
        printf(" Head\n");
    }
}
void in2pos(Node *Postfix, Stack *S, char *Input, int Input_size)
{
    char tempInt[15] = {'\0'};
    int tempsize = 0;
    for (size_t i = 0; i < Input_size; i++)
    {
        if (isdigit(Input[i]))
        {
            tempInt[tempsize++] = Input[i];
            if (i != Input_size - 1)
                continue;
        }
        if (tempsize != 0)
        {
            double temp = atof((const char *)tempInt);
            // printf("temp: %f\n", temp);
            insert_operand(Postfix, temp);
            tempsize = 0;
            memset(tempInt,'\0',sizeof(tempInt));
        }
        if (Input[i] == '(')
        {
            push(S, '(');
            continue;
        }
        else if (Input[i] == ')')
        {
            while (S->top != '#' && S->top != '(')
            {
                insert_operator(Postfix, pop(S));
            }
            //clear '('
            pop(S);
        }
        else
        {
            if (preced(Input[i]) > preced(S->top))
                push(S, Input[i]);
            else
            {
                while (S->top != '#' && preced(Input[i]) <= preced(S->top))
                    insert_operator(Postfix, pop(S));
                if (i != Input_size - 1)
                    push(S, Input[i]);
            }
        }
    }
    while (S->top != '#')
    {
        print_top(S);
        // printf("%d\n", S->size);
        insert_operator(Postfix, pop(S));
        print_top(S);
        // printf("%d\n", S->size);
    }
}
double pos_evaluate(Node *Postfix, Stack *S, int Input_size)
{
    double A, B;
    for (Node *listptr = Postfix->next; listptr != Postfix; listptr = listptr->next)
    {
        if (listptr->isoperator)
        {
            B = pop_double(S);
            A = pop_double(S);
            switch (listptr->operator)
            {
            case '+':
                push_double(S, A + B);
                break;
            case '-':
                push_double(S, A - B);

                break;
            case '*':
                push_double(S, A * B);
                break;
            case '/':
                push_double(S, A / B);
                break;
            default:
                break;
            }
        }
        else
        {
            push_double(S, listptr->operand);
        }
    }
    return S->top_double;
}
