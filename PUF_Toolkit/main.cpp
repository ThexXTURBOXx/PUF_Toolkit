#include "PUF_Toolkit.h"

// Arrays/Vectors for the general computation of the BCH implementation
std::vector<int> p(21);
std::vector<int> alpha_to(16385);
std::vector<int> index_of(16385);
std::vector<int> g(16385);
std::vector<int> bb(16385);
std::vector<int> errpos(1024);

// Arrays/Vectors for the input/output data en-/de-coding
std::vector<int> recd(16385);
std::vector<int> bdata(16385);

// Dynamic Arrays/Vectors for the input/output data
std::vector<int> input;
std::vector<int> output;

// Global settings used during computation
int             m, n, length, k, t, d;
long            numerr = 0, decerror = 0, corerr = 0;
long            filesize;
int             isSet = 0; //specifically for decoder

int main()
/*
 * Main Menu for the PUF-Toolkit */
{
    Main_Menu();
    return 0;
}
