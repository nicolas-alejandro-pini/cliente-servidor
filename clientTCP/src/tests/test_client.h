#ifndef TEST_CLIENT_H_
#define TEST_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <CUnit/Basic.h>

/*
CU_ASSERT_TRUE(value): Verifica que una expresión sea verdadera
CU_ASSERT_FALSE(value): Verifica que una expresión sea falsa
CU_ASSERT_EQUAL(actual, expected): Verifica que actual == expected
CU_ASSERT_NOT_EQUAL(actual, expected)
CU_ASSERT_STRING_EQUAL(actual, expected): Verifica que dos strings sean equivalentes
CU_ASSERT_STRING_NOT_EQUAL(actual, expected)
CU_ASSERT_PTR_EQUAL(actual, expected): Verifica que los punteros sean equivalentes
CU_ASSERT_PTR_NOT_EQUAL(actual, expected)
CU_ASSERT_PTR_NULL(value): Verifica que un puntero es NULL
CU_ASSERT_PTR_NOT_NULL(value)
CU_ASSERT_NSTRING_EQUAL(actual, expected, count): Verifica que los primeros count caracteres de las cadenas coinciden
 */

int test_unit();
void agregar_tests();
int inicializar();
int limpiar();

// Funciones casos de prueba
void test_cliente();

#endif
