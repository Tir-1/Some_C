#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "queue.h"
#include "limits.h"

Graph* create_graph(){
    Graph* g = malloc(sizeof(Graph));
    if(g == NULL)return NULL;
    g->capacity = 10;
    g->size = 0;
    g->data = malloc(sizeof(Vertex*) * 10);
    if(g->data == NULL){free(g);return NULL;}
    for(int i = 0; i < 10; i++){
        g->data[i] = NULL;
    }
    return g;
}

void free_list(Near* ptr){
	while(ptr != NULL){
			Near* next = ptr->next;
			free(ptr->allowed_ports);
			free(ptr);
			ptr = next;
	}
}

void free_graph(Graph* g){
    for(unsigned int i = 0; i < g->size; i++){
    	if(g->data[i] != NULL){
			free_list(g->data[i]->head);
        	free(g->data[i]->name);
        	free(g->data[i]);
        }
    }
    free(g->data);
    free(g);
}

Vertex* find_Vertex(Graph* g, char* name){
    for(unsigned int i = 0; i < g->size; i++){
    	if(strcmp(g->data[i]->name, name) == 0){return g->data[i];}
    }
    return NULL;
}

Vertex* create_Vertex(char* name, unsigned int port){
    Vertex* x = malloc(sizeof(Vertex));
    if(x == NULL)return NULL;
    x->name = strdup(name);
    if(x->name == NULL){free(x); return NULL;}
    x->port = port;
    x->head = NULL;
    return x;
}

 int add_Vertex(Graph* g, char* name, unsigned int port){
    if(find_Vertex(g, name) != NULL)return 1;
    Vertex* new = create_Vertex(name, port);
    if(new == NULL)return 1;
    unsigned int capacity = g->capacity;
    if(g->size == capacity){
        Vertex** temp = realloc(g->data, capacity * 2 * sizeof(Vertex*));
        if(temp == NULL){free(new);return 1;}
        g->data = temp;
        for(unsigned int i = g->capacity; i < capacity * 2; i++){
            g->data[i] = NULL;
        }
        g->capacity = capacity * 2;
    }
    g->data[g->size] = new;
    new->index = g->size;
    g->size = g->size + 1;
    return 0;
 }

Near* find_near(Vertex* v, char* name){
    Near* ptr = v->head;
    while(ptr != NULL){
        if(strcmp(ptr->v->name, name) == 0)return ptr;
        ptr = ptr->next;
    }
    return NULL;
}
 

Near* create_Near(Vertex* v, unsigned int time, unsigned int* allowed_ports, unsigned int size){
    Near* new = malloc(sizeof(Near));
    if(new == NULL)return NULL;
    new->allowed_ports = malloc(sizeof(unsigned int) * size);
    if(new->allowed_ports == NULL){free(new); return NULL;}
    new->v = v;
    new->time = time;
    new->size = size;
    new->next = NULL;
    for(unsigned int i = 0; i < size; i++){
        new->allowed_ports[i] = allowed_ports[i];
    }
    return new;
}

int add_Edge(Graph* g, char* first, char* second, unsigned int time, unsigned int* allowed_ports, unsigned int size){
    Vertex* v1 = find_Vertex(g, first);
    Vertex* v2 = find_Vertex(g, second);
    if(v1 == NULL || v2 == NULL)return 2;
    if(find_near(v1, second) != NULL)return 3; 
    Near* for_v1 = create_Near(v2, time, allowed_ports, size);
    if(for_v1 == NULL)return 1;
    Near* for_v2 = create_Near(v1, time, allowed_ports, size);
    if(for_v2 == NULL){free(for_v1->allowed_ports);free(for_v1);return 1;}
    for_v1->next = v1->head;
    v1->head = for_v1;
    for_v2->next = v2->head;
    v2->head = for_v2;
    return 0;
}


void delete_Near(Vertex* v, char* name){
    Near* ptr = v->head;
    Near* pred = NULL;
    while(ptr != NULL){
        if(strcmp(ptr->v->name, name) == 0){
            if(pred == NULL){v->head = ptr->next;}
            else{
                pred->next = ptr->next;
            }
            Near* next = ptr->next;
            free(ptr->allowed_ports);
            free(ptr);
            ptr = next;
        }
        else{pred = ptr; ptr = ptr->next;}
    }
}

