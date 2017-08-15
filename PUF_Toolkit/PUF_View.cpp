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
    //system("cls");
}

void ErrorMessages(int error, int pos)
/*
 * Error Informations and output */
{

    if(error == 1) cout << endl << "ERROR! Invalid input - Only digits are allowed! No digit at " << pos+1 << endl;
    if(error == 2) cout << endl << "ERROR! Invalid input - Input to long." << endl;
    if(error == 3) cout << endl << "ERROR! Invalid input - Try again..." << endl;
    if(error == 4) cout << endl << "ERROR! Invalid input - 'length' has to be greater the zero." << endl;
    if(error == 5) cout << endl << "ERROR! Invalid input - 'Filename' too long." << endl;
    if(error == 6) cout << endl << "ERROR! Invalid input - No input." << endl;
    if(error == 7) cout << endl << "ERROR! Invalid input - Amount has to be greater then zero." << endl;
    if(error == 8) cout << endl << "ERROR! Invalid input - To many paths [Maximum is 99]." << endl;
    if(error == 9) cout << endl << "ERROR! Invalid input - To less paths [Minimum is 2]." << endl;
    if(error == 10) cout << endl << "ERROR! Invalid input - 'Path' to long." << endl;
    if(error == 11) cout << endl << "ERROR! File could not be opened/read." << endl;
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
    if(error == 25) cout << endl << "WARNING! Looks like the 'offset' is not set." << endl;
    if(error == 26) cout << endl << "ERROR! Looks like the 'Output file and/or Result' is not set/calculated." << endl;
}

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

void ErrorMessages_decode(int error, int pos)
/*
 * Error Informations and output */
{

    if(error == 1) cout << endl << "ERROR! Invalid input - Only digits are allowed! No digit at " << pos+1 << endl;
    if(error == 2) cout << endl << "ERROR! Invalid input - Input to long." << endl;
    if(error == 3) cout << endl << "ERROR! Invalid input - Try again..." << endl;
    if(error == 4) perror("\nError! Opening output file");
    if(error == 5) cout << endl << "ERROR! Invalid input - 'Filename' to long." << endl;
    if(error == 6) cout << endl << "ERROR! Invalid input - No input." << endl;
    if(error == 7) cout << endl << "ERROR! Reading PUF file." << endl;
    if(error == 8) perror("\nERROR! Opening PUF file");
    if(error == 9) perror("\nERROR! Invalid input - PUF filename is not valid");
    if(error == 10) perror("\nERROR! Invalid input - HelperData filename is not valid");
    if(error == 11) perror("\nERROR! Opening HelperData file ");
    if(error == 12) cout << endl << "ERROR! Reading HelperData file." << endl;
    if(error == 13) cout << endl << "ERROR! You have selected an invalid choice." << endl;
    if(error == 14) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 'm' is out of range" << endl;
    if(error == 15) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 't' is out of range" << endl;
    if(error == 16) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 'length' is out of range" << endl;
    if(error == 17) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 'LR factor' is not 7 or 15" << endl;
    if(error == 18) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 'offSet' is out of range" << endl;
    if(error == 19) cout << endl << "ERROR! HelperData: The recovered 'decoding information' are invalid!" << endl << "                   -> Parameter 'filesize' is out of range" << endl;
    if(error == 20) perror("\nERROR! Opening file to view");
    if(error == 21) cout << endl << "WARNING! Looks like no decoding was performed." << endl;

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

int ViewFile(char *filename)
/*
 * View a text file */
{
    string text;
    ifstream view;
    char buf[2];
    unsigned int ch;
    char *h;


    view.open(filename);

    if(view.fail()) {
		view.close();
		return 20;
	}

    // Show secret when 'return' is pressed, skip all other input
    ClearScreen();
    cout << endl << endl << endl << endl << endl;
    cout << "*******************************************************************************" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*                                   WARNING!                                  *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*        The content of the file might be privacy / security relevant!        *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*            Please verify that no-one else is watching your monitor!         *" << endl;
    cout << "*                                                                             *" << endl;
    cout << "*******************************************************************************" << endl << endl;
    cout << "To continue press 'return'";
    if (fgets(buf, sizeof(buf), stdin)) {
        /* fgets succeeds, scan for newline character */
        h = strchr(buf, '\n');
        if (h) {
        *h = '\0';
        }
        else {
            /* newline not found, flush stdin to end of line */
            while (((ch = getchar()) != '\n')
            && !feof(stdin)
            && !ferror(stdin)
            );
        }
    }
    else {
        cin.clear();
    }
    ClearScreen();

    // Output of the file content
    while(!view.eof())
        {
            getline(view,text);
            cout << "" << text << endl;
        }

    view.close();

	// Show output until 'return' is pressed, skip all other input
    cout << "To continue press 'return'";
    if (fgets(buf, sizeof(buf), stdin)) {
        /* fgets succeeds, scan for newline character */
        h = strchr(buf, '\n');
        if (h) {
        *h = '\0';
        return 0;
        }
        else {
            /* newline not found, flush stdin to end of line */
            while (((ch = getchar()) != '\n')
            && !feof(stdin)
            && !ferror(stdin)
            );
            return 0;
        }
    }
    else {
        cin.clear();
        return 0;
    }
  return 0;
}
