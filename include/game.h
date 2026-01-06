#ifndef GAME_H
#define GAME_H

#include "config.h"
#include "domino.h"
#include "player.h"

#define MAX_PLAYERS 4
#define DECK_SIZE 28

// GameState: Estrutura que representa o estado completo do jogo.
// Campos:
// - game_id: ID único do jogo.
// - boneyard: Array de peças no baralho.
// - boneyard_count: Número de peças restantes no baralho.
// - board: Array de peças no tabuleiro.
// - board_left_end, board_right_end: Valores expostos nas extremidades.
// - board_count: Número de peças no tabuleiro.
// - players: Array de jogadores.
// - player_count: Número de jogadores.
// - current_player_index: Índice do jogador atual.
// - config: Configurações do jogo.
// - winner_index: Índice do vencedor (-1 se nenhum).
typedef struct {
  int game_id;
  DominoPiece boneyard[DECK_SIZE];
  int boneyard_count;

  DominoPiece board[DECK_SIZE];
  int board_left_end;  // O valor exposto à esquerda
  int board_right_end; // O valor exposto à direita
  int board_count;

  Player players[MAX_PLAYERS];
  int player_count;
  int current_player_index;
  GameConfig config;

  int winner_index; // -1 se ainda não há vencedor
} GameState;

// Protótipos de funções para gerenciar o jogo.
// init_game: Inicializa um novo jogo.
// can_play_piece: Verifica se uma peça pode ser jogada.
// play_piece: Executa uma jogada.
// calculate_score: Calcula pontuação baseada no tabuleiro.
// distribute_pieces: Distribui peças iniciais.
// draw_piece: Permite pescar uma peça.

void init_game(GameState *game, int num_players, int humans);
void distribute_pieces(GameState *game);
int can_play_piece(const GameState *game, DominoPiece piece, int *side);
int play_piece(GameState *game, int player_idx, int piece_idx, int side);
int draw_piece(GameState *game, int player_idx);
int calculate_score(const GameState *game);

#endif // GAME_H