int delete_Vertex(Graph* g, char* name){
    Vertex* v_delete = find_Vertex(g, name);
    if(v_delete == NULL)return 1;
    unsigned int index = UINT_MAX;
    for(unsigned int i = 0; i < g->size; i++){
        if(g->data[i] != v_delete){
            delete_Near(g->data[i], name);
        }
        else{
            index = i;
        }
    }
    if(index != g->size - 1){
        g->data[index] = g->data[g->size - 1];
        g->data[index]->index = index;
    }
    g->data[g->size - 1] = NULL;
    g->size = g->size - 1;
    free_list(v_delete->head);
    free(v_delete->name);
    free(v_delete);
    return 0;
}

int delete_Edge(Graph* g, char* name1, char* name2){
    Vertex* v1 = find_Vertex(g, name1);
    Vertex* v2 = find_Vertex(g, name2);
    if(v1 == NULL || v2 == NULL){return 1;}
    delete_Near(v1, name2);
    delete_Near(v2, name1);
    return 0;
}

int change_Vertex(Graph* g, Vertex* v, unsigned int port, char* name){
    if(strcmp(v->name, name) != 0){
        if(find_Vertex(g, name) != NULL)return 1;
        free(v->name);
        v->name = strdup(name);
        if(v->name == NULL){return 2;}
    }
    v->port = port;
    return 0;
}

int change_Near(Near* ptr, unsigned int time, int flag, unsigned int* allowed_ports, unsigned int size){
    if(flag == 1){
        free(ptr->allowed_ports);
        ptr->allowed_ports = malloc(sizeof(unsigned int) * size);
        if(ptr->allowed_ports == NULL)return 1;
        for(unsigned int i = 0; i < size; i++){
            ptr->allowed_ports[i] = allowed_ports[i];
        }
    }
    ptr->time = time;
    ptr->size = size;
    return 0;
}

int change_Edge(Vertex* v1, Vertex* v2, unsigned int time, int flag, unsigned int* allowed_ports, unsigned int size){
    Near* near1 = find_near(v1, v2->name);
    Near* near2 = find_near(v2, v1->name);
    int code = change_Near(near1, time, flag, allowed_ports, size);
    if(code == 1)return 1;
    code = change_Near(near2, time, flag, allowed_ports, size);
    if(code == 1)return 1;
    return 0;
}

void format_print(Graph* g){
    if(g->size == 0) {
        printf("Пусто\n");
        return;
    }
    printf("Списки смежности:\n");
    for(unsigned int i = 0; i < g->size; i++) {
        Vertex* v = g->data[i];
        printf("%s:%u", v->name, v->port);
        Near* ptr = v->head;
        while(ptr != NULL) {
            printf(" -> %s:%u (время %u, порты: ", ptr->v->name, ptr->v->port, ptr->time);
            for(unsigned int j = 0; j < ptr->size; j++){
                printf("%u ", ptr->allowed_ports[j]);
            }
            printf(" )");
            ptr = ptr->next;
        }
        printf("\n");
    }
}

