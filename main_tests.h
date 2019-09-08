#include<stdio.h>
#include"matrizv3.h"
#include"matriz-operacoesv3.h"
#include"test.h"

int test_msoma_diferente() {
  mymatriz matriz_a;
  mymatriz matriz_b;

  matriz_a.lin = 2;
  matriz_a.col = 2;
  matriz_a.matriz = NULL;
  matriz_b.lin = 2;
  matriz_b.col = 3;
  matriz_b.matriz = NULL;

  mymatriz* matriz_resultado = msomar(&matriz_a, &matriz_b, 0);
  return ASSERT(
		matriz_resultado->lin == 0 &&
		matriz_resultado->col == 0
		, "test_msoma_diferente");
}

int test_msoma_sem_alocar() {
  mymatriz matriz_a;
  mymatriz matriz_b;

  matriz_a.lin = 2;
  matriz_a.col = 2;
  matriz_a.matriz = NULL;
  matriz_b.lin = 2;
  matriz_b.col = 2;
  matriz_b.matriz = NULL;

  mymatriz* matriz_resultado = msomar(&matriz_a, &matriz_b, 0);
  return ASSERT(
		matriz_resultado->lin == 0 &&
		matriz_resultado->col == 0
		, "test_msoma_sem_alocar");
}

int test_msoma_somada(int tipo) {
  mymatriz matriz_a;
  mymatriz matriz_b;
  
  matriz_a.lin = 2;
  matriz_a.col = 3;
  malocar(&matriz_a);
  mgerar(&matriz_a, -9999);
  
  matriz_b.lin = 2;
  matriz_b.col = 3;
  malocar(&matriz_b);
  mgerar(&matriz_b, -9999);

  mymatriz* matriz_resultado = msomar(&matriz_a, &matriz_b, tipo);
  char buffer[43];
  sprintf(buffer, "test_matriz_somada  %d", tipo);
  
  return ASSERT(
	matriz_resultado->matriz[0][0] ==
	matriz_a.matriz[0][0] + matriz_b.matriz[0][0] &&

	matriz_resultado->matriz[0][1] ==
	matriz_a.matriz[0][1] + matriz_b.matriz[0][1], buffer);
}

mymatriz new_matriz(int lin, int col) {
  mymatriz matriz;
  matriz.lin = lin;
  matriz.col = col;
  malocar(&matriz);
  mgerar(&matriz, -9999);
  return matriz;
}

int test_mmultiplicar_multiplicado(int tipo) {
  mymatriz mat_a = new_matriz(2, 3);
  mymatriz mat_b = new_matriz(3, 2);
  mymatriz* mat_r = mmultiplicar(&mat_a, &mat_b, tipo);
  char buffer[43];
  sprintf(buffer, "test_mmultiplicar_multiplicado %d", tipo);
  return ASSERT(
		mat_r->matriz[0][0] ==
		mat_a.matriz[0][0] * mat_b.matriz[0][0] +
		mat_a.matriz[0][1] * mat_b.matriz[1][0] +
		mat_a.matriz[0][2] * mat_b.matriz[2][0]
		, buffer);
}

int test_matrizes() {
  return test_msoma_diferente() &&
    test_msoma_sem_alocar() &&
    test_msoma_somada(0) &&
    test_msoma_somada(1) &&
    test_mmultiplicar_multiplicado(0) &&
    test_mmultiplicar_multiplicado(1) &&
    test_mmultiplicar_multiplicado(2) &&
    test_mmultiplicar_multiplicado(3) &&
    test_mmultiplicar_multiplicado(4) &&
    test_mmultiplicar_multiplicado(5) &&
    test_mmultiplicar_multiplicado(6);
}

int tests() {
  return test_matrizes();
}


int can_execute_tests(int argc, char** argv) {
  return argc > 1 && strcmp("--test", argv[1]) == 0;
}

int execute_tests() {
  return tests() == 1 ? 0 : -1;
}



