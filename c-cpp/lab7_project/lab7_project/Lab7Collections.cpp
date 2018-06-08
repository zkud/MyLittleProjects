# include "Lab7Collections.h"
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>


# define TEMP_PRE_VALUE 0
# define TEMP_POST_VALUE 1
# define PRE_VALUE_WORD1 2
# define POST_VALUE_WORD1 3
# define PRE_VALUE_WORD2 4
# define POST_VALUE_WORD2 5
# define TEMP_DEPTH 6
# define WORD1_DEPTH 7
# define WORD2_DEPTH 8
# define COMMON_ANCESTOR_DEPTH 9
# define COMPARE_RESULT1 10
# define COMPARE_RESULT2 11


// NOTE: added at the last moment, other code can't be refactored
# define mloop(count) for(size_t counter = 0; counter < count; counter++)
// needed in showing in breadth
# define mhandle_push_errors(expression) if(expression == ErrorWithMemoryAllocation) {free_queue(&next_queue); free_queue(&cur_queue); return;}
# define mprint_spases(count) mloop(count) putchar(' ');
# define and &&
# define or ||
# define not !
# define true 1
# define false 0


////// treap:
// split treap on two subtreaps
// left is less than value and right is bigger
// args: root of treap, pointers on left and right roots of subtreaps
void split_treap(char* value, size_t n, Vertex* root, Vertex** left_root, Vertex** right_root) {
	// handle case if root is Null
	if (root == NULL) {
		*left_root = NULL, *right_root = NULL;
		return;
	}

	// if root value is less than value split the right subtreap
	if (strncmp(root->data.word, value, n) < 0) {
		*left_root = root;
		split_treap(value, n, root->right, &root->right, right_root);
	}
	// else split the left subtreap
	else {
		*right_root = root;
		split_treap(value, n, root->left, left_root, &root->left);
	}
}


// merge two treaps in one
// returns result treap
// all values in left are less than in right
Vertex* merge_treaps(Vertex* left_treap, Vertex* right_treap) {
	// handle case with empty left or rigth tree or both
	if (left_treap == NULL && right_treap == NULL)
		return NULL;
	if (left_treap == NULL || right_treap == NULL)
		return left_treap == NULL ? right_treap : left_treap;

	// if priority of left tree is bigger than right merge right subtree of left tree and right tree
	// and return left tree
	if (left_treap->priority > right_treap->priority) {
		left_treap->right = merge_treaps(left_treap->right, right_treap);
		return left_treap;
	}
	// else merge left tree and left subtree of right tree and return right tree
	else {
		right_treap->left = merge_treaps(left_treap, right_treap->left);
		return right_treap;
	}
}


// erase vertex from treap if it exist
// args: pointer on pointer on root, pointer on data
// returns result codes;
int erase_vertex(Vertex** root, Data* data) {
	// if root is NULL such vertex doesn't exist
	if (*root == NULL)
		return ErrorWithExisting;

	else {
		// compare data and roots data
		int comparison_result = strncmp(data->word, (*root)->data.word, WORDS_SIZE);

		// if needed vertex has been founded merge its subtreaps and
		// set the root of new result subtreap as needed vertex
		if (comparison_result == 0) {
			Vertex* merged_treap = merge_treaps((*root)->left, (*root)->right);

			free(*root);

			*root = merged_treap;

			return AllAreOk;
		}
		else
			// if needed vertex hasn't been founded go to the left subtree if data < roots data
			if (comparison_result < 0)
				return erase_vertex(&((*root)->left), data);
			else
				// or to the right subtree 
				return erase_vertex(&((*root)->right), data);
	}
}


// removes vertex from treap if it exists
// args: Treap structure pointer and pointer on comparable data
// returns result codes
int remove_vertex(Treap *treap, Data* data) {
	// check if data is not NULL
	if (data == NULL)
		return ErrorWithExisting;
	else
		return erase_vertex(&(treap->root), data);
}


