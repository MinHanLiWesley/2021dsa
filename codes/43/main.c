#include <stdio.h>
#include <stdlib.h>

typedef struct treeNode
{
    // struct treeNode *left;
    // struct treeNode *right;
    int leftID;
    int rightID;
    //for range determination
    // long int L;
    // long int R;
    long int key;
} treeNode;

treeNode *construction(treeNode link[], int size);
void give_range(treeNode link[], int idx, long int l, long int r);
void inorder(treeNode *root);
int count = 0;
int main()
{
    int N;
    scanf("%d", &N);
    int readline = 0;
    // treeNode *list = (treeNode *)malloc(sizeof(treeNode) * N);
    treeNode list[N];
    while (readline < N)
    {
        long int a;
        int b, c;

        scanf("%ld %d %d", &a, &b, &c);
        list[readline].key = a;
        list[readline].leftID = b - 1;
        list[readline].rightID = c - 1;
        // list[readline].L = -3;
        // list[readline].R = 10E10;
        readline++;
    }
    // treeNode *root = construction(list, N);
    // give_range(root, -1, 10E9 + 1);
    give_range(list, 0, -1, 10E9 + 1);
    // inorder(root);
    printf("%d\n", count);
    return 0;
}

// treeNode *construction(treeNode link[], int size)
// {
//     for (size_t i = 0; i < size; i++)
//     {
//         if (link[i].leftID == -2)
//         {
//             link[i].left = NULL;
//         }
//         else
//         {
//             link[i].left = &link[link[i].leftID];
//         }
//         if (link[i].rightID == -2)
//         {
//             link[i].right = NULL;
//         }
//         else
//         {
//             link[i].right = &link[link[i].rightID];
//         }
//     }
//     return &link[0];
// }

// void give_range(treeNode *root, long int l, long int r)
// {
//     if (root != NULL)
//     {
//         root->L = l;
//         root->R = r;
//         if (root->L < root->key && root->R > root->key)
//             count++;
//         give_range(root->left, root->L, root->key > root->R ? root->R : root->key);
//         give_range(root->right, root->key < root->L ? root->L : root->key, root->R);
//     }
// }

// void give_range(treeNode *root, long int l, long int r)
// {
//     if (root != NULL && l < r)
//     {
//         if (l < root->key && r > root->key)
//             count++;
//         give_range(root->left, l, root->key > r ? r : root->key);
//         give_range(root->right, root->key < l ? l : root->key, r);
//     }
// }
void give_range(treeNode link[], int idx, long int l, long int r)
{
    if (l < r)
    {
        long int key = link[idx].key;
        int leftID = link[idx].leftID;
        int rightID = link[idx].rightID;
        if (l < key && r > key)
            count++;
        if (leftID != -2)
            give_range(link, leftID, l, key > r ? r : key);
        if (rightID != -2)
            give_range(link, rightID, key < l ? l : key, r);
    }
}

// void inorder(treeNode *root)
// {
//     if (root != NULL)
//     {
//         inorder(root->left);
//         if (root->L < root->key && root->R > root->key)
//             count++;
//         inorder(root->right);
//     }
// }
