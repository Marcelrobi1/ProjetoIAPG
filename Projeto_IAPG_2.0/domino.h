#ifndef DOMINO_H
#define DOMINO_H

typedef struct {
    int side1;
    int side2;
    int is_double; // 1 if side1 == side2, 0 otherwise
} DominoPiece;

// Function prototypes
void init_deck(DominoPiece deck[]);
void shuffle_deck(DominoPiece deck[], int count);

#endif // DOMINO_H