// add new vertex to treap
// args: treap structure pointer and pointer on Data
// returns result codes
int add_vertex(Treap* treap, Data* data) {
	// check arguments
	if (treap == NULL || data == NULL)
		return ErrorWithExisting;
	if (find_vertex(treap->root, data->word, WORDS_SIZE) != NULL)
		return AllAreOk;

	// alloc vertex and set it, or return ErrorWithMemoryAllocation because memory hasn't been allocated
	Vertex* new_vertex = alloc_vertex();
	if (new_vertex == NULL)
		return ErrorWithMemoryAllocation;
	else {
		new_vertex->data = *data;
		new_vertex->priority = rand() % MAX_PRIORITY;

		// try to insert it in treap
		insert_vertex(&(treap->root), new_vertex);
		
		return AllAreOk;
	}
}


// insert vertex in treap
// pointer on pointer on root, pointer on new_vertex
// return AllAreOk if all is ok or ErrorWithExisting if such vertex is already exist
int insert_vertex(Vertex** root, Vertex* new_vertex) {
	// if root is null just set it as new_vertex
	if (*root == NULL)
		*root = new_vertex;
	else {
		// check if such vertex doesn't already exist
		int comparison_result = strncmp(new_vertex->data.word, (*root)->data.word, WORDS_SIZE);
		if (comparison_result == 0)
			return ErrorWithExisting;

		// if priority of root is less then new vertex priority
		// split root on two subtreaps of new vertex
		// and set pointer on root as pointer on vertex 
		if ((*root)->priority < new_vertex->priority) {
			split_treap(new_vertex->data.word, (size_t)WORDS_SIZE, *root, &new_vertex->left, &new_vertex->right);
			*root = new_vertex;
		}
		else
			// else go to the left subtreap if new vertex word < root word
			if (comparison_result < 0)
				return insert_vertex(&(*root)->left, new_vertex);
		// or go to the right subtreap if new vertex word > root word
			else
				return insert_vertex(&(*root)->right, new_vertex);
	}

	return AllAreOk;
}


// free memory of all vertexes in treap
// args: pointer on treaps root
void free_all_vertexes(Vertex* root) {
	if (root != NULL) {
		free_all_vertexes(root->left);
		free_all_vertexes(root->right);
		free(root);
	}
}


// free memory of treap in heap
// args: pointer on treap
void free_treap(Treap treap) {
	free_all_vertexes(treap.root);
}


// find node in treap
// returns pointer if vertex exist or NULL if it is not exist
// args: root - root of the treap, key_string - searching value,
// n - lenght of key_string
Vertex* find_vertex(Vertex* root, char* key_string, size_t n) {
	// handle if root or key_string is NULL
	if (root == NULL || key_string == NULL)
		return NULL;

	// compare root value and key_string value
	int comparison_result = strncmp(key_string, root->data.word, n);

	// if key_string < root value
	// go to the left subtreap
	if (comparison_result < 0)
		return find_vertex(root->left, key_string, n);
	else {

		// if key_string is equal root value
		// needed node has been founded, return it
		if (comparison_result == 0)
			return root;

		// else go to the right subtreap
		else
			return find_vertex(root->right, key_string, n);
	}
}


// shows treap
// args: treap structure and showing mode
void show_treap(Treap treap, int show_mode) {
	switch (show_mode) {
	case PREORDER:
		show_treap_preorder(treap.root);
		break;

	case INORDER:
		show_treap_inorder(treap.root);
		break;

	case POSTORDER:
		show_treap_postorder(treap.root);
		break;
	}
}


void show_treap_preorder(Vertex* root) {
	if (root != NULL) {
		show_data(&root->data);
		puts("\n");
		show_treap_preorder(root->left);
		show_treap_preorder(root->right);
	}
}


void show_treap_inorder(Vertex* root) {
	if (root != NULL) {
		show_treap_inorder(root->left);
		show_data(&root->data);
		puts("\n");
		show_treap_inorder(root->right);
	}
}


void show_treap_postorder(Vertex* root) {
	if (root != NULL) {
		show_treap_postorder(root->left);
		show_treap_postorder(root->right);
		show_data(&root->data);
		puts("\n");
	}
}


