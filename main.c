/** 
 
 Serkan Hizir (shizir1@binghamton.edu)
 Systems Programming Project 1 -  Word Count
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "word-store.h"
#include "word-count.h"

int getWordLength(FILE *fp);
char *getWord(FILE *fp, int wn);
void read(struct BST* bst, struct BST* stop, FILE *fp, FILE *fp2);
void catch_words(struct wordN*, struct node*);
char *strdup(const char *s);

/*  Copied some of the code in getWordNumber(), getWord(), read() from:
    http://stackoverflow.com/questions/26349528/reading-word-by-word-from-text-file-in-c
    Modified them to meet the project requirements.
    TA said he will accept it for this time.
 */

/*
    Returns the next word's length. If it is the end of file, returns 0 to indicate EOF.
 */
int getWordLength(FILE *fp){
    int ch, i=0;
    
    while(EOF!=(ch=fgetc(fp)) && !isalnum(ch) && ch !=39);
    
    if(ch == EOF)
        return 0;
    
    do{
        i++;
    }while(EOF!=(ch=fgetc(fp)) && (isalnum(ch) || ch == 39) );
    
    return i;
}

/*
    Returns the next word. If it is the end of the file, returns NULL to indicate EOF.
 */
char *getWord(FILE *fp, int wn){
    char *word = (char *) my_malloc(sizeof(char) * (wn+1));
    int ch, i=0;

    while(EOF!=(ch=fgetc(fp)) && !isalnum(ch) && ch!=39);   //  Skip until there's EOF, alphanumerical character or single quote.
    if(ch == EOF)
        return NULL;
    
    do{ /* Since in project description it says: "Words which differ merely in case are regarded as identical." */
        word[i++] = tolower(ch);
    }while(EOF!=(ch=fgetc(fp)) && (isalnum(ch) || ch == 39));
    
    word[i]='\0';
    
    return word;
}

/*
    One file pointer calculates the length of the next word so that we can dynamically allocate memory for that length.
    Second file pointer writes the word into dynamically allocated memory.
 */
void read(struct BST* bst, struct BST* stop, FILE *fp, FILE *fp2){
    char *word;
    int wn = getWordLength(fp2);
    
    while(wn){
        word = getWord(fp, wn);
        wn = getWordLength(fp2);
        
        if(stop == NULL || search(stop, word) == NULL){enter(bst, word);}       //  If stop is NULL, it means we are reading STOP_WORDS file.
        else{free(word);}                                                       // This means the word is also in STOP_WORDS file, so we are not saving it.
    }
}

int
main(int argc, const char *argv[]){
    
    int N;
    struct BST *bst, *stop_words;   //  bst: to store words that are not in STOP_WORDS, stop_words: to store words that are in STOP_WORDS.
    struct wordN* wn;               //  to store N most frequently used word.
    FILE *fp, *fp2;
    
    // there should be at least 3 command line arguments.
    if ( argc < 4 ){
        fprintf(stderr, "usage: %s N STOP_WORDS FILE1...\n", argv[0] );
        return -1;
    }
    
    
    // checks if N is really a number before using atoi...
    for(size_t i = 0; i<strlen(argv[1]); i++){
        if(!isdigit(argv[1][i])){
            fprintf(stderr, "ERROR: N should only consist of digits!!!! (No, you cannot enter negative values)\nYou entered: %s\n", argv[1]);
            return -1;
        }
    }
    
    N = atoi(argv[1]);
    
    // N should be at least 1.
    if(N < 1){
        fprintf(stderr, "ERROR: N should be greater than 0.\n");
        return -1;
    }
    
    /*  Initialization */
    bst = init();
    stop_words = init();
    wn = init_wordN(N);
    
    for(int i = 2; i<argc; i++){
        fp = fopen(argv[i], "r");
        fp2 = fopen(argv[i], "r");
        
        if(fp == NULL || fp2 == NULL){
            fprintf(stderr, "ERROR: File: %s cannot be found or opened.\nPlease make sure you typed the file's name correctly.\n", argv[i]);
            destruct_BST(bst);
            destruct_BST(stop_words);
            destruct(wn);
            return -1;
        }
        
        if(i==2){read(stop_words, NULL, fp, fp2);}  // reading STOP_WORDS inside a seperate binary search tree. NULL means do not check for STOP_WORDS since we are reading STOP_WORDS.
        else{read(bst, stop_words, fp, fp2);}       // reading words which are not in STOP_WORDS inside a binary tree
        
        fclose(fp);
        fclose(fp2);
        
        fp = NULL;
        fp2 = NULL;
        
    }
    
    //  If N is greater than number of words in the input files which are not in STOP_WORDS, an error message is printed.
    if(N > bst->num_of_elements){
        fprintf(stderr, "ERROR: Cannot print %d most frequently used words since there are only %d words in the input files that are not in the stop_words file.\n", N, bst->num_of_elements);
        destruct_BST(bst);
        destruct_BST(stop_words);
        destruct(wn);
        return -1;
    }
    
    catch_words(wn, bst->root);         //  counts the words, stores N frequently used word into a dynamically allocated array.
    sort_store(wn, 0, N-1);             //  sorting the dynamically allocated array.
    print_wordN(wn);                    //  prints N frequently used word in decreasing order by count.
    
    /*  Free memories */
    destruct_BST(bst);
    destruct_BST(stop_words);
    destruct(wn);
    
    
    return 0;
}
