#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include"multiplicacao_matriz_paralela.h"
#include"matrizv3.h"

#define DEBUG 0



//////////////////////////////////////////////////
typedef struct {
  matriz_bloco_t *direita;
  matriz_bloco_t *esquerda;
  matriz_bloco_t *resultado;
  int altura;
  int largura;
} multiplicacao_matriz_bloco;

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


//////////////////////////////////////////////////

typedef struct  {
  multiplicacao_matriz_bloco *bloco;
  int inicio_x;
  int inicio_y;
  int fim_x;
  int fim_y;
} multiplicacao_matriz_bloco_fragmento;

multiplicacao_matriz_bloco_fragmento *criar_multiplicacao_matriz_bloco_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz_bloco *bloco) {
  #if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_bloco_fragmento(%d, %d, %d, %d, %p)\n", inicio_x, inicio_y, fim_x, fim_y, bloco);
  #endif
  multiplicacao_matriz_bloco_fragmento *self = malloc(sizeof(multiplicacao_matriz_bloco_fragmento));
  self->inicio_x = inicio_x;
  self->inicio_y = inicio_y;
  self->fim_x = fim_x;
  self->fim_y = fim_y;
  self->bloco = bloco;
  return self;
}

void multiplicacao_matriz_bloco_fragmento_processar(multiplicacao_matriz_bloco_fragmento *self){
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_bloco_processar({ inicio_x: %d, inicio_y: %d, fim_x: %d, fim_y: %d })\n", self->inicio_x, self->inicio_y, self->fim_x, self->fim_y);
  #endif
  for(int x=self->inicio_x; x<self->fim_x; x++) {
    for(int y=self->inicio_y; y<self->fim_y; y++) {
      multiplicacao_matriz_bloco_celula_calcular(self->bloco, x, y);
    }
  }
}
//////////////////////////////////////////////////

typedef struct {
  int capacidade;
  multiplicacao_matriz_bloco *matriz_bloco;
  paralelismo *paralelismo;
} multiplicacao_matriz_bloco_paralela;


void *multiplicacao_matriz_bloco_fragmento_processar_paralelamente(void * arg) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_processar_paralelamente \n");
  #endif
  multiplicacao_matriz_bloco_fragmento_processar(arg);
  return NULL;
}

multiplicacao_matriz_bloco_paralela *criar_multiplicacao_matriz_bloco_paralela(int capacidade, multiplicacao_matriz_bloco *matriz_bloco) {
#if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_paralela \n");
#endif
  multiplicacao_matriz_bloco_paralela *self = malloc(sizeof(multiplicacao_matriz_bloco_paralela));
  self->capacidade = capacidade;
  self->paralelismo = criar_paralelismo(capacidade, multiplicacao_matriz_bloco_fragmento_processar_paralelamente);
  self->matriz_bloco = matriz_bloco;
  return self;
}

void multiplicacao_matriz_bloco_paralela_processar(multiplicacao_matriz_bloco_paralela *self) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_bloco_paralela_processar\n");
  #endif
  multiplicacao_matriz_bloco_fragmento *fragmentos[self->capacidade];
  int tamanho = self->capacidade > self->matriz_bloco->largura
    ? 1
    : self->capacidade / self->matriz_bloco->largura;
  for(int i=0; i<self->capacidade; i++) {
    int inicio = i * tamanho + self->matriz_bloco->resultado->bloco->col_inicio;
    int fim = i == self->capacidade - 1
      ? self->matriz_bloco->resultado->bloco->col_fim
      : (i + 1) * tamanho + self->matriz_bloco->resultado->bloco->col_inicio;
    fragmentos[i] = criar_multiplicacao_matriz_bloco_fragmento(
                                                               self->matriz_bloco->resultado->bloco->lin_inicio,
                                                               inicio,
                                                               self->matriz_bloco->resultado->bloco->lin_fim,
                                                               fim,
                                                               self->matriz_bloco);
    paralelismo_inserir(self->paralelismo, fragmentos[i]);
  }
  paralelismo_aguardar(self->paralelismo);
}


//////////////////////////////////////////////////

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
