#include <stdio.h>
#include <locale.h>
#include <limits.h>

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
int obtemMatriz(ProblemaTransporte *problema);
void exibeProblema(ProblemaTransporte *problema);
void inicializaSolucao(ProblemaTransporte *problema, SolucaoTransporte *solucao);
void calculaCustoTotal(ProblemaTransporte *problema, SolucaoTransporte *solucao);
void exibeSolucao(ProblemaTransporte *problema, SolucaoTransporte *solucao, char *nome_metodo);
void cantoNoroeste(ProblemaTransporte *problema, SolucaoTransporte *solucaoCantoNoroeste);
void custoMinimo(ProblemaTransporte *problema, SolucaoTransporte *solucaoCustoMinimo);
int calculaPenalidade(ProblemaTransporte *problema, int flagColLinham, int coord, int *recursoDisponivel, int tipo);
void vam(ProblemaTransporte *problema, SolucaoTransporte *solucaoVam);

// Função principal
int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Portuguese_Brazil");

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
    cantoNoroeste(&problema, &solCanNor);
    exibeSolucao(&problema, &solCanNor, "Canto Noroeste");

    // Solução pelo método do custo mí­nimo
    custoMinimo(&problema, &solCustMin);
    exibeSolucao(&problema, &solCustMin, "Custo Mínimo");

    // Solução pelo método Vam
    vam(&problema, &solVam);
    exibeSolucao(&problema, &solVam, "VAM");

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

    printf("Digite a quantidade de destino (ex: depósitos) - MAX %d: ", MAX_SIZE);
    if (scanf("%d", &M) != 1 || M <= 0 || M > MAX_SIZE) {
        printf("ERRO: Valor invÃ¡lido para M.\n");
        return 1;
    }

    // Passa os valores para o problema
    problema->qtdOrigens = N;
    problema->qtdDestinos = M;
    
    // Coleta a matriz de custos
    printf("\nEntre com os valores da matriz de custos\n");

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            printf("Custo C%d%d: ", i + 1, j + 1);
            if (scanf("%d", &problema->custo[i][j]) != 1 || problema->custo[i][j] < 0) {
                printf("ERRO: Custo invÃ¡lido!\n");
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
            printf("ERRO: Oferta invÃ¡lida!\n");
            return 1;
        }

        sumOferta += problema->oferta[i];
    }

    // Coleta os valores das demandas dos destinos
    printf("\nEntre com os valores das demandas dos destinos\n");

    for (int j = 0; j < M; j++) {
        printf("Demanda de D%d: ", j + 1);
        if (scanf("%d", &problema->demanda[j]) != 1 || problema->demanda[j] < 1) {
            printf("ERRO: Demanda invÃ¡lida!\n");
            return 1;
        }

        sumDemanda += problema->demanda[j];
    }

    // Verifica o equilí­brio do problema
    if (sumOferta != sumDemanda) {
        printf("ERRO: O problema não está equilibrado (Oferta e demanda são diferentes).");
        return 1;
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

// Função para inicializar a matriz da solução com 0 (evitar dados inconsistentes)
void inicializaSolucao(ProblemaTransporte *problema, SolucaoTransporte *solucao) {
    for (int i = 0; i < problema->qtdOrigens; i++) {
        for (int j = 0; j < problema->qtdDestinos; j++) {
            solucao->alocacao[i][j] = 0;
        }
    }

    solucao->custoTotal = 0;
}

// Funcao para calcular o custo total de uma solucao
void calculaCustoTotal(ProblemaTransporte *problema, SolucaoTransporte *solucao) {
    for (int i = 0; i < problema->qtdOrigens; i++) {
        for (int j = 0; j < problema->qtdDestinos; j++) {
            solucao->custoTotal += solucao->alocacao[i][j] * problema->custo[i][j];
        }
    }
}

// Funcao para exibir o resultado da solução
void exibeSolucao(ProblemaTransporte *problema, SolucaoTransporte *solucao, char *nomeMetodo) {
    int sum = 0;

    printf("-- Solução pelo método %s --\n", nomeMetodo);

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
            printf("  %d  |", solucao->alocacao[i][j]);
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

    printf("\nCusto total final: %d", solucao->custoTotal);

    printf("\n\n");
}

// Função para calcular a solução pelo método do canto noroeste
void cantoNoroeste(ProblemaTransporte *problema, SolucaoTransporte *solucaoCantoNoroeste) {
    // Variáveis com a quantidade de origens e destinos
    int N = problema->qtdOrigens, M = problema->qtdDestinos;

    // Arrays com a oferta e demanda de cada origem e destino
    int ofertaDisponivel[N], demandaDisponivel[M];

    // Variáveis para acompanhar a celula atual, começando em [0][0] (canto noroeste)
    int i = 0, j = 0;

    // Inicializa a matriz da solução 
    inicializaSolucao(problema, solucaoCantoNoroeste);

    // Calcula a oferta e demanda inicial
    for (int i = 0; i < N; i++)
        ofertaDisponivel[i] = problema->oferta[i];
    for (int i = 0; i < M; i++)
        demandaDisponivel[i] = problema->demanda[i];

    // Percorre enquanto i e j não chegaram na coordenada da celula final
    while (i < N && j < M) {
        // Atribui a maior alocação possí­vel considerando a demanda e a oferta (o menor dos dois)
        int alocacao = (ofertaDisponivel[i] < demandaDisponivel[j]) ? ofertaDisponivel[i] : demandaDisponivel[j];

        // Faz a alocação
        solucaoCantoNoroeste->alocacao[i][j] = alocacao;

        // Atualiza a demanda e a oferta
        ofertaDisponivel[i] -= alocacao;
        demandaDisponivel[j] -= alocacao;

        // Move para a proxima celula levando em consideracao a linha e/ou coluna que foi finalizada
        if (ofertaDisponivel[i] == 0) {
            i++;
        }

        if (demandaDisponivel[j] == 0) {
            j++;
        }
    }

    // Calcula o custo total da solucao
    calculaCustoTotal(problema, solucaoCantoNoroeste);
}

// Função para calcular a solução pelo método do Custo Mínimo
void custoMinimo(ProblemaTransporte *problema, SolucaoTransporte *solucaoCustoMinimo) {
    // Variáveis com a quantidade de origens e destinos
    int N = problema->qtdOrigens, M = problema->qtdDestinos;

    // Arrays com a oferta e demanda de cada origem e destino
    int ofertaDisponivel[N], demandaDisponivel[M];    

    // Inicializa a matriz da solução 
    inicializaSolucao(problema, solucaoCustoMinimo);

    // Calcula a oferta e demanda inicial
    for (int i = 0; i < N; i++)
        ofertaDisponivel[i] = problema->oferta[i];
    for (int i = 0; i < M; i++)
        demandaDisponivel[i] = problema->demanda[i];

    // Loop até que todas as ofertas e demandas sejam zeradas
    while (1) {
        int custoMin = INT_MAX, minI = -1, minJ, alocacao, flagOfertas = 0;

        // Percorre todas as celulas disponiveis até encontrar a de menor custo
        for (int i = 0; i < N; i++) {
            // Ainda existe oferta na linha?
            if (ofertaDisponivel[i] > 0) { 
                flagOfertas = 1;
                for (int j = 0; j < M; j++) {
                    // Ainda existe demanda na coluna? E A celula atual é a de menor custo?
                    if (demandaDisponivel[j] > 0 && problema->custo[i][j] < custoMin) {
                        // Salva o menor custo e as coordenadas da celula
                        custoMin = problema->custo[i][j];
                        minI = i;
                        minJ = j;
                    }
                }
            }
        }

        // Saída do loop: se não houver mais ofertas ou não for encontrado um valor mínimo
        if (!flagOfertas || minI == -1)
            break;

        // Salva a quantidade a ser alocada dependendo da oferta e demanda restante
        alocacao = (ofertaDisponivel[minI] < demandaDisponivel[minJ]) ? ofertaDisponivel[minI] : demandaDisponivel[minJ];
        
        // Passa para a solucao o valor
        solucaoCustoMinimo->alocacao[minI][minJ] = alocacao;

        // Atualiza a oferta e demanda
        ofertaDisponivel[minI] -= alocacao;
        demandaDisponivel[minJ] -= alocacao;
    }

    // Calcula o custo total da solucao
    calculaCustoTotal(problema, solucaoCustoMinimo);
}

// Função para calcular as penalidades para o método de VAM
int calculaPenalidade(ProblemaTransporte *problema, int qtd, int coord, int *recursoDisponivel, int tipo) {
    int menor1 = INT_MAX, menor2 = INT_MAX, custo;

    // Percorre os recursos
    for (int i = 0; i < qtd; i++) {
        // Valida se existe recurso 
        if (recursoDisponivel[i] > 0) {
            // Passa a penalidade dependendo do tipo (linha ou coluna)
            custo = (tipo == 0) ? problema->custo[coord][i] : problema->custo[i][coord];
            
            // Busca o menor custo
            if (custo < menor1) {
                menor2 = menor1;
                menor1 = custo;
            } else if (custo < menor2) {
                menor2 = custo;
            }
        }
    }

    // Valor inalterado se só existir um custo disponível ou nenhum disponível
    if (menor2 == INT_MAX) 
        return 0;

    // Retorna a penalidade 
    return menor2 - menor1;
}

// Função para calcular a solução pelo método de VAM
void vam(ProblemaTransporte *problema, SolucaoTransporte *solucaoVam) {
    // Variáveis com a quantidade de origens e destinos
    int N = problema->qtdOrigens, M = problema->qtdDestinos;

    // Arrays com a oferta e demanda de cada origem e destino
    int ofertaDisponivel[N], demandaDisponivel[M];   
    
    // Inicializa a matriz da solução 
    inicializaSolucao(problema, solucaoVam);

    // Calcula a oferta e demanda inicial
    for (int i = 0; i < N; i++)
        ofertaDisponivel[i] = problema->oferta[i];
    for (int i = 0; i < M; i++)
        demandaDisponivel[i] = problema->demanda[i];

    // Loop até que todas as ofertas e demandas sejam zeradas
    while (1) {
        // Variaveis para as penalidades de linhas e colunas
        int penalidadeI[problema->qtdOrigens], penalidadeJ[problema->qtdDestinos];

        // Variaveis da maior penalidade
        int maiorPenalidade = -1, coordMaiorPen = -1, tipoMaiorPen = -1; // 0 = linha, 1 = coluna
        
        // Flag para continuar
        int flagOfertas = 0;

        // Variaveis da posicao de alocacao
        int alocacaoI = -1, alocacaoJ = -1;

        // Variavel do custo minimo para alocar
        int custoMinAlocacao = INT_MAX;

        // Variavel do valor para alocar
        int alocacao;

        // Calcula as penalidades das linhas
        for (int i = 0; i < N; i++) {
            if (ofertaDisponivel[i] > 0) {
                flagOfertas = 1;
                penalidadeI[i] = calculaPenalidade(problema, M, i, demandaDisponivel, 0);
            }
        }

        // Se não houver mais ofertas, finaliza o loop
        if (!flagOfertas)
            break;

        // Calcula as penalidades das colunas
        for (int j = 0; j < M; j++) {
            if (demandaDisponivel[j] > 0) {
                penalidadeJ[j] = calculaPenalidade(problema, N, j, ofertaDisponivel, 1);
            }
        }

        // Busca a maior penalidade
        for (int i = 0; i < N; i++) {
            if (ofertaDisponivel[i] > 0 && penalidadeI[i] > maiorPenalidade ) {
                maiorPenalidade = penalidadeI[i];
                coordMaiorPen = i;
                tipoMaiorPen = 0; 
            }
        }

        for (int j = 0; j < M; j++) {
            if (demandaDisponivel[j] > 0 && penalidadeJ[j] > maiorPenalidade) {
                maiorPenalidade = penalidadeJ[j];
                coordMaiorPen = j;
                tipoMaiorPen = 1; 
            }
        }

        // Se não foi encontrada penalidade, sai do loop
        if (maiorPenalidade == -1) 
            break;

        // Se a maior penalidade for de linha
        if (tipoMaiorPen == 0) {
            // coordenada da maior penalidade
            alocacaoI = coordMaiorPen;

            // Procura o menor curso da linha
            for (int j = 0; j < M; j++) {
                if (demandaDisponivel[j] > 0) {
                    if (problema->custo[alocacaoI][j] < custoMinAlocacao) {
                        custoMinAlocacao = problema->custo[alocacaoI][j];
                        alocacaoJ = j;
                    }
                }
            }
        } else { // Se for de coluna
            // Coordenada da maior penalidade
            alocacaoJ = coordMaiorPen;

            // Procura o menor custo da coluna
            for (int i = 0; i < N; i++) {
                if (ofertaDisponivel[i] > 0) {
                    if (problema->custo[i][alocacaoJ] < custoMinAlocacao) {
                        custoMinAlocacao = problema->custo[i][alocacaoJ];
                        alocacaoI = i;
                    }
                }
            }
        }

        // Busca o valor a ser alocado
        alocacao = (ofertaDisponivel[alocacaoI] < demandaDisponivel[alocacaoJ]) ? ofertaDisponivel[alocacaoI] : demandaDisponivel[alocacaoJ];

        // Aloca o valor na solucao
        solucaoVam->alocacao[alocacaoI][alocacaoJ] = alocacao;

        // Atualiza a oferta e demanda
        ofertaDisponivel[alocacaoI] -= alocacao;
        demandaDisponivel[alocacaoJ] -= alocacao;
    }

    // Calcula o custo total da solucao
    calculaCustoTotal(problema, solucaoVam);
}