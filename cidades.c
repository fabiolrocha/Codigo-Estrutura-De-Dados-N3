#include "cidades.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Estrada *getEstrada(const char *nomeArquivo);
double calcularMenorVizinhanca(const char *nomeArquivo);
char *cidadeMenorVizinhanca(const char *nomeArquivo);

int compararCidades(const void *a, const void *b) {
    return ((Cidade *)a)->Posicao - ((Cidade *)b)->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    Estrada *estrada = (Estrada *) malloc(sizeof(Estrada));
    fscanf(arquivo, "%d", &(estrada->T));
    fscanf(arquivo, "%d", &(estrada->N));

    estrada->C = (Cidade *) malloc(estrada->N * sizeof(Cidade));
    for (int i = 0; i < estrada->N; i++) {
        fscanf(arquivo, "%d", &estrada->C[i].Posicao);
        fgets(estrada->C[i].Nome, 100, arquivo);
    }

    fclose(arquivo);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    if (estrada == NULL) {
        return -1; // Retorna um valor negativo para indicar erro
    }
    
    double menorVizinhanca = 1000000;
    double vizinhanca = 0;
    double *fronteira = (double *) malloc((estrada->N - 1) * sizeof(double));
    
    if (fronteira == NULL) {
        free(estrada->C);
        free(estrada);
        return -1; // Retorna um valor negativo para indicar erro
    }

    // Calcula as fronteiras
    for (int i = 0; i < estrada->N - 1; i++) {
        fronteira[i] = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
    }

    // Calcula as vizinhanças
    for (int i = 0; i < estrada->N; i++) {
        if (i == 0) {
            vizinhanca = fronteira[i];
        } else if (i == estrada->N - 1) {
            vizinhanca = estrada->T - fronteira[i - 1];
        } else {
            vizinhanca = fronteira[i] - fronteira[i - 1];
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(fronteira);
    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    
    if (estrada == NULL) {
        return NULL; // Retorna NULL para indicar erro
    }
    
    double menorVizinhanca = 1000000;
    double vizinhanca = 0;
    int posicao = 0;
    double *fronteira = (double *) malloc((estrada->N - 1) * sizeof(double));

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);
    
    if (fronteira == NULL) {
        free(estrada->C);
        free(estrada);
        return NULL; // Retorna NULL para indicar erro
    }

    // Calcula as fronteiras
    for (int i = 0; i < estrada->N - 1; i++) {
        fronteira[i] = (estrada->C[i].Posicao + estrada->C[i + 1].Posicao) / 2.0;
    }
    
    // Calcula as vizinhanças e encontra a cidade com a menor vizinhança
    for (int i = 0; i < estrada->N; i++) {
        

        if (i == 0) {
            vizinhanca = fronteira[i];
        } else if (i == estrada->N - 1) {
            vizinhanca = estrada->T - fronteira[i - 1];
        } else {
            vizinhanca = fronteira[i] - fronteira[i - 1];
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            posicao = i;
        }
    }

    free(fronteira);
    char *resultado = strdup(estrada->C[posicao].Nome + 1); // Copia o nome da cidade para retornar
    free(estrada->C);
    free(estrada);
    return resultado;
}


int main() {
    const char *nomeArquivo = "entrada.txt";

    Estrada *estrada = getEstrada("entrada.txt");

    printf("Tamanho da estrada: %d\n", estrada->T);
    printf("Número de cidades: %d\n", estrada->N);

    for (int i = 0; i < estrada->N; i++) {
        printf("Cidade -> %d\n", i + 1);
        printf("Posição: %d\n", estrada->C[i].Posicao);
        printf("Cidade:%s", estrada->C[i].Nome);
    }

    printf("\n\n");

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    printf("Menor vizinhança: %.2f\n", menorVizinhanca);

    char *cidade = cidadeMenorVizinhanca(nomeArquivo);
    printf("\nCidade com menor vizinhança: %s\n", cidade);

    // Libera a memória alocada para o nome da cidade
    free(cidade);
    return 0;
}
