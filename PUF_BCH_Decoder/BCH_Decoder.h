#ifndef BCH_DECODER_H_INCLUDED
#define BCH_DECODER_H_INCLUDED

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <sys/stat.h>
#include <sstream>
#include <fstream>
#include "BCH_Decoder_View.h"
#include "BCH_Decoder_File.h"
#include "BCH_Decoder_Settings.h"
#include "BCH_Decoder_Calculation.h"
using namespace std;


// Data structure definition
    struct Item {
        long offSet;
        char input_HD_name[102];
        char input_PUF_name[102];
        char output_Key_name[102];
        char BCHmode[25];
        int LR;
        char result[52];
    } ;

/*
 *
 * Pattern struct which will be appended to the HelperData
 *
 * errorCode:           defines if Golay or BCH Error correction codes are used
 *                      errorCode = 0 -> Golay
 *                      errorCode = 1 -> BCH
 *
 * Golay_BCH_length:    defines the length of one codeword for Golay and BCH
 *
 * Golay_d_BCH_t:       defines for Golay the distance 'd' and for BCH the correction capability 't'
 *
 * Golay_k_BCH_m:       defines the message length for Golay and the parameter 'm' for BCH
 *
 *                      NOTE: With this parameter the Golay Code looks like
 *
 *                          -> Golay(Golay_BCH_length, Golay_k_BCH_m, Golay_d_BCH_t) e.g.Golay(23,12,7)
 *
 *                      NOTE: With this parameter the BCH mode looks like
 *
 *                          -> BCH(Golay_BCH_length, will be calculated , Golay_d_BCH_t*2+1)
 *                           -> Golay_k_BCH_m is needed for the calculation of the correct BCH(mode)
 *
 * linearRep:           defines the Linear Repetition factor (e.g. 7 or 15)
 *
 * puf_offSet:          defines the offset of the PUF Response which has to be skipped
 *
 * original_filesize:   defines the filesize of the original input file
 *
 */
    struct Pattern {
        int errorCode;
        int Golay_BCH_length;
        int Golay_d_BCH_t;
        int Golay_k_BCH_m;
        int linearRep;
        long puf_offSet;
        long original_filesize;
    } ;

// Arrays/Vectors for the general computation of the BCH implementation
extern std::vector<int> p;
extern std::vector<int> alpha_to;
extern std::vector<int> index_of;
extern std::vector<int> g;
extern std::vector<int> bb;
extern std::vector<int> errpos;

// Array/Vectors for the input/output data
extern std::vector<int> recd;
extern std::vector<int> input;
extern std::vector<int> output;

// global settings used during computation
extern int             m, n, length, k, t, d;
extern long            numerr, decerror, corerr;
extern long            filesize;
extern int             isSet;

// Main Menu for the BCH-Decoder
int Main_Menu();

#endif // BCH_DECODER_H_INCLUDED
