/**
 * @file processos.c
 * @brief Manipulação da lista de processos.
 *
 * Implementa funções para inserir, remover e reinicializar processos,
 * além de alocar e desalocar memória.
 * Essas funções são utilizadas pela interface gráfica e pelos algoritmos.
 *
 * @author Carolina Mafra Sada, Amanda Victória Almeida Silva
 */

#include <stdlib.h>               // Biblioteca padrão para funções como malloc, realloc, free
#include "processos.h"           // Header com a definição da struct Processo

// Função para inserir um novo processo na lista
// - lista: ponteiro para o vetor de processos atual
// - n: ponteiro para a quantidade de processos
// - novo: struct Processo a ser inserido
Processo* inserir_processo(Processo *lista, int *n, Processo novo) {
    // Realoca memória para adicionar mais um processo à lista
    Processo *temp = realloc(lista, (*n + 1) * sizeof(Processo));
    if (temp == NULL) {
        // Falha na alocação, mantém a lista original
        return lista;
    }
    lista = temp;              // Atualiza o ponteiro da lista com o novo espaço realocado
    lista[*n] = novo;          // Insere o novo processo na última posição da lista
    (*n)++;                    // Incrementa o contador de processos
    return lista;              // Retorna a nova lista
}

// Função para reinicializar a lista de processos (liberar memória e zerar contagem)
void reinicializar_lista(Processo **lista, int *n) {
    if (*lista != NULL) {      // Se a lista não for nula, libera a memória alocada
        free(*lista);          // Libera a memória da lista
        *lista = NULL;         // Define o ponteiro como NULL para evitar uso indevido
    }
    *n = 0;                    // Reseta o contador de processos
}

// Função para remover um processo da lista com base no PID
// - lista: vetor de processos
// - n: ponteiro para o número de processos
// - pid: identificador do processo a ser removido
Processo* remover_processo(Processo *lista, int *n, int pid) {
    int i, pos = -1;           // Inicializa variáveis: índice do laço e posição do processo a remover

    // Encontrar índice do processo com pid
    for (i = 0; i < *n; i++) {
        if (lista[i].pid == pid) {
            pos = i;           // Armazena o índice quando o processo é encontrado
            break;             // Interrompe o laço após encontrar
        }
    }

    if (pos == -1) {
        // Processo não encontrado, retorna lista sem mudanças
        return lista;
    }

    // Deslocar os processos após o removido
    for (i = pos; i < *n - 1; i++) {
        lista[i] = lista[i + 1];
    }

    (*n)--;                    // Decrementa o número total de processos

    // Realoca para tamanho menor
    Processo *temp = realloc(lista, (*n) * sizeof(Processo));
    if (temp == NULL && *n > 0) {
        // Falha em realocar, mantém a lista original
        return lista;
    }

    return temp;               // Retorna a nova lista (ou NULL se n == 0)
}
