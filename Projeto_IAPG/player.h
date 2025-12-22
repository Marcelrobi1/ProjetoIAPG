#ifndef PLAYER_H
#define PLAYER_H

#include "domino.h"

// Max pieces a player might end up with (unlikely to exceed 28, but safe limit)
#define MAX_HAND_SIZE 28
#define PLAYER_NAME_LEN 50

typedef struct {
    char username[PLAYER_NAME_LEN];
    char name[PLAYER_NAME_LEN];
    DominoPiece hand[MAX_HAND_SIZE];
    int hand_count;
    int score;
    int is_human; // 1 = human, 0 = AI
} Player;

#endif // PLAYER_H
