#include<stdio.h>
#include"matriz-operacoesv3.h"
#include"matrizv3.h"

mymatriz* matriz_vazia() {
  mymatriz* matriz = malloc(sizeof(mymatriz));
  matriz->lin = 0;
  matriz->col = 0;
  matriz->matriz = NULL;
  return matriz;
}


mymatriz* msomar(mymatriz* matriza, mymatriz* matrizb, int tipo) {
  int matriz_compativeis = matriza->lin != matrizb->lin || matriza->col != matrizb->col;
  int matriz_nula = matriza->matriz == NULL || matrizb->matriz == NULL;
  if(matriz_compativeis || matriz_nula) {
    return matriz_vazia();
  }

  mymatriz* matriz = malloc(sizeof(mymatriz));
  matriz->lin = matriza->lin;
  matriz->col = matriza->col;
  malocar(matriz);

  if(tipo == 0){
    for(int i=0; i < matriza->lin; i++) {
      for(int j=0; j < matrizb->col; j++) {
	matriz->matriz[i][j] = matriza->matriz[i][j] + matrizb->matriz[i][j];
      }
    }
  }
  else if(tipo == 1) {
    for(int j=0; j < matrizb->col; j++) {
      for(int i=0; i < matriza->lin; i++) {
	matriz->matriz[i][j] = matriza->matriz[i][j] + matrizb->matriz[i][j];
      }
    }
  }
  else
    return NULL;
  
  return matriz;
}