int create_dot(Graph* g, char* name) {
    FILE* file = fopen(name, "w");
    if(file == NULL){return 1;}
    fprintf(file, "graph G {\n    node [shape=circle];\n");
    for(unsigned int i = 0; i < g->size; i++) {
        Vertex* v = g->data[i];
        fprintf(file, "    \"%s:%u\";\n", v->name, v->port);
    }
    for(unsigned int i = 0; i < g->size; i++) {
        Vertex* v = g->data[i];
        Near* ptr = v->head;
        while (ptr != NULL) {
            if (strcmp(v->name, ptr->v->name) < 0) {
                fprintf(file, "    \"%s:%u\" -- \"%s:%u\" [label=\"%u\"];\n", v->name, v->port, ptr->v->name, ptr->v->port, ptr->time);
            }
            ptr = ptr->next;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    return 0;
}

int ifallow(unsigned int* allowed_ports, unsigned int size, unsigned int port){
    for(unsigned int i = 0; i < size; i++){
        if(allowed_ports[i] == port)return 0;
    }
    return 1;
}

int obhod_BFS(Graph* g, char* name, unsigned int port, char** data){
    unsigned int s;
    int flag = 0;
    for(unsigned int i = 0; i < g->size; i++){
        if(strcmp(g->data[i]->name, name) == 0){s = i;flag = 1;break;}
    }
    if(flag == 0)return 1;
    int* color = calloc(g->size, sizeof(int));
    if(color == NULL)return 2;
    color[s] = 1;
    Queue* q = init();
    if(q == NULL){free(color);return 2;}
    push(q, s);
    while(ifempty(q) == 0){
        int u = pop(q);
        Near* ptr = g->data[u]->head;
        while(ptr != NULL){
            if(ifallow(ptr->allowed_ports, ptr->size, port) == 0){
                int v = ptr->v->index;
                if(color[v] == 0){
                    if(ptr->v->port == port){
                        free(color);
                        free_q(q);
                        *data = strdup(ptr->v->name);
                        if(*data == NULL)return 2;                        
                        return 0;
                   	}
                    color[v] = 1;
                    push(q, v);
                }
            }
            ptr = ptr->next;
        }
    }
    free_q(q);
    free(color);
    return 1;
}

unsigned int find_index(Graph* g, char* name){
    for(unsigned int i = 0; i < g->size; i++){
        if(strcmp(g->data[i]->name, name) == 0)return i;
    }
    return UINT_MAX;
}

void free_path(Path* p){
    for(unsigned int i = 0; i < p->len_arr; i++){
        free(p->names[i]);
    }
    free(p->names);
    free(p);
}

void sh_path(Graph* g, unsigned int start, unsigned int end, Path* path){
    unsigned int size = g->size; 
    unsigned int* dist = malloc(size * sizeof(unsigned int));
    if(dist == NULL){
        path->complete = 3;
        return;
    }
    int* prev = malloc(size * sizeof(int));
    if(prev == NULL){
        free(dist);
        path->complete = 3;
        return;
    }
    for(unsigned int i = 0; i < size; i++){
        dist[i] = UINT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;
    unsigned int index;
    for(unsigned int i = 0; i < size - 1; i++){
        for(unsigned int u = 0; u < size; u++){
            Near* cur = g->data[u]->head;
            while(cur != NULL) {
                if(ifallow(cur->allowed_ports, cur->size, path->port) == 0){
                    index = cur->v->index;
                    if (dist[u] != UINT_MAX && dist[u] + cur->time < dist[index]) {
                        dist[index] = dist[u] + cur->time;
                        prev[index] = u;
                    }
                }
                cur = cur->next;
            }
        }
    }   
    if (dist[end] == UINT_MAX) {
        free(dist);
        free(prev);
        path->complete = 2;
        return;
    }
    path->complete = 0;
    path->len = dist[end];
    unsigned int len = 0;
    int ind = end;
    while (ind != -1) {
        len += 1;
        ind = prev[ind];
    }
    path->names = malloc(len * sizeof(char*));
    if(path->names == NULL){
        path->complete = 3;
        free(dist);
        free(prev);
        return;
    }
    path->len_arr = len;
    ind = end;
    int i = len - 1;
    while(i >= 0){
        path->names[i] = strdup(g->data[ind]->name);
        if(path->names[i] == NULL){
            int j = len - 1;
            path->complete = 3;
            while(j > i){
                free(path->names[j]);
                path->names[j] = NULL;
                j--;
            }
            free(path->names);
            return;
        }
        ind = prev[ind];
        i -= 1;
    }
    free(dist);
    free(prev);
}

Path* sh_path_form(Graph* g, char* name1, char* name2, unsigned int port){ // 1 нет вершин 2 пути нет 3илиNULLошибка памяти 0 успех
    unsigned int start = find_index(g, name1);
    unsigned int end = find_index(g, name2);
    Path* path = malloc(sizeof(Path));
    if(path == NULL)return NULL;
    path->complete = 1; 
    path->len_arr = 0;
    path->port = port;
    path->names = NULL;
    if(end == UINT_MAX || start == UINT_MAX)return path;
    sh_path(g, start, end, path);
    return path;
}

void Make_Set(Tree *t, int x) {
    t->parent[x] = x;
    t->rank[x] = 0;
}

int Find_Set(Tree *t, int x) {
    if (t->parent[x] != x) {
        t->parent[x] = Find_Set(t, t->parent[x]);
    }
    return t->parent[x];
}

void Link(Tree* t, int x, int y){
    if (t->rank[x] > t->rank[y]) {
        t->parent[y] = x;
    } else {
        t->parent[x] = y;
        if (t->rank[x] == t->rank[y]) {
            t->rank[y]++;
        }
    }
}

void Union(Tree *t, int x, int y) {
    Link(t, Find_Set(t, x), Find_Set(t, y));
}

int compare(const void *a, const void *b) {
    Edge *ptr_a = (Edge *)a;
    Edge *ptr_b = (Edge *)b;
    return ptr_a->time - ptr_b->time; 
}

int in_arr(Edge* arr, unsigned int len, unsigned int v1, unsigned int v2){
    for(unsigned int i = 0; i < len; i++){
        if((arr[i].v1 == v1 && arr[i].v2 == v2) || (arr[i].v1 == v2 && arr[i].v2 == v1)) {
            return 1;
        }
    }
    return 0;
}

void free_arr(Edge* arr, int len){
    for(int i = 0; i < len; i++){
        free(arr[i].allowed_ports);
    }
    free(arr);
}


Edge* Kruskal(Edge* edges, int n, int m, unsigned int* count){
    Tree t;
    t.parent = malloc(n * sizeof(int));
    if(t.parent == NULL)return NULL;
    t.rank = malloc(n * sizeof(int));
    if(t.rank == NULL){
        free(t.parent);
        return NULL;
    }
    Edge* ostov = malloc(m * sizeof(Edge));
    *count = 0;
    for (int i = 0; i < n; i++) {
        Make_Set(&t, i);
    }
    for(int i = 0; i < m; i++){
        int u = edges[i].v1;
        int v = edges[i].v2;
        if (Find_Set(&t, u) != Find_Set(&t, v)) {
            ostov[*count] = edges[i];
            *count += 1;            
            Union(&t, u, v);
        }
    }
    free(t.parent);
    free(t.rank);
    return ostov;
}

Graph* create_min_ostov(Graph* g, unsigned int port){
    Graph* g_ostov = create_graph();
    if(g_ostov == NULL)return NULL;
    for(unsigned int i = 0; i < g->size; i++){
        if((g)->data[i]->port == port){
            if(add_Vertex(g_ostov, g->data[i]->name, g->data[i]->port) == 1){free_graph(g_ostov);return NULL;}
        }
    }
    int capacity = 10;
    Edge* arr = malloc(capacity * sizeof(Edge));
    if(arr == NULL) {
        free_graph(g_ostov);
        return NULL;
    }
    int len = 0;
    for(unsigned int i = 0; i < g->size; i++) {
        Vertex* v1 = (g)->data[i];
        Near* cur = v1->head;
        if(v1->port == port){
            while(cur != NULL) {
                if(cur->v->port == port){
                    unsigned int ind_v2 = cur->v->index;
                    if(in_arr(arr, len, i, ind_v2) == 0 && ifallow(cur->allowed_ports, cur->size, port) == 0) {
                        arr[len].v1 = i;
                        arr[len].v2 = ind_v2;
                        arr[len].time = cur->time;
                        arr[len].size = cur->size;
                        arr[len].allowed_ports = malloc(cur->size * sizeof(unsigned int));
                        if(arr[len].allowed_ports == NULL){
                            free_arr(arr, len);
                            free_graph(g_ostov);
                        }
                        for(unsigned int j = 0; j < cur->size; j++){
                            arr[len].allowed_ports[j] = cur->allowed_ports[j];
                        }
                        len += 1;
                        if(len == capacity){
                            capacity *= 2;
                            Edge* temp = realloc(arr, capacity * sizeof(Edge));
                            if(temp == NULL){
                                free_arr(arr, len);
                                free_graph(g_ostov);
                                return NULL;
                            }
                            arr = temp;
                        }
                    }
                }
                cur = cur->next;
            }
        }
    }
    qsort(arr, len, sizeof(Edge), compare);
    unsigned int len_ostov;
    Edge* ostov = Kruskal(arr, g_ostov->size, len, &len_ostov);
    if(ostov == NULL){free_graph(g_ostov);free_arr(arr, len);return NULL;}
    for(unsigned int i = 0; i < len_ostov; i++){
        int ind_v1 = ostov[i].v1;
        int ind_v2 = ostov[i].v2;
        int code = add_Edge(g_ostov, (g)->data[ind_v1]->name, (g)->data[ind_v2]->name, ostov[i].time, ostov[i].allowed_ports, ostov[i].size);
        if(code != 0){free(g_ostov);free_arr(arr, len);free(ostov);return NULL;}
    }
    free_arr(arr, len);
    free(ostov);
    return g_ostov;
}
