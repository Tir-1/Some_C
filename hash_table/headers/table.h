#ifndef TABLEH_H
#define TABLEH_H
typedef struct KeySpace{
	int busy; // 0 свободная, 1 записано, -1 удалено
	unsigned int key;
	char* info;
} KeySpace;

typedef struct Table{
	KeySpace *ks;
	size_t msize;
	size_t step;
} Table;

typedef enum ERR{
	ERR_OK = 0,
	ERR_FAIL = 1,
	ERR_MEM = 2,
	ERR_CAPACITY = 3,
	ERR_FILE = 4,
	ERR_FORMAT = 5,	
	ERR_DUPLICATE = 6,
	ERR_ARGUM = 7,
	ERR_READ = 8,
	ERR_WRITE = 9,
} ERR;


ERR init_table(Table** t, size_t size, size_t step);
void clear_table(Table* t);
void free_table(Table** t);
size_t hash(unsigned int key);
ERR insert(Table* t, unsigned int key, char* info);
ERR delete(Table* t, unsigned int key);
void print_table(Table* t);
ERR find(Table* t, unsigned int key, size_t* pos);
ERR import(Table* t, const char* name);
ERR export(Table* t, const char* name);
#endif
