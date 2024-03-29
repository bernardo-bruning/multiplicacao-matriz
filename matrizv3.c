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
	printf("matrizA[%d][%d] = %d � diferente de matrizB[%d][%d] = %d\n", i, j, matriza->matriz[i][j], i, j, matrizb->matriz[i][j]);
	igual = 0;
      }
    }
  }

  if(igual)
    printf("matrizes s�o iguais!\n");
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
      if(valor == -9999) matriz->matriz[i][j] = rand() % 100; 
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


mymatriz *criar_matriz(int altura, int largura, int valores[largura][altura]) {
  mymatriz *self = malloc(sizeof(mymatriz));
  self->lin = altura;
  self->col = largura;
  malocar(self);

  for(int x=0;x<largura;x++) {
    for(int y=0;y<altura;y++) {
      self->matriz[x][y] = valores[x][y];
    }
  }

  return self;
}


mymatriz *criar_matriz_vazia(int altura, int largura) {
  mymatriz *self = malloc(sizeof(mymatriz));
  self->lin = altura;
  self->col = largura;
  malocar(self);
  mzerar(self);

  return self;
}

bloco_t *criar_bloco(int lin_inicio, int col_inicio, int lin_fim, int col_fim) {
  bloco_t *bloco = malloc(sizeof(bloco_t));
  bloco->lin_inicio = lin_inicio;
  bloco->col_inicio = col_inicio;
  bloco->lin_fim = lin_fim;
  bloco->col_fim = col_fim;
  return bloco;
}

matriz_bloco_t *criar_matriz_bloco(int lin_inicio, int col_inicio, int lin_fim, int col_fim, mymatriz *matriz) {
  matriz_bloco_t *self = malloc(sizeof(matriz_bloco_t));
  self->bloco = criar_bloco(lin_inicio, col_inicio, lin_fim, col_fim);
  self->matriz = matriz;
  return self;
}
