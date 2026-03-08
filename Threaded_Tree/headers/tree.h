typedef struct Node{
	unsigned int key;
	unsigned int info;
	struct Node* left;
	struct Node* right;
	struct Node* next;
	struct Node* pred;
	
} Node;
typedef struct Tree{
	Node* root;

} Tree;

Tree* create_tree();
void free_tree(Tree* t);
void obhod(Tree* t);
int insert(Tree* t, unsigned int key, unsigned int info);
int delete(Tree* t, unsigned int key);
Node* find(Tree* t, unsigned int key);
void formatprint(Tree* t);
Node* find_spec(Tree*t, unsigned int key);
int import(Tree* t, char* name);
int create_dot(Tree* t, char* name);
