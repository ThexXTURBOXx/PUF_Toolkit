#ifndef PUF_CALCULATION_H_INCLUDED
#define PUF_CALCULATION_H_INCLUDED

#include "PUF_Toolkit.h"

// Calculation Functions
int Median_AVG(struct Item *item);
int HammingWeight(struct Item *item, int option);
int Entropy(struct Item *item, int option);
int InterHD(struct Item *item, int option);
int IntraHD(struct Item *item, int option);
int MinEntropy(struct Item *item);

// Calculation Functions for BCH menu
void read_p();
void generate_gf();
void gen_poly();
void encode_bch();
void decode_bch();
int MajorityVoting(unsigned long pos, struct Item *item);
int Calculation_encode(struct Item *item);
int Calculation_decode(struct Item *item);

#endif // PUF_CALCULATION_H_INCLUDED
