#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "goodio.h"
// File contains functions in these order:
// 1. Binary input and output
// 2. Text input and output
// 3. Memory and Stream functions
// 4. Others
// архитектура либы:
// 4 функции для безопасного и удобного ввода/вывода
// + новый тип для файлов


// binary io:
int boutput(void* variable_pointer, int size, 
			int element_count, char* assist_string, Stream stream) {
	// check for bad stream type
	if (!stream.is_binary)
		return print_error("| bad streams type in the binput|");

	// check arguments for potential errors
	if (!variable_pointer || is_nullstream(stream))
		return print_error("| bad arguments in the binput|");

	// print an assist string
	if (assist_string)
		puts(assist_string);

	// read a variable
	fwrite(variable_pointer, size, element_count, stream.stream_file);

	return 0;
}

int binput(void* variable_pointer, int size,
		   int element_count, char* greatings, Stream stream) {
	// check for bad stream type
	if (!stream.is_binary)
		return print_error("| bad streams type in the binput|");

	// check arguments for potential errors
	if (!variable_pointer || is_nullstream(stream))
		return print_error("| bad arguments in the binput|");

	// say hello
	if (greatings)
		puts(greatings);

	// read a variable
	fread(variable_pointer, size, element_count, stream.stream_file);

	return 0;
}

// text io:
int output(void* variable_pointer, int type,
		   char* assist_string, char end, Stream stream) {

	// check fast for potential errors
	if (!variable_pointer || is_nullstream(stream))
		return print_error("| bad arguments in the output|");

	// check for wrong streams type
	if (stream.is_binary)
		return print_error("| bad streams type in the output|");

	// print an assist string
	if (assist_string)
		puts(assist_string);

	// print variable
	switch (type) {
	case MYINT:
		fprintf(stream.stream_file, "%d", *(int*)variable_pointer);
		if(end)
			putc(end, stream.stream_file);
		break;
	
	case MYFLOAT:
		fprintf(stream.stream_file, "%f", *(float*)variable_pointer);
		if (end)
			putc(end, stream.stream_file);
		break;

	case MYSTRING:
		fprintf(stream.stream_file, "%s", (char*)variable_pointer);
		if (end)
			putc(end, stream.stream_file);
		break;

	default:
		return print_error("| bad type in the output|");
	}

	return 0;
}

int input(void* variable_pointer, int type,
		  int length, char* greatings, Stream stream) {
	// You can see types in the InputTypes; lenght = string length
	// check if all parametrs are good
	if (stream.is_binary || !stream.stream_file || stream.type == WRITE)
		return print_error("|error parameters in the input|");

	// say hello
	if (greatings)
		puts(greatings);

	switch (type) {
	case MYINT: // int
		get_int((int*)variable_pointer, stream);
		break;

	case MYFLOAT: // float
		get_float((float*)variable_pointer, stream);
		break;

	case MYSTRING: // c_string
		rewind(stream.stream_file);
		get_string((char*)variable_pointer, length, stream);
		break;
	default:
		return print_error("|Error in the inputs type|");
	}

	return 0;
}

void get_float(float* variable_pointer, Stream stream) {
	char buffer[MAX_SIZE * 5] = {};
	get_string(buffer, MAX_SIZE * 5, stream);
	*variable_pointer = (float)atof(buffer);
}

void get_int(int* variable_pointer, Stream stream) {
	char buffer_variable = 1; // buffer variable for input
	int sign = 1; // 1 - plus, -1 - minus
	*variable_pointer = 0;

	/*
	there are only ten demical numerals in 4 bytes (maximum int size)
	and only 5 in 2 bytes (minimum int size)
	*/
	// set a max int size in demical numerals
	int max_count;
	if (sizeof(int) == 4)
		max_count = 11;
	else
		max_count = 6;

	// skip all unsuitable symbols
	skip_trash_characters(&buffer_variable, stream);

	// handle case if there isn't any variables
	if (feof(stream.stream_file) || buffer_variable == '\0') {
		*(int*)variable_pointer = 0;
		return;
	}

	//handle with sign
	sign = define_sign(&buffer_variable, stream);

	//read integer numbers
	while (buffer_variable >= '0' && buffer_variable <= '9'
		&& !feof(stream.stream_file) && max_count) {
		*variable_pointer *= 10;
		*variable_pointer += buffer_variable - '0';
		buffer_variable = getc(stream.stream_file);
		max_count--;
	}

	*variable_pointer *= sign;
}

