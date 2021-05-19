
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int ARRAYSIZE = 852;
typedef int BigInt[852];
int INPUTSIZE = 258;
// BigInt is address of the start of an array with 852 elements; aka a pointer

void Initalize_char(char *Num, int size, char initial)
{
    //set initial char to array

    for (int i = 0; i < size; i++)
    {

        Num[i] = initial;
    }

    return;
}

void Initalize(int *Num, int size, int initial)
{

    //set initial int to array
    for (int i = 0; i < size; i++)
    {

        Num[i] = initial;
    }

    return;
}

int GetLen_dec(int *Number)
{
    for (int i = INPUTSIZE - 1; i > 0; i--)
    {
        if (Number[i] != 0)
            return (i + 1);
    }
    return 1;
}
int GetLen(BigInt Number)
{
    // 1 ~ 851 digits
    // 0 is overflow check
    // return 1 ~ 851

    for (int i = ARRAYSIZE - 1; i > 0; i--)
    {
        if (Number[i] == 1)
            return (i + 1);
    }

    // 0 has length 1
    return 1;
}
int Transform_dec(int *Input, int size)
{
    int out = 0;
    for (int i = 0; i < size; i++)
    {
        out += Input[i] * pow(10, i);
    }

    return out;
}

bool is_zero(int *Input, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        if (Input[i] != 0)
            return false;
    }
    return true;
}
int *Dec2Bi(int *Input, int Dec_size)
{
    // decimal to binary

    int *Output = malloc(sizeof(int) * ARRAYSIZE);
    Initalize(Output, ARRAYSIZE, 0);
    int BiIndex = 0;

    while (1)
    {
        int carry = 0;
        int temp = 0;
        int quotient = 0;
        int remainder = 0;

        //check if Input = 0
        // printf("a: %d\n", a);
        if (is_zero(Input, Dec_size))
            break;

        // index 0 = units digits
        // index ++ -> higher digits
        // index size -1 = should always be zero; handling overflow
        for (int i = Dec_size - 2; i >= 0; i--)
        {
            temp = carry * 10 + Input[i];
            quotient = temp / 2;
            remainder = temp % 2;
            carry = remainder;
            Input[i] = quotient;
        }

        Output[BiIndex] = remainder;
        BiIndex++;
    }

    return Output;
}

int *Bi2Dec(int *Input, int Bi_size)
{
    // decimal to binary

    int *Output = malloc(sizeof(int) * INPUTSIZE);
    Initalize(Output, INPUTSIZE, 0);
    int DecIndex = 0;

    while (1)
    {
        int carry = 0;
        int temp = 0;
        int quotient = 0;
        int remainder = 0;

        //check if Input = 0
        // printf("a: %d\n", a);
        if (is_zero(Input, Bi_size))
            break;

        // index 0 = units digits
        // index ++ -> higher digits
        // index size -1 = should always be zero; handling overflow
        for (int i = Bi_size - 2; i >= 0; i--)
        {
            temp = carry * 2 + Input[i];
            quotient = temp / 10;
            remainder = temp % 10;
            carry = remainder;
            Input[i] = quotient;
        }

        Output[DecIndex] = remainder;
        DecIndex++;
    }

    return Output;
}

int Transform(BigInt Input)
{
    //binary to int
    int out = 0;
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        out += Input[i] * pow(2, i);
    }

    return out;
}

int *Addition(BigInt A, BigInt B)
{

    int *Sum = malloc(sizeof(int) * ARRAYSIZE);

    int carry = 0;

    for (int i = 0; i < ARRAYSIZE; i++)
    {

        if (A[i] + B[i] + carry < 2)
        {
            Sum[i] = A[i] + B[i] + carry;

            carry = 0;
        }
        else
        {
            //overflow digit is just for checking, no need to minus 2
            if (i != ARRAYSIZE - 1)
            {
                Sum[i] = A[i] + B[i] + carry - 2;
                carry = 1;
            }
            else
            {
                Sum[i] = A[i] + B[i] + carry;
            }
        }
    }

    // overflow
    if (Sum[ARRAYSIZE - 1] != 0)
    {
        Sum[ARRAYSIZE - 1] = 0;
    }

    return Sum;
}
int *Multiplication(BigInt A, BigInt B)
{
    //no need to care about onreflow digit
    //i guess

    int *product = malloc(sizeof(int) * ARRAYSIZE);
    Initalize(product, ARRAYSIZE, 0);
    BigInt tmp;

    int lenA = GetLen(A);

    for (size_t i = 0; i < GetLen(B); i++)
    {
        if (B[i] == 1)
        {
            Initalize(tmp, ARRAYSIZE, 0);

            for (size_t j = 0; j < lenA; j++)
            {
                tmp[i + j] = A[j];
            }
            int *tmp2 = Addition(tmp, product);
            for (size_t k = 0; k < ARRAYSIZE; k++)
            {
                product[k] = tmp2[k];
            }
            free(tmp2);
        }
    }

    return product;
}

