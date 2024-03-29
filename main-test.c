#include<stdio.h>
#include"matrizv3.h"
#include"matriz-operacoesv3.h"
#include"test.h"
#include"main_tests.h"
#include"matriz-operacoes-thread.h"

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

mymatriz gerar_matriz(int lin, int col, int matriz[lin][col]) {
  mymatriz mmatriz;
  mmatriz.col = col;
  mmatriz.lin = lin;
  malocar(&mmatriz);

  mmatriz.matriz = malloc(sizeof(int*)*lin);
  for(int i=0;i<lin;i++){
    mmatriz.matriz[i] = malloc(sizeof(int)*col);
    for(int j=0;j<col;j++) {
      mmatriz.matriz[i][j] = matriz[i][j];
    }
  }
  
  return mmatriz;
}

int test_particionar_matriz_vertical() {
  int valores[2][2] = {
    {1,2},
    {3,4}
  };

  mymatriz matriz = gerar_matriz(2, 2, valores);
  matriz_bloco_t **submatrizes = particionar_matriz(matriz.matriz, matriz.lin, matriz.col, 0, 2);
  
  return ASSERT(
		submatrizes != NULL &&
		submatrizes[0] != NULL &&
		submatrizes[1] != NULL &&
		submatrizes[0]->bloco->lin_inicio == 0 &&
		submatrizes[0]->bloco->lin_fim == 1 &&
		submatrizes[0]->bloco->col_inicio == 0 &&
		submatrizes[0]->bloco->col_fim == 2 &&
		submatrizes[1]->bloco->lin_inicio == 1 &&
		submatrizes[1]->bloco->lin_fim == 2 &&
		submatrizes[1]->bloco->col_inicio == 0 &&
		submatrizes[1]->bloco->col_fim == 2
		, "test_particionar_matriz_vertical");
}

int test_particionar_matriz_horizontal() {
  int valores[2][2] = {
    {1,2},
    {3,4}
  };

  mymatriz matriz = gerar_matriz(2, 2, valores);
  matriz_bloco_t** submatrizes = particionar_matriz(matriz.matriz, matriz.lin, matriz.col, 1, 2);
  
  return ASSERT(
		submatrizes != NULL &&
		submatrizes[0] != NULL &&
		submatrizes[1] != NULL &&
		submatrizes[0]->bloco->lin_inicio == 0 &&
		submatrizes[0]->bloco->lin_fim == 2 &&
		submatrizes[0]->bloco->col_inicio == 0 &&
		submatrizes[0]->bloco->col_fim == 1 &&
		submatrizes[1]->bloco->lin_inicio == 0 &&
		submatrizes[1]->bloco->lin_fim == 2 &&
		submatrizes[1]->bloco->col_inicio == 1 &&
		submatrizes[1]->bloco->col_fim == 2
		, "test_particionar_matriz_horizontal");
}

int test_aloca_matrizes_bloco() {
  matriz_bloco_t** submatrizes = csubmatrizv2(4, 4, 2);
  return ASSERT(
		submatrizes != NULL &&
		submatrizes[0] != NULL &&
		submatrizes[1] != NULL &&
		submatrizes[0]->matriz->lin == 4 &&
		submatrizes[1]->matriz->col == 4 &&
		submatrizes[0]->bloco != NULL &&
		submatrizes[1]->bloco != NULL
		, "test_aloca_matrizes_bloco");
}

int test_multiplica_submatriz() {
  int valores[2][2] = {
    {83,86},
    {77,15}
  };


  mymatriz matriz = gerar_matriz(2, 2, valores);
  
  matriz_bloco_t** submatrizesa = particionar_matriz(matriz.matriz, matriz.lin, matriz.col, 1, 2);
  matriz_bloco_t** submatrizesb = particionar_matriz(matriz.matriz, matriz.lin, matriz.col, 0, 2);

  matriz_bloco_t** submatrizesc = csubmatrizv2(2, 2, 2);

  mmsubmatriz(submatrizesa[0],submatrizesb[0], submatrizesc[0]);
  mmsubmatriz(submatrizesa[1],submatrizesb[1], submatrizesc[1]);
  
  mymatriz *matriz_result = msomar(submatrizesc[0]->matriz, submatrizesc[1]->matriz, 0);
  
  return ASSERT(matriz_result->matriz[0][0] == 13511 &&
		matriz_result->matriz[0][1] == 8428 &&
		matriz_result->matriz[1][0] == 7546 &&
		matriz_result->matriz[1][1] == 6847, "test_multiplica_submatriz");
}


int teste_multiplicacao_bloco_thread() {
  mymatriz *matriz_a = criar_matriz_vazia(50, 50);
  mymatriz *matriz_b = criar_matriz_vazia(50, 50);
  
  mymatriz *matriz_sequencial =  mmultiplicar(matriz_a, matriz_b, 0);
  mymatriz *matriz_paralela = mmultiplicar_thread(matriz_a, matriz_b, 0);

  return ASSERT(
		mcomparar(matriz_sequencial, matriz_paralela),
		"teste_multiplicacao_bloco_thread");
}

int test_matrizes() {
  int result = 0;
  result += test_msoma_diferente();
  result += test_msoma_sem_alocar();
  result += test_particionar_matriz_vertical();
  result += test_particionar_matriz_horizontal();
  result += test_aloca_matrizes_bloco();
  result += test_msoma_somada(0);
  result += test_msoma_somada(1);
  result += test_mmultiplicar_multiplicado(0);
  result += test_mmultiplicar_multiplicado(1);
  result += test_mmultiplicar_multiplicado(2);
  result += test_mmultiplicar_multiplicado(3);
  result += test_mmultiplicar_multiplicado(4);
  result += test_mmultiplicar_multiplicado(5);
  result += test_mmultiplicar_multiplicado(6);
  result += test_multiplica_submatriz();
  result += teste_multiplicacao_bloco_thread();
  return result;
}

int tests() {
  return test_matrizes();
}


int can_execute_tests(int argc, char** argv) {
  return argc > 1 && strcmp("--test", argv[1]) == 0;
}

int execute_tests() {
  return tests() > 0 ? 0 : -1;
}
