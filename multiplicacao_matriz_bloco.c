#include"multiplicacao_matriz_bloco.h"

multiplicacao_matriz_bloco* criar_multiplicacao_matriz_bloco(matriz_bloco_t *esquerda, matriz_bloco_t *direita, matriz_bloco_t *resultado) {
  multiplicacao_matriz_bloco *self = malloc(sizeof(multiplicacao_matriz_bloco));
  self->direita = direita;
  self->esquerda = esquerda;
  self->resultado = resultado;
  self->altura = esquerda->bloco->lin_inicio - esquerda->bloco->lin_fim + 1;
  self->largura = direita->bloco->col_inicio - direita->bloco->col_fim + 1;
  return self;
}

void multiplicacao_matriz_bloco_celula_calcular(multiplicacao_matriz_bloco *self, int x, int y) {
#if DEBUG
  printf("DEBUG: multiplicacao_matriz_bloco_celula_calcular(%d %d)\n", x, y);
#endif
  for(int i = self->esquerda->bloco->col_inicio; i<self->esquerda->bloco->col_fim; i++) {
    self->resultado->matriz->matriz[x][y] += self->esquerda->matriz->matriz[x][i] * self->direita->matriz->matriz[i][y];
  }
}
