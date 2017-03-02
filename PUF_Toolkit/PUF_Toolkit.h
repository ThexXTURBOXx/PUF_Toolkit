#ifndef PUF_TOOLKIT_H_INCLUDED
#define PUF_TOOLKIT_H_INCLUDED

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <cstdlib>
#include <sys/stat.h>
#include <cmath>
#include <sstream>
#include <iomanip>
#include "PUF_View.h"
#include "PUF_File.h"
#include "PUF_Settings.h"
#include "PUF_Calculation.h"
using namespace std;


// Data structure definition
    struct Item {
        unsigned long offset_begin;
        unsigned long offset_end;
        unsigned long input_length;
        char input_file_name[102];
        char output_file_name[102];
        vector<string> input_path_name;
        unsigned long zeros;
        unsigned long ones;
        double entrp;
        double median;
        double average;
        char result[52];
        unsigned int HD_error_pos;
        unsigned int HD_mode;
        unsigned int HW_ENTP_mode;
    } ;

// Menu Functions
void HammingWeight_Menu(struct Item *item);
void Entropy_Menu(struct Item *item);
void IntraHD_Menu(struct Item *item);
void InterHD_Menu(struct Item *item);
void MinEntropy_Menu(struct Item *item);
int Main_Menu();


#endif // PUF_TOOLKIT_H_INCLUDED
