#include "tree.h"

struct qNode{
    tNode* node;
    struct qNode* next;
};
typedef struct qNode qNode;

qNode* qCreateNode(tNode* node){
    qNode* new = (qNode*)malloc(sizeof(qNode));
    new->node = node;
    new->next = NULL;
    return new;
}


void push(qNode *head, tNode* node){
    qNode* tail = head;
    qNode* newNode = qCreateNode(node);
    if (newNode == NULL)
        return;
    
    if (head == NULL)
        return;

    while (tail->next != NULL)  
        tail = tail->next; 
    tail->next = newNode;
}

tNode* pop(qNode** head){
    tNode* node = (*head)->node;
    qNode* tmp = *head;
    *head = (*head)->next;
    free(tmp);
    return node;
}

qNode* sortQueue(qNode* head){
    qNode* q = head;
    while(q != NULL){
        qNode* inner = q;
        while(inner != NULL){
            if(q->node->amount > inner->node->amount){
                tNode* tmp = q->node;
                q->node = inner->node;
                inner->node = tmp;
            } 
            inner = inner->next;
        }
        q = q->next;
    }
    return head;
}

int qLength(qNode* head){
    qNode* tmp = head;
    int counter = 0;
    while(tmp != NULL){
        counter++;
        tmp = tmp->next;
    }
    return counter;
}

void freeQueue(qNode* head){
    if(head == NULL){
        return;
    }

   qNode* tmp;
   while (head != NULL){
       tmp = head;
       head = head->next;
       free(tmp);
   }
}