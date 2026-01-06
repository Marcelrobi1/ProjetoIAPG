#ifndef PLAYER_H
#define PLAYER_H

#include "domino.h"

#define MAX_HAND_SIZE 28
#define PLAYER_NAME_LEN 50

// Player: Estrutura representando um jogador.
// Campos:
// - username, name: Nomes (username não usado).
// - hand: Array de peças na mão.
// - hand_count: Número de peças na mão.
// - score: Pontuação acumulada.
// - is_human: 1 para humano, 0 para IA.
typedef struct {
    char username[PLAYER_NAME_LEN];
    char name[PLAYER_NAME_LEN];
    DominoPiece hand[MAX_HAND_SIZE];
    int hand_count;
    int score;
    int is_human; // 1 = humano, 0 = IA
} Player;

#endif // PLAYER_H
