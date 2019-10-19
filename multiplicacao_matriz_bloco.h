#include"matrizv3.h"

typedef struct {
  matriz_bloco_t *direita;
  matriz_bloco_t *esquerda;
  matriz_bloco_t *resultado;
  int altura;
  int largura;
} multiplicacao_matriz_bloco;

multiplicacao_matriz_bloco* criar_multiplicacao_matriz_bloco(matriz_bloco_t *esquerda, matriz_bloco_t *direita, matriz_bloco_t *resultado);

void multiplicacao_matriz_bloco_celula_calcular(multiplicacao_matriz_bloco *self, int x, int y);
