/*
 * tests.h
 *
 *  Created on: 16/6/2016
 *      Author: utnso
 */

#ifndef TESTS_TESTS_H_
#define TESTS_TESTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <pthread.h>
#include "../threads.h"

void test_thread_join();
void test_thread_detached();

#endif /* TESTS_TESTS_H_ */
