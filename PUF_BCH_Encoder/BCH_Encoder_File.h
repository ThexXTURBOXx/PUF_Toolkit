#ifndef BCH_ENCODER_FILE_H_INCLUDED
#define BCH_ENCODER_FILE_H_INCLUDED

#include "BCH_Encoder.h"

// File-system Functions
bool IsFile(const char* input);
int ReadKeyFile(struct Item *item);

#endif // BCH_ENCODER_FILE_H_INCLUDED
