// lab 7 source
// L1ttl3S1st3r 13.05.2018
# include "app.h"


int main() {
	Treap treap = { 0 };

	main_loop(&treap);

    free_treap(treap);

    return 0;
}






