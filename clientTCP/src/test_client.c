#include "test_client.h"

int test_unit() {

	CU_initialize_registry();
	agregar_tests();
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();

	return CU_get_error();
}

void agregar_tests(){
	CU_pSuite suite_cliente = CU_add_suite("Suite de cliente", inicializar, limpiar);
	CU_add_test(suite_cliente, "test cliente", test_cliente);
}

int inicializar() {
	//if(error)
	//	return -1;
	return 0;
}

int limpiar(){
	return 0;
}

void test_cliente() {
	int i = 2;
	CU_ASSERT_EQUAL(i, 2);
}
