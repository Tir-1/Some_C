#include "my_string.h"
#include <stdio.h>
#include <stdlib.h>
size_t my_strlen(const char *s){
	size_t res = 0;
	while(*s){
		++s; ++res;
	}
	return res;
}

void my_memcpy(char * str, const char* source, size_t len){
	for(size_t i = 0; i < len; i++){
		str[i] = source[i];
	}
}


char *get_str(){
	char buf[81] = {0};
	char *res = NULL;
	size_t len = 0, n = 0;
	do {
		n = scanf("%80[^\n]", buf);
		if(n<0){
			if(!res){return NULL;}
		}
		else if(n > 0){
			size_t chunk_len = my_strlen(buf);
			size_t str_len = len + chunk_len;
			char* temp = realloc(res, str_len + 1);
			if (!temp) {free(res);return NULL;}
			res = temp;
			my_memcpy(res+len, buf, chunk_len);
			len = str_len;
		}
		else{
			scanf("%*c");
		}
	} while (n > 0);
	
	if (len > 0){
		res[len] = '\0';
	}
	else{
		res = calloc(1, sizeof(char));
	}
	return res;
}

char *my_strdup(const char *str){
	char *res;
	size_t len = my_strlen(str);
	res = calloc(len+1, sizeof(char));
	if(res == NULL)return NULL;
	my_memcpy(res, str, len);
	return res;
}

size_t my_poisk(const char* str, int c){
	size_t key = 0;
	while(*str){
		if(*str == c)return key;
		str++;
		key++;
	}
	return -1;
}


char *my_strtok(char *str, const char *sep, size_t *index, size_t* word_len){
	while (str[*index] != '\0' && my_poisk(sep, str[*index]) != -1) {
        (*index)++;
    }
	if (str[*index] == '\0')return NULL;
	size_t capacity = 64;
	char *word = malloc(sizeof(char)*capacity);
	if(word == NULL){return NULL;}
	
	size_t i = 0;
	while(str[*index] != 0 && my_poisk(sep, str[*index]) == -1){
		if (i + 1 >= capacity) {
            capacity *= 2;
            char *temp = realloc(word, capacity);
            if (temp == NULL) {
                free(word);
                return NULL;
            }
            word = temp;
        }
		word[i++] = str[*index];
		(*index)++;
	}
	word[i] = '\0';
	*word_len = i;
	return word;
}


