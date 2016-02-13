#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "word-count.h"
#include "word-store.h"


/*  Initialize and return the data structure to store first N words */
struct wordN *
init_wordN(int N){
    struct wordN* result = (struct wordN *) my_malloc(sizeof(struct wordN));
    result->words = (char **) my_malloc(sizeof(char *) * N);
    result->counts = (int *) my_malloc(sizeof(int) * N);
    result->N = N;
    result->empty_spot = 0;
    return result;
}

/*  Return first empty spot in the data structure.
    If there's no empty spot, return -1 */
int
first_empty_spot(struct wordN *wn){
    if(wn->empty_spot == wn->N){
        return -1;
    }
    return wn->empty_spot++;
}

/*  Finds the element with the smallest word count in the data structure.
    Returns it, if it is also smaller than the new element's word count. Otherwise returns -1. */
int
find_smallest_spot(struct wordN *wn, int nc){
    int smallest_spot = -1;
    int smallest_value = nc;
    for(int i = 0; i<wn->N; i++){
        if(wn->counts[i] < smallest_value){
            smallest_spot = i;
            smallest_value = wn->counts[i];
        }
    }
    
    return smallest_spot;
}

/*  Store first N words in the given data structure recursively. */
void
catch_words(struct wordN *wn, struct node *ptr){
    if(ptr == NULL){return;}
    
    catch_words(wn, ptr->left);
    
    int spot = first_empty_spot(wn);
    if(spot != -1){
        int l = strlen(ptr->word) + 1;
        wn->words[spot] = (char *) my_malloc(sizeof(char) * l);
        strcpy(wn->words[spot], ptr->word);
        wn->counts[spot] = ptr->count;
    }
    else{
        spot = find_smallest_spot(wn, ptr->count);
        if(spot != -1){
            free(wn->words[spot]);
            int l = strlen(ptr->word) + 1;
            wn->words[spot] = (char *) my_malloc(sizeof(char) * l);
            strcpy(wn->words[spot], ptr->word);
            wn->counts[spot] = ptr->count;
        }
    }
    
    catch_words(wn, ptr->right);
    
    return;
}

/*  Quicksort */
void
sort_store(struct wordN *wn, int low, int high){
    if(low<high){
        int piv = partition(wn, low, high);
        sort_store(wn, low, piv-1);
        sort_store(wn, piv+1, high);
    }
}

/* Quicksort partition */
int
partition(struct wordN *wn, int low, int high){
    int x = wn->counts[high];
    int i = low-1;
    for(int j = low; j<high; j++){
        if(wn->counts[j] > x){
            i++;
            swap_counts(wn, i, j);
            swap_words(wn, i, j);
        }
    }
    swap_counts(wn, i+1, high);
    swap_words(wn, i+1, high);
    return i+1;
}

/* Quicksort helper function... */
void
swap_counts(struct wordN *wn, int i, int j){
    int temp = wn->counts[i];
    wn->counts[i] = wn->counts[j];
    wn->counts[j] = temp;
    
}

/* Another quicksort helper function... */
void
swap_words(struct wordN *wn, int i, int j){
    if(i == j){return;}
    
    int l = strlen(wn->words[i]) + 1;
    char *temp = (char *) my_malloc(sizeof(char) * l);
    strcpy(temp, wn->words[i]);
    
    free(wn->words[i]);
    
    l = strlen(wn->words[j]) + 1;
    wn->words[i] = (char *) my_malloc(sizeof(char) * l);
    strcpy(wn->words[i], wn->words[j]);
    
    free(wn->words[j]);
    
    l = strlen(temp) + 1;
    wn->words[j] = (char *) my_malloc(sizeof(char) * l);
    strcpy(wn->words[j], temp);
    
    free(temp);
}

/*  Prints what's inside the data structure. */
void
print_wordN(struct wordN *wn){
    for(int i = 0; i<wn->N; i++){
        printf("%s %d\n", wn->words[i], wn->counts[i]);
    }
}

/*  Free dynamically allocated memory. */
void
destruct(struct wordN *wn){
    for(int i = 0; i < wn->empty_spot; i++){
        free(wn->words[i]);
    }
    free(wn->words);
    free(wn->counts);
    
    free(wn);
}


