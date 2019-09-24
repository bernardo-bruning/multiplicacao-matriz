#include"matrizv3.h"
#define DEBUG 0

int mcomparar(mymatriz* matriza, mymatriz* matrizb) {
  int linha_diferente = matriza->lin != matrizb->lin;
  int coluna_diferente = matriza->col != matrizb->col;
 
  if(linha_diferente||coluna_diferente){
    printf("tamanho das matrizes diferentes!\n");
    return 0;
  }
  
  int igual = 1; 
  for(int i=0; i<matriza->lin; i++) {
    for(int j=0; j<matriza->col; j++) {
      if(matriza->matriz[i][j] != matrizb->matriz[i][j]){
	printf("matrizA[%d][%d] = %d é diferente de matrizB[%d][%d] = %d\n", i, j, matriza->matriz[i][j], j, i, matrizb->matriz[j][i]);
	igual = 0;
      }
    }
  }
  
  return igual;
}
  
int mimprimir(mymatriz* matriz) {
  if(matriz == NULL || matriz->matriz == NULL || matriz->lin == 0 || matriz->col == 0)
    return 0;
  
  for(int i=0; i<matriz->lin; i++) {
    for(int j=0; j<matriz->col; j++){
      if(matriz->matriz[i] == NULL) return 0;
      printf("matriz[%d][%d] = %d \n", i, j, matriz->matriz[i][j]);
    }
  }  

  return 1;
}

int mgerar(mymatriz* matriz, int valor) {
  if(matriz == NULL || matriz->matriz == NULL || matriz->lin == 0 || matriz->col == 0)
    return 0;
  
  for(int i=0; i<matriz->lin; i++) {
    for(int j=0; j<matriz->col; j++){
      if(matriz->matriz[i] == NULL) return 0;
      if(valor == -9999) matriz->matriz[i][j] = rand(); 
      else matriz->matriz[i][j] = valor;
    }
  }

  return 1;
}

int mzerar(mymatriz* matriz){
  return mgerar(matriz, 0);
}

int mliberar(mymatriz* matriz){
  if(matriz->matriz == NULL || matriz->lin == 0 || matriz->col == 0)
    return 0;
  
  for(int i=0; i<matriz->lin; i++) {
    free(matriz->matriz[i]);
  }
  free(matriz->matriz);
  return 1;
}

int malocar(mymatriz* matriz) {
  if(matriz == NULL || matriz->lin == 0 || matriz->col ==0)
    return 0;

  matriz->matriz = malloc(matriz->lin*sizeof(int*));
  for(int i=0; i<matriz->lin; i++){
    matriz->matriz[i] = malloc(matriz->col*sizeof(int));
    for(int j=0; j<matriz->col; j++)
      matriz->matriz[i][j] = 0;
  }
    
  return 1;
  if(matriz == NULL) return 0;
  matriz->matriz = NULL;
  matriz->matriz = malloc(matriz->lin*sizeof(int*));
  if(matriz->matriz == NULL) return 0;
  for(int i=0; i<matriz->lin; i++) {
    matriz->matriz[i] = malloc(matriz->col*sizeof(int));
    if(matriz->matriz[i] == NULL) return 0;
    for(int j=0; j<matriz->col; i++){
      matriz->matriz[i][j] = 0;
    }
  }
  return 1;
}

int tamanho_bloco(int numero_linhas, int numero_colunas, int orientacao, int divisor) {
  return orientacao == 1
    ? numero_linhas / divisor
    : numero_colunas / divisor;
}

matriz_bloco_t **particionar_matriz (mymatriz *matriz, int mat_lin, int mat_col, int orientacao, int divisor) {
  int tamanho = tamanho_bloco(mat_lin, mat_col, orientacao, divisor);
  matriz_bloco_t **blocos = malloc(sizeof(matriz_bloco_t) * divisor);
  
  for(int i=0; i<divisor; i++) {
    matriz_bloco_t* bloco = malloc(sizeof(matriz_bloco_t));
    bloco->matriz = matriz;
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