// show treap iterative variation
// returns result codes
int iterative_show_treap(Treap treap, int show_mode) {
	switch (show_mode) {
	case PREORDER:
		return iterative_show_treap_preorder(treap.root);

	case INORDER:
		return iterative_show_treap_inorder(treap.root);

	case POSTORDER:
		return iterative_show_treap_postorder(treap.root);
	}
}


int iterative_show_treap_preorder(Vertex* root) {
	// initialize stack
	Stack stack = { NULL };
	if (stack_push(&stack, root) == ErrorWithMemoryAllocation)
		return ErrorWithMemoryAllocation;

	// while stack isn't empty
	while (stack.head != NULL) {
		// pop node
		Node* temporary_node_pointer = stack_pop(&stack);

		// show it
		show_data(&temporary_node_pointer->vertex->data);
		putc('\n', stdout);

		// add left and right children of popped node
		if (stack_push(&stack, temporary_node_pointer->vertex->right) == ErrorWithMemoryAllocation
			|| stack_push(&stack, temporary_node_pointer->vertex->left) == ErrorWithMemoryAllocation) {
			free_stack(&stack);
			return ErrorWithMemoryAllocation;
		}

		// free node
		free(temporary_node_pointer);
	}
}


int iterative_show_treap_inorder(Vertex* root) {
	// initialize stack and hashtable of visited vertexes
	Stack stack = { NULL };
	HashTable hashtable = { 0 };

	// push root in stack and set it as visited
	if (stack_push(&stack, root) == ErrorWithMemoryAllocation)
		return ErrorWithMemoryAllocation;
	if (hashtable_add_node(&hashtable, root) == ErrorWithMemoryAllocation) {
		free_stack(&stack);
		return ErrorWithMemoryAllocation;
	}

	// while stack isn't empty
	while (stack.head != NULL)
		// push and set as visited left vertex if it exist and has not visited
		if (stack.head->vertex->left != NULL && hashtable_check(&hashtable, stack.head->vertex->left) == 0) {
			if (stack_push(&stack, stack.head->vertex->left) == ErrorWithMemoryAllocation
				|| hashtable_add_node(&hashtable, stack.head->vertex) == ErrorWithMemoryAllocation) {
				free_stack(&stack);
				free_hashtable(&hashtable);
				return ErrorWithMemoryAllocation;
			}
		}
		else {
			// pop node
			Node* temporary_node_pointer = stack_pop(&stack);

			// show it
			show_data(&temporary_node_pointer->vertex->data);
			putc('\n', stdout);

			// add right child of popped node if it exist
			if (stack_push(&stack, temporary_node_pointer->vertex->right) == ErrorWithMemoryAllocation) {
				free_stack(&stack);
				free_hashtable(&hashtable);
				return ErrorWithMemoryAllocation;
			}

			// free node
			free(temporary_node_pointer);
		}

		free_hashtable(&hashtable);
		return AllAreOk;
}


int iterative_show_treap_postorder(Vertex* root) {
	// algorithm was stealed from www.wikipedia.org (https://en.wikipedia.org/wiki/Tree_traversal#Post-order)
	// variables:
	Stack stack = { NULL };
	Node* last_node_visited = NULL;

	// while stack isn't empty 
	while (stack.head != NULL || root != NULL)
		// go left if it's possible
		if (root != NULL) {
			if (stack_push(&stack, root) == ErrorWithMemoryAllocation) {
				free_stack(&stack);
				return ErrorWithMemoryAllocation;
			}

			root = root->left;
		}
	// else go right if it's possible
		else {
			Vertex* head_node = stack.head->vertex;
			
			if (head_node->right != NULL && last_node_visited->vertex != head_node->right) {
				root = head_node->right;
			}
			else {
				show_data(&head_node->data);
				putc('\n', stdout);

				if (last_node_visited != NULL)
					free(last_node_visited);
				last_node_visited = stack_pop(&stack);
			}
		}

		return AllAreOk;
}


