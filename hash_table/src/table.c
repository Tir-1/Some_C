#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"


unsigned int evklid(unsigned int a, unsigned int b) {
    while (b != 0) {
        unsigned int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


ERR init_table(Table** t, size_t size, size_t step){
	if (size == 0 || step == 0)return ERR_ARGUM;
	if(evklid(size, step) != 1)return ERR_ARGUM;
	*t = malloc(sizeof(Table));
	if((*t) == NULL)return ERR_MEM;
	(*t)->ks = malloc(sizeof(KeySpace) * size);
	if((*t)->ks == NULL){free(*t);return ERR_MEM;}
	for(size_t i = 0; i < size; i++){
		(*t)->ks[i].info = NULL;
		(*t)->ks[i].busy = 0;
	}
	(*t)->msize = size;
	(*t)->step = step;
	return ERR_OK;
}	

void clear_table(Table* t){
	for(size_t i = 0; i < t->msize; i++){
		free(t->ks[i].info);
		t->ks[i].info = NULL;
		t->ks[i].busy = 0;
	}
}

void free_table(Table** t){
	clear_table(*t);
	free((*t)->ks);
	free(*t);
	*t = NULL;
}

size_t hash(unsigned int key){
	size_t hash = 0;
	unsigned char* bytes = (unsigned char*)&key;
	for (size_t i = 0; i < sizeof(unsigned int); i++){
		hash = 37*hash + bytes[i];
	}
	return hash;
}


ERR insert(Table *t, unsigned int key, char* info){
	size_t temp, step = t->step;
	if(find(t, key, &temp) == ERR_OK){return ERR_FAIL;}
	size_t index = hash(key) % t->msize;
	size_t old_index = index;
	do{
		if((t->ks)[index].busy == 0 || (t->ks)[index].busy == -1){
			(t->ks)[index].info = strdup(info);
			if((t->ks)[index].info == NULL){return ERR_MEM;}
			(t->ks)[index].busy = 1;
			(t->ks)[index].key = key;
			return ERR_OK;
		}
		index = (index + step) % t->msize;
	}while(index != old_index);
	return ERR_CAPACITY;
}

ERR delete(Table *t, unsigned int key){
	size_t pos;
	if(find(t, key, &pos) == ERR_FAIL){return ERR_FAIL;}
	(t->ks)[pos].busy = -1;
	free((t->ks)[pos].info);
	(t->ks)[pos].info = NULL;
	return ERR_OK;
}

ERR find(Table *t, unsigned int key, size_t* pos){
	size_t index = hash(key) % t->msize;
	size_t step = t->step;
	size_t old_index = index;
	do{
		if((t->ks)[index].busy == 0)return ERR_FAIL;
		if((t->ks)[index].busy == 1 && (t->ks)[index].key == key){
			*pos = index;
			return ERR_OK;
		}
		index = (index + step) % t->msize;
	}while(index != old_index);
	return ERR_FAIL;
}

void print_table(Table* t){
	int flag = 1;
	for(size_t i = 0; i < t->msize; i++){
		if((t->ks)[i].busy == 1){
			printf("%u %s\n", (t->ks)[i].key, (t->ks)[i].info);
			flag = 0;
		}
	}
	if(flag){printf("Таблица пуста\n");}
}

ERR import(Table* t, const char* name){
	clear_table(t);
	char* info = NULL;
	FILE *file = fopen(name, "rb");
	if(file == NULL){return ERR_FILE;}
	while(1){
		unsigned int key;
		if(fread(&key, sizeof(unsigned int), 1, file) != 1){
			free(info);
			if(feof(file)){break;}
			fclose(file);
			return ERR_READ;
		}
		unsigned int len;
		if(fread(&len, sizeof(unsigned int), 1, file) != 1){
			free(info);
			fclose(file);
			return ERR_READ;
		}
		free(info);
		info = malloc((len + 1) * sizeof(char));
		if(info == NULL){
			fclose(file);
			return ERR_MEM;
		}
		if(fread(info, sizeof(char), len, file) != len){
			fclose(file);
			free(info);
			return ERR_READ;
		}
		info[len] = '\0';
		ERR code = insert(t, key, info);
		if(code != 0){
			fclose(file);
			free(info);
			return code;
		}
	}
	fclose(file);
	return ERR_OK;
}

ERR export(Table *t, const char* name){
	FILE *file = fopen(name, "wb");
	unsigned int key;
	if(file == NULL){return ERR_FILE;}
	for(size_t i = 0; i < t->msize; i++){
		if(t->ks[i].busy == 1){
			key = t->ks[i].key;
			if(fwrite(&key, sizeof(unsigned int), 1, file) != 1){
				fclose(file);
				return ERR_WRITE;
			}
			unsigned int len = (unsigned int)strlen(t->ks[i].info); 
			if(fwrite(&len, sizeof(unsigned int), 1, file) != 1){
				fclose(file);
				return ERR_WRITE;
			}
			if(fwrite(t->ks[i].info, sizeof(char), len, file) != len){
				fclose(file);
				return ERR_WRITE;
			}
		}
	}
	fclose(file);
	return ERR_OK;
}
