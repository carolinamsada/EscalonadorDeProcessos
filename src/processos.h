/**
 * @file processos.h
 * @brief Definições e protótipos das funções de gerenciamento de processos.
 *
 * Define a estrutura `Processo` e declara funções para manipulação da lista
 * de processos usados no simulador de escalonamento.
 */

#ifndef PROCESSOS_H                // Verifica se o macro PROCESSOS_H já foi definido
#define PROCESSOS_H                // Define o macro PROCESSOS_H para evitar múltiplas inclusões

// Definição da estrutura que representa um processo
typedef struct {
    int pid;                       // Identificador único do processo
    int tempo_chegada;            // Momento em que o processo chega no sistema
    int tempo_execucao;           // Duração da execução do processo
    int prioridade;               // Nível de prioridade (menor valor = maior prioridade)
} Processo;

// Declaração da função que insere um novo processo na lista
// - lista: vetor atual de processos
// - n: ponteiro para o número atual de processos
// - novo: struct do novo processo a ser adicionado
// Retorna: ponteiro para a nova lista atualizada
Processo* inserir_processo(Processo *lista, int *n, Processo novo);

// Declaração da função que remove um processo com base no PID
// - lista: vetor atual de processos
// - n: ponteiro para o número atual de processos
// - pid: identificador do processo a ser removido
// Retorna: ponteiro para a nova lista com o processo removido
Processo* remover_processo(Processo *lista, int *n, int pid);

// Declaração da função que reinicializa a lista de processos
// - lista: ponteiro para o vetor de processos
// - n: ponteiro para o número de processos (será resetado para 0)
void reinicializar_lista(Processo **lista, int *n);

#endif                            // Fim da diretiva de inclusão condicional
