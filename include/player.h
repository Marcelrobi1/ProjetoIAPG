#ifndef PLAYER_H
#define PLAYER_H

#include "domino.h"

// Máximo de peças que um jogador pode acabar com (improvável exceder 28, mas limite seguro)
#define MAX_HAND_SIZE 28
#define PLAYER_NAME_LEN 50

typedef struct {
    char username[PLAYER_NAME_LEN];
    char name[PLAYER_NAME_LEN];
    DominoPiece hand[MAX_HAND_SIZE];
    int hand_count;
    int score;
    int is_human; // 1 = humano, 0 = IA
} Player;

#endif // PLAYER_H
