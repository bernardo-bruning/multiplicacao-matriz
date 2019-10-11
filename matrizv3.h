#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef SOME_HEADER_FILE_H
#define SOME_HEADER_FILE_H

typedef struct {
  int **matriz;
  int lin;
  int col;
} mymatriz;

typedef struct {
  int lin_inicio;
  int lin_fim;
  int col_inicio;
  int col_fim;
} bloco_t;

typedef struct {
  mymatriz *matriz;
  bloco_t *bloco;
} matriz_bloco_t;

#endif

// gerencia de matrizes
int malocar (mymatriz *matriz);
int mgerar(mymatriz *matriz, int valor);
int mimprimir (mymatriz *matriz);
int mzerar (mymatriz *matriz);
int mliberar (mymatriz *matriz);
int mcomparar (mymatriz *mat_a, mymatriz *mat_b);

mymatriz *criar_matriz(int altura, int largura, int valores[largura][altura]);
mymatriz *criar_matriz_vazia(int altura, int largura);

matriz_bloco_t *criar_matriz_bloco(int lin_inicio, int col_inicio, int lin_fim, int col_fim, mymatriz *matriz);

// debug de matriz
//void dump_matriz(mymatriz *matriz);
//void dump_matriz_bloco(matriz_bloco_t *matriz_bloco);
//void dump_bloco(bloco_t *bloco);
