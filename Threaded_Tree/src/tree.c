#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include <string.h>

Tree* create_tree(){
	Tree* t = calloc(1, sizeof(Tree));
	return t;
}
/*
void destroy_nodes(Node* ptr){
	if(ptr != NULL){
		destroy_nodes(ptr->left);
		destroy_nodes(ptr->right);
		free(ptr);
	}
}*/
void destroy_nodes(Tree* t){
	Node* ptr = t->root;
	Node* next;
	if(ptr == NULL){return;}
	while(ptr->left != NULL){ptr = ptr->left;}
	while(ptr != NULL){
			next = ptr->next;
			free(ptr);
			ptr = next;
	}
}

void free_tree(Tree* t){
	if(t == NULL){return;}
	destroy_nodes(t);
	t->root = NULL;
	free(t);
}

void obhod(Tree* t){
	if(t->root == NULL){printf("Пусто\n");return;}
	Node* ptr = t->root;
	while(ptr->left != NULL){ptr = ptr->left;}
	while(ptr != NULL){
		printf("%u\n", ptr->info);
		ptr = ptr->next;
	}
	printf("\n");
}

Node* find(Tree* t, unsigned int key){
	Node* ptr = t->root;
	while(ptr != NULL){
		if(ptr->key == key){return ptr;}
		if(key < ptr->key){
			ptr = ptr->left;
		}
		else{
			ptr = ptr->right;
		}
	}	
	return NULL;
}

Node* find_spec(Tree* t, unsigned int max){
	Node* ptr = t->root;
	Node* result = NULL;
	while(ptr != NULL){
		if(ptr->key == max){return ptr;}
		if(ptr->key < max){
			result = ptr;
			ptr = ptr->right;
		}
		else{
			ptr = ptr->left;
		}
	}
	return result;
}

int insert(Tree* t, unsigned int key, unsigned int info){
	if(find(t, key) != NULL){return 1;}
	Node* n = malloc(sizeof(Node));
	if(n == NULL){return 2;}
	n->key = key;
	n->info = info;
	n->left = NULL;
	n->right = NULL;
	n->next = NULL;
	n->pred = NULL;
	if(t->root == NULL){
		t->root = n;
		return 0;
	}
	Node* ptr = t->root;
	Node* par = NULL;
	while(ptr != NULL){
		par = ptr;
		if(key < ptr->key){
			ptr = ptr->left;
		}
		else{
			ptr = ptr->right;
		}
	}
	if(key < par->key){
		par->left = n;
	}
	else{
		par->right = n;
	}
	if(par->left == n){
		n->next = par;
		if(par->pred != NULL){
			(par->pred)->next = n;
			n->pred = (par->pred);
		}
		par->pred = n;
	}
	if(par->right == n){
		n->next = par->next;
		if(par->next != NULL){par->next->pred = n;}
		n->pred = par;
		par->next  = n;
	}
	return 0;
}

Node* find_parent(Node* x, unsigned int key) {
    if (x == NULL || x->key == key){return NULL;}
    Node* ptr = x;
    Node* parent = NULL;
    while (ptr != NULL) {
        if (ptr->key == key) {return parent;}
		if (key < ptr->key){
            parent = ptr;
            ptr = ptr->left;
        } 
		else{
            parent = ptr;
            ptr = ptr->right;
        }
    }
    return NULL;
}

int delete(Tree* t, unsigned int key){
	Node* x = find(t, key);
	if(x == NULL){return 1;}
	Node* ptr = NULL;
	if(x->left == NULL || x->right == NULL){
		ptr = x;
		if(ptr->pred != NULL){
			(ptr->pred)->next = ptr->next;
		}
		if(ptr->next != NULL){
			(ptr->next)->pred = ptr->pred;
		}
	}
	else{
		ptr = x->next;
		x->next = x->next->next;
		if(x->next != NULL){
			x->next->pred = x;
		}
	}
	Node* p = ptr->right;
	if(ptr->left != NULL){
		p = ptr->left;
	}
	Node* par = find_parent(t->root, ptr->key);
	if(par == NULL){
		t->root = p;
	}
	else{
		if(par->left == ptr){par->left = p;}
		else{par->right = p;}
	}
	if(ptr != x){
		x->key = ptr->key;
		x->info = ptr->info;
	}
	free(ptr);
	return 0;
}

void putTree(Node *ptr, int level){
	if(ptr == NULL){return;}
	putTree(ptr->right, level + 1);
	for(int i = 0; i < level; i++){printf("   ");}
	printf("%d\n", ptr->key);
	putTree(ptr->left, level + 1);
}

void formatprint(Tree* t){
	if(t->root == NULL){printf("Дерево пустое\n");}
	else{
		printf("-------------------------------------------\n");
		putTree(t->root, 0);
		printf("-------------------------------------------\n");
	}
}

int import(Tree* t, char* name){
	destroy_nodes(t);
	t->root = NULL;
	FILE* file = fopen(name, "r");
	if(file == NULL){return 1;}
	char skey[256];
	char sinfo[256];
	while(fgets(skey, sizeof(skey), file)){
		if(fgets(sinfo, sizeof(sinfo), file)){
			unsigned int key, info;
			if(sscanf(skey, "%u%*[ \n]", &key) == 1 && sscanf(sinfo, "%u%*[ \n]", &info) == 1){
				int code = insert(t, key, info);
				if(code == 1){fclose(file);return 3;}
				if(code == 2){fclose(file);return 4;}
			}
			else{
				fclose(file);
				return 2;
			}
		}
		else{
			fclose(file);
			return 2;
		}		
	}
	fclose(file);
	return 0;
}

void element_dot(Node* ptr, FILE* file){
	if(ptr == NULL)return;
	 fprintf(file, "    node_%u [label=\"%u\"];\n", ptr->key, ptr->key);
	 if (ptr->left != NULL) {
        fprintf(file, "    node_%u -> node_%u [label=\"left\"];\n", ptr->key, (ptr->left)->key);
        element_dot(ptr->left, file);
    }
    if (ptr->right != NULL) {
        fprintf(file, "    node_%u -> node_%u [label=\"right\"];\n", ptr->key, (ptr->right)->key );
        element_dot(ptr->right, file);
    }
}

int create_dot(Tree* t, char* name){
	FILE* file = fopen(name, "w");
	if(file == NULL){return 1;}
	fprintf(file, "digraph BST {\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=green];\n");
    if(t->root != NULL){element_dot(t->root, file);}
	fprintf(file, "}\n");
	fclose(file);
	return 0;
}