// calculate distance between two vertexes in treap
// return number of edges, or -1 if arguments are bad (error with existing)
int calculate_distance(Treap *treap, char* word_one, char* word_two) {
	// variables:
	// data array consist of:
	// 0 - temp pre value, 1 - temp post value, 2 - pre value of word 1,
	// 3 - post value of word 1, 4 - pre value of word 2, 5 - post value of word 2,
	// 6 - temp depth, 7 - word 1 depth, 8 - word 2 depth, 9 - common least ancestor depth
	// 10 - compare result 1, 11 - compare result 2
	short int data_array[12] = { -1, -1, -1, -1, -1, -1, 0, 0, 0, -1, 0, 0 };

	// handle bad arguments
	if (treap == NULL || word_one == NULL || word_two == NULL)
		return ErrorWithExisting;

	// handle case if word_one == word_two
	if (strcmp(word_one, word_two) == 0)
		return 0;
	
	// explore treap
	explore_treap(treap->root, word_one, word_two, data_array);

	// handle results:
	// if such words don't exist in turple return (-1)
	if (data_array[PRE_VALUE_WORD1] < 0 || data_array[PRE_VALUE_WORD2] < 0)
		return ErrorWithExisting;

	// if word1 is parent of word2 return subtraction of depths
	if (data_array[PRE_VALUE_WORD1] < data_array[PRE_VALUE_WORD2]
		&& data_array[POST_VALUE_WORD1] > data_array[POST_VALUE_WORD2])
		return data_array[WORD2_DEPTH] - data_array[WORD1_DEPTH];
	
	// if word2 is parent of word1 return subtraction of depths
	if (data_array[PRE_VALUE_WORD2] < data_array[PRE_VALUE_WORD1]
		&& data_array[POST_VALUE_WORD2] > data_array[POST_VALUE_WORD1])
		return data_array[WORD1_DEPTH] - data_array[WORD2_DEPTH];

	// else return distance using common ancestor
	return (data_array[WORD1_DEPTH] + data_array[WORD2_DEPTH] - (data_array[COMMON_ANCESTOR_DEPTH] << 2));
}


// explores treap to calculate distance and save results in data array
void explore_treap(Vertex* root, char* word1, char* word2, short int* data_array) {
	// check if root is not NULL
	if (root == NULL)
		return;

	// add temp pre value
	data_array[TEMP_PRE_VALUE]++;

	// check vertex
	data_array[COMPARE_RESULT1] = strcmp(root->data.word, word1);
	data_array[COMPARE_RESULT2] = strcmp(root->data.word, word2);

	// if it's common least ancestor, remember depth
	if (((data_array[COMPARE_RESULT1] < 0 && data_array[COMPARE_RESULT2] > 0)
		|| (data_array[COMPARE_RESULT1] > 0 && data_array[COMPARE_RESULT2] < 0))
		&& data_array[COMMON_ANCESTOR_DEPTH] == (-1))
		data_array[COMMON_ANCESTOR_DEPTH] = data_array[TEMP_DEPTH];

	// if it's word1 remember depth, pre value
	if (data_array[COMPARE_RESULT1] == 0)
		data_array[WORD1_DEPTH] = data_array[TEMP_DEPTH], data_array[PRE_VALUE_WORD1] = data_array[TEMP_PRE_VALUE];
	
	// if it's word2 remember depth, pre value
	if (data_array[COMPARE_RESULT2] == 0)
		data_array[WORD2_DEPTH] = data_array[TEMP_DEPTH], data_array[PRE_VALUE_WORD2] = data_array[TEMP_PRE_VALUE];

	// add temp depth
	data_array[TEMP_DEPTH]++;

	// go to the left subtree
	explore_treap(root->left, word1, word2, data_array);

	// go to the right subtree
	explore_treap(root->right, word1, word2, data_array);

	// subtract temp depth
	data_array[TEMP_DEPTH]--;

	// add temp post value
	data_array[TEMP_POST_VALUE]++;

	// check vertex
	data_array[COMPARE_RESULT1] = strcmp(root->data.word, word1);
	data_array[COMPARE_RESULT2] = strcmp(root->data.word, word2);

	// if it's word1 remember post value
	if (data_array[COMPARE_RESULT1] == 0)
		data_array[POST_VALUE_WORD1] = data_array[TEMP_POST_VALUE];

	// if it's word2 remember post value
	if (data_array[COMPARE_RESULT2] == 0)
		data_array[POST_VALUE_WORD2] = data_array[TEMP_POST_VALUE];
}


