#include "../include/ui.h"
#include "../lib/pdcurses/PDCurses-3.9/curses.h"
#include <stdlib.h>
#include <string.h>

// Funções para gerenciar a interface do usuário usando PDCurses.
// Inclui inicialização, menus, desenho do jogo, entrada do usuário.

// init_ui: Inicializa a biblioteca curses para o jogo.
// Passos:
// 1. Inicia curses com initscr.
// 2. Configura modos: cbreak, noecho, keypad.
// 3. Inicia cores e define pares de cores para números e destaques.
void init_ui() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Fundo
  init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Destaque
  // Pares de cores para números de dominó 0-6
  init_pair(3, COLOR_RED, COLOR_BLACK);     // 0
  init_pair(4, COLOR_GREEN, COLOR_BLACK);   // 1
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);  // 2
  init_pair(6, COLOR_BLUE, COLOR_BLACK);    // 3
  init_pair(7, COLOR_MAGENTA, COLOR_BLACK); // 4
  init_pair(8, COLOR_CYAN, COLOR_BLACK);    // 5
  init_pair(9, COLOR_WHITE, COLOR_BLACK);   // 6
}

void close_ui() { endwin(); }

// show_main_menu: Exibe o menu principal e retorna a escolha do usuário.
// Passos:
// 1. Limpa a tela.
// 2. Imprime opções: Novo Jogo, Carregar, Regras, Sair.
// 3. Aguarda entrada e retorna o dígito escolhido.
int show_main_menu() {
  clear();
  mvprintw(2, 2, "JOGO DE DOMINO");
  mvprintw(4, 2, "1. Iniciar Novo Jogo");
  mvprintw(5, 2, "2. Carregar Jogo Salvo");
  mvprintw(6, 2, "3. Regras/Ajuda");
  mvprintw(7, 2, "4. Sair");
  mvprintw(9, 2, "Escolha: ");
  refresh();

  int ch = getch();
  return ch - '0';
}

// draw_domino: Desenha uma peça de dominó na posição especificada.
// Passos:
// 1. Imprime '['.
// 2. Imprime side1 com cor correspondente.
// 3. Imprime '|'.
// 4. Imprime side2 com cor correspondente.
// 5. Imprime ']'.
void draw_domino(int y, int x, DominoPiece p) {
  mvprintw(y, x, "[");
  attron(COLOR_PAIR(p.side1 + 3));
  printw("%d", p.side1);
  attroff(COLOR_PAIR(p.side1 + 3));
  printw("|");
  attron(COLOR_PAIR(p.side2 + 3));
  printw("%d", p.side2);
  attroff(COLOR_PAIR(p.side2 + 3));
  printw("]");
}

// draw_game_state: Desenha o estado completo do jogo na tela.
// Passos:
// 1. Limpa a tela.
// 2. Imprime ID do jogo e número de jogadores.
// 3. Desenha o tabuleiro (mesa) com peças.
// 4. Imprime pontuações dos jogadores.
// 5. Se jogador atual for humano, mostra sua mão.
// 6. Se IA, mostra mensagem de pensamento.
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
      move(y_off, 2 + (i * 8));
      printw("%d:[", i + 1);
      attron(COLOR_PAIR(game->players[p_idx].hand[i].side1 + 3));
      printw("%d", game->players[p_idx].hand[i].side1);
      attroff(COLOR_PAIR(game->players[p_idx].hand[i].side1 + 3));
      printw("|");
      attron(COLOR_PAIR(game->players[p_idx].hand[i].side2 + 3));
      printw("%d", game->players[p_idx].hand[i].side2);
      attroff(COLOR_PAIR(game->players[p_idx].hand[i].side2 + 3));
      printw("]");
    }
  } else {
    y_off += 2;
    mvprintw(y_off, 2, "IA %s esta a pensar...", game->players[p_idx].name);
  }

  refresh();
}

// display_message: Exibe uma mensagem na parte inferior da tela.
// Passos:
// 1. Move para a linha de mensagens.
// 2. Limpa a linha.
// 3. Imprime a mensagem.
// 4. Atualiza a tela.
void display_message(const char *msg) {
  move(LINES - 2, 2);
  clrtoeol();
  printw("MSG: %s", msg);
  refresh();
}

// wait_for_key: Pausa aguardando entrada do usuário.
// Passos:
// 1. Imprime instrução para pressionar tecla.
// 2. Aguarda getch().
void wait_for_key() {
  mvprintw(LINES - 1, 2, "Pressione qualquer tecla...");
  refresh();
  getch();
}

