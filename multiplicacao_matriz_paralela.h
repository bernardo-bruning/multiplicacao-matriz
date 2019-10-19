#include"paralelismo.h"
#include"multiplicacao_matriz_fragmento.h"

typedef struct {
  int capacidade;
  multiplicacao_matriz *matriz;
  paralelismo *paralelismo;
} multiplicacao_matriz_paralela;


multiplicacao_matriz_paralela *criar_multiplicacao_matriz_paralela(int capacidade, multiplicacao_matriz *matriz);

void multiplicacao_matriz_paralela_processar(multiplicacao_matriz_paralela *self);
