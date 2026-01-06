#ifndef DOMINO_H
#define DOMINO_H

// DominoPiece: Estrutura representando uma peça de dominó.
// Campos:
// - side1, side2: Valores dos lados (0-6).
// - is_double: 1 se side1 == side2, 0 caso contrário.
typedef struct {
    int side1;
    int side2;
    int is_double; // 1 se side1 == side2, 0 caso contrário
} DominoPiece;

// Protótipos de funções para dominós.
// init_deck: Inicializa baralho completo.
// shuffle_deck: Embaralha o baralho.

void init_deck(DominoPiece deck[]);
void shuffle_deck(DominoPiece deck[], int count);

#endif // DOMINO_H
