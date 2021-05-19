#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>

// Board max size 5 * 5
bool succeed = false;
bool valid(char *Board, int r, int c, int *clues);
void printboard(char *Board,int r,int c);
bool recur(char *Board, int r, int c, char *p, int *clues);
int main(int argc, char *argv[]) {
    int r, c;
    scanf("%d %d", &r, &c);
    char *Board = (char *)malloc(sizeof(char) * r * c);
    for (size_t i = 0; i < r*c; i++)
    {
        Board[i] = '\0';
    }
    
    // int *clues = (int *)malloc(sizeof(int) * 100);
    static int clues[300] = {};
    int rccount = r + c;
    int rc_i = 0;
    int size = 0;
    while(rc_i != rccount){
        fscanf(stdin, "%d", &clues[size]);
        int single_clue_count = clues[size];
        for (size_t i = 0; i < single_clue_count; i++)
        {
            fscanf(stdin, "%d", &clues[++size]);
        }
        size++;
        rc_i++;
    }

    // static int clues[] = {
    //     1, 4,
    //     1, 2,
    //     1, 3,
    //     1, 2,
    //     1, 4,
    //     2, 1, 1,
    //     2, 3, 1,
    //     3, 1, 1, 1,
    //     2, 1, 3,
    //     2, 1, 1};
    //static int clues[] = {2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 4, 2, 1, 2, 2, 1, 1, 2, 1, 2, 2, 1, 1};
    //static int clues[] = {2, 1, 1, 2, 1, 1, 2, 1, 1, 1, 4, 2, 1, 2, 2, 1, 1, 2, 1, 2, 2, 1, 1};
    //static int clues[] = {2, 1,1, 1, 4, 1, 3, 1, 2, 1, 4, 1, 3, 1, 3, 1, 1};
    // static int clues[] = {
    //     2,1,1,
    //     1,1,
    //     2,1,1,
    //     2,1,1,
    //     1,1,
    //     2,1,1
    // };
    bool success = false;
    success = recur(Board, r, c, &Board[0], clues);
    // if (success)
    //     printf("Great!\n");
    // else
    //     printf("Not good!\n");


    return 0;
}

bool valid(char *Board, int r,int c,int* clues){
    //r = row num
    // c = col num
    // index os clues:
    int clues_i = 0;
    //check rows
    for (size_t i = 0; i < r; i++)
    {
        //number of segment
        int Num_Seg = 0;
            // see if it is consecutive black
        bool cont = false;
        // elements of each segment
        int consec = 0;
        int j = 0;
        // checking array, store num_seg and consecs 
        int *check = malloc(sizeof(int) * 50);
        // index of consec in check[5], check[0] is num_seg
        int count = 1;
        while (j != c)
        {
            //white
            if(Board[i*c + j] == '_'){
                if (cont == true){
                    cont = false;
                }
                    
                if(consec != 0){
                    check[count] = consec;
                    consec = 0;
                    count++;
                }
            }
            // black
            else if(Board[i*c + j] == 'o'){
                if (cont == false)
                    Num_Seg += 1;
                cont = true;
                consec += 1;
            }
            j++;
        }
        if (consec != 0){
            check[Num_Seg] = consec;
            consec = 0;
        }
        check[0] = Num_Seg;
        if (check[0]!= clues[clues_i]){
            free(check);
            return false;
        }
        else{
            for (size_t i =  1 ; i <  1 + Num_Seg; i++)
            {
                if(check[i] != clues[clues_i + i]){
                    free(check);
                    return false;
                }
                    
            }
        }
        clues_i += (1 + Num_Seg);
        free(check);
    }

    //check columns
    for (size_t j = 0; j < c; j++)
    {
        int i = 0;
        //number of segment
        int Num_Seg = 0;
            // see if it is consecutive black
        bool cont = false;
        // elements of each segment
        int consec = 0;
        // checking array, store num_seg and consecs 
        int *check = malloc(sizeof(int) * 50);
        // index of consec in check[5], check[0] is num_seg
        int count = 1;
        while (i != r)
        {
            //white
            if(Board[i*c + j] == '_'){
                if (cont == true){
                    cont = false;
                }
                    
                if(consec != 0){
                    check[count] = consec;
                    consec = 0;
                    count++;
                }
            }
            // black
            else if(Board[i*c + j] == 'o'){
                if (cont == false)
                    Num_Seg += 1;
                cont = true;
                consec += 1;
            }
            i++;
        }
        if (consec != 0){
            check[Num_Seg] = consec;
            consec = 0;
        }
        check[0] = Num_Seg;
        if (check[0]!= clues[clues_i]){
            free(check);
            return false;
        }
        else{
            for (size_t i =  1 ; i <  1 + Num_Seg; i++)
            {
                if(check[i] != clues[clues_i + i]){
                    free(check);
                    return false;
                }
                    
            }
        }
        clues_i += (1 + Num_Seg);
        free(check);
    }
    return true;
}
char* find_blank(char *Board,int r,int c){
    for (size_t i = 0; i < r; i++)
    {
        for (size_t j = 0; j < c; j++)
        {
            if(Board[i*c+j] == '\0')
                return &Board[i * c + j];
        }
    }
    return NULL;
}
bool recur(char *Board,int r,int c,char* p,int *clues){
    
    if (Board[r*c-1] != '\0'){
        if(valid(Board,r,c,clues)){
            printboard(Board, r, c);
            return true;
        }
        else
            return false;
    }
    else{
          
          *p = 'o';
          p++;
          succeed = recur(Board,  r,  c, p, clues);
          if(succeed)
              return true;
          p--;
          *p = '_';
          p++;
          succeed = recur(Board,  r,  c, p, clues);
          if(succeed)
              return true;
          p--;
          *p = '\0';
          return false;
                
        
        
    }
}

void printboard(char *Board,int r,int c){
    for (size_t i = 0; i < r; i++)
    {
        for (size_t j = 0; j < c; j++)
        {
            printf("%c", Board[i * c + j]);
        }
        printf("\n");
    }
    
}