#pragma once
// Little Sister
// "Good input and output" version 1.1 beta
// README:
/*
	This is a library for good and safe input/ output and memory
	allocation. 

	Main functions are:
	1. input() and output()
	2. binary input and output() (binput() and boutput())
	3. memalloc()

	Main type - stream.
	Stream is a safe substitute of usial C FILE struct.
	It contains FILE* pointer and variables, which describe 
	a file mode.
	To create a stream just use create_stream() and 
	to delete - del_stream().

	You can look at other streams functions at the end of this file.
*/
// TO DO: add list, array, set and others types
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// structs:

typedef struct Stream {
	FILE* stream_file;
	int is_binary;
	int type; // StreamTypes
} Stream;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constants:
enum InputTypes { // Types for input
	MYINT = 0, MYFLOAT, MYSTRING 
};
enum StreamTypes { // type of work with file. Common type means that you can write, add, read and ect 
	READ = 0, WRITE, ADD, COMMON, NULL_STREAM
};

const int INPUT_PARAMETERS = 3;
const int PANIC_ENABLED = 1;
const Stream standart_input = { stdin, 0, COMMON };
const Stream standart_output = { stdout, 0, COMMON };
const Stream null_stream = { NULL, 0, NULL_STREAM };
const  int MAX_SIZE = 20; // the max size of a string
const  int MAX_ATTEMPT = 20; // the max number of attempts to allocate memory
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// functions:

//// binary input/output:

// get various variables
// args: ptr on a variable, its size, number of elements
// string, which will appear before input and source stream
int binput(void* variable_pointer = NULL, int size = 0,
		   int element_count = 0, char* greatings = NULL, 
		   Stream stream = null_stream);

// write various variables
// args: ptr on a variable, its size, number of elements
// string, which will appear before output and dest stream
int boutput(void* variable_pointer = NULL, int size = 0,
			int element_count = 0, char* assist_string = NULL,
			Stream stream = null_stream);

//// text input/output:

// get various variables
// args: pointer on a variable, its type, 
// length of the string, string which will appear before input.
// source stream
int input(void* variable_pointer = NULL, int type = 0, 
		  int length = 0, char* greatings = NULL,
		  Stream stream = standart_input);

// print various variables
// args: pointer on a variable, its type, 
// length of the string, string which will appear before input.
// dest stream, extra end character
int output(void* variable_pointer = NULL, int type = 0,
		   char* assist_string = NULL, char end = 0, Stream stream = standart_output);

// get a string from a source stream
// my alternative to fgets, 
// because I hate '\n' in strings, 
// args: string pointer, 
// max quantity of symbols with '\0', (stream) source
void get_string(char* str, int n, Stream stream = standart_input);

// get an integer from a stream
// pointer to an integer and (stream) source
void get_int(int* p, Stream stream = standart_input);

// get a float froam a stream
// pointer to a float and (stream) source
void get_float(float* p, Stream stream = standart_input);

//// memory and Stream options

// allocates memory with size <size * n>
void* memalloc(int size, int n = 1);

// returns true if the end of the stream file has been reached
// args: stream
int stream_end(Stream stream);

// determines streams type and if it is binary
// args: stream pointer an args of create_stream
void define_stream(Stream* stream_ptr, char* args);

// creates a stream with this file name and condition
// args: file name and args for fopen [simple example - "ba+"]
Stream create_stream(char* file_name, char* args);

// set files pointer in stream
// args: position (>= 0 from start, <0 from end),
// stream
void stream_setpos(int position, Stream stream);

// returns files pointer in stream
// args: stream
int stream_getpos(Stream stream);

// fis possible errors in stream
// args: stream
void stream_fix(Stream stream);

// deletes a stream and set stream as null stream
void del_stream(Stream* stream_ptr);

// check if argument is equal to null stream
// returns true if it is
int is_nullstream(Stream stream);

//// extra io functions:

// reads characters until fun don't meet some integer numberals 
void skip_trash_characters(char* buffer_variable_pointer, Stream stream);

// determine sign in my analog of atoi
// args: pointer on the character with potential sign
// source stream
int define_sign(char* buffer_variable_pointer, Stream stream);

// print an error with such text
int print_error(char* error_text);
