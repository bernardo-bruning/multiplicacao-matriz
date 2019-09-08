#define N 10
#define M 10
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"main_tests.h"

#include"matrizv3.h"
#include"matriz-operacoesv3.h"

int main(int argc, char** argv) {
  if(can_execute_tests(argc, argv))
    return execute_tests();
  
  mymatriz mat_a;
  mat_a.matriz = NULL;
  mat_a.lin = N;
  mat_a.col = M;
  if(malocar(&mat_a)) printf("Erro ao alocar matriz.\n");
  mimprimir(&mat_a);
  printf("teste");
  mzerar(&mat_a);
  mimprimir(&mat_a);
  mgerar(&mat_a, -9999);
  mimprimir(&mat_a);
  mliberar(&mat_a);
  return 0;
}
