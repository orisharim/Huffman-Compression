#include <stdlib.h>

typedef struct tNode tNode;

struct tNode{
    char val;
    unsigned int amount;
    tNode* right;
    tNode* left;
};


tNode* tCreateNode(char val, int amount){
    tNode* new = (tNode*)malloc(sizeof(tNode));
    new->amount = amount;
    new->val = val;
    new->right= NULL;
    new->left = NULL;
    return new;
}

void freeTree(tNode* node) {
    if(node == NULL) 
        return;

    freeTree(node->left);
    freeTree(node->right);   
    free(node);
} 
