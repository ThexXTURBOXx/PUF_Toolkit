#ifndef BCH_ENCODER_VIEW_H_INCLUDED
#define BCH_ENCODER_VIEW_H_INCLUDED

#include "BCH_Encoder.h"

// Output / View Functions
void ClearScreen();
void ErrorMessages(int error, int pos, int pos2);
std::string StatusProgress(int x);


#endif // BCH_ENCODER_VIEW_H_INCLUDED
