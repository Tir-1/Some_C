#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tree.h"

void obhod_for_time(Tree* t){
	if(t->root == NULL){return;}
	Node* ptr = t->root;
	while(ptr->left != NULL){ptr = ptr->left;}
	while(ptr != NULL){
		ptr = ptr->next;
	}
}

int find_Timing(){
	Tree* t = create_tree();
	if(t == NULL)return 1;
	int n = 10, key[10000], k, cnt = 1000000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0){
		for (i = 0; i < 10000; ++i){key[i] = rand() * rand();}
		for (i = 0; i < cnt; ){
			k = rand() * rand();
			if (insert(t, k, k)==0)++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		if(find(t, key[i]) != NULL)++m;
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 -n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
		}
	free_tree(t);
	return 1;
}

int insert_Timing(){
	Tree* t = create_tree();
	if(t == NULL)return 1;
	int n = 10, key[10000], k, cnt = 1000000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0){
		for (i = 0; i < 10000; ++i){key[i] = rand() * rand();}
		for (i = 0; i < cnt; ){
			k = rand() * rand();
			if (insert(t, k, k)==0)++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		if(insert(t, key[i], key[i]) == 0)++m;
		last = clock();
		printf("%d items was insert\n", m);
		printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 -n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
		}
	free_tree(t);
	return 1;
}

int delete_Timing(){
	Tree* t = create_tree();
	if(t == NULL)return 1;
	int n = 10, key[10000], k, cnt = 1000000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0){
		for (i = 0; i < 10000; ++i){key[i] = rand() * rand();}
		for (i = 0; i < cnt; ){
			k = rand() * rand();
			if (insert(t, k, k)==0)++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		if(delete(t, key[i]) == 0)++m;
		last = clock();
		printf("%d items was deleted\n", m);
		printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 -n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
		}
	free_tree(t);
	return 1;
}

int obhod_Timing(){
	Tree* t = create_tree();
	if(t == NULL)return 1;
	int n = 10, k, cnt = 1000000, i;

	clock_t first, last;
	srand(time(NULL));
	int key[10000];
	while (n-- > 0){
		for (i = 0; i < 10000; ++i){key[i] = rand() * rand();}
		for (i = 0; i < cnt; ){
			k = rand() * rand();
			if (insert(t, k, k)==0)++i;
		}
		first = clock();
	    obhod_for_time(t);
		last = clock();
		printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 -n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
		}
	free_tree(t);
	return 1;
}

int find2_Timing(){
	Tree* t = create_tree();
	if(t == NULL)return 1;
	int n = 10, key[10000], k, cnt = 1000000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0){
		for (i = 0; i < 10000; ++i){key[i] = rand() * rand();}
		for (i = 0; i < cnt; ){
			k = rand() * rand();
			if (insert(t, k, k)==0)++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 10000; ++i)
		if(find_spec(t, key[i]) != NULL)++m;
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %f\n", 10 - n, (10 -n)*cnt, (double)(last - first)/CLOCKS_PER_SEC );
		}
	free_tree(t);
	return 1;
}

int main(){
    find_Timing();
//	insert_Timing();
//	delete_Timing();
//  obhod_Timing();
//	find2_Timing();
return 0;
}
