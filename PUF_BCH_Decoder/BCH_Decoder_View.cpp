#include "BCH_Decoder_View.h"

/*
 * BCH-Decoder - View
 *
 * Author: Sebastian Schurig
 *
 * Visual Functions for the BCH-Decoder
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

int ViewFile(struct Item *item)
/*
 * View a text file */
{
    string text;
    ifstream view;
    char buf[2];
    unsigned int ch;
    char *h;


    view.open(item->output_Key_name);

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
