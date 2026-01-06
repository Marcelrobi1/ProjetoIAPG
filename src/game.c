#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Funções para gerenciar o estado do jogo de dominó.
// Inclui inicialização, distribuição de peças, verificação de jogadas,
// execução de jogadas, cálculo de pontuação e pescar peças.

// init_game: Inicializa um novo estado de jogo.
// Passos:
// 1. Define ID do jogo baseado no tempo.
// 2. Inicializa tabuleiro vazio.
// 3. Cria e embaralha o baralho (boneyard).
// 4. Inicializa jogadores com nomes padrão e marca humanos/IA.
// 5. Distribui 5 peças para cada jogador.
void init_game(GameState *game, int num_players, int num_humans) {
  game->game_id = (int)time(NULL); // ID simples baseado no tempo
  game->board_count = 0;
  game->board_left_end = -1;
  game->board_right_end = -1;
  game->player_count = num_players;
  game->current_player_index =
      0; // Começa com jogador 0, tecnicamente deveria ser o com a dupla mais
         // alta, mas simplificado.
  game->winner_index = -1;
  game->boneyard_count = 28; // Baralho completo inicialmente

  // Inicializar baralho
  init_deck(game->boneyard);
  shuffle_deck(game->boneyard, 28);

  // Inicializar jogadores
  for (int i = 0; i < num_players; i++) {
    sprintf(game->players[i].name, "Jogador %d", i + 1);
    game->players[i].hand_count = 0;
    game->players[i].score = 0;
    game->players[i].is_human = (i < num_humans);

  }

  distribute_pieces(game);
}

// distribute_pieces: Distribui peças do baralho para os jogadores.
// Passos:
// 1. Dá 5 peças para cada jogador do topo do baralho.
// 2. Remove essas peças do baralho, deslocando o restante para o início.
void distribute_pieces(GameState *game) {
  // Distribuir 5 peças para cada jogador
  int card_idx = 0;
  for (int p = 0; p < game->player_count; p++) {
    for (int i = 0; i < 5; i++) {
      game->players[p].hand[i] = game->boneyard[card_idx++];
      game->players[p].hand_count++;
    }
  }

  // Deslocar baralho
  // Estratégia rápida: deslocar o restante para o início
  int remaining = 28 - card_idx;
  for (int i = 0; i < remaining; i++) {
    game->boneyard[i] = game->boneyard[card_idx + i];
  }
  game->boneyard_count = remaining;
}

// can_play_piece: Verifica se uma peça pode ser jogada no tabuleiro atual.
// Passos:
// 1. Se tabuleiro vazio, qualquer peça é válida.
// 2. Verifica se side1 ou side2 da peça corresponde às extremidades esquerda ou direita.
// 3. Retorna 1 se válida, e define side_to_match indicando possibilidades (1=esquerda, 2=direita, 3=ambas).
int can_play_piece(const GameState *game, DominoPiece piece,
                   int *side_to_match) {
  // Se o tabuleiro estiver vazio, qualquer peça é válida
  if (game->board_count == 0) {
    *side_to_match = 0; // Qualquer
    return 1;
  }

  // Verificar contra a extremidade esquerda e direita
  // Peça tem s1, s2. Tabuleiro tem L, R.
  // Lógica:
  // Se peça s1 == L -> corresponder esquerda (conectar s1 a L, novo L é s2)
  // Se peça s2 == L -> corresponder esquerda (conectar s2 a L, novo L é s1)
  // Se peça s1 == R -> corresponder direita (conectar s1 a R, novo R é s2)
  // Se peça s2 == R -> corresponder direita (conectar s2 a R, novo R é s1)

  // Nota: Esta função verifica se *pelo menos uma* jogada é possível.
  // Se múltiplas são possíveis, a Interface precisa perguntar ao utilizador.
  // Aqui retornamos 1 se válida. side_to_match pode ser parâmetro de saída
  // indicando possibilidades: 1 = esquerda, 2 = direita, 3 = ambas.

  int valid = 0;
  if (piece.side1 == game->board_left_end ||
      piece.side2 == game->board_left_end)
    valid |= 1;
  if (piece.side1 == game->board_right_end ||
      piece.side2 == game->board_right_end)
    valid |= 2;

  if (valid)
    *side_to_match = valid;
  return valid > 0;
}