// calculate distance using much more memory
int calculate_distance(Treap *treap, char* word_one, char* word_two, int* distance) {
	// variables block
	Stack way1 = {}, way2 = {};
	int result[2];
	// variables block
	
	// find way of the first and the second words
	result[0] = find_vertex_way(treap->root, word_one, &way1);
	result[1] = find_vertex_way(treap->root, word_two, &way2);

	// if one of words doesn't exist in treap return ErrorWithExisting
	if(result[0] == ErrorWithExisting || result[1] == ErrorWithExisting) {
		free_stack(&way1);
		free_stack(&way2);
		return ErrorWithExisting;
	}

	// similar to error with exising
	if (result[0] == ErrorWithMemoryAllocation || result[1] == ErrorWithMemoryAllocation) {
		free_stack(&way1);
		free_stack(&way2);
		return ErrorWithMemoryAllocation;
	}

	// compare stack, save distance between two vertexes, return "ok" code
	*distance = stack_compare(&way1, &way2);

	return AllAreOk;
}


// checked push in stack for find_vertex_way
inline int checked_push(Stack* stack, Vertex* vertex) {
	if (stack_push(stack, vertex) == ErrorWithMemoryAllocation)
		return ErrorWithMemoryAllocation;
	else
		return AllAreOk;
}


// finds way to the vertex in treap and save it in stack
int find_vertex_way(Vertex* vertex, char* key, Stack* way) {
	// handle if vertex doesn't exist
	if (vertex == NULL)
		return ErrorWithExisting;

	// variables block
	int compare_result = strncmp(key, vertex->data.word, WORDS_SIZE);
	static int result;
	// variables block

	// if vertex has been founded push it and go higher
	if (compare_result == 0) {
		return checked_push(way, vertex);
	}
	// if has not go deeper
	else if (compare_result > 0) {
		result = find_vertex_way(vertex->right, key, way);

		// handle errors and try to push vertex
		if (result == ErrorWithMemoryAllocation || result == ErrorWithExisting)
			return result;
		else 
			return checked_push(way, vertex);
	}
	else {
		result = find_vertex_way(vertex->left, key, way);

		// handle errors and try to push vertex
		if (result == ErrorWithMemoryAllocation || result == ErrorWithExisting)
			return result;
		else
			return checked_push(way, vertex);
	}	
}


// show treap in length
// args: root of treap and 0 level
void show_in_length(Vertex* vertex, size_t level) {
	// check args
	if (vertex == NULL)
		return;

	// if right exist go to the right
	if (vertex->right != NULL) {
		show_in_length(vertex->right, level + 1);

		// show edge
		mprint_spases(level * SHOWING_SIZE + SHOWING_SIZE / 2);
		printf("/\n");
	}

	// show vertex
	mprint_spases(level * SHOWING_SIZE);
	show_word(vertex);
	putchar('\n');

	// similar to the right
	if (vertex->left != NULL) {
		// show edge
		mprint_spases(level * SHOWING_SIZE + SHOWING_SIZE / 2);
		printf("\\\n");

		show_in_length(vertex->left, level + 1);
	}
}


