#include<stdio.h>
#include<stdlib.h>
#include"multiplicacao_matriz.h"
#include"matrizv3.h"

/**
 * Permite retornar a largura de uma matriz
 **/
int multiplicacao_matriz_largura(multiplicacao_matriz *matriz) {
  return matriz->matriz_direita->col;
}

/**
 * Permite retornar a altura de uma matriz
 **/
int multiplicacao_matriz_altura(multiplicacao_matriz *matriz) {
  return matriz->matriz_esquerda->lin;
}

int multiplicacao_matriz_valido(multiplicacao_matriz *matriz) {
  return matriz->matriz_esquerda->lin == matriz->matriz_direita->col;
}

multiplicacao_matriz *criar_multiplicacao_matriz(mymatriz *matriz_esquerda, mymatriz *matriz_direita) {
  multiplicacao_matriz *self = malloc(sizeof(multiplicacao_matriz));
  self->matriz_esquerda = matriz_esquerda;
  self->matriz_direita = matriz_direita;
  self->altura = multiplicacao_matriz_altura(self);
  self->largura = multiplicacao_matriz_largura(self);
  self->matriz_resultado = criar_matriz_vazia(self->altura, self->largura);

  return self;
}

void multiplicacao_matriz_celula_calcular(multiplicacao_matriz *self, int x, int y){
#if DEBUG
  printf("DEBUG: multiplicacao_matriz_celula_calcular\n");
#endif
  for(int i = 0; i<self->matriz_esquerda->col; i++) {
    self->matriz_resultado->matriz[x][y] += self->matriz_esquerda->matriz[x][i] * self->matriz_direita->matriz[i][y];
  }
}

void multiplicacao_matriz_calcular(multiplicacao_matriz *self) {
  for(int x=0;x<self->altura;x++) {
    for(int y=0;y<self->largura;y++) {
      printf("%d %d \n", x, y);
      multiplicacao_matriz_celula_calcular(self, x, y);
    }
  }
}
