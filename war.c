// ============================================================================
//      PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (NÍVEL NOVATO)
// ============================================================================
//
// OBJETIVO:
// - Criar uma struct para armazenar dados de territórios.
// - Usar um vetor estático para 5 territórios.
// - Ler os dados de cada território via terminal.
// - Exibir o estado final do mapa.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias
#include <stdio.h>  // Para funções de entrada/saída (printf, scanf, fgets)
#include <string.h> // Para manipulação de strings (strcspn)

// --- Constantes Globais ---
#define NUM_TERRITORIOS 5 // Número de territórios a serem cadastrados
#define MAX_NOME 51       // Máximo de 50 caracteres para nomes + 1 para o terminador nulo '\0'
#define MAX_COR 21        // Máximo de 20 caracteres para cores + 1 para o terminador nulo '\0'

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome,
// a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int numeroTropas;
} Territorio;

// --- Protótipos das Funções ---
void cadastrarTerritorios(Territorio mapa[]);
void exibirMapa(const Territorio mapa[]);
void limparBufferEntrada();

// --- Função Principal (main) ---
// Orquestra a execução do programa.
int main() {
    // Cria um vetor estático para armazenar os 5 territórios.
    // "mapa" é o nosso mundo do jogo.
    Territorio mapa[NUM_TERRITORIOS];

    printf("Bem-vindo ao Desafio WAR Estruturado - Nível Novato!\n");
    printf("Vamos cadastrar os 5 territórios iniciais.\n");

    // 1. Chama a função para cadastrar os dados dos territórios
    cadastrarTerritorios(mapa);

    // 2. Chama a função para exibir o estado atual do mapa
    exibirMapa(mapa);

    return 0; // Indica que o programa terminou com sucesso
}

// --- Implementação das Funções ---

/**
 * @brief Solicita ao usuário que insira os dados de cada território.
 * @param mapa O vetor de territórios que será preenchido.
 */
void cadastrarTerritorios(Territorio mapa[]) {
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Cadastrando Território %d ---\n", i + 1);

        // Leitura do Nome do Território
        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, MAX_NOME, stdin);
        // Remove a quebra de linha ('\n') que o fgets captura
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Leitura da Cor do Exército
        printf("Digite a cor do exército: ");
        fgets(mapa[i].corExercito, MAX_COR, stdin);
        mapa[i].corExercito[strcspn(mapa[i].corExercito, "\n")] = '\0';

        // Leitura do Número de Tropas
        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].numeroTropas);

        // Limpa o buffer de entrada para evitar problemas na próxima
        // leitura do fgets() no loop.
        limparBufferEntrada();
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios em formato de tabela.
 * @param mapa O vetor de territórios a ser exibido (const para garantir que não será alterado).
 */
void exibirMapa(const Territorio mapa[]) {
    printf("\n\n==============================================\n");
    printf("          ESTADO ATUAL DO MAPA\n");
    printf("==============================================\n");
    printf("%-20s | %-15s | %s\n", "Território", "Cor do Exército", "Tropas");
    printf("----------------------------------------------\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-20s | %-15s | %d\n", mapa[i].nome, mapa[i].corExercito, mapa[i].numeroTropas);
    }
    printf("==============================================\n");
}

/**
 * @brief Função utilitária para limpar o buffer de entrada (stdin).
 * Essencial ao misturar scanf() com fgets().
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Apenas descarta o caractere
    }
}