// lab 7 source
// L1ttl3S1st3r 13.05.2018
# include "app.h"

// list of testing&debbuging:
// 1. recursive shows
// a) pre +
// b) in +
// c) post +
// 2. deleting node +
// 3. iterative shows
// a) pre +
// b) in +
// c) post +
// 4. calc of distance


int main() {
	Treap treap = { 0 };

	main_loop(&treap);

    free_treap(treap);

    return 0;
}






