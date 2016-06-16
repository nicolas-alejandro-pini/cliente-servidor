/*
 ============================================================================
 Name        : threads.c
 Author      : Nicolas Pini
 Version     :
 Copyright   : http://www.gnu.org/licenses/
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "threads.h"
#include "tests/tests.h"

int main(int argc, char* argv[]) {

	if(strcmp(argv[1], "--cunit") == 0)
		//test_thread_join();
		test_thread_detached();

	printf("Fin.\n");
	return EXIT_SUCCESS;
}

void start_routine_example(void *arg){
	char *msg = (char*) arg;
	printf("Thread ID: [%u] | %s\n", (unsigned int)pthread_self(), msg);
}
