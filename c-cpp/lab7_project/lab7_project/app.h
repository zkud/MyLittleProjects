#pragma once

# include "Lab7Collections.h"

// function prototypes:
// shows result of operation
void report_task_result(char* string, int result_code);


// does setted task
void do_task(unsigned char task, Treap* treap);


// sets word in data structure from stdin
void set_word_from_input(Data* data);


// app main loop
void main_loop(Treap* treap);


// menu in main_loop
void show_menu();


// menu in do_task
void show_showing_menu();