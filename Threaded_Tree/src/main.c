#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include <string.h>
#include <readline/readline.h>

void print_menu(){
	printf("//////////////////////////////////////////\n");
	printf("1.Добавить элемент\n");
	printf("2.Удалить элемент\n");
	printf("3.Обход дерева\n");
	printf("4.Найти элемент\n");
	printf("5.Спецаильный поиск\n");
	printf("6.Вывод в виде дерева\n");
	printf("7.Графический вывод дерева\n");
	printf("8.Импорт\n");
	printf("9.Завершить работу\n");
}

int input_key(unsigned int *key){
	char* input = readline("> ");
	if(input == NULL){return 1;}
	int temp;
	if(sscanf(input, "%d", &temp) != 1 || temp < 0){printf("Ожидалось неотрицательное число\n");free(input);return 2;}
	free(input);
	*key = temp;
	return 0;
}

int menu_insert(Tree* t){
	unsigned int key, info;
	printf("Введите ключ\n");
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	printf("Введите информацию\n");
	code = input_key(&info);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	code = insert(t, key, info);
	switch(code){
		case 1:
				printf("Этот ключ уже есть в дереве\n");
				return 0;
		case 2:
				printf("Ошибка выделения памяти\n");
				return 1;
		default:
				printf("Элемент добавлен\n");
				return 0;
	}
}

int menu_delete(Tree *t){
	unsigned int key;
	printf("Введите ключ\n");
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	if(delete(t, key) == 1){printf("Такого ключа нет\n");}
	return 0;
}

int menu_find(Tree* t){
	unsigned int key;
	printf("Введите ключ\n");
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	Node* ptr = find(t, key);
	if(ptr == NULL){printf("Ничего не нашлось\n");}
	else{
		printf("%u\n", ptr->info);
	}
	return 0;
}

int menu_find2(Tree* t){
	unsigned int key;
	printf("Введите ключ\n");
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	Node* ptr = find_spec(t, key);
	if(ptr == NULL){printf("Ничего не нашлось\n");}
	else{
		printf("%u\n", ptr->info);
	}
	return 0;
}

int menu_import(Tree* t){
	printf("Введите имя файла\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	int code = import(t, name);
	free(name);
	switch(code){
		case 1:
			printf("Невозможно открыть файл\n");		
			return 0;
		case 2:
			printf("Неверный формат файла\n");
			return 1;
		case 3:
			printf("Ключи повторяются, что недопустимо\n");
			return 1;
		case 4:
			printf("Ошибка выделения памяти\n");
			return 1;
		default:
			printf("Дерево загружено\n");
			return 0;
	}
}

int menu_visual(Tree* t){
	printf("Введите имя файла\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	if(create_dot(t, name) == 1){printf("Невозможно открыть файл\n");}
	free(name);
	return 0;
}

int input_processing(Tree* t, char* input){
	int key = atoi(input);
	switch(key){
		case 1:
				return menu_insert(t);
		case 2:
				return menu_delete(t);
		case 3:
				obhod(t);
				return 0;
		case 4:
				return menu_find(t);
		case 5:
				return menu_find2(t);
		case 6:
				formatprint(t);
				return 0;
		case 7:
				return menu_visual(t);

		case 8:
				return menu_import(t);
		case 9:
				printf("\n");
				return 1;
		default:
				printf("wrong input\n");
				return 0;
	}
}

int main(){
	Tree* t = create_tree();
	if(t == NULL){return 1;}
	char* input;
	int flag = 0;
	while(flag == 0){
		print_menu();
		input = readline("> ");
		if(input == NULL){free_tree(t);return 1;}
		flag = input_processing(t, input);
		free(input);
		input = NULL;
	}
	free_tree(t);
	return 0;
}