void Division(BigInt A)
{
    //no need to care about overflow digit
    //i guess

    for (int i = 0; i <= ARRAYSIZE - 2; i++)
    {
        A[i] = A[i + 1];
    }
    A[ARRAYSIZE - 2] = 0;
    A[ARRAYSIZE - 1] = 0;
}
int *Subtraction(BigInt A, BigInt B)
{
    //A > B
    int *diff = malloc(sizeof(int) * ARRAYSIZE);
    // 一補數
    int B_comp[ARRAYSIZE];
    Initalize(B_comp, ARRAYSIZE, 0);
    int one[ARRAYSIZE];
    Initalize(one, ARRAYSIZE, 0);
    one[0] = 1;

    for (int i = 0; i < ARRAYSIZE - 1; i++)
    {
        if (B[i] == 1)
        {
            B_comp[i] = 0;
        }
        else
        {
            B_comp[i] = 1;
        }
    }
    int *tmp = Addition(B_comp, one);
    for (size_t i = 0; i < ARRAYSIZE; i++)
    {
        B_comp[i] = tmp[i];
    }
    free(tmp);

    diff = Addition(A, B_comp);

    return diff;
}

bool Bigthan(BigInt A, BigInt B, int size)
{

    for (int i = size - 1; i >= 0; i--)
    {
        if (A[i] > B[i])
        {
            return true;
        }
        else if (A[i] < B[i])
        {
            return false;
        }
    }
    return false;
}

void swap(int **A, int **B)
{
    int *temp;
    temp = *A;
    *A = *B;
    *B = temp;
}

bool equal2zero(int *A, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (A[i] != 0)
            return false;
    }
    return true;
}
bool isEven(int *A)
{
    if (A[0] == 1)
        return false;
    else
        return true;
}
int *GCD(BigInt A, BigInt B)
{
    BigInt two = {0};
    two[1] = 1;

    if (Bigthan(A, B, ARRAYSIZE))
        swap(&A, &B);

    BigInt ans = {1};
    while (!equal2zero(A, ARRAYSIZE) && !equal2zero(B, ARRAYSIZE))
    {

        if (isEven(A) && isEven(B))
        {
            int *tmp = Multiplication(ans, two);
            for (size_t i = 0; i < GetLen(tmp); i++)
            {
                ans[i] = tmp[i];
            }
            free(tmp);

            Division(A);
            Division(B);
        }
        else if (isEven(A))
        {
            Division(A);
        }
        else if (isEven(B))
        {
            Division(B);
        }
        if (Bigthan(A, B, ARRAYSIZE))
            swap(&A, &B);

        B = Subtraction(B, A);
    }
    int *gcd = Multiplication(A, ans);

    return gcd;
}

int main(void)
{

    BigInt two = {0};
    two[1] = 1;

      char Input1[1000];
      int INPUTSIZE_tmp = sizeof(Input1) / sizeof(Input1[0]);
      char Input2[INPUTSIZE_tmp];
      Initalize_char(Input1, INPUTSIZE_tmp, '0');
      Initalize_char(Input2, INPUTSIZE_tmp, '0');

      // printf("\n");
      // printf("Input1: \n");
      scanf("%s %s", Input1, Input2);
          //decimal array
      int Input1_dec[INPUTSIZE];
      int Input2_dec[INPUTSIZE];
      Initalize(Input1_dec, INPUTSIZE, 0);
      Initalize(Input2_dec, INPUTSIZE, 0);

      for (int i = strlen(Input1) - 1, dec_count = 0; i >= 0; i--, dec_count++)
      {
          Input1_dec[dec_count] = Input1[i] - '0';
      }
      for (int i = strlen(Input2) - 1, dec_count = 0; i >= 0; i--, dec_count++)
      {
          Input2_dec[dec_count] = Input2[i] - '0';
      }
      // printf("Input_dec\n");

      //binary array
      int *A;
      A = Dec2Bi(Input1_dec, INPUTSIZE);
      int *B;
      B = Dec2Bi(Input2_dec, INPUTSIZE);

      // int A_int = Transform(A);
      // printf("A_before\n");
      // printf("A_int: %d\n", A_int);

      // int B_int = Transform(B);
      // printf("B_int: %d\n", B_int);

      int *gcd = GCD(A, B);
      int *gcddec = Bi2Dec(gcd, ARRAYSIZE);

      // printf("%d\n",GetLen_dec(gcddec));

      for (int i = GetLen_dec(gcddec)-1; i >= 0; i--)
      {
          printf("%d", gcddec[i]);
      }
      // printf("\n");
      // int abc = Transform(gcd);
      // printf("gcd: %d\n", abc);

      free(A);
      free(B);
      free(gcd);
        


        
    

    return 0;
}