// get_user_input_move: Solicita entrada de jogada do usuário.
// Passos:
// 1. Ativa echo.
// 2. Imprime prompt para entrada (Peça# Lado).
// 3. Lê string.
// 4. Desativa echo.
// 5. Parseia entrada: 'q' para sair, '0' para passar, ou 'num lado'.
// 6. Retorna código: 1=valido, 2=passar invalido, 3=sair.
int get_user_input_move(int *piece_idx, int *side) {
  echo();
  mvprintw(LINES - 3, 2,
           "Insira jogada (Peca# Lado[L/R]) (ex: 1 L), 0 para passar, 'q' para "
           "menu: ");
  char buf[10];
  getnstr(buf, 9);
  noecho();

  if (strcmp(buf, "q") == 0 || strcmp(buf, "Q") == 0) {
    return 3; // Sair para menu
  }

  int p;
  char s;
  if (sscanf(buf, "%d %c", &p, &s) == 2) {
    *piece_idx = p - 1; // Índice base 0
    if (s == 'L' || s == 'l')
      *side = 1;
    else if (s == 'R' || s == 'r')
      *side = 2;
    else
      *side = 1; // verificação padrão?
    return 1;
  } else if (sscanf(buf, "%d", &p) == 1) {
    if (p == 0)
      return 2; // Passar
    *piece_idx = p - 1;
    *side = 1; // Padrão Esquerda (Utilizador deve especificar R se necessário)
    return 1;
  }

  return 0; // Inválido
}

// get_player_count: Solicita o número de jogadores.
// Passos:
// 1. Ativa echo.
// 2. Imprime prompt.
// 3. Lê string e converte para int.
// 4. Limita entre 2 e 4.
// 5. Desativa echo.
int get_player_count() {
  echo();
  mvprintw(LINES - 5, 2, "Numero de jogadores (2-4): ");
  char buf[10];
  getnstr(buf, 9);
  noecho();
  int n = atoi(buf);
  if (n < 2)
    n = 2;
  if (n > 4)
    n = 4;
  return n;
}

// show_mode_menu: Exibe menu de seleção de modo de jogo.
// Passos:
// 1. Limpa a tela.
// 2. Imprime opções: Jogador vs IA, Jogador vs Jogador.
// 3. Aguarda entrada e retorna escolha.
int show_mode_menu() {
  clear();
  mvprintw(2, 2, "MODO DE JOGO");
  mvprintw(4, 2, "1. Jogador vs AI");
  mvprintw(5, 2, "2. Jogador vs Jogador (Hotseat)");
  mvprintw(7, 2, "Escolha: ");
  int ch = getch();
  return ch - '0';
}

// get_player_name: Solicita nome para um jogador.
// Passos:
// 1. Limpa a tela.
// 2. Imprime prompt com número do jogador.
// 3. Ativa echo e lê string.
// 4. Desativa echo.
// 5. Se vazio, define como "Jogador".
void get_player_name(char *name, int player_num) {
  clear();
  mvprintw(2, 2, "Nome do Jogador %d: ", player_num);
  echo();
  mvgetstr(4, 2, name);
  noecho();
  if (strlen(name) == 0) {
    strcpy(name, "Jogador");
  }
}

void show_rules() {
  clear();
  mvprintw(1, 2, "REGRAS DO JOGO - Domino Muggins");
  mvprintw(
      3, 2,
      "Objetivo: Ser o primeiro a esvaziar a mao ou ter a menor pontuacao.");
  mvprintw(4, 2, "Como Jogar:");
  mvprintw(5, 2, "1. Cada jogador recebe 5 pecas.");
  mvprintw(6, 2, "2. O jogador com a dupla mais alta comeca.");
  mvprintw(7, 2, "3. Coloque pecas conectando numeros iguais nos extremos.");
  mvprintw(8, 2, "4. Se nao puder jogar, compre do baralho.");
  mvprintw(9, 2, "5. Ganha quem esvaziar a mao primeiro.");
  mvprintw(10, 2, "6. Pontuacao: Multiplos de 5 contam pontos.");
  mvprintw(12, 2, "Controles:");
  mvprintw(13, 2, "- Digite 'indice L/R' para jogar (ex: 1 L)");
  mvprintw(14, 2, "- Digite 0 para passar se nao puder jogar.");
  mvprintw(15, 2, "- Digite 'q' para voltar ao menu principal.");
  mvprintw(17, 2, "Pressione qualquer tecla para voltar...");
  refresh();
  getch();
}

void get_input_string(const char *prompt, char *buffer, int max_len) {
  clear();
  mvprintw(LINES / 2, 2, "%s", prompt);
  echo();
  getnstr(buffer, max_len - 1);
  noecho();
}

int show_load_menu(char **filenames, int count) {
  int selected = 0;
  int ch;

  while (1) {
    clear();
    mvprintw(2, 2, "CARREGAR JOGO");

    if (count == 0) {
      mvprintw(4, 2, "Nenhum jogo salvo encontrado.");
      mvprintw(6, 2, "Pressione qualquer tecla para voltar.");
      refresh();
      getch();
      return -1;
    }

    for (int i = 0; i < count; i++) {
      if (i == selected)
        attron(A_REVERSE);
      mvprintw(4 + i, 4, "%s", filenames[i]);
      if (i == selected)
        attroff(A_REVERSE);
    }

    mvprintw(
        LINES - 2, 2,
        "Use as setas para escolher e ENTER para carregar. 'q' para voltar.");
    refresh();

    ch = getch();
    if (ch == KEY_UP) {
      selected--;
      if (selected < 0)
        selected = count - 1;
    } else if (ch == KEY_DOWN) {
      selected++;
      if (selected >= count)
        selected = 0;
    } else if (ch == 10) { // Enter
      return selected;
    } else if (ch == 'q' || ch == 'Q') {
      return -1;
    }
  }
}
