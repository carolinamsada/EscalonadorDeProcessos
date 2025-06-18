/**
 * @file interface.h
 * @brief Prototipação das funções relacionadas à interface gráfica.
 *
 * Permite iniciar e controlar as janelas da interface gráfica do simulador.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>

void abrir_tela_processos();
void abrir_dialogo_adicionar_processo(GtkWidget *parent);
void atualizar_lista(GtkWidget *listbox);
void on_btn_remover_clicked(GtkButton *button, gpointer user_data);
void on_btn_reinicializar_clicked(GtkButton *button, gpointer user_data);
void abrir_tela_selecao_algoritmo(GtkWidget *widget);
void executar_escalonamento(int algoritmo_index, GtkWidget *widget);
void abrir_resultado_simulacao(const char *resultado);

#endif