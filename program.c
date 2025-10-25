#include <stdio.h>
#include <locale.h>

#define MAX_SIZE 10

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
int obtemMatriz();
void exibeProblema();

//int cantoNoroeste();
//int custoMinimo();
//int vam();

// Função principal
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Portuguese");

    // Variáveis das structs
    ProblemaTransporte problema;
    SolucaoTransporte solCanNor, solCustMin, solVam;

    printf("=== Problemas de Transporte ===\n\n");
    
    // Chama a função de coleta de dados, caso retorne 1, algum erro aconteceu e sai do programa.
    if (obtemMatriz(&problema)) {
        return 1;
    }

    // Exibição do problema após a leitura (para confirmação do usuário)
    exibeProblema(&problema);

    // Solução pelo método do canto noroeste

    // Solução pelo método do custo mínimo

    // Solução pelo método Vam

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
    printf("\nEntre com os valores da matriz de custos\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("Custo C%d%d: ", i + 1, j + 1);
            if (scanf("%d", &problema->custo[i][j]) != 1 || problema->custo[i][j] < 0) {
                printf("ERRO: Custo inválido!\n");
                return 1;
            } 
        }
        printf("\n");
    }

    // Coleta os valores das ofertas das origens
    printf("Entre com os valores das ofertas das origens\n");

    for (int i = 0; i < N; i++) {
        printf("Oferta de O%d: ", i + 1);
        if (scanf("%d", &problema->oferta[i]) != 1 || problema->oferta[i] < 1) {
            printf("ERRO: Oferta inválida!\n");
            return 1;
        }

        sumOferta += problema->oferta[i];
    }

    // Coleta os valores das demandas dos destinos
    printf("\nEntre com os valores das demandas dos destinos\n");

    for (int j = 0; j < M; j++) {
        printf("Oferta de D%d: ", j + 1);
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

    printf("\n\n");
    return 0;
}

// Função para exibir os dados do problema
void exibeProblema(ProblemaTransporte *problema) {
    int sum = 0;

    printf("-- Exibição do problema --\n\n");

    printf("---------------------------------------------\n"); 
    printf("|  XXXXX  ");
    for (int i = 0; i < problema->qtdDestinos; i++) {
        printf("|  D%d  ", i + 1);
    }

    printf("| Suprimento |\n");
    printf("---------------------------------------------\n"); 

    for (int i = 0; i < problema->qtdOrigens; i++) {
        printf("|    O%d   |", i + 1);

        for (int j = 0; j < problema->qtdDestinos; j++) {
            printf("  %d  |", problema->custo[i][j]);
        }

        printf("     %d     |\n", problema->oferta[i]);
        printf("---------------------------------------------\n"); 
    }

    printf("| Demanda ");

    for (int i = 0; i < problema->qtdDestinos; i++) {
        printf("|  %d  ", problema->demanda[i]);
        sum += problema->demanda[i];
    }

    printf("|     %d     |\n", sum);
    printf("---------------------------------------------\n"); 

    printf("\n\n");
}