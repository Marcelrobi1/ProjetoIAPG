#ifndef UI_H
#define UI_H

#include "game.h"

// Protótipos de funções para interface do usuário.
// init_ui: Inicializa curses.
// close_ui: Fecha curses.
// show_main_menu: Exibe menu principal.
// draw_game_state: Desenha estado do jogo.
// display_message: Mostra mensagem.
// get_user_input_move: Obtém entrada de jogada.
// wait_for_key: Aguarda tecla.
// get_player_count: Obtém número de jogadores.
// show_mode_menu: Menu de modo.
// get_player_name: Obtém nome do jogador.
// show_rules: Exibe regras.
// get_input_string: Entrada genérica de string.
// show_load_menu: Menu de carregamento.

void init_ui();
void close_ui();
int show_main_menu();
void draw_domino(int y, int x, DominoPiece p);
void draw_game_state(const GameState *game);
void display_message(const char *msg);
void wait_for_key();
int get_user_input_move(int *piece_idx, int *side);
int get_player_count();
int show_mode_menu();
void get_player_name(char *name, int player_num);
void show_rules();
void get_input_string(const char *prompt, char *buffer, int max_len);
int show_load_menu(char **filenames, int count);

#endif // UI_H
