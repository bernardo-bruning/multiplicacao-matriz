#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "toolsv3.h"
#include "matrizv3.h"
#include "matriz-operacoesv3.h"
#include "matriz-operacoes-omp.h"

double speedup(double tempo_sequencial, double tempo_paralelo);
void imprime_speedup(char* descricao, double tempo_sequencial, double tempo_paralelo);
float soma_tempo(float tempo[10]);

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int main(int argc, char *argv[]) {

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  // DECLARAÇÃO de VARIÁVEIS
  float tempos[4][10];
  mymatriz mat_a, mat_b;
  mymatriz **mmultbloco, **mmult;
  char filename[100];
  FILE *fmat;
  int nr_line;
  int *vet_line = NULL;
  int N, M, La, Lb;
  double start_time, end_time;

  matriz_bloco_t **Vsubmat_a = NULL;
  matriz_bloco_t **Vsubmat_b = NULL;
  matriz_bloco_t **Vsubmat_c = NULL;
  int nro_submatrizes=2;

  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  if (argc != 3){
    printf ("ERRO: Numero de parametros %s <matriz_a> <matriz_b>\n", argv[0]);
    exit (1);
  }

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //                Leitura da Matriz A (arquivo)
  fmat = fopen(argv[1],"r");
  if (fmat == NULL) {
    printf("Error: Na abertura dos arquivos.");
    exit(1);
  }
  extrai_parametros_matriz(fmat, &N, &La, &vet_line, &nr_line);
  //return 1;
  mat_a.matriz = NULL;
  mat_a.lin = N;
  mat_a.col = La;
  if (malocar(&mat_a)) {
    printf ("ERROR: Out of memory\n");
  }
  filein_matriz (mat_a.matriz, N, La, fmat, vet_line, nr_line);
  free (vet_line);
  fclose(fmat);
  printf("\n Matriz A.\n");
  //mimprimir(&mat_a);
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //               Leitura da Matriz B (arquivo)
  fmat = fopen(argv[2],"r");
  if (fmat == NULL) {
    printf("Error: Na abertura dos arquivos.");
    exit(1);
  }
  extrai_parametros_matriz(fmat, &Lb, &M, &vet_line, &nr_line);
  mat_b.matriz = NULL;
  mat_b.lin = Lb;
  mat_b.col = M;
  if (malocar(&mat_b)) {
    printf ("ERROR: Out of memory\n");
  }
  filein_matriz (mat_b.matriz, Lb, M, fmat, vet_line, nr_line);
  free (vet_line);
  fclose(fmat);
  printf("\n Matriz B\n");
  //mimprimir(&mat_b);
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  printf("\n%%%%%%%%%%%%%%%%\n");
	
  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //               Operações de Multiplicação
  for(int i=0; i<10; i++) {
    mmult = (mymatriz **) malloc (sizeof(mymatriz *));
    printf("\n ##### multiplicar_t1 de Matrizes - MULTIPLICACAO <mmultiplicar> #####\n");
    start_time = wtime();
    mmult[0] = mmultiplicar(&mat_a, &mat_b, 1);
    end_time = wtime();
    //mimprimir(mmult[0]);
    tempos[0][i] = end_time - start_time;
    printf("\tRuntime: %f\n", end_time - start_time);
    sprintf(filename, "mult_t1.result");
    fmat = fopen(filename,"w");
    fileout_matriz(mmult[0], fmat);
    fclose(fmat);
  }
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //               Operações de Multiplicação (em bloco)
  for(int i=0; i<10; i++) {
    mmultbloco = (mymatriz **) malloc (sizeof(mymatriz *));
    printf("\n ##### multiplicar_t1 de Matrizes - MULTIPLICACAO <por bloco> #####\n");
    start_time = wtime();

    Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
    Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
    Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

    mmsubmatriz (Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
    mmsubmatriz (Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]); 
    mmultbloco[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
	 
    end_time = wtime();
    printf("\n Matriz Resultado da AxB por bloco <mmultbloco> \n");
    //mimprimir(mmultbloco[0]);
    tempos[1][i] = end_time - start_time;
    printf("\tRuntime: %f\n", end_time - start_time);
    sprintf(filename, "mult_t1.result");
    fmat = fopen(filename,"w");
    fileout_matriz(mmultbloco[0], fmat);
    fclose(fmat);
  }
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //               Operações de Multiplicação thread
  mymatriz **mmult_thread = (mymatriz **) malloc (sizeof(mymatriz *));
  for(int i=0; i<10; i++) {
    printf("\n ##### multiplicar_t1 de Matrizes thread - MULTIPLICACAO <mmultiplicar> #####\n");
    start_time = wtime();
    mmult_thread[0] = multiplicar_omp(&mat_a, &mat_b, 1);
    end_time = wtime();
    //mimprimir(mmult_thread[0]);
    tempos[2][i] = end_time - start_time;
    printf("\tRuntime: %f\n", end_time - start_time);
    sprintf(filename, "mult_t1.result");
    fmat = fopen(filename,"w");
    fileout_matriz(mmult_thread[0], fmat);
    fclose(fmat);
  }
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //               Operações de Multiplicação thread (em bloco)
  mymatriz **mmultbloco_thread = (mymatriz **) malloc (sizeof(mymatriz *));
  for(int i=0; i<10; i++) {
    printf("\n ##### multiplicar_t1 de Matrizes thread - MULTIPLICACAO <por bloco> #####\n");
    start_time = wtime();

    Vsubmat_a = particionar_matriz (mat_a.matriz, N, La, 1, 2);
    Vsubmat_b = particionar_matriz (mat_b.matriz, Lb, M, 0, 2);
    Vsubmat_c = csubmatrizv2 (N, M, nro_submatrizes);

    mmsubmatriz_omp(Vsubmat_a[0], Vsubmat_b[0], Vsubmat_c[0]);
    mmsubmatriz_omp(Vsubmat_a[1], Vsubmat_b[1], Vsubmat_c[1]); 
    mmultbloco_thread[0] = msomar(Vsubmat_c[0]->matriz, Vsubmat_c[1]->matriz, 1);
	 
    end_time = wtime();
    printf("\n Matriz Resultado da AxB por bloco <mmultbloco> \n");
    //mimprimir(mmultbloco_thread[0]);
    tempos[3][i] = end_time - start_time;
    printf("\tRuntime: %f\n", end_time - start_time);
    sprintf(filename, "mult_t1.result");
    fmat = fopen(filename,"w");
    fileout_matriz(mmultbloco_thread[0], fmat);
    fclose(fmat);
  }
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
	
  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //              Comparação dos resultados
  printf("\n ##### Comparação dos resultados da Multiplicação de matrizes #####\n");
  printf("[mult_t0 vs multbloco_t0]\t");
  mcomparar (mmult[0],mmultbloco[0]);
  printf("[mult_t0 vs mmult_thread_t0]\t");
  mcomparar (mmult[0],mmult_thread[0]);
  printf("[mult_t0 vs mmultbloco_thread_t0]\t");
  mcomparar (mmult[0],mmultbloco_thread[0]);

  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //              Comparação dos resultados

  printf("tempo médio matriz sequencial: %.6f\n", soma_tempo(tempos[0]));
  printf("tempo médio matriz sequencial bloco: %.6f\n", soma_tempo(tempos[1]));
  printf("tempo médio matriz paralela: %.6f\n", soma_tempo(tempos[2]));
  printf("tempo médio matriz paralela bloco: %.6f\n", soma_tempo(tempos[3]));
  imprime_speedup("speedup matriz:", soma_tempo(tempos[0]), soma_tempo(tempos[2]));
  imprime_speedup("speedup matriz bloco:", soma_tempo(tempos[1]), soma_tempo(tempos[3]));
  
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%
  // %%%%%%%%%%%%%%%%%%%%%%%% BEGIN %%%%%%%%%%%%%%%%%%%%%%%%
  //                   Liberação de memória
  mliberar(mmult[0]);
  free (mmult[0]);
  mliberar(mmultbloco[0]);
  free (mmultbloco[0]);

  mliberar(&mat_a);
  mliberar(&mat_b);
  free(mmult);
  free(mmultbloco);
  // %%%%%%%%%%%%%%%%%%%%%%%% END %%%%%%%%%%%%%%%%%%%%%%%%

  return 0;
}

double speedup(double tempo_sequencial, double tempo_paralelo) {
  if(tempo_paralelo == 0) return 0;
  return tempo_sequencial / tempo_paralelo;
}

void imprime_speedup(char* descricao, double tempo_sequencial, double tempo_paralelo) {
  printf("%s: %f\n", descricao, speedup(tempo_sequencial, tempo_paralelo));
}

float soma_tempo(float tempo[10]){
  float valor = 0;
  for(int i=0; i<10; i++) {
    valor += tempo[i];
  }

  return valor/10;
}
