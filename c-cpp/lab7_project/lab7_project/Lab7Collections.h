#pragma once
# include "Data.h"


//// constants:
const int ATTEMPTS_COUNT = 20; // count of attempts to allocate memory
const int MAX_PRIORITY = 256; // max priority of node in treap
const int HASH_TABLE_SIZE = 30; // size of hash table array
const int SHOWING_SIZE = 10;


// show modes in show treap
enum ShowModes {
	PREORDER = 0, INORDER, POSTORDER
};


// results of various functions
enum ResultCodes {
	AllAreOk = 0,
	ErrorWithMemoryAllocation = 1,
	ErrorWithExisting = (-1)
};


//// structures:
// treap node structure:
typedef struct Vertex {
	short int priority; // random priority
	Data data; // word is equal to key
	Vertex* left;
	Vertex* right;
} Vertex;


// treap descriptor 
typedef struct Treap {
	Vertex* root;
} Treap;


// list node structure
typedef struct Node {
	Vertex* vertex;
	Node* next;
} Node;


// stack descriptor
typedef struct Stack {
	Node* head;
} Stack;


// queue Node
typedef struct QueueNode {
	Vertex* vertex;
	size_t index;
	QueueNode* next;
} QueueNode;


// queue descriptor
typedef struct Queue {
	QueueNode* head;
	QueueNode* tail;
} Queue;


// hash table descriptor
typedef struct HashTable {
	Node* array[HASH_TABLE_SIZE];
} HashTable;


//// functions prototypes:
// split treap on two subtreaps
// left is less than value and right is bigger
// args: root of treap, pointers on left and right roots of subtreaps
void split_treap(char* value, size_t n, Vertex* root, Vertex** left_root, Vertex** right_root);


// merge two treaps in one
// returns result treap
// all values in left are less than in right
Vertex* merge_treaps(Vertex* left_treap, Vertex* right_treap);


// erase vertex from treap if it exist
// args: pointer on pointer on root, pointer on data
// returns result codes;
int erase_vertex(Vertex** root, Data* data);


// removes vertex from treap if it exists
// args: Treap structure pointer and pointer on comparable data
// returns result codes
int remove_vertex(Treap* treap, Data* data);


// add new vertex to treap
// args: treap structure pointer and pointer on Data
// returns result codes
int add_vertex(Treap* treap, Data* data);


// insert vertex in treap
// pointer on pointer on root, pointer on new_vertex
// return AllAreOk if all is ok or ErrorWithExisting if such vertex is already exist
int insert_vertex(Vertex** root, Vertex* new_vertex);


// free memory of all vertexes in treap
// args: pointer on treaps root
void free_all_vertexes(Vertex* root);


// free memory of treap in heap
// args: pointer on treap
void free_treap(Treap treap);


// find node in treap
// returns pointer if vertex exist or NULL if it is not exist
// args: root - root of the treap, key_string - searching value,
// n - lenght of key_string
Vertex* find_vertex(Vertex* root, char* key_string, size_t n);


// shows treap
// args: treap structure and showing mode
void show_treap(Treap treap, int show_mode);


void show_treap_preorder(Vertex* root);


void show_treap_inorder(Vertex* root);


void show_treap_postorder(Vertex* root);


// show treap iterative variation
// returns result codes
int iterative_show_treap(Treap treap, int show_mode);


int iterative_show_treap_preorder(Vertex* root);


int iterative_show_treap_inorder(Vertex* root);


int iterative_show_treap_postorder(Vertex* root);


// calculates deep of the treap
// args: pointer on treap root, pointer on size_t variable, where deep will be saved
void calculate_deep(Vertex* vertex, size_t* deep);


// calculate distance between two vertexes in treap
// return number of edges, or -1 if arguments are bad (error with existing)
int calculate_distance(Treap *treap, char* word_one, char* word_two);


// explores treap to calculate distance and save results in data array
void explore_treap(Vertex* root, char* word1, char* word2, short int* data_array);


// calculate distance using much more memory
int calculate_distance(Treap *treap, char* word_one, char* word_two, int* distance);


// checked push in stack for find_vertex_way
inline int checked_push(Stack* stack, Vertex* vertex);


// finds way to the vertex in treap and save it in stack
int find_vertex_way(Vertex* vertex, char* key, Stack* way);


// show treap in length in console
// args: root of treap
void show_in_length(Vertex* vertex, size_t level);


// show treap in breadth in console
void show_in_breadth(Treap treap);


// add node to treap
// args: treap descriptor pointer, index and pointer on vertex, that will be added
// returns result codes
int queue_push(Queue* queue, Vertex* vertex, size_t index);


// remove node from queue
// args: queue descriptor
QueueNode* queue_pop(Queue* queue);


// removes all nodes from stack
// returns result codes
int free_queue(Queue* queue);


// add node to stack
// args: stack descriptor and pointer on vertex, that will be added
// returns result codes
int stack_push(Stack* stack, Vertex* vertex);


// remove node from stack
// args: stack descriptor
Node* stack_pop(Stack* stack);


// removes all nodes from stack
// returns result codes
int free_stack(Stack* stack);


// comparing of two stacks for calculating distance
int stack_compare(Stack* stack1, Stack* stack2);


// remove similar nodes from top in 2 stacks
void stack_remove_similar(Stack* stack1, Stack* stack2);


// add nodes to hash table
// returns result codes
int hashtable_add_node(HashTable* hashtable, Vertex* value);


// check if node in hashtable exist
// result 1 - exist, 0 - not
int hashtable_check(HashTable* hashtable, Vertex* value);


// free all lists from hash table
void free_hashtable(HashTable* hashtable);


// print word of vertex in stdout in breadth showing
void show_word(Vertex* vertex, char left_symbol, char right_symbol);


// print word in showing in lenght
void show_word(Vertex* vertex);


// calculates hash of vertex
int vertex_hash(Vertex* value);


// try to allocate node ATTEMPTS_COUNT times
// returns pointer on node or NULL
Node* alloc_node();


// try to allocate queue node ATTEMPTS_COUNT times
// returns pointer on node or NULL
QueueNode* alloc_qnode();


// allocate memory in heap to vertex structure
// returns NULL if memory hasn't been allocated
// returns pointer on allocated vertex if all is ok
Vertex* alloc_vertex();

