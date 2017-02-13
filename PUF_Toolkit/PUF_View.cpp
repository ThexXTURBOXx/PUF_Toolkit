#include "PUF_View.h"

/*
 * PUF-ToolKit - Views
 *
 * Author: Sebastian Schurig
 *
 * Visual Functions for the PUF-ToolKit
 *
 */


void ClearScreen()
/*
 * Clear the console */
{
    // Check if system is too unsecure!
    system("cls");
}

void ErrorMessages(int error, int pos)
/*
 * Error Informations and output */
{

    if(error == 1) cout << endl << "ERROR! Invalid input - Only digits are allowed! No digit at " << pos+1 << endl;
    if(error == 2) cout << endl << "ERROR! Invalid input - Input to long." << endl;
    if(error == 3) cout << endl << "ERROR! Invalid input - Try again..." << endl;
    if(error == 4) cout << endl << "ERROR! Invalid input - 'length' has to be greater the zero." << endl;
    if(error == 5) cout << endl << "ERROR! Invalid input - 'Filename' to long." << endl;
    if(error == 6) cout << endl << "ERROR! Invalid input - No input." << endl;
    if(error == 7) cout << endl << "ERROR! Invalid input - Amount has to be greater then zero." << endl;
    if(error == 8) cout << endl << "ERROR! Invalid input - To many paths [Maximum is 99]." << endl;
    if(error == 9) cout << endl << "ERROR! Invalid input - To less paths [Minimum is 2]." << endl;
    if(error == 10) cout << endl << "ERROR! Invalid input - 'Path' to long." << endl;
    if(error == 11) cout << endl << "ERROR! File could not be open." << endl;
    if(error == 12) perror("\nERROR! Opening file / directory / path");
    if(error == 13) cout << endl << "ERROR! Chosen part of the PUF-Response is out of range." << endl;
    if(error == 14) cout << endl << "ERROR! Reading file." << endl;
    if(error == 15) cout << endl << "ERROR! You have selected an invalid choice." << endl;
    if(error == 16) cout << endl << "ERROR! Unable to create file." << endl;
    if(error == 17) cout << endl << "ERROR! Chosen folder contains to less files." << endl;
    if(error == 18) cout << endl << "ERROR! Set a Output-Filename." << endl;
    if(error == 19) perror("\nERROR! Input-Path is not valid");
    if(error == 20) {
            cout << endl << "ERROR! Input-Path " << pos << " is not valid: ";
            perror("");
    }
    if(error == 21) {
            cout << endl << "ERROR! Chosen folder " << pos << " contains to less files." << endl;
            perror("");
    }
    if(error == 22) cout << endl << "WARNING! Looks like the calculation was not started." << endl;
    if(error == 23) cout << endl << "ERROR! Looks like the file was empty." << endl;
    if(error == 24) cout << endl << "ERROR! Looks like the file was not in the supported format." << endl;
}
