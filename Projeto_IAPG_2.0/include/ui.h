#ifndef UI_H
#define UI_H

#include "game.h"

void init_ui();
void close_ui();
int show_main_menu();
void draw_game_state(const GameState *game);
void display_message(const char *msg);
int get_user_input_move(
    int *piece_idx,
    int *side); // Retorna 1 se jogada válida, 2 se passar, 3 se sair, 0 se inválido
void wait_for_key();
int get_player_count();
int show_mode_menu();
void get_player_name(char *name, int player_num);
void show_rules();

#endif // UI_H
