/*
 ============================================================================
 Name        : CUnit-test.c
 Author      : Nicolas Pini
 Version     :
 Copyright   : http://www.gnu.org/licenses/
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <unistd.h>
#include "archivo.h"

static char* g_path = "prueba.txt";
static FILE* g_archivo = NULL;

int inicializar() {
	unlink(g_path); //borra el archivo
	g_archivo = fopen(g_path, "w+");
	return g_archivo != NULL ? 0 : -1;
}

int limpiar() {
	fclose(g_archivo);
	return unlink(g_path);
}

void test1() {
	printf("Soy el test 1!, y pruebo que 2 sea igual a 1+1");
	CU_ASSERT_EQUAL(1+1, 2);
}

void test2() {
	printf("Soy el test 2!, y doy segmentation fault");
	char* ptr = NULL;
	*ptr = 9;
}

void test3() {
	printf("Soy el test 3!");
}

void test_contar_devuelve_menos1_si_el_archivo_no_existe() {
	int cantidad = archivo_contar("askjd.txt", 'f');
	CU_ASSERT_EQUAL(cantidad, -1);
}

void test_contar_devuelve_el_numero_exacto_de_ocurrencias() {
	inicializar();
	fprintf(g_archivo, "aca hay como 4 as");
	fflush(g_archivo); //para que se guarde en disco ahora
	int cantidad = archivo_contar("prueba.txt", 'a');
	CU_ASSERT_EQUAL(cantidad, 4);
}

void test_contar_devuelve_0_si_no_hay_ocurrencias() {
	inicializar();
	fprintf(g_archivo, "no escribo ninguna equis"); fflush(g_archivo);
	int cantidad = archivo_contar(g_path, 'x');
	CU_ASSERT_EQUAL(cantidad, 0);
}

void agregar_test(){

	// CU_add_suite(strName, pInit, pClean)
	// donde                 pInit -> funcion de inicializacion
	//                       pClean -> funcion limpieza luego de correr la suite
	// retorna un puntero a la suite creada o null en caso de error
	CU_pSuite prueba = CU_add_suite("Suite de prueba", inicializar, limpiar);

	// CU_add_test(pSuite, strName, pTest)
	// donde               pSuite -> puntero a la suite donde agregamos el test
	//                     strName -> nombre de referencia de la prueba
	//                     pTest -> puntero a la fuincion del test.
	CU_add_test(prueba, "uno", test1);
	//CU_add_test(prueba, "dos", test2);
	CU_add_test(prueba, "tres", test3);
	CU_add_test(prueba, "test_contar_devuelve_menos1_si_el_archivo_no_existe", test_contar_devuelve_menos1_si_el_archivo_no_existe);
	CU_add_test(prueba, "test_contar_devuelve_el_numero_exacto_de_ocurrencias", test_contar_devuelve_el_numero_exacto_de_ocurrencias);

}

int main(void) {

	CU_initialize_registry();
	agregar_test();
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}