void get_string(char* str, int n, Stream stream) {
	char buffer = 1; // true value

	while (n && buffer) { // add symbols to a string, while max lenght, '\n', '\0' or EOF aren't reached
		buffer = getc(stream.stream_file);
		if (buffer == '\0' || buffer == '\n' || feof(stream.stream_file))
			buffer = '\0';
		*str = buffer;
		str++, n--;
	}

	--str, *str = '\0'; // set the last symbol of a string as '\0'
}

// memory and Stream functions:
void* memalloc(int size, int n) {
	void* pointer = NULL;
	int i;
	for (i = 0; i < MAX_ATTEMPT && !pointer; i++) {
		pointer = malloc(size * n);
	}
	if (i == MAX_ATTEMPT) {
		if (pointer)
			free(pointer);
		fprintf(stderr, "ERROR IN  ALLOCATION");
		fprintf(stdout, "ERROR IN  ALLOCATION");
		return NULL;
	}
	return pointer;
}

void define_stream(Stream* stream_ptr, char* args) {
	// variables:
	char* ptr = args;
	int has_common_type = 0;

	// normally stream type is text
	stream_ptr->is_binary = 0;

	while (*ptr) { // cycle on args to define streams type
		switch (*ptr) {
		case 'b':
			stream_ptr->is_binary = 1;
			break;

		case 'w':
			stream_ptr->type = WRITE;
			break;

		case 'a':
			stream_ptr->type = ADD;
			break;

		case 'r':
			stream_ptr->type = READ;
			break;

		case 't':
			break;

		case '+':
			has_common_type = 1;
			break;

		default:
			fprintf(stderr, "|Error in the streams type definition|");
			if (PANIC_ENABLED)
				fprintf(stdout, "|Error in the streams type definition|");
		}

		ptr++;
	}

	//if stream has common type set streams type as COMMON
	if (has_common_type)
		stream_ptr->type = COMMON;
}

Stream create_stream(char* file_name, char* args) {
	Stream new_stream; 
	// variable of a new stream
	
	//try to open file
	fopen_s(&(new_stream.stream_file), file_name, args);

	// if some strange shit has happened try say it
	if (!new_stream.stream_file) {
		print_error("|Error in the stream creation|");
		return null_stream;
	}

	// check if stream may be binary and define stream type
	define_stream(&new_stream, args);

	return new_stream;
}

int stream_end(Stream stream) {
	return feof(stream.stream_file);
}

void stream_setpos(int position, Stream stream) {
	// set files pointer from start
	if (position >= 0)
		fseek(stream.stream_file, position, SEEK_SET);
	// set files pointer from end
	else
		fseek(stream.stream_file, position + 1, SEEK_END);
}

int stream_getpos(Stream stream) {
	return ftell(stream.stream_file);
}

void stream_fix(Stream stream) {
	rewind(stream.stream_file);
}

void del_stream(Stream* stream_ptr) {
	fclose(stream_ptr->stream_file);
	stream_ptr->is_binary = 0;
	stream_ptr->type = NULL_STREAM;
}

int is_nullstream(Stream stream) {
	// check if stream is null
	if (!stream.is_binary && !stream.stream_file && stream.type == NULL_STREAM)
		return 1; // true
	else
		return 0; // false
}

// extra io functions:
void skip_trash_characters(char* buffer_variable_pointer, Stream stream) {
	do { // read next characters until fun don't meet good characters
		*buffer_variable_pointer = getc(stream.stream_file);
	} while (*buffer_variable_pointer != '\0' && *buffer_variable_pointer != '-'
		&& *buffer_variable_pointer != '+'
		&& (*buffer_variable_pointer < '0' || *buffer_variable_pointer > '9')
		&& !feof(stream.stream_file));
}

int define_sign(char* buffer_variable_pointer, Stream stream) {
	int sign = 1; // -1 = minus, 1 = plus

	switch (*buffer_variable_pointer) {
	case '+':
		sign = 1;
		*buffer_variable_pointer = getc(stream.stream_file);
		break;

	case '-':
		sign = -1;
		*buffer_variable_pointer = getc(stream.stream_file);
		break;
	}

	return sign;
}

int print_error(char* error_text) {
	fprintf(stderr, error_text);

	if (PANIC_ENABLED) // if panic enabled show error in stdout out 
		fprintf(stdout, error_text);

	return (-1);
}