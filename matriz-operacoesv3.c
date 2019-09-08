#include<stdio.h>
#include"matriz-operacoesv3.h"

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
