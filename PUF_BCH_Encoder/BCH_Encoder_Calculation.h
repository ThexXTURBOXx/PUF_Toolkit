#ifndef BCH_ENCODER_CALCULATION_H_INCLUDED
#define BCH_ENCODER_CALCULATION_H_INCLUDED

#include "BCH_Encoder.h"

// Calculation Functions
void read_p();
void generate_gf();
void gen_poly();
void encode_bch();
int Calculation(struct Item *item);

#endif // BCH_ENCODER_CALCULATION_H_INCLUDED
