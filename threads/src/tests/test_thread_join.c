/*
 * test_thread_join.c
 *
 *  Created on: 16/6/2016
 *      Author: utnso
 */
#include "tests.h"

void test_thread_join(){

	int i;
	char *msg[] = { "Soy el Thread 1!", "Soy el Thread 2!", "Soy el Thread 3!", "Soy el Thread 4!",
			        "Soy el Thread 5!"};
	pthread_t threads[5];

	for( i=0; i<5; i++)
		if( pthread_create(&threads[i], NULL, (void*)&start_routine_example, msg[i]) ){
			perror("Ocurrio un error durante la creacion del Thread.");
		}

	for( i=0; i<5; i++)
		pthread_join(threads[i], NULL);
}

void test_thread_detached(){
	pthread_attr_t attr;
	pthread_t threads[5];
	int i;
	char *msg[] = { "Soy el Thread 1!", "Soy el Thread 2!", "Soy el Thread 3!", "Soy el Thread 4!",
			        "Soy el Thread 5!"};

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	for( i=0; i<5; i++)
		if( pthread_create(&threads[i], NULL, (void*)&start_routine_example, msg[i])){
			perror("Ocurrio un error durante la creacion del Thread.");
		}

	pthread_attr_destroy(&attr);
}
