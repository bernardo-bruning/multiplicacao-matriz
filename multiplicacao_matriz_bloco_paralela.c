#include"paralelismo.h"
#include"multiplicacao_matriz_bloco_paralela.h"

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
