#include "paralelismo.h"
#include"multiplicacao_matriz_bloco_fragmento.h"

typedef struct {
  int capacidade;
  multiplicacao_matriz_bloco *matriz_bloco;
  paralelismo *paralelismo;
} multiplicacao_matriz_bloco_paralela;


void *multiplicacao_matriz_bloco_fragmento_processar_paralelamente(void * arg);

multiplicacao_matriz_bloco_paralela *criar_multiplicacao_matriz_bloco_paralela(int capacidade, multiplicacao_matriz_bloco *matriz_bloco);

void multiplicacao_matriz_bloco_paralela_processar(multiplicacao_matriz_bloco_paralela *self);
