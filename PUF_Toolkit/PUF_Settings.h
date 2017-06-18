#ifndef PUF_SETTINGS_H_INCLUDED
#define PUF_SETTINGS_H_INCLUDED

#include "PUF_Toolkit.h"

// Setting Functions
unsigned int SetInputLen(struct Item *item, int option = 0);
void DefineMode(struct Item *it1);
void DefineOffSetLength(struct Item *it1);
void DefineFilename(struct Item *item, int option);
void DefineFilename_BCH(struct Item *item, int option);
void DefinePathname(struct Item *item, int option);
void DefineSettings(struct Item *item, int option);

#endif // PUF_SETTINGS_H_INCLUDED
