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

// calculation function for jaccard index
int Jaccard_Index(struct Item *item);
int Jaccard_Intra(struct Item *item);
int Jaccard_Inter(struct Item *item);

// calculation functions for Golay
int Golay_encode(struct Item *item);
int Golay_decode(struct Item *item);

//
//*****************************************************************************
// Define GOLAY DATA
#define X22             0x00400000   /* vector representation of X^{22} */
#define X11             0x00000800   /* vector representation of X^{11} */
#define MASK12          0xfffff800   /* auxiliary vector for testing */
#define GENPOL          0x00000c75   /* generator polinomial, g(x) */

#endif // PUF_CALCULATION_H_INCLUDED
