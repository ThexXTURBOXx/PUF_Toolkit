#include "BCH_Encoder_View.h"

/*
 * BCH-Encoder - View
 *
 * Author: Sebastian Schurig
 *
 * Visual Functions for the BCH-Encoder
 *
 */


void ErrorMessages(int error, int pos, int pos2)
/*
 * Error Informations and output */
{

    if(error == 1) cout << endl << "ERROR! Invalid input - Only digits are allowed! No digit at " << pos+1 << endl;
    if(error == 2) cout << endl << "ERROR! Invalid input - Input to long." << endl;
    if(error == 3) cout << endl << "ERROR! Invalid input - Try again..." << endl;
    if(error == 4) cout << endl << "ERROR! Invalid input - The Linear Repetition factor has to be 7 or 15." << endl;
    if(error == 5) cout << endl << "ERROR! Invalid input - 'Filename' to long." << endl;
    if(error == 6) cout << endl << "ERROR! Invalid input - No input." << endl;
    if(error == 7) cout << endl << "ERROR! Invalid input - Only digits between 4 - 14 are allowed." << endl;
    if(error == 8) cout << endl << "ERROR! Invalid input - Only digits from the interval (" << pos << ", " << pos2 <<"] are allowed." << endl;
    if(error == 9) cout << endl << "ERROR! Invalid input:" << endl << endl << "For the maximal length (= " << pos << ") the maximal correcting capability is " << pos2 << endl << endl << "Note:" << endl << "The error correcting capability might be reduced if a shorter length is used." << endl;
    if(error == 10) perror("\nERROR! Invalid input - Key filename is not valid");
    if(error == 11) perror("\nERROR! Opening file ");
	if(error == 12) perror("\nERROR! Invalid input - PUF filename is not valid");
    if(error == 13) cout << endl << "ERROR! Chosen part of the PUF-Response is out of range." << endl;
    if(error == 14) cout << endl << "ERROR! Reading file." << endl;
    if(error == 15) cout << endl << "ERROR! You have selected an invalid choice." << endl;
    if(error == 16) cout << endl << "ERROR! Unable to create file." << endl;
    if(error == 17) cout << endl << "WARNING! Looks like the BCH mode is not set." << endl;
    if(error == 18) cout << endl << "WARNING! Looks like the Linear Repetition factor is not set." << endl;
    if(error == 19) cout << endl << "WARNING! Looks like the 'offset' is not set." << endl;
    if(error == 20) cout << endl << "WARNING! Looks like the Key-file is not set." << endl;
    if(error == 21) cout << endl << "WARNING! Looks like the PUF-file is not set." << endl;
    if(error == 22) cout << endl << "WARNING! Looks like the HelperData-filename is not set." << endl;

}

void ClearScreen()
/*
 * Clear the console */
{
    // Check if system is too unsecure!
    system("cls||clear");
}

string
StatusProgress(int x)
/*
 * Status Progress visualization */
{
    string s;
	int i;

    s = "[";
    for (i = 1;i <= (100/2); i++){
        if (i <= (x/2) || x == 100)
            s += "=";
        else if (i == (x/2))
            s += ">";
        else
            s += " ";
    }

    s += "]";

    return s;
}
