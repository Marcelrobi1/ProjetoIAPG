#include "../include/domino.h"
#include <stdlib.h>
#include <time.h>

// Funções para gerenciar peças de dominó.

// init_deck: Inicializa o baralho com todas as 28 peças de dominó (0-0 a 6-6).
// Passos:
// 1. Para cada combinação i,j onde i <= j (0 a 6), cria uma peça com side1=i, side2=j.
// 2. Marca como duplo se i==j.

void init_deck(DominoPiece deck[]) {
  int count = 0;
  for (int i = 0; i <= 6; i++) {
    for (int j = i; j <= 6; j++) {
      deck[count].side1 = i;
      deck[count].side2 = j;
      deck[count].is_double = (i == j);
      count++;
    }
  }
}

void shuffle_deck(DominoPiece deck[], int count) {
  for (int i = count - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    DominoPiece temp = deck[i];
    deck[i] = deck[j];
    deck[j] = temp;
  }
}
