# include <stdio.h>
# include "Data.h"

// show data in stdout
void show_data(Data* data) {
	printf("word: %s", data->word);
	printf("translation: %s", data->translation);
}


// get data from stdin
void get_data(Data* data) {
	puts("Input word:");
	fgets(data->word, WORDS_SIZE, stdin);
	puts("Input its translation:");
	fgets(data->translation, WORDS_SIZE, stdin);
}