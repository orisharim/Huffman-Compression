#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

typedef enum{false, true} boolean; 

//compression and decompression functions
void compress(char*);
void extract(char*);

//side functions
qNode* countOccs(char* fileName);
tNode* buildHuffmanTree(qNode* occs);
void createCompressedFile(char* fileName, tNode* tree);
int getCharCode(tNode* tree, char c, int current);
int switchBit(int num, int bit, int pos);
int getBitAtPosition( int num, int pos);
int getCodeLength(tNode* tree, char c);


int main(int argc, char* args[]){
    if(argc != 3){
        fprintf(stderr, "must be 3 arguments");
        exit(1);
    }

    char* fileName = args[2];
    if(strcmp(args[1], "-c") == 0){
        compress(fileName);
    }
    else if(strcmp(args[1], "-d") == 0){
        extract(fileName);
    }
    else{
        fprintf(stderr, "use -c to compress a file ,-d to decompress/exctract a file");
        exit(1);
    }
    return 0;
}

void compress(char* fileName){
    qNode* l = countOccs(fileName);
    tNode* huffmanTree = buildHuffmanTree(l);
    
    createCompressedFile(fileName, huffmanTree);

    freeTree(huffmanTree);
    freeQueue(l);
}

void extract(char* fileName){
    if(strstr(fileName, ".huff") == NULL){
        fprintf(stderr, "cannot extract a file that is not compressed(.huff)");
        exit(1);
    }

    FILE* file = fopen(fileName, "rb");

    if(file == NULL){
        fprintf(stderr, "%s could not be opened", fileName);
        exit(1);
    }

    int length = fgetc(file);
    char val = fgetc(file);
    int amount = fgetc(file);
   
    qNode* queue = qCreateNode(tCreateNode(val, amount));

    for(int i = 1; i < length ; i++){
        char val = fgetc(file);
        int amount = fgetc(file);
        push(queue, tCreateNode(val, amount));
    }

    tNode* huffmanTree = buildHuffmanTree(queue);

    char name[strlen(fileName) - 4];
    for(int i = 0 ;i <= strlen(fileName) - 4; i++){
        name[i] = '\0';
    }
    memcpy(name, fileName, strlen(fileName) - 5);

    FILE* new = fopen(name, "wb");

    if(new == NULL){
        fprintf(stderr, "%s could not be opened", name);
        exit(1);
    }

    int pos = ftell(file);
    fseek(file, -1, SEEK_END);
    int amountOfBits = (ftell(file) - pos) * 8;
    fseek(file, pos, SEEK_SET);
    int bits[amountOfBits];
    int current;
    pos = 0;
    while((current = fgetc(file)) != EOF){
        for(int i = 0;i < 8;i++){
            bits[pos + i] = getBitAtPosition(current, 7 - i);
        }
        pos += 8;
    }

    fseek(file, -1, SEEK_END);
    int uselessBits = fgetc(file);

    tNode* tree = huffmanTree;
    for(int i = 0; i < (amountOfBits - uselessBits + 1); i++){
        if(tree->right == NULL && tree->left == NULL){
            fputc(tree->val, new);
            tree = huffmanTree;
        }

        if(bits[i] == 0){
            tree = tree->left;
        }
        else if(bits[i] == 1){
            tree = tree->right;
        }
        
    }
    
    freeTree(huffmanTree);    
    fclose(file);
}


void createCompressedFile(char* fileName, tNode* tree){
    char name[strlen(fileName) + 5];
    name[strlen(fileName) + 5] = '\0';
    strcpy(name, fileName);

    FILE* new = fopen(strcat(name, ".huff"), "w");
    
     if(new == NULL){
        fprintf(stderr, "%s could not be opened", name);
        exit(1);
    }
    
    qNode* occs = countOccs(fileName);

    qNode* queue = occs;
    putc(qLength(queue), new);//put amount of different characters

    //put characters + amount of occurences
    while(queue != NULL){
        putc(queue->node->val, new);
        putc(queue->node->amount, new);
        queue = queue->next;
    }

    freeQueue(occs);

    FILE* file = fopen(fileName, "rb");

    if(file == NULL){
        fprintf(stderr, "%s could not be opened", fileName);
        exit(1);
    }

    char current;
    int amountOfBits = 0;
    while((current = fgetc(file)) != EOF){
        amountOfBits += getCodeLength(tree, current);
    }
    int amountOfBytes = amountOfBits / 8;
    if(amountOfBits % 8 != 0){
        amountOfBytes++;
    }
    char codes[amountOfBytes];
    codes[amountOfBytes] = '\0';
    int pos = 0;
    int bitsEntered = 0;
    rewind(file);
    while((current = fgetc(file)) != EOF){
        int length = getCodeLength(tree, current);
        int code = getCharCode(tree, current, 0);
        for(int i = length - 1; i >= 0; i--){
            int bit = getBitAtPosition(code, i);
            codes[pos] = switchBit(codes[pos], bit, 7 - bitsEntered);
            bitsEntered++;
            if(bitsEntered == 8){
                pos++;
                bitsEntered = 0;
            }
        }
    }
    fputs(codes, new);
    fputc(amountOfBytes * 8 - amountOfBits, new); //put amount of useless bits at the end
    fclose(new);
    fclose(file);

}

int switchBit(int num, int bit, int pos) {
    int mask = 1 << pos;
    if (bit == 1)
        num = num | mask;
    else
        num = num & ~mask;
    return num;
}

int getBitAtPosition( int num, int pos) {
    int mask = 1 << pos;
    int bitValue = (num & mask) ? 1 : 0;
    return bitValue;
}

int getCharCode(tNode* tree, char c, int current){
    if(tree->val == c){
        return current;
    }
    else if(tree->left == NULL && tree->right == NULL){
        return 0;
    }
    int left = current << 1;
    int leftRes = getCharCode(tree->left, c, left);
    int right = left + 1;
    int rightRes = getCharCode(tree->right, c, right);
    if(leftRes != 0){
        return leftRes;
    }
    else if(rightRes != 0){
        return rightRes;
    }

    return 0;
}


int getCodeLength(tNode* tree, char c){
    if (tree == NULL){
        return -1;
    } 
    int len = -1;
    if ((tree->val == c) || (len = getCodeLength(tree->left, c)) >= 0 || (len = getCodeLength(tree->right, c)) >= 0)
        return len + 1;
    return len;
}

//gets a queue of characters and occurences of the characters
tNode* buildHuffmanTree(qNode* occs){
    qNode* queue = sortQueue(occs);
    
    while(qLength(queue) != 2){
        tNode* left = pop(&queue);
        tNode* right = pop(&queue);

        tNode* new = tCreateNode(0, left->amount + right->amount);
        new->left = left;
        new->right = right;
        push(queue, new);
        queue = sortQueue(queue);
    }
    tNode* left = pop(&queue);
    tNode* right = pop(&queue);

    tNode* new = tCreateNode(0, left->amount + right->amount);
    new->left = left;
    new->right = right;
    
    return new;
}

//counts the occurences of all the characters in a file and returns a queue of tree nodes with the character and the amount of occurences
qNode* countOccs(char* fileName){
    FILE* file = fopen(fileName, "r");
    qNode* head = qCreateNode(tCreateNode(fgetc(file), 1));
    char current;
    while((current = fgetc(file)) != EOF){
        qNode* tmp = head;
        boolean flag = true;
        while(tmp){
            if(tmp->node->val == current){
                tmp->node->amount++;
                flag = false;
            }
            tmp = tmp->next;
        }
        if(flag)
            push(head, tCreateNode(current, 1));    
        
    }
    return head;
}
