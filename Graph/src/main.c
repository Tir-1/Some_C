#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "graph.h"

void print_menu(){
	printf("//////////////////////////////////////////\n");
	printf("1.Добавить вершину\n");
    printf("2.Добавить ребро\n");
	printf("3.Изментьб данные вершины/ребра\n");
	printf("4.Удалить вершину/ребро\n");
	printf("5.Обход\n");
	printf("6.Кратчайший путь\n");
	printf("7.Минимальное остовное дерево с заданным портом на компьютерах\n");
	printf("8.Вывод в виде списков смежности\n");
    printf("9.Графический вывод\n");
	printf("10.Завершить работу\n");
}

int input_u(unsigned int* info){
	char* input = readline("> ");
	if(input == NULL){return 1;}
	int temp;
	if(sscanf(input, "%d", &temp) != 1 || temp < 0){printf("Ожидалось неотрицательное число\n");free(input);return 2;}
	free(input);
	*info = temp;
	return 0;
}

int menu_insert_vertex(Graph* g){
	printf("Введите имя вершины\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	printf("Введите номер порта\n");
	unsigned int port;
	int code = input_u(&port);
	if(code == 1){free(name);return 1;}
	if(code == 2){free(name);return 0;}
	code = add_Vertex(g, name, port);
	free(name);
	switch(code){
		case 1:
				printf("Имя вершины должно быть уникальным\n");
				return 0;
		case 2:
				printf("Ошибка выделения памяти\n");
				return 1;
		default:
				printf("Элемент добавлен\n");
				return 0;
	}
}

int input_arr(unsigned int* arr, unsigned int size){
	for(unsigned int i = 0; i < size; i++){
		printf("Введите элемент массива(введено %u из %u)", i, size);
		unsigned int el;
		int code = input_u(&el);
		if(code != 0){return code;}
		arr[i] = el;
	}
	return 0;
}

int menu_insert_edge(Graph* g){
	printf("Введите имя первой вершины\n");
	char* first = readline("> ");
	if(first == NULL){return 1;}
    printf("Введите имя второй вершины\n");
	char* second = readline("> ");
	if(second == NULL){return 1;}
	printf("Введите время задержки\n");
	unsigned int time;
	int code = input_u(&time);
	if(code == 1){free(first);free(second);return 1;}
	if(code == 2){free(first);free(second);return 0;}
    printf("Введите количество разрешённых портов для передачи\n");
	unsigned int size;
	code = input_u(&size);
	if(code == 1){free(first);free(second);return 1;}
	if(code == 2){free(first);free(second);return 0;}
    unsigned int* allowed_ports = malloc(sizeof(unsigned int) * size);
	if(allowed_ports == NULL){free(first);free(second);return 1;}
	code = input_arr(allowed_ports, size);
	if(code == 1){free(first);free(second);free(allowed_ports);return 1;}
	if(code == 2){free(first);free(second);free(allowed_ports);return 0;}
	code = add_Edge(g, first, second, time, allowed_ports, size);
	free(first);
	free(second);
	free(allowed_ports);
	if(code == 1){printf("Ошибка выделения памяти\n");return 1;}
	if(code == 2){printf("Указанных вершин не существует\n");return 0;}
	if(code == 3){printf("Такое ребро уже существует. Вы можете изменить его или удалить\n");return 0;}
	printf("Элемент добавлен\n");
	return 0;
}

int menu_delete_vertex(Graph* g){
	printf("Введите имя вершины\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	if(delete_Vertex(g, name) == 1){printf("Такой вершины нет\n");}
	free(name);
	printf("Вершина удалена\n");
	return 0;
}

int menu_delete_edge(Graph* g){
	printf("Введите имя первой вершины\n");
	char* name1 = readline("> ");
	if(name1 == NULL){return 1;}
    printf("Введите имя второй вершины\n");
	char* name2 = readline("> ");
	if(name2 == NULL){return 1;}
	if(delete_Edge(g, name1, name2) == 1){printf("Такого ребра нет\n");}
	free(name1);
	free(name2);
	return 0;
}

int menu_visual(Graph* g){
	printf("Введите имя файла\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	if(create_dot(g, name) == 1){printf("Невозможно открыть файл\n");}
	free(name);
	printf("Успешно\n");
	return 0;
}

int menu_change_vertex(Graph* g){
	printf("Введите имя\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	Vertex* v = find_Vertex(g, name);
	free(name);
	if(v == NULL){printf("Такой вершины нет\n");return 0;}
	printf("Введите новое имя вершины\n");
	name = readline("> ");
	if(name == NULL){return 1;}
	printf("Введите новый порт вершины\n");
	unsigned int port;
	int code = input_u(&port);
	if(code == 1){free(name);return 1;}
	if(code == 2){free(name);return 0;}
	code = change_Vertex(g, v, port, name);
	free(name);
	if(code == 1){printf("Имя должно быть уникальным");return 0;}
	if(code == 2){printf("Ошибка выделения памяти\n");return 1;}
	return 0;
}

int menu_change_edge(Graph* g){
	printf("Введите имя первой вершины\n");
	char* name1 = readline("> ");
	if(name1 == NULL){return 1;}
    printf("Введите имя второй вершины\n");
	char* name2 = readline("> ");
	if(name2 == NULL){return 1;}
	Vertex* v1 = find_Vertex(g, name1);
	Vertex* v2 = find_Vertex(g, name2);
	free(name1);
	if(v1 == NULL || v2 == NULL){printf("Таких вершин нет\n");free(name2);return 0;}
	Near* n = find_near(v1, name2);
	if(n == NULL){printf("Такого ребра нет\n");free(name2);return 0;}
	free(name2);
	printf("Введите новое время задержки(Старое равнялось %u)", n->time);
	unsigned int time;
	int code = input_u(&time);
	if(code == 1){return 1;}if(code == 2){return 0;}
	printf("Вы хотите поменять разрешённые порты? Введите 'y' или 'n'");
	char* choice = readline("> ");
	if(choice == NULL){return 1;}
	unsigned int* allowed_ports = NULL;
	unsigned int size = 0;
	int flag = 0;
	int sr = strcmp(choice, "y");
	free(choice);
	if(sr == 0){
		flag = 1;
		printf("Введите количество разрешённых портов для передачи\n");
		code = input_u(&size);
		if(code == 1){return 1;}if(code == 2){return 0;}
		allowed_ports = malloc(sizeof(unsigned int) * size);
		if(allowed_ports == NULL){return 1;}
		code = input_arr(allowed_ports, size);
		if(code == 1){free(allowed_ports);return 1;}if(code == 2){free(allowed_ports);return 0;}
	}
	if(change_Edge(v1, v2, time, flag, allowed_ports, size) == 1){printf("Ошибка выделения памяти");free(allowed_ports);return 1;}
	free(allowed_ports);
	return 0;
}

int menu_change(Graph* g){
	printf("Вы хотите изменить:\n 1 - Вершину\n 2 - Ребро\n ");
	unsigned int choice;
	int code;
	code = input_u(&choice);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	if(choice == 1){return menu_change_vertex(g);}
	if(choice == 2){return menu_change_edge(g);}
	printf("Ожидалось 1 или 2\n");
	return 0;
}

int menu_delete(Graph* g){
	printf("Вы хотите удалить:\n 1 - Вершину\n 2 - Ребро\n ");
	unsigned int choice;
	int code = input_u(&choice);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	if(choice == 1){return menu_delete_vertex(g);}
	if(choice == 2){return menu_delete_edge(g);}
	printf("Ожидалось 1 или 2\n");
	return 0;
}

int menu_obhod(Graph* g){
	printf("Укажите имя компьютера, от которого начинать поиск\n");
	char* name = readline("> ");
	if(name == NULL){return 1;}
	printf("Укажите искомый порт\n");
	unsigned int port;
	int code = input_u(&port);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	char* data = NULL;
	code = obhod_BFS(g, name, port, &data);
	free(name);
	if(code == 0){printf("Искомый компьютер: %s\n", data);}
	free(data);
	if(code == 2){printf("Ошибка памяти\n");return 1;}
	if(code == 1){printf("Поиск завершился безрезультатно\n");}
	return 0;
}

void print_path(Path* p){
	switch(p->complete){
	case 1:
		printf("Указанных вершин нет\n");
		return;
	case 2:
		printf("Пути не существует\n");
		return;
	case 3:
		printf("Ошибка памяти\n");
		return;
	}
    printf("Время за которое можно пройти путь: %u\n", p->len);
    printf("Путь: %s -> ", p->names[0]);
    for(unsigned int i = 1; i < p->len_arr - 1; i++){
        printf("%s -> ", p->names[i]);
    }
    printf("%s\n", p->names[p->len_arr - 1]);
}

int menu_shortest(Graph* g){
	printf("Укажите имя исходной вершины\n");
	char* name1 = readline("> ");
	if(name1 == NULL){return 1;}
	printf("Укажите имя конечной вершины\n");
	char* name2 = readline("> ");
	if(name2 == NULL){free(name1); return 1;}
	printf("Введите порт по которому будет идти информация\n");
	unsigned int port;
	int code = input_u(&port);
	if(code == 1){free(name1);free(name2);return 1;}
	if(code == 2){free(name1);free(name2);return 0;}
	Path* path = sh_path_form(g, name1, name2, port);
	free(name1);
	free(name2);
	if(path == NULL){printf("Ошибка памяти\n");return 1;}
	print_path(path);
	free_path(path);
	return 0;
}

int menu_ostov(Graph** g){
	printf("Укажите порт компьютеров для которых нужно построить минимальный остов\n");
	unsigned int port;
	int code = input_u(&port);
	if(code == 1){return 1;}
	if(code == 2){return 0;}
	Graph* ostov = create_min_ostov(*g, port);
	if(ostov == NULL){printf("Ошибка памяти\n");return 1;}
	free_graph(*g);
	*g = ostov;
	printf("Ваш граф перезаписан на остов\n");
	return 0;
}

int input_processing(Graph** g, char* input){
	int key = atoi(input);
	switch(key){
		case 1:
				return menu_insert_vertex(*g);
		case 2:
				return menu_insert_edge(*g);
		case 3:
				return menu_change(*g);
		case 4:
				return menu_delete(*g);
		case 5:
				return menu_obhod(*g);
		case 6:
				return menu_shortest(*g);
		case 7:
				return menu_ostov(g);
		case 8:
                format_print(*g);
				return 0;
      	case 9:
                return menu_visual(*g);
		case 10:
				printf("\n");
				return 1;
		default:
				printf("wrong input\n");
				return 0;
	}
}

int main(){
	Graph* g = create_graph();
	if(g == NULL){return 1;}
	char* input;
	int flag = 0;
	while(flag == 0){
		print_menu();
		input = readline("> ");
		if(input == NULL){free_graph(g);return 1;}
		flag = input_processing(&g, input);
		free(input);
		input = NULL;
	}
	free_graph(g);
	return 0;
}
