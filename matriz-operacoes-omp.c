#include <omp.h>
#include"multiplicacao_matriz.h"
#include"multiplicacao_matriz_bloco.h"
#include"matriz-operacoes-omp.h"

#define NUM_THREADS 8

mymatriz *multiplicar_omp(mymatriz *matriza, mymatriz *matrizb, int tipo) {
  multiplicacao_matriz *multiplicacao_matriz = criar_multiplicacao_matriz(matriza, matrizb);

  #pragma omp parallel for
  for(int x=0;x<multiplicacao_matriz->altura;x++) {
    for(int y=0;y<multiplicacao_matriz->largura;y++) {
       multiplicacao_matriz_celula_calcular(multiplicacao_matriz, x, y);
    }
  }

  mymatriz *resultado = multiplicacao_matriz->matriz_resultado; 

  free(multiplicacao_matriz);
  return resultado;
}

void mmsubmatriz_omp(matriz_bloco_t *mat_a, matriz_bloco_t *mat_b, matriz_bloco_t *mat_c) {
  multiplicacao_matriz_bloco *self = criar_multiplicacao_matriz_bloco(mat_a, mat_b, mat_c);

  #pragma omp parallel for
  for(int i=self->esquerda->bloco->lin_inicio; i<self->esquerda->bloco->lin_fim; i++){
    for(int j = self->direita->bloco->col_inicio; j<self->direita->bloco->col_fim; j++) {
      multiplicacao_matriz_bloco_celula_calcular(self, i, j);
    } 
  }
}


#define TEST 0

#if TEST
#include"timer.h"
int main(int argc, char** argv) {
  timer *timer = criar_timer();
  mymatriz *esquerda = criar_matriz_vazia(10000, 10000);
  mymatriz *direita = criar_matriz_vazia(10000, 10000);
  mymatriz *resultado = criar_matriz_vazia(10000, 10000);
  
  mgerar(esquerda, 2);
  mgerar(direita, 2);

  matriz_bloco_t *esquerda_bloco = criar_matriz_bloco(0, 0, 1000, 10000, esquerda);
  matriz_bloco_t *direita_bloco = criar_matriz_bloco(0, 0, 10000, 1000, direita);
  matriz_bloco_t *resultado_bloco = criar_matriz_bloco(0, 0, 1000, 1000, resultado);

  timer_iniciar(timer);
  mmsubmatriz_omp(esquerda_bloco, direita_bloco, resultado_bloco); 
  timer_encerrar(timer);
  timer_imprimir(timer);
  printf("concluiu!\n");

}
#endif
