#ifndef PUF_VIEW_H_INCLUDED
#define PUF_VIEW_H_INCLUDED

#include "PUF_Toolkit.h"

void ClearScreen();
void ErrorMessages(int error, int pos);
//bch view functions
void ErrorMessages(int error, int pos, int pos2);
void ErrorMessages_decode(int error, int pos);
std::string StatusProgress(int x);
int ViewFile(char *filename);
#endif // PUF_VIEW_H_INCLUDED
