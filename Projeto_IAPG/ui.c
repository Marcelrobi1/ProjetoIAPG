#include "ui.h"
#include <curses.h>
#include <string.h>

void init_ui() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Background
  init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Highlight
}

void close_ui() { endwin(); }

int show_main_menu() {
  clear();
  mvprintw(2, 2, "DOMINO MUGGINS (FIVE-UP)");
  mvprintw(4, 2, "1. Iniciar Novo Jogo");
  mvprintw(5, 2, "2. Carregar Jogo (Nao Impl)");
  mvprintw(6, 2, "3. Regras/Ajuda");
  mvprintw(7, 2, "4. Sair");
  mvprintw(9, 2, "Escolha: ");
  refresh();

  int ch = getch();
  return ch - '0';
}

void draw_domino(int y, int x, DominoPiece p) {
  mvprintw(y, x, "[%d|%d]", p.side1, p.side2);
}

void draw_game_state(const GameState *game) {
  clear();
  mvprintw(1, 2, "Jogo #%d - Jogadores: %d", game->game_id, game->player_count);

  // Draw Board
  mvprintw(3, 2, "Mesa:");
  int x_off = 2;
  int y_off = 4;
  for (int i = 0; i < game->board_count; i++) {
    // Wrap if too long
    if (x_off > 70) {
      x_off = 2;
      y_off++;
    }
    draw_domino(y_off, x_off, game->board[i]);
    x_off += 6;
  }

  // Draw Scores
  y_off += 2;
  for (int i = 0; i < game->player_count; i++) {
    mvprintw(y_off, 2 + (i * 20), "%s: %d", game->players[i].name,
             game->players[i].score);
  }

  // Draw Current Player's Hand
  int p_idx = game->current_player_index;
  if (game->players[p_idx].is_human) {
    y_off += 2;
    mvprintw(y_off, 2, "Sua Mao (%s):", game->players[p_idx].name);
    y_off++;
    for (int i = 0; i < game->players[p_idx].hand_count; i++) {
      mvprintw(y_off, 2 + (i * 8), "%d:[%d|%d]", i + 1,
               game->players[p_idx].hand[i].side1,
               game->players[p_idx].hand[i].side2);
    }
  } else {
    y_off += 2;
    mvprintw(y_off, 2, "IA %s esta a pensar...", game->players[p_idx].name);
  }

  refresh();
}

void display_message(const char *msg) {
  mvprintw(LINES - 2, 2, "MSG: %s", msg);
  refresh();
}

void wait_for_key() {
  mvprintw(LINES - 1, 2, "Pressione qualquer tecla...");
  refresh();
  getch();
}

int get_user_input_move(int *piece_idx, int *side) {
  echo();
  mvprintw(LINES - 3, 2,
           "Insira jogada (Peca# Lado[L/R]) (ex: 1 L) ou 0 para passar: ");
  char buf[10];
  getnstr(buf, 9);
  noecho();

  int p;
  char s;
  if (sscanf(buf, "%d %c", &p, &s) == 2) {
    *piece_idx = p - 1; // 0-indexed
    if (s == 'L' || s == 'l')
      *side = 1;
    else if (s == 'R' || s == 'r')
      *side = 2;
    else
      *side = 1; // default check?
    return 1;
  } else if (sscanf(buf, "%d", &p) == 1) {
    if (p == 0)
      return 2; // Pass
    *piece_idx = p - 1;
    *side = 1; // Try left/auto?
    return 1;
  }

  return 0; // Invalid
}
