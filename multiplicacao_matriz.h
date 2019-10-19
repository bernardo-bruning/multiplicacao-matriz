#include<stdio.h>
#include<stdlib.h>
#include"matrizv3.h"

/**
 * Estrutura de dados que permite executar multiplicação de matrizes
 **/
typedef struct multiplicacao_matriz {
  mymatriz *matriz_esquerda;
  mymatriz *matriz_direita;
  mymatriz *matriz_resultado;
  int largura;
  int altura;
} multiplicacao_matriz;

/**
 * Permite retornar a largura de uma matriz
 **/
int multiplicacao_matriz_largura(multiplicacao_matriz *matriz);

/**
 * Permite retornar a altura de uma matriz
 **/
int multiplicacao_matriz_altura(multiplicacao_matriz *matriz);

int multiplicacao_matriz_valido(multiplicacao_matriz *matriz);

multiplicacao_matriz *criar_multiplicacao_matriz(mymatriz *matriz_esquerda, mymatriz *matriz_direita);

void multiplicacao_matriz_celula_calcular(multiplicacao_matriz *self, int x, int y);

void multiplicacao_matriz_calcular(multiplicacao_matriz *self);
