#pragma once
// simple lib with data structure and linked input/output

//// constants:
const int WORDS_SIZE = 256; // size of char* string in Data structure

//// structures:
// data structure:
typedef struct Data {
	char word[WORDS_SIZE];
	char translation[WORDS_SIZE];
} Data;


//// functions prototypes:
// show data in stdout
void show_data(Data* data);


// get data from stdin
void get_data(Data* data);


