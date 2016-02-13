// will use an unbalanced BST to store words.

struct node{
    char *word;
    size_t count;
    struct node *left;
    struct node *right;
    struct node *top;
};

struct BST{
    struct node* root;
    int num_of_elements;
};

struct BST* init();
void* my_malloc(size_t sz);
struct node* create_node(char *wordIn);
void print_BST(struct node* ptr);
struct node* search(struct BST* bst, char *wordIn);
void enter(struct BST* bst, char *wordIn);
int get_count(struct BST* bst, char *wordIn);
void destruct_BST(struct BST* bst);
void destruct_nodes(struct node* n);