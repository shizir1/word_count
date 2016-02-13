#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "word-store.h"


/*  Allocating memory.. */
struct BST*
init(){
    struct BST* bst = (struct BST *) my_malloc(sizeof(struct BST));
    bst->num_of_elements = 0;
    bst->root = NULL;
    return bst;
}

/*  Personal wrapper for malloc() */
void *
my_malloc(size_t sz){
    void *ptr;
    if (!(ptr = malloc(sz))){
        fprintf(stderr, "Memory error.\n");
        exit(100);
    }
    
    return ptr;
}

/*  Allocate memory for nodes   */
struct node*
create_node(char *wordIn){
    struct node *nNode = (struct node*) my_malloc(sizeof(struct node));
    nNode->word = wordIn;
    nNode->count = 1;
    nNode->left = NULL;
    nNode->right = NULL;
    nNode->top = NULL;
    
    return nNode;
}

/*  Mostly for testing purposes.    */
void
print_BST(struct node *ptr){
    if(ptr != NULL){
        print_BST(ptr->left);
        printf("%s %zu\n", ptr->word, ptr->count);
        print_BST(ptr->right);
    }
    return;
}

/*  Searches inside a unbalanced binary search tree */
struct node*
search(struct BST* bst, char *wordIn){
    struct node *ptr = bst->root;
    
    while(ptr != NULL && strcmp(wordIn, ptr->word) != 0){          // strcmp: for comparing 2 null-terminated!!! strings.
        if(strcmp(wordIn, ptr->word) < 0){
            ptr = ptr->left;
        }
        else{
            ptr = ptr->right;
        }
    }
    
    return ptr;
    
}

/*  Enters new word inside the search tree. If the word is already there, increases count. */
void
enter(struct BST* bst, char *wordIn){
    if(bst->num_of_elements == 0){
        struct node *nNode = create_node(wordIn);
        bst->root = nNode;
        bst->num_of_elements++;
        return;
    }
    
    struct node *result = search(bst, wordIn);
    if(result == NULL){                  //  If the word does not exist in the tree.
        struct node *nNode = create_node(wordIn);
        struct node *ptr = bst->root;
        struct node *before = ptr;
        int side = -1;   // 0 means left, 1 means right.
        
        while(ptr != NULL){
            before = ptr;
            if(strcmp(wordIn, ptr->word) < 0){
                ptr = ptr->left;
                side = 0;
            }
            else{
                ptr = ptr->right;
                side = 1;
            }
        }
        
        ptr = nNode;
        if(side == 0){
            before->left = ptr;
        }
        else if(side == 1){
            before->right = ptr;
        }
        else{
            fprintf(stderr, "This shouldn't happen (function: enter(char *))\n");
            exit(1);
        }
        
        ptr->top = before;
        bst->num_of_elements++;
        
    }
    else{                               //  If the word does exists in the tree.
        result->count++;
    }
}

int
get_count(struct BST* bst, char *wordIn){
    struct node *result = search(bst, wordIn);
    if(result == NULL){return -1;}
    return result->count;
}

/*  To free memory  */
void
destruct_BST(struct BST* bst){
    destruct_nodes(bst->root);
    free(bst);
}

void
destruct_nodes(struct node* n){

    if(n==NULL){return;}
    
    destruct_nodes(n->left);
    destruct_nodes(n->right);
    
    free(n->word);
    n->left = NULL;
    n->right = NULL;
    n->top = NULL;
    
    free(n);
    
    n= NULL;
}