// shows treap in breath
void show_in_breadth(Treap treap) {
	// check arguments
	if (treap.root == NULL)
		return;

	// variables block
	Queue next_queue = {}, cur_queue = {}; // next and current queue
	QueueNode* temporary_node;
	size_t deep = 0; // deep of treap
	calculate_deep(treap.root, &deep);
	size_t size = pow(2, deep) - 1;
	size_t previous_index;
	size_t gap = pow(2, deep - 1);
	// size is the size of array that would be created by this treap (inorder)
	// gap is the distance between two nodes in level in indexes (*2 because of loop)
	// variables block

	// try to add the first node
	mhandle_push_errors(queue_push(&next_queue, treap.root, size / 2));

	// while next queue is not empty
	while (next_queue.head != NULL) {
		// set as zero prev index, change queues, calculate gap
		previous_index = 0, gap /= 2;
		cur_queue = next_queue;
		next_queue = {};
		
		// while current level hasn't ended
		while(cur_queue.head != NULL) {
			// get node and try to add it's childs to the next queue
			temporary_node = queue_pop(&cur_queue);
			if(temporary_node->vertex->left != NULL)
				mhandle_push_errors(queue_push(&next_queue, temporary_node->vertex->left, temporary_node->index - gap));
			if(temporary_node->vertex->right != NULL)
				mhandle_push_errors(queue_push(&next_queue, temporary_node->vertex->right, temporary_node->index + gap));

			// print needed spases
			mprint_spases(SHOWING_SIZE * (temporary_node->index - previous_index));

			// print word
			show_word(temporary_node->vertex, '/', '\\');

			// change prev index and free temporary node
			previous_index = temporary_node->index + 1;
			free(temporary_node);
		}

		putchar('\n');
	}
}


// calculate max deep of treap
void calculate_deep(Vertex* vertex, size_t* deep) {
	// variables block
	static size_t temporary_deep;
	// variables block

	if(vertex != NULL) {
		temporary_deep++;

		if(temporary_deep > *deep)
			*deep = temporary_deep;

		// if left or right subtree exist go deeper in them
		if(vertex->left != NULL){
			calculate_deep(vertex->left, deep);
			temporary_deep--;
		}
		if(vertex->right != NULL){
			calculate_deep(vertex->right, deep);
			temporary_deep--;
		}
	}
}


////// queue
// add node to treap
// args: treap descriptor pointer and pointer on vertex, that will be added
// returns result codes
int queue_push(Queue* queue, Vertex* vertex, size_t index=0) {
	if (vertex != NULL && queue != NULL) {
		// if queue is empty just set head and tail as new node
		if (queue->head == NULL) {
			// try to allocate memory
			queue->head = alloc_qnode();
			if (queue->head == NULL)
				return ErrorWithMemoryAllocation;

			// set new node
			queue->head->index = index;
			queue->tail = queue->head;
			queue->head->vertex = vertex;
			queue->head->next = NULL;
		}

		// else add new node in bottom
		else {
			// try to set new node
			QueueNode* new_node = alloc_qnode();
			if (new_node == NULL)
				return ErrorWithMemoryAllocation;
			new_node->vertex = vertex;
			new_node->next = NULL;
			new_node->index = index;

			// link new node with tail and set tail as new node
			queue->tail->next = new_node;
			queue->tail = new_node;
		}

		return AllAreOk;
	}
	// hadle case is stack pointer is NULL or vertex pointer is NULL
	else
		return ErrorWithExisting;
}


// remove node from queue
// args: queue descriptor
QueueNode* queue_pop(Queue* queue) {
	// handle if queue pointer is Null
	if (queue == NULL)
		return NULL;
 
	// handle if queue is empty
	if (queue->head == NULL)
		return NULL;

	if (queue->head != queue->tail) {
		// remove node from top of queue 
		// remember pointer on head
		QueueNode* head_pointer = queue->head;

		// link pointer in descriptor with queue next node
		queue->head = queue->head->next;

		return head_pointer;
	}
	else { // else remove single node and set all as NULL
		QueueNode* temp_pointer = queue->head;

		queue->head = queue->tail = NULL;

		return temp_pointer;
	}
}


