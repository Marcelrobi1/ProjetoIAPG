#include "domino.h"
#include <stdlib.h>
#include <time.h>


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
