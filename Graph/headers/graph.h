#ifndef GRAPH_H
#define GRAPH_H
typedef struct Near{
    struct Vertex* v;
    unsigned int time;
    unsigned int* allowed_ports;
    unsigned int size;
    struct Near* next;
} Near;

typedef struct Vertex{
    Near* head;
    char* name;
    unsigned int port;
    unsigned int index;
} Vertex;

typedef struct Graph{
    Vertex** data;
    unsigned int size;
    unsigned int capacity;
} Graph;


typedef struct Path{
    int complete;
    unsigned int port;
    unsigned int len;
    char** names;
    unsigned int len_arr;
}Path;

typedef struct Edge{
	unsigned int v1;
    unsigned int v2;
    int time;
    unsigned int* allowed_ports;
    unsigned int size;
} Edge;

typedef struct Tree{
    int *parent;
    int *rank;
} Tree;

Graph* create_graph();
void free_graph(Graph* g);
Vertex* find_Vertex(Graph* g, char* name);
int add_Vertex(Graph* g, char* name, unsigned int port);
int add_Edge(Graph* g, char* first, char* second, unsigned int time, unsigned int* allowed_ports, unsigned int size);
int delete_Vertex(Graph* g, char* name);
int delete_Edge(Graph* g, char* name1, char* name2);
int create_dot(Graph* g, char* name);
int change_Vertex(Graph* g, Vertex* v, unsigned int port, char* name);
int change_Edge(Vertex* v1, Vertex* v2, unsigned int time, int flag, unsigned int* allowed_ports, unsigned int size);
void format_print(Graph* graph);
Vertex* find_Vertex(Graph* g, char* name);
Near* find_near(Vertex* v, char* name);
int obhod_BFS(Graph* g, char* name, unsigned int port, char** data);
void free_path(Path* p);
Path* sh_path_form(Graph* g, char* name1, char* name2, unsigned int port);
Graph* create_min_ostov(Graph* g, unsigned int port);
#endif
