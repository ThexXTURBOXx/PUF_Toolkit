#ifndef BCH_DECODER_VIEW_H_INCLUDED
#define BCH_DECODER_VIEW_H_INCLUDED

#include "BCH_Decoder.h"

// Output Functions
void ClearScreen();
void ErrorMessages(int error, int pos);
std::string StatusProgress(int x);
int ViewFile(struct Item *item);


#endif // BCH_DECODER_VIEW_H_INCLUDED
