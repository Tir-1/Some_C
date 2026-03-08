#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include <string.h>
#include <readline/readline.h>

void print_menu(){
	printf("//////////////////////////////////////////\n");
	printf("1.Добавить элемент\n");
	printf("2.Удалить элемент\n");
	printf("3.Вывести таблицу\n");
	printf("4.Найти элемент\n");
	printf("5.Импорт\n");
	printf("6.Экспорт\n");
	printf("7.Завершить работу\n");
}

int input_key(unsigned int *key){
	printf("Введите ключ\n");
	char* input = readline("> ");
	if(input == NULL){return 1;}
	if(sscanf(input, "%u", key) != 1){printf("Ключ должен быть неотрицательным числом\n");free(input);return 2;}
	free(input);
	return 0;
}

int menu_insert(Table* t){
	unsigned int key;
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	printf("Введите информацию\n");
	char* info = readline("> ");
	if(info == NULL){return 1;}
	ERR code_insert = insert(t, key, info);
	free(info);
	switch(code_insert){
		case ERR_FAIL:
				printf("Этот ключ уже есть в талице\n");
				return 0;
		case ERR_CAPACITY:
				printf("В таблице нет свободных ячеек\n");
				return 0;
		case ERR_MEM:
				printf("Ошибка выделения памяти\n");
				return 1;
		default:
				printf("Элемент добавлен\n");
				return 0;
	}
}

int menu_delete(Table *t){
	unsigned int key;
	int code = input_key(&key);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	if(delete(t, key) == ERR_FAIL){printf("Такого ключа нет\n");}
	return 0;
}

int menu_find(Table* t){
	unsigned int key;
	size_t code = input_key(&key), pos;
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	if(find(t, key, &pos) == ERR_FAIL){printf("Ничего не нашлось\n");}
	else{
		printf("%s\n", t->ks[pos].info);
	}
	return 0;
}

int menu_import(Table* t){
	printf("Введите имя бинарного файла\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	ERR code = import(t, name);
	free(name);
	switch(code){
		case ERR_DUPLICATE:
			printf("Ключи повторяются, что недопустимо\n");
			return 0;
		case ERR_CAPACITY:
			printf("В таблице не хватает места\n");
			return 0;
		case ERR_MEM:
			printf("Ошибка выделения памяти\n");
			return 1;
		case ERR_FILE:
			printf("Невозможно открыть файл\n");
			return 1;
		case ERR_READ:
			printf("Ошибка чтения из файла\n");
			return 1;
		default:
			printf("Таблица загружена\n");
			return 0;
	}
}

int menu_export(Table *t){
	printf("Введите имя файла, куда записать таблицу\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	ERR code = export(t, name);
	free(name);
	switch(code){
		case ERR_FILE:
			printf("Невозможно открыть файл\n");
			return 1;
		case ERR_WRITE:
			printf("Ошибка записи в файл\n");
			return 1;
		default:
			printf("Таблица Сохранена\n");
			return 0;
	}
}

int input_processing(Table* t, char* input){
	int key = atoi(input);
	switch(key){
		case 1:
				return menu_insert(t);
		case 2:
				return menu_delete(t);
		case 3:
				print_table(t);
				return 0;
		case 4:
				return menu_find(t);
		case 5:
				return menu_import(t);
		case 6:
				return menu_export(t);
		case 7:
				printf("\n");
				return 1;
		default:
				printf("wrong input\n");
				return 0;
	}
}
int read_num(const char* message) {
	printf("%s", message);
	char* size = readline("> ");
	if(size == NULL){return 1;}
	int num = atoi(size);
	free(size);
	return num;
}

int main(){
	Table* t;
	int size = read_num("Введите размер таблицы\n");
	int step = read_num("Введите шаг перемешивания");
	ERR code = init_table(&t, size, step);
	if (code == ERR_ARGUM){printf("Неверные аргументы, размер таблицы и шаг должны быть взаимно простыми и >1");return 1;}
	if (code == ERR_MEM){printf("Ошибка памяти");return 1;}
	char* input;
	int flag = 0;
	while(flag == 0){
		print_menu();
		input = readline("> ");
		if(input == NULL){free_table(&t);return 1;}
		flag = input_processing(t, input);
		free(input);
		input = NULL;
	}
	free_table(&t);
	return 0;
}
