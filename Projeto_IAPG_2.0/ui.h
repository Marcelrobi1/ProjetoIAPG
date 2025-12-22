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
    int *side); // Returns 1 if valid syntax, 0 if escape/invalid
// Returns 1 if valid syntax, 0 if escape/invalid
void wait_for_key();
int get_player_count();
int show_mode_menu();

#endif // UI_H
