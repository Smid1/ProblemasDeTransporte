#include <stdio.h>

#define MAX_SIZE 10;

// Struct para representar o problema
typedef struct {
    int qtdOrigens;
    int qtdDestinos;
    int custo[MAX_SIZE][MAX_SIZE];
    int oferta[MAX_SIZE];
    int demanda[MAX_SIZE];
} ProblemaTransporte;

// Struct para representar a solução
typedef struct {
    int alocacao[MAX_SIZE][MAX_SIZE];
    int custoTotal;
} SolucaoTransporte;

// Assinaturas das funções do programa
int obtemMariz();

//int cantoNoroeste();
//int custoMinimo();
//int vam();

// Função principal
int main(int argc, char* argv[]) {
    // Variáveis das structs
    ProblemaTransporte problema;
    SolucaoTrasporte solCanNor, solCustMin, solVam;

    printf("=== Problemas de Transporte ===\n");
    
    // Chama a função de coleta de dados, caso retorne 1, algum erro aconteceu e sai do programa.
    if (obtemMartriz(&problema)) {
        return 1;
    }

    return 0;
}

// Função para coletar os dados do problema
int obtemMatriz(ProblemaTransporte *problema) {
    int N, M, sumOferta = 0, sumDemanda = 0;

    printf("- Coleta de dados do problema -\n");

    // Faz a leitura do tamanho da matriz (NxM)
    printf("Digite a quantidade de origens (ex: fábricas) - MAX %d: ", MAX_SIZE);
    if (scanf("%d", &N) != 1 || N <= 0 || N > MAX_SIZE) {
        printf("ERRO: Valor inválido para N.\n");
        return 1;
    }

    printf("Digite a quantidade de destino (ex: depósitos) - MAX%d: ", MAX_SIZE);
    if (scanf("%d", &M) != 1 || M <= 0 || M > MAX_SIZE) {
        printf("ERRO: Valor inválido para M.\n");
        return 1;
    }

    // Passa os valores para o problema
    problema->qtdDestinos = N;
    problema->qtdOrigens = M;

    // Coleta a matriz de custos
    printf("Entre com os valores da matriz de custos\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("Custo C%d%d", i + 1, j + 1);
            if (scanf("%d", &problema->custo[i][j]) != 1 || problema->custo[i][j] < 0) {
                printf("ERRO: Custo inválido!\n");
                return 1;
            } 
        }
    }

    // Coleta os valores das ofertas das origens
    for (int i = 0; i < N; i++) {
        printf("Oferta de N%d: ", i);
        if (scanf("%d", &problema->oferta[i]) != 1 || problema->oferta[i] < 1) {
            printf("ERRO: Oferta inválida!\n");
            return 1;
        }

        sumOferta += problema->oferta[i];
    }

    // Coleta os valores das demandas dos destinos
    for (int j = 0; j < M; j++) {
        printf("Oferta de M%d: ", j);
        if (scanf("%d", &problema->demanda[j]) != 1 || problema->demanda[j] < 1) {
            printf("ERRO: Demanda inválida!\n");
            return 1;
        }

        sumDemanda += problema->demanda[j];
    }

    // Verifica o equilíbrio do problema
    if (sumOferta != sumDemanda) {
        printf("ERRO: O problema não está equilibrado (Oferta e demanda são diferentes).");
    }

    return 0;
}