mymatriz* mmultiplicar(mymatriz* matriza, mymatriz* matrizb, int tipo) {
  int matriz_compativeis = matriza->lin != matrizb->col || matriza->col != matrizb->lin;
  int matriz_nula = matriza->matriz == NULL || matrizb->matriz == NULL;
  if(matriz_compativeis || matriz_nula) {
    #if DEBUG
    printf("matrizes incompativeis\n");
    printf("matriza->lin: %d\n", matriza->lin);
    printf("matriza->col: %d\n", matriza->col);
    printf("matrizb->lin: %d\n", matrizb->lin);
    printf("matrizb->col: %d\n", matrizb->col);
    #endif
    return matriz_vazia();
  }

  mymatriz* matriz = malloc(sizeof(mymatriz));
  matriz->lin = matriza->lin;
  matriz->col = matriza->col;
  malocar(matriz);
  mzerar(matriz);

  switch(tipo) {
  case 0:
    for(int i=0; i<matriza->lin; i++) {
      for(int j=0; j<matrizb->col; j++) {
	for(int k=0;k<matrizb->lin;k++){
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 1:
    for(int k=0;k<matrizb->lin;k++){
      for(int j=0; j<matrizb->col; j++) {
	for(int i=0; i<matriza->lin; i++) {
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 2:
    for(int j=0; j<matrizb->col; j++) {
      for(int i=0; i<matriza->lin; i++) {
	for(int k=0;k<matrizb->lin;k++){
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 3:
    for(int j=0; j<matrizb->col; j++) {
      for(int k=0;k<matrizb->lin;k++){
	for(int i=0; i<matriza->lin; i++) {
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 4:
    for(int i=0; i<matriza->lin; i++) {
      for(int k=0;k<matrizb->lin;k++){
	for(int j=0; j<matrizb->col; j++) {
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 5:
    for(int k=0;k<matrizb->lin;k++){
      for(int i=0; i<matriza->lin; i++) {
 	for(int j=0; j<matrizb->col; j++) {

	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  case 6:
    for(int k=0;k<matrizb->lin;k++){
      for(int j=0; j<matrizb->col; j++) {
	for(int i=0; i<matriza->lin; i++) {
	  matriz->matriz[i][j] += matriza->matriz[i][k] * matrizb->matriz[k][j];
	}
      }
    }
    break;
  }
  
  return matriz;
}



int tamanho_bloco(int numero_linhas, int numero_colunas, int orientacao, int divisor) {
  return orientacao == 1
    ? numero_linhas / divisor
    : numero_colunas / divisor;
}

matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {

  mymatriz *mymatriz = malloc(sizeof(mymatriz));
  mymatriz->lin = mat_lin;
  mymatriz->col = mat_col;
  mymatriz->matriz = matriz;
  

  
  int tamanho = tamanho_bloco(mat_lin, mat_col, orientacao, divisor);
  matriz_bloco_t **blocos = malloc(sizeof(matriz_bloco_t) * divisor);
  
  for(int i=0; i<divisor; i++) {
    matriz_bloco_t* bloco = malloc(sizeof(matriz_bloco_t));
    bloco->matriz = mymatriz;
    bloco->bloco = malloc(sizeof(bloco_t));

    if(orientacao == 0) {
      bloco->bloco->lin_inicio = i;
      bloco->bloco->lin_fim = i+tamanho;
    } else {
      bloco->bloco->lin_inicio = 0;
      bloco->bloco->lin_fim = mat_lin;
    }

    if(orientacao == 1) {
      bloco->bloco->col_inicio = i;
      bloco->bloco->col_fim = i+tamanho;
    } else {
      bloco->bloco->col_inicio = 0;
      bloco->bloco->col_fim = mat_col;
    }

    //msubimprimir(bloco);
    blocos[i] = bloco;
  }

  return blocos;
}

matriz_bloco_t** csubmatrizv2(int mat_lin, int mat_col, int divisor){
  matriz_bloco_t** matrizes_blocos = malloc(sizeof(matriz_bloco_t*) * divisor);

  for(int i=0; i<divisor; i++){
    matriz_bloco_t* matriz_bloco = malloc(sizeof(matriz_bloco_t));
    matriz_bloco->bloco = malloc(sizeof(bloco_t));
    matriz_bloco->bloco->lin_inicio = 0;
    matriz_bloco->bloco->lin_fim = mat_lin;
    matriz_bloco->bloco->col_inicio = 0;
    matriz_bloco->bloco->col_fim = mat_col;

    mymatriz* matriz = malloc(sizeof(mymatriz));
    matriz->lin = mat_lin;
    matriz->col = mat_col;
    malocar(matriz);

    matriz_bloco->matriz = matriz;
    matrizes_blocos[i] = matriz_bloco;
  }
  return matrizes_blocos;
}

void msubimprimir(matriz_bloco_t* matriz_bloco) {
  printf("======================\n");
  printf("matriz:\n");
  printf("======================\n");
  printf("lin_inicio: %d\n", matriz_bloco->bloco->lin_inicio);
  printf("lin_fim: %d\n", matriz_bloco->bloco->lin_fim);
  printf("col_inicio: %d\n", matriz_bloco->bloco->col_inicio);
  printf("col_fim: %d\n", matriz_bloco->bloco->col_fim);
  printf("======================\n");
}

int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc) {
  int mat_suba_col = mat_suba->bloco->col_fim - mat_suba->bloco->col_inicio;
  int mat_subb_lin = mat_subb->bloco->lin_fim - mat_subb->bloco->lin_inicio;

#if DEBUG
  printf("mat_suba_col: %d mat_subb_lin: %d\n", mat_suba_col, mat_subb_lin);
  printf("mat_suba:\n");
  msubimprimir(mat_suba);
  printf("mat_subb:\n");
  msubimprimir(mat_subb);
#endif

  
  if(mat_suba_col != mat_subb_lin)
    return 0;
 
  for(int i=mat_suba->bloco->lin_inicio; i<mat_suba->bloco->lin_fim; i++){
    for(int j=mat_subb->bloco->col_inicio; j<mat_subb->bloco->col_fim; j++){
      for(int k=mat_suba->bloco->col_inicio; k<mat_suba->bloco->col_fim;k++){
	mat_subc->matriz->matriz[i][j] +=
	  mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j];
	#if DEBUG
	printf("matriz i: %d j: %d k: %d\n", i, j, k);
	mimprimir(mat_subc->matriz);
	#endif
      }
    }
  }
  
  return 1;
}
