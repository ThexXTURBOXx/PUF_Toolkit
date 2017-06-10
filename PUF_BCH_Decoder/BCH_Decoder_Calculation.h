#ifndef BCH_DECODER_CALCULATION_H_INCLUDED
#define BCH_DECODER_CALCULATION_H_INCLUDED

#include "BCH_Decoder.h"

// Calculation Functions
void read_p();
void generate_gf();
void gen_poly();
void decode_bch();
int MajorityVoting(unsigned long pos, struct Item *item);
int Calculation(struct Item *item);

#endif // BCH_DECODER_CALCULATION_H_INCLUDED
