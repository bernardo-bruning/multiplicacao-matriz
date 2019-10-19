#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include"paralelismo.h"
#include"multiplicacao_matriz_paralela.h"
#include"multiplicacao_matriz_bloco_paralela.h"
#include"matrizv3.h"

#define DEBUG 0


mymatriz *mmultiplicar_thread (mymatriz *mat_a, mymatriz *mat_b, int tipo) {
  multiplicacao_matriz *mm = criar_multiplicacao_matriz(mat_a, mat_b);
  multiplicacao_matriz_paralela *mmp = criar_multiplicacao_matriz_paralela(4, mm);
  multiplicacao_matriz_paralela_processar(mmp);
  return mm->matriz_resultado;
}

void mmsubmatriz_thread(matriz_bloco_t *mat_a, matriz_bloco_t *mat_b, matriz_bloco_t *mat_c){
  multiplicacao_matriz_bloco *multiplicacao_matriz_bloco = criar_multiplicacao_matriz_bloco(mat_a, mat_b, mat_c);
  multiplicacao_matriz_bloco_paralela *multiplicacao_matriz_bloco_paralela = criar_multiplicacao_matriz_bloco_paralela(4, multiplicacao_matriz_bloco);
  multiplicacao_matriz_bloco_paralela_processar(multiplicacao_matriz_bloco_paralela);
}

int main_test(int argc, char** argv) {
  mymatriz *matriza = criar_matriz_vazia(3, 2);
  mgerar(matriza, 2);
  mymatriz *matrizb = criar_matriz_vazia(2, 3);
  mgerar(matrizb, 2);
  mymatriz *resultado = criar_matriz_vazia(3, 3);

  matriz_bloco_t *bloco_esquerda = criar_matriz_bloco(0, 0, 2, 2, matriza);
  matriz_bloco_t *bloco_direita = criar_matriz_bloco(0, 0, 2, 2, matrizb);
  matriz_bloco_t *bloco_resultado = criar_matriz_bloco(0, 0, 2, 2, resultado);

  multiplicacao_matriz *mm = criar_multiplicacao_matriz(matriza, matrizb);
  multiplicacao_matriz_calcular(mm);

  multiplicacao_matriz_bloco *multiplicacao_matriz_bloco = criar_multiplicacao_matriz_bloco(bloco_esquerda, bloco_direita, bloco_resultado);
  multiplicacao_matriz_bloco_paralela *multiplicacao_matriz_bloco_paralela = criar_multiplicacao_matriz_bloco_paralela(2, multiplicacao_matriz_bloco);

  multiplicacao_matriz_bloco_paralela_processar(multiplicacao_matriz_bloco_paralela);

  /* multiplicacao_matriz_bloco_celula_calcular(multiplicacao_matriz_bloco, 1, 0); */

  mimprimir(mm->matriz_resultado);
  printf("=============================\n");
  mimprimir(resultado);
  printf("concluido.");
  return 0;
}
