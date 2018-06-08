# include "app.h"
# include <stdio.h>

// shows result of operation
void report_task_result(char* string, int result_code) {
	if(string != NULL)
		puts(string);

	switch (result_code) {
	case AllAreOk:
		puts("All is alright");
		break;

	case ErrorWithExisting:
		puts("Error with existing of something");
		break;

	case ErrorWithMemoryAllocation:
		puts("Error with memory allocation in heap");
	}
}


// does setted task
void do_task(unsigned char task, Treap* treap) {
	// variables:
	char buffer1[WORDS_SIZE];
	char buffer2[WORDS_SIZE];
	char choose;
	Data data_buffer;
	int distance = 0;
	int buffer;

	switch (task) {
	case 1: // add node
		rewind(stdin);
		get_data(&data_buffer);
		report_task_result("adding node: ", add_vertex(treap, &data_buffer));
		break;

	case 2: // show nodes
		show_showing_menu();

		// get choose
		rewind(stdin);
		choose = getchar();
		
		// do task
		if (choose <= 'c' && choose >= 'a')
			show_treap(*treap, choose - 'a');
		else
			puts("Wrong choose");
		
		break;

	case 3: // delete node
		rewind(stdin);
		set_word_from_input(&data_buffer);
		report_task_result("deleting node:", remove_vertex(treap, &data_buffer));
		break;

	case 4: // show nodes iterative
		show_showing_menu();

		// get choose
		rewind(stdin);
		choose = getchar();

		// do task
		if (choose <= 'c' && choose >= 'a')
			report_task_result("showing nodes: ", iterative_show_treap(*treap, choose - 'a'));
		else
			puts("Wrong choose");

		break;

	case 5: // extra
		// get data
		rewind(stdin);
		puts("Input word one:");
		fgets(buffer1, WORDS_SIZE, stdin);
		rewind(stdin);
		puts("Input word two:");
		fgets(buffer2, WORDS_SIZE, stdin);

		// calculate distance
		distance = calculate_distance(treap, buffer1, buffer2);

		// show results:
		if (distance == (-1)) {
			puts("Error arguments");
			break;
		}
		printf("Distance between %s and %s is %i\n", buffer1, buffer2, distance);

		break;

	case 6: // extra x2
		// get data
		rewind(stdin);
		puts("Input word one:");
		fgets(buffer1, WORDS_SIZE, stdin);
		rewind(stdin);
		puts("Input word two:");
		fgets(buffer2, WORDS_SIZE, stdin);

		// calculate distance
		buffer = calculate_distance(treap, buffer1, buffer2, &distance);

		// show results:
		if (buffer == ErrorWithExisting) {
			puts("Error: such node doesn't exist");
			break;
		}
		if (buffer == ErrorWithMemoryAllocation) {
			puts("Error: can't allocate memory");
			break;
		}
		printf("Distance between %s and %s is %i\n", buffer1, buffer2, distance);

	break;

	case 7: // extra x3
		show_in_breadth(*treap);
		break;

	case 8: // extra x4
		show_in_length(treap->root, 0);
		break;

	case 9: // exit
		break;

	default:
		puts("Wrong number");
	}
}


void set_word_from_input(Data* data) {
	printf("Input word:");
	fgets(data->word, WORDS_SIZE, stdin);
}


void main_loop(Treap* treap) {
	// variables:
	int choose = 0;

	do {
		// show simple menu
		show_menu();

		// get choose
		rewind(stdin);
		scanf_s("%i", &choose);

		// do task
		do_task(choose, treap);
	} while (choose != 9);
}


void show_menu() {
	puts("Choose your option:");
	puts("(1) Add node");
	puts("(2) Show nodes");
	puts("(3) Delete node");
	puts("(4) Show nodes (without recursion)");
	puts("(5) Extra task: find distance between two nodes");
	puts("(6) Extra task: find distance between two nodes using much more memory");
	puts("(7) Extra task: show nodes in breadth");
	puts("(8) Extra task: show nodes in lenght");
	puts("(9) Exit");
}


void show_showing_menu() {
	puts("Choose how do you want to show nodes:");
	puts("(a) in preorder");
	puts("(b) in inorder");
	puts("(c) in postorder");
}
