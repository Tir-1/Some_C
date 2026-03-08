#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "my_string.h"
#define DELIM " \t"
#define PROMPT "> "
#define PREF "!"
#define SOGL  "BCDFGHJKLMNPQRSTVWXZbcdfghjklmnpqrstvwxz"

char* process(const char*);
void clear_two(char* first, char* second){
	if(first != NULL)free(first);
	if(second != NULL)free(second);
}

int main(){
	printf("> ");
	char* input = get_str();
	while(input != NULL){
		float timer = 0;
		printf("\"%s\"\n", input);
		clock_t start = clock();
		char* output = process(input);
		if(output == NULL){printf("MEMORYERROR\n");	free(input); return -1;}
		clock_t end = clock();
		timer = (float)(end - start) / CLOCKS_PER_SEC;
		printf("time %f\n", timer);
		printf("\"%s\"\n", output);
		clear_two(input, output);
		printf("> ");
		input = get_str();}
	return 0;
}

char* process(const char* str){
	if(str == NULL)return NULL;
	if(*str == '\0'){
		char* empty_str = malloc(sizeof(char));
		if(empty_str != NULL)empty_str[0] = '\0';
		return empty_str;
	}
	char* s = my_strdup(str);
	size_t s_len = my_strlen(s);
	char* res = malloc((s_len) * sizeof(char));
	char* temp;
	if(res == NULL){free(s);return NULL;}
	size_t index = 0;
	size_t word_len = 0;
	char *word = my_strtok(s, DELIM, &index, &word_len);
	size_t len = 0, d_len = 0;
	while(word != NULL){
		if(word_len > 0){
			if(my_poisk(SOGL, *word) != -1){
				d_len += 2;
				temp = realloc(res,( s_len + d_len) * sizeof(char));
				if(temp == NULL){clear_two(s, res); free(word); return NULL;}
				res = temp;
				my_memcpy(res + len, PREF, sizeof(char));
				len++;
			}
		my_memcpy(res + len, word, word_len * sizeof(char));
		len += word_len;
		res[len] = ' ';
		len++;
	}
		free(word);
		word = my_strtok(s, DELIM, &index, &word_len);
	}
	free(s);
	if(len > 0)len--;
	res[len] = '\0';
	temp = realloc(res, (len+1) * sizeof(char));
	if(temp == NULL){free(res);return NULL;}
	return temp;
}


