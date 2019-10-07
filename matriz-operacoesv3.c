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

void msubimprimir(matriz_bloco_t* matriz_bloco) {
  printf("======================\n");
  printf("matriz:\n");
  for(int i=matriz_bloco->bloco->lin_inicio;i<matriz_bloco->bloco->lin_fim;i++){
    for(int j=matriz_bloco->bloco->col_inicio;j<matriz_bloco->bloco->col_fim;j++){
      printf("matriz[%d][%d] = %d\n", i, j, matriz_bloco->matriz->matriz[i][j]);
    }
  }
  printf("======================\n");
  printf("lin_inicio: %d\n", matriz_bloco->bloco->lin_inicio);
  printf("lin_fim: %d\n", matriz_bloco->bloco->lin_fim);
  printf("col_inicio: %d\n", matriz_bloco->bloco->col_inicio);
  printf("col_fim: %d\n", matriz_bloco->bloco->col_fim);
  printf("======================\n");
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
      for(int j=0; j < matriza->col; j++) {
	matriz->matriz[i][j] = matriza->matriz[i][j] + matrizb->matriz[i][j];
      }
    }
  }
  else if(tipo == 1) {
    for(int j=0; j < matriza->col; j++) {
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
  matriz->col = matrizb->col;
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

bloco_t *alocar_bloco(int lin_inicio, int lin_fim, int col_inicio, int col_fim){
  bloco_t *bloco = malloc(sizeof(bloco_t));
  bloco->lin_inicio = lin_inicio;
  bloco->lin_fim = lin_fim;
  bloco->col_inicio = col_inicio;
  bloco->col_fim = col_fim;

  return bloco;
}


matriz_bloco_t **particionar_matriz (int **matriz, int mat_lin, int mat_col, int orientacao, int divisor) {

  mymatriz *mymatriz = malloc(sizeof(mymatriz));
  mymatriz->lin = mat_lin;
  mymatriz->col = mat_col;
  mymatriz->matriz = matriz;
  
  matriz_bloco_t** blocos = malloc(sizeof(matriz_bloco_t*) * divisor);

  for(int i = 0; i < divisor; i++) {
    int tamanho = orientacao == 0 ? mat_lin/divisor : mat_col/divisor;
    int deslocamento_inicio = i * tamanho;
    int deslocamento_final = (i+1) * tamanho;
    matriz_bloco_t* matriz_bloco = malloc(sizeof(matriz_bloco_t));
    matriz_bloco->bloco = alocar_bloco(
				       !orientacao ? deslocamento_inicio : 0,
				       !orientacao ? deslocamento_final : mat_lin,
				       orientacao ? deslocamento_inicio : 0,
				       orientacao ? deslocamento_final: mat_col);
    matriz_bloco->matriz = mymatriz;
    //msubimprimir(matriz_bloco);
    blocos[i] = matriz_bloco;
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


int mmsubmatriz (matriz_bloco_t *mat_suba, matriz_bloco_t *mat_subb, matriz_bloco_t *mat_subc) {
  if(!mat_suba || !mat_subb || !mat_subc) {
    printf("Matrizes nulas!");
    return 0;
  }
  
  if(mat_suba->matriz->col != mat_subb->matriz->lin){
    printf("Matrizes com tamanhos diferentes!");
    return 0;
  }
  
  for(int i=mat_suba->bloco->lin_inicio; i<mat_suba->bloco->lin_fim; i++){
    for(int k = mat_suba->bloco->col_inicio; k<mat_suba->bloco->col_fim; k++) {
      for(int j = mat_subb->bloco->col_inicio; j<mat_subb->bloco->col_fim; j++) {
	mat_subc->matriz->matriz[i][j] += mat_suba->matriz->matriz[i][k] * mat_subb->matriz->matriz[k][j]; 
      }
    }
  }
  return 1;
}
