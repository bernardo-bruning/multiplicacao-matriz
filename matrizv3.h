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


// debug de matriz
//void dump_matriz(mymatriz *matriz);
//void dump_matriz_bloco(matriz_bloco_t *matriz_bloco);
//void dump_bloco(bloco_t *bloco);