// removes all nodes from stack
// returns result codes
int free_queue(Queue* queue) {
	// handle case if queue pointer is null
	if (queue == NULL)
		return ErrorWithExisting;
	else {
		// while queue isn't empty pop elements
		while (queue->head != NULL) {
			// remember pointer on stacks head
			QueueNode* temporary_pointer = queue->head;

			// set next node as head
			queue->head = queue->head->next;

			// free popped node
			free(temporary_pointer);
		}

		return AllAreOk;
	}
}


////// stack:
// add node to stack
// args: stack descriptor and pointer on vertex, that will be added
// returns result codes
int stack_push(Stack* stack, Vertex* vertex) {
	if (vertex != NULL && stack != NULL) {
		// if stack is empty just set head as new node
		if (stack->head == NULL) {
			// try to allocate memory
			stack->head = alloc_node();
			if (stack->head == NULL)
				return ErrorWithMemoryAllocation;

			// set new node
			stack->head->vertex = vertex;
			stack->head->next = NULL;
		}

		// else add new node on top
		else {
			// try to set new node
			Node* new_node = alloc_node();
			if (new_node == NULL)
				return ErrorWithMemoryAllocation;
			new_node->vertex = vertex;

			// link new node with stack head and set head as new node
			new_node->next = stack->head;
			stack->head = new_node;
		}

		return AllAreOk;
	}
	// hadle case is stack pointer is NULL or vertex pointer is NULL
	else
		return ErrorWithExisting;
}


// remove node from stack
// args: stack descriptor
Node* stack_pop(Stack* stack) {
	// handle if stack pointer is Null
	if (stack == NULL)
		return NULL;

	// or if stack is empty 
	else if (stack->head == NULL)
		return NULL;

	else { // remove node from top of stack 
		   // remember pointer on head
		Node* head_pointer = stack->head;

		// link pointer in descriptor with stack next node
		stack->head = stack->head->next;

		return head_pointer;
	}
}


// removes all nodes from stack
// returns result codes
int free_stack(Stack* stack) {
	// handle case if stack pointer is null
	if (stack == NULL)
		return ErrorWithExisting;
	else {
		// while stack isn't empty pop elements
		while (stack->head != NULL) {
			// remember pointer on stacks head
			Node* temporary_pointer = stack->head;

			// set next node as head
			stack->head = stack->head->next;

			// free popped node
			free(temporary_pointer);
		}

		return AllAreOk;
	}
}


// comparing of two stacks for calculating distance
int stack_compare(Stack* stack1, Stack* stack2) {
	// variables block
	int count = 0;
	// variables block

	// remove equal nodes
	stack_remove_similar(stack1, stack2);

	// count node that were left after removal
	while (stack1->head != NULL || stack2->head != NULL) {
		if (stack1->head != NULL) {
			free(stack_pop(stack1));
			count++;
		}
		if (stack2->head != NULL) {
			free(stack_pop(stack2));
			count++;
		}
	}

	return count;
}


// remove similar nodes from top in 2 stacks
void stack_remove_similar(Stack* stack1, Stack* stack2) {
	while (stack1->head != NULL && stack2->head != NULL)
		// if two nodes are equal, remove both, else end operation
		if (strncmp(stack1->head->vertex->data.word, stack2->head->vertex->data.word, WORDS_SIZE) == 0) {
			free(stack_pop(stack1));
			free(stack_pop(stack2));
		}
		else
			break;
}


////// hashtable:
// add nodes to hash table
// returns result codes
int hashtable_add_node(HashTable* hashtable, Vertex* value) {
	// calculate hash function
	size_t hash = vertex_hash(value);

	// try to allocate memory and set new node
	Node* new_node = alloc_node();
	if (new_node == NULL)
		return ErrorWithMemoryAllocation;
	new_node->vertex = value;

	// if array bin is empty just add new node
	if (hashtable->array[hash] == NULL)
		hashtable->array[hash] = new_node, new_node->next = NULL;
	// else add on top like in stack 
	else
		new_node->next = hashtable->array[hash], hashtable->array[hash] = new_node;

	return AllAreOk;
}


