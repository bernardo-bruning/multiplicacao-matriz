# GNU Makefile
# RevisÃ£o: ago/2019

CC = gcc
CCFLAGS = -Wall -O3 -g
LDFLAGS = -fopenmp
TARGET = mainEx02 gmat mainEx01 help main mainEx03

all: $(TARGET)

%.o: %.c
	$(CC) $(CCFLAGS) -c $<

%: %.o
	$(CC) $(LDFLAGS) $^ -o $@

main: main.c matrizv3.c matriz-operacoesv3.c main-test.c matriz_thread.c 
		$(CC) $(CCFLAGS) main.c matriz-operacoesv3.c matrizv3.c matriz_thread.c main-test.c -pthread -o $@ $(LDFLAGS)

run: main
	./main

test: main
	./main --test

teste_thread:
	$(CC) $(CCFLAGS) matriz_thread.c matrizv3.c -lpthread -o matriz_thread


mainEx02: mainEx02.c matrizv3.o toolsv3.o matriz-operacoesv3.o
		$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx02.c -o $@ $(LDFLAGS)

mainEx03: mainEx03a.c matrizv3.o toolsv3.o matriz-operacoesv3.o
	$(CC) $(CCFLAGS) matriz-operacoesv3.o matrizv3.o toolsv3.o mainEx03a.c -o $@ $(LDFLAGS)

mainEx04: mainEx04.o matrizv3.o toolsv3.o matriz-operacoesv3.o matriz_thread.o paralelismo.o multiplicacao_matriz.o multiplicacao_matriz_fragmento.o multiplicacao_matriz_paralela.o multiplicacao_matriz_bloco.o multiplicacao_matriz_bloco_fragmento.o multiplicacao_matriz_bloco_paralela.o
	$(CC) $(CCFLAGS) matriz-operacoesv3.o matriz_thread.o matrizv3.o toolsv3.o mainEx04.o paralelismo.o multiplicacao_matriz.o multiplicacao_matriz_fragmento.o multiplicacao_matriz_paralela.o multiplicacao_matriz_bloco.o multiplicacao_matriz_bloco_fragmento.o multiplicacao_matriz_bloco_paralela.o -lpthread -o $@ $(LDFLAGS)

mainEx05: mainEx05.c matriz-operacoes-omp.c matrizv3.o matriz-operacoesv3.o multiplicacao_matriz.o multiplicacao_matriz_bloco.o toolsv3.o
		$(CC) $(CCFLAGS) matriz-operacoes-omp.c matrizv3.o matriz-operacoesv3.o multiplicacao_matriz.o multiplicacao_matriz_bloco.o toolsv3.o mainEx05.c -fopenmp -o $(LDFLAGS) 
gmat: matrizv3.o toolsv3.o gera_matrizv3.c
		$(CC) $(CCFLAGS) matrizv3.o toolsv3.o gera_matrizv3.c -o $@ $(LDFLAGS)

help:
	@echo
	@echo
	@echo "####### Exemplo de ExecuÃ§Ã£o #######"
	@echo "./main mat_a3x4.example mat_b4x3.example"

clean:
	rm -f *.o *~ $(TARGET) *.map *.result