// play_piece: Executa uma jogada, colocando uma peça no tabuleiro.
// Passos:
// 1. Valida a jogada (assumindo pré-validação).
// 2. Orienta a peça corretamente baseada no lado (esquerda/direita).
// 3. Insere a peça no tabuleiro (deslocando para esquerda se necessário).
// 4. Atualiza extremidades do tabuleiro.
// 5. Remove a peça da mão do jogador.
// 6. Calcula e adiciona pontuação se aplicável.
// 7. Verifica se o jogador venceu (mão vazia).
int play_piece(GameState *game, int player_idx, int piece_idx, int side) {
  // lado: 1 = esquerda, 2 = direita
  Player *p = &game->players[player_idx];
  DominoPiece piece = p->hand[piece_idx];

  // Validar novamente, lógica poderia estar aqui, mas assumindo válido chamado
  // antes.

  // Adicionar ao tabuleiro
  // O GameState tem array `board`. Devemos adicionar a ele.
  // Mas como adicionamos às *extremidades*, um append simples de array só
  // funciona para uma extremidade para manter a ordem visual simplificada. Para
  // armazenar cadeia adequadamente, precisamos saber o layout. Nota:
  // Implementação simplificada para rastrear extremidades logicamente. A UI
  // pode redesenhar baseada no estado completo se necessário.

  if (game->board_count == 0) {
    game->board[0] = piece;
    game->board_count = 1;
    game->board_left_end = piece.side1;
    game->board_right_end = piece.side2;
    // Usuário poderia rotacionar primeira peça? geralmente não importa a menos
    // que Spinner.
  } else {
    if (side == 1) { // Esquerda
      // Deve corresponder ao board_left_end
      // Verificar orientação
      int connected_val = -1;
      int new_end = -1;

      if (piece.side2 == game->board_left_end) {
        // [s1|s2] -> corresponde s2 à Esquerda. Visual: [s1|s2] [Esquerda..].
        // Correto.
        connected_val = piece.side2;
        new_end = piece.side1;
      } else if (piece.side1 == game->board_left_end) {
        // [s1|s2] -> corresponde s1 à Esquerda. Visual: [s1|s2] [Esquerda..].
        // Queremos s2 como lado conectante para jogada à Esquerda (lado direito
        // da peça toca tabuleiro). Então trocamos.
        int temp = piece.side1;
        piece.side1 = piece.side2;
        piece.side2 = temp;

        connected_val = piece.side2; // Agora s2 é o antigo s1
        new_end = piece.side1;
      } else {
        return 0; // Inválido
      }

      // Inserir em 0, deslocar outros
      for (int i = game->board_count; i > 0; i--) {
        game->board[i] = game->board[i - 1];
      }
      game->board[0] = piece;
      game->board_count++;
      game->board_left_end = new_end;

    } else if (side == 2) { // Direita
      // Deve corresponder ao board_right_end
      int new_end = -1;
      if (piece.side1 == game->board_right_end) {
        // [...Direita] [s1|s2]. Correto.
        new_end = piece.side2;
      } else if (piece.side2 == game->board_right_end) {
        // [...Direita] [s1|s2]. Corresponde s2.
        // Queremos s1 para conectar. Trocar.
        int temp = piece.side1;
        piece.side1 = piece.side2;
        piece.side2 = temp;

        new_end = piece.side2; // novo exposto é antigo s1
      } else {
        return 0; // Inválido
      }

      game->board[game->board_count] = piece;
      game->board_count++;
      game->board_right_end = new_end;
    }
  }

  // Remover da mão
  for (int i = piece_idx; i < p->hand_count - 1; i++) {
    p->hand[i] = p->hand[i + 1];
  }
  p->hand_count--;

  // Pontuação
  int points = calculate_score(game);
  if (points > 0) {
    p->score += points;
  }

  // Verificar vitória
  if (p->hand_count == 0) {
    game->winner_index = player_idx;

  }

  return 1;
}

// calculate_score: Calcula pontuação baseada nas extremidades abertas (regra Muggins).
// Passos:
// 1. Soma os valores das extremidades.
// 2. Para duplos, conta o valor total.
// 3. Se a soma for múltiplo de 5, retorna a pontuação; senão, 0.
int calculate_score(const GameState *game) {
  if (game->board_count == 0)
    return 0;

  // Soma das pontas abertas.
  // Se for duplo, conta o valor total (ex: 6-6 conta 12).
  // Regra Muggins: Se a soma for múltiplo de 5, ganha pontos.

  // Verificando peça no índice 0 para Esquerda, índice count-1 para Direita.

  int left_val = game->board_left_end;
  DominoPiece left_piece = game->board[0];
  if (left_piece.is_double)
    left_val += left_val;

  int score = 0;

  // Esquerda
  if (game->board[0].is_double)
    score += (game->board_left_end * 2);
  else
    score += game->board_left_end;

  // Direita (se apenas 1 peça, esquerda=direita fisicamente)
  if (game->board_count == 1) {
    // Apenas uma peça. É ambas as extremidades.
    // ex. 5-5. Pontuação total da peça.
    score = game->board[0].side1 + game->board[0].side2;
  } else {
    if (game->board[game->board_count - 1].is_double)
      score += (game->board_right_end * 2);
    else
      score += game->board_right_end;
  }

  if (score % 5 == 0)
    return score;
  return 0;
}

// draw_piece: Permite a um jogador pescar uma peça do baralho.
// Passos:
// 1. Verifica se o baralho não está vazio e a mão não está cheia.
// 2. Adiciona a peça do topo do baralho à mão do jogador.
// 3. Remove a peça do baralho, deslocando o restante.
// 4. Retorna 1 se sucesso, 0 se falha.
int draw_piece(GameState *game, int player_idx) {
  if (game->boneyard_count == 0)
    return 0; // Vazio

  Player *p = &game->players[player_idx];
  if (p->hand_count >= MAX_HAND_SIZE)
    return 0; // Mão cheia?

  // Pegar da frente
  p->hand[p->hand_count] = game->boneyard[0];
  p->hand_count++;

  // Deslocar baralho
  for (int i = 0; i < game->boneyard_count - 1; i++) {
    game->boneyard[i] = game->boneyard[i + 1];
  }
  game->boneyard_count--;

  return 1;
}
