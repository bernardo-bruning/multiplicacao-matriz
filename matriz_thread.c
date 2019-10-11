#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include"matrizv3.h"

#define DEBUG 1

//////////////////////////////////////////////////
typedef struct paralelismo {
  int capacidade;
  void* (*funcao)(void *);
  int quantidade;
  pthread_t *threads;
  int tamanho_elemento;
  void** elementos;
} paralelismo;

/**
 * Permite criar uma estrutura de paralelismo.
 **/
paralelismo *criar_paralelismo(int capacidade, void* (*funcao)(void *)) {
  paralelismo *self = malloc(sizeof(paralelismo));
  self->funcao = funcao;
  self->capacidade = capacidade;
  self->quantidade = 0;
  self->threads = malloc(sizeof(pthread_t)*capacidade);
  self->elementos = malloc(sizeof(void *)*capacidade);
  return self;
}

/**
 * Permite inserir um elemento no paralelismo.
 **/
void paralelismo_inserir(paralelismo *self, void* elemento) {
#if DEBUG
  printf("DEBUG: paralelismo_inserir\n");
#endif
  pthread_t *thread = &self->threads[self->quantidade];
  pthread_create(thread, NULL, self->funcao, elemento);
  self->quantidade++;
}

/**
 * Permite aguardar o paralelismo ser concluído.
 **/
void paralelismo_aguardar(paralelismo *self) {
#if DEBUG
  printf("DEBUG: paralelismo_aguardar\n");
#endif
  for(int i=0; i<self->quantidade; i++) {
    pthread_t thread = self->threads[i];
    pthread_join(thread, NULL);
  }

  self->quantidade = 0;
}

/**
 * Permite processar sem gerenciar os elementos a serem inclusos
 **/
void paralelismo_processar(paralelismo *self, void *elemento){
#if DEBUG
  printf("DEBUG: paralelismo_processar\n");
#endif
   paralelismo_inserir(self, elemento);
   if(self->quantidade==self->capacidade) {
     paralelismo_aguardar(self);
   }
}

/**
 * Permite deletar o paralelismo da memoria sem garantia de finalização.
 **/
void forca_deletar_paralelismo(paralelismo *self) {
  free(self->threads);
  free(self);
}

/**
 * Permite deletar o paralelismo da memoria garantindo que todos as threads foram finalizadas.
 **/
void deletar_paralelismo(paralelismo *self) {
  paralelismo_aguardar(self);
  forca_deletar_paralelismo(self);
}
//////////////////////////////////////////////////

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


//////////////////////////////////////////////////
typedef struct  {
  multiplicacao_matriz *multiplicacao_matriz;
  int inicio_x;
  int inicio_y;
  int fim_x;
  int fim_y;
} multiplicacao_matriz_fragmento;

multiplicacao_matriz_fragmento *criar_multiplicacao_matriz_fragmento(int inicio_x, int inicio_y, int fim_x, int fim_y, multiplicacao_matriz *multiplicacao_matriz) {
  #if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_fragmento\n");
  #endif
  multiplicacao_matriz_fragmento *self = malloc(sizeof(multiplicacao_matriz_fragmento));
  self->inicio_x = inicio_x;
  self->inicio_y = inicio_y;
  self->fim_x = fim_x;
  self->fim_y = fim_y;
  self->multiplicacao_matriz = multiplicacao_matriz;
  return self;
}

void multiplicacao_matriz_fragmento_processar(multiplicacao_matriz_fragmento *self){
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_processar({ inicio_x: %d, inicio_y: %d, fim_x: %d, fim_y: %d })\n", self->inicio_x, self->inicio_y, self->fim_x, self->fim_y);
  #endif
  for(int x=self->inicio_x; x<self->fim_x; x++) {
    for(int y=self->inicio_y; y<self->fim_y; y++) {
      multiplicacao_matriz_celula_calcular(self->multiplicacao_matriz, x, y);
    }
  }
}

//////////////////////////////////////////////////
typedef struct {
  int capacidade;
  multiplicacao_matriz *matriz;
  paralelismo *paralelismo;
} multiplicacao_matriz_paralela;


void *multiplicacao_matriz_fragmento_processar_paralelamente(void * arg) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_fragmento_processar_paralelamente \n");
  #endif
  multiplicacao_matriz_fragmento_processar(arg);
  return NULL;
}

multiplicacao_matriz_paralela *criar_multiplicacao_matriz_paralela(int capacidade, multiplicacao_matriz *matriz) {
#if DEBUG
  printf("DEBUG: criar_multiplicacao_matriz_paralela \n");
#endif
  multiplicacao_matriz_paralela *self = malloc(sizeof(multiplicacao_matriz_paralela));
  self->capacidade = capacidade;
  self->paralelismo = criar_paralelismo(capacidade, multiplicacao_matriz_fragmento_processar_paralelamente);
  self->matriz = matriz;
  return self;
}

void multiplicacao_matriz_paralela_processar(multiplicacao_matriz_paralela *self) {
  #if DEBUG
  printf("DEBUG: multiplicacao_matriz_paralela_processar\n");
  #endif
  multiplicacao_matriz_fragmento *fragmentos[self->capacidade];
  int tamanho = self->capacidade > self->matriz->largura ? 1 : self->capacidade / self->matriz->largura;
  for(int i=0; i<self->capacidade; i++) {
    int inicio = i * tamanho;
    int fim = i == self->capacidade - 1 ? self->matriz->largura : (i + 1) * tamanho;
    fragmentos[i] = criar_multiplicacao_matriz_fragmento(0, inicio, self->matriz->altura, fim, self->matriz);
    paralelismo_inserir(self->paralelismo, fragmentos[i]);
  }
  paralelismo_aguardar(self->paralelismo);
}

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
