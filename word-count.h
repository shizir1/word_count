struct wordN{
    char **words;
    int *counts;
    int N;
    int empty_spot;
};

struct wordN* init_wordN(int N);
void print_wordN(struct wordN *wn);
int first_empty_spot(struct wordN *wn);
int find_smallest_spot(struct wordN *wn, int nc);
void sort_store(struct wordN *wn, int, int);
int partition(struct wordN *, int, int);
void swap_counts(struct wordN *, int, int);
void swap_words(struct wordN *, int, int);
void destruct(struct wordN *wn);