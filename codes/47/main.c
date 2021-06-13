#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef DEBUG

#endif

#define DEBUG
/*  garbled text table
    only contain 'a' ~ 'z' and 'A' ~ 'Z'
    everyone must minus '@' to get the index*/
int *table;

/* code the table and [0] is the char num */
void garbled_text_char(int **table, const char *garbled);

void table_init(int **table);

int *findheadtail(const char *Input, const char *garbled, int *table);

void printline(const char *input, int len, int head, int tail);


void table_init(int **table)
{
    if (*table)
    {
        for (size_t i = 0; i < 70; i++)
        {
            (*table)[i] = -1;
        }
    }
}
void garbled_text_char(int **table, const char *garbled)
{
    int len = strlen(garbled);
    int garbled_char_count = 0;
    for (size_t i = 0; i < len; i++)
    {
        if ((*table)[garbled[i] - '@'] == -1)
        {
            (*table)[garbled[i] - '@'] = 0;
            garbled_char_count++;
        }
        (*table)[garbled[i] - '@']++;
    }
    (*table)[0] = garbled_char_count;
}

int *findheadtail(const char *Input, const char *garbled, int *table)
{
    int len = strlen(Input);

    int head = 0, tail = 0,valid_count=0;
    // tail
    int counttable[70] = {0};
    int found_garbled = 0;
    int *headtail = (int *)calloc(2, sizeof(int));
    int end = 0;
    // printf("%d\n", table['e' - '@']);
    while (!end)
    {
        valid_count = 0;
        memset(counttable, 0, sizeof(counttable));

        //tail
        for (size_t i = tail; i < len; i++)
        {
            int idx = Input[i] - '@';
            // printf("%d\n", idx);
            counttable[idx] += 1;
            // printf("%d\n",counttable[idx]);
            // printf("%d\n", table[idx]);
            if (counttable[idx] == table[idx])
            {

                valid_count++;
            }

            if (valid_count == table[0])
            {
                tail = i;

                found_garbled = 1;
                headtail[1] = tail;
                // printf("HAHA!\n");
                break;
            }
            if (i == len - 1)
                end = 1;
        }
        if (!found_garbled)
            return headtail;

        //head
        for (size_t i = head; i <= tail; i++)
        {
            int idx = Input[i] - '@';
            counttable[idx]--;
            if (counttable[idx] < table[idx])
            {
                head = i;
                break;
            }
        }

        if ((tail - head) < (headtail[1] - headtail[0]))
        {
            headtail[1] = tail;
            headtail[0] = head;
        }
        tail++;
        // printf("tail: %d", tail);
    }
    return headtail;
}

void printline(const char *input, int len, int head, int tail)
{
    len = strlen(input);
    for (size_t i = 0; i < head; i++)
    {
        printf("%c", input[i]);
    }
    for (size_t i = tail + 1; i < len; i++)
    {
        printf("%c", input[i]);
    }
    printf("\n");
}

// void breakdown(const char* input, char* )
// {

// }

int main()
{
    table = (int *)malloc(70 * sizeof(int));
    table_init(&table);
    const char *garbled = "ee";
    char *input = "HelloWorldolleH";
    garbled_text_char(&table, garbled);
    int *result = findheadtail(input, garbled, table);
    // printf("%d\n", result[0]);
    // printf("%d\n", result[1]);

    printline(input, 0, result[0], result[1]);
    return 0;
}