/**
 * @file simulador.h
 * @brief Declaração dos algoritmos de escalonamento.
 *
 * Fornece protótipos das funções de simulação utilizadas pelo simulador.
 * Deve ser incluído por qualquer módulo que deseje executar os algoritmos.
 */

#ifndef SIMULADOR_H           // Verifica se SIMULADOR_H já foi definido (evita inclusão duplicada)
#define SIMULADOR_H           // Define SIMULADOR_H para garantir que este header só seja incluído uma vez

#include "processos.h"        // Inclui a definição da struct Processo e outras dependências necessárias

// Declara a função de simulação do algoritmo FIFO (First-Come, First-Served)
// - lista_processos: vetor de processos a serem simulados
// - n: quantidade de processos
// - output_buffer: buffer de string onde o resultado formatado será armazenado
void simular_FIFO(Processo *lista_processos, int n, char *output_buffer);

// Declara a função de simulação do algoritmo SJF (Shortest Job First, não-preemptivo)
// Mesmos parâmetros: vetor de processos, número de processos e buffer de saída
void simular_sjf(Processo *lista_processos, int n, char *output_buffer);

// Declara a função para simulação do algoritmo Round Robin (preemptivo com quantum fixo)
// Executa os processos em fatias de tempo iguais 
// Parâmetros seguem o mesmo padrão
void simular_round_robin(Processo *lista_processos, int n, char *output_buffer);

// Declara a função para simulação do algoritmo de prioridade preemptiva
// Executa o processo com maior prioridade (menor valor numérico) a cada ciclo
void simular_prioridade(Processo *lista_processos, int n, char *output_buffer);

#endif  // Finaliza a diretiva de inclusão condicional
