#ifndef PUF_FILE_H_INCLUDED
#define PUF_FILE_H_INCLUDED

#include "PUF_Toolkit.h"

// File-system Functions
bool IsFile(const char* input);
bool IsDir(const char* input);
int ViewFile(struct Item *item, int option);
int SaveFile(struct Item *item, int option);
int ReadKeyFile(struct Item *item);

#endif // PUF_FILE_H_INCLUDED