// check if node in hashtable exist
// result 1 - exist, 0 - not
int hashtable_check(HashTable* hashtable, Vertex* value) {
	// check args:
	if (hashtable == NULL || value == NULL)
		return 0;

	// variables:
	size_t hash = vertex_hash(value); // calculated vertex hash
	Node* temporary_pointer = hashtable->array[hash];

	// if array bin is empty return zero
	if (temporary_pointer == NULL)
		return 0;

	// while list hasn't ended search for needed value
	while (temporary_pointer != NULL) {
		// if value has been founded return true
		if (temporary_pointer->vertex == value)
			return 1;

		// go to the next node
		temporary_pointer = temporary_pointer->next;
	}

	// value hasn't been founded
	return 0;
}


// free all lists from hash table
void free_hashtable(HashTable* hashtable) {
	// variables
	Node* temporary_pointers[2];

	// clear all lists in hashtable while they aren't empty
	for (size_t counter = 0; counter < HASH_TABLE_SIZE; counter++) {
		temporary_pointers[0] = hashtable->array[counter];

		while (temporary_pointers[0] != NULL) {
			// remember current node
			temporary_pointers[1] = temporary_pointers[0];

			// go to the next
			temporary_pointers[0] = temporary_pointers[0]->next;

			// free current
			free(temporary_pointers[1]);
		}
	}
}


////// extra:
// shows word in breadth show
void show_word(Vertex* vertex, char left_symbol, char right_symbol) {
	// variables block
	char left_exist = false;
	char right_exist = false;
	// variables block

	// check arguments
	if (vertex == NULL)
		return;

	// check if childs exist
	if (vertex->right != NULL)
		right_exist = true;
	if (vertex->left != NULL)
		left_exist = true;

	// show word
	if (left_exist)
		putchar(left_symbol);
	if (((long int)SHOWING_SIZE - left_exist - right_exist) >= 0) {
		mloop(SHOWING_SIZE - left_exist - right_exist)
			if (vertex->data.word[counter] == '\0' || vertex->data.word[counter] == '\n')
				break;
			else
				putc(vertex->data.word[counter], stdout);
	}
	if (right_exist)
		putchar(right_symbol);
}


// show word in showing in lenght
void show_word(Vertex* vertex) {
	// check arguments
	if (vertex == NULL)
		return;

	mloop(300000)
		if (vertex->data.word[counter] == '\0' || vertex->data.word[counter] == '\n')
			break;
		else
			putc(vertex->data.word[counter], stdout);
}


// calculates hash of vertex
int vertex_hash(Vertex* value) {
	return (int)value % HASH_TABLE_SIZE;
}


// try to allocate node ATTEMPTS_COUNT times
// returns pointer on node or NULL
Node* alloc_node() {
	// variables:
	int count = 0;
	Node* temporary_pointer = NULL;

	// try ATTEMPTS_COUNT times to allocate memory for stack node in heap
	while (temporary_pointer == NULL && count < ATTEMPTS_COUNT)
		count++, temporary_pointer = (Node*)calloc(1, sizeof(Node));

	return temporary_pointer;
}


// allocate queue node
QueueNode* alloc_qnode () {
		// variables:
	int count = 0;
	QueueNode* temporary_pointer = NULL;

	// try ATTEMPTS_COUNT times to allocate memory for stack node in heap
	while (temporary_pointer == NULL && count < ATTEMPTS_COUNT)
		count++, temporary_pointer = (QueueNode*)calloc(1, sizeof(QueueNode));

	return temporary_pointer; 
}


// allocate memory in heap to vertex structure
// returns NULL if memory hasn't been allocated
// returns pointer on allocated vertex if all is ok
Vertex* alloc_vertex() {
	// variables:
	int count = 0; // number of allocation attempts
	Vertex* temporary_pointer = NULL;

	// try ATTEMPTS_COUNT times to allocate memory
	while (temporary_pointer == NULL && count < ATTEMPTS_COUNT)
		count++, temporary_pointer = (Vertex*)calloc(1, sizeof(Vertex));

	return temporary_pointer;
}
