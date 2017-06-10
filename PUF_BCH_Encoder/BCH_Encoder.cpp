#include "BCH_Encoder.h"

/*
 * BCH-Encoder - Menu
 *
 * Author: Sebastian Schurig
 *
 * Menu for the BCH-Encoder
 *
 */


int Main_Menu()
/*
 * Main Menu for the PUF-BCH-Encoder */
{

    char menuChoice[3];
    char *h;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;
    unsigned int offset_isSet = 0;
    unsigned int LR_isSet = 0;
    struct Item item;
    stringstream BCH;
    string bch_mode;


    // Set the initial values for the data struct
    item.offSet = 0;
    item.input_Key_length = 0;
    item.LR = 0;
    strcpy(item.input_Key_name, "none");
    strcpy(item.input_PUF_name, "none");
    strcpy(item.output_HD_name, "none");
    strcpy(item.BCHmode, "none");
    strcpy(item.result, "none");

    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "********************************* BCH-Encoder *********************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                        1 : Set BCH mode                                     *" << endl;
        cout << "*                        2 : Set Linear Repetition factor                     *" << endl;
        cout << "*                        3 : Set 'offset' for the PUF file                    *" << endl;
        cout << "*                        4 : Set Key file                                     *" << endl;
        cout << "*                        5 : Set PUF file                                     *" << endl;
        cout << "*                        6 : Set HelperData output filename                   *" << endl;
        cout << "*                        7 : Generate HelperData                              *" << endl;
        cout << "*                        8 : Exit Program                                     *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "          Settings:                                                            " << endl;
        cout << "                     BCH mode    = " << item.BCHmode                             << endl;
        if(LR_isSet){
        cout << "                     LR Factor   = " << item.LR                                  << endl;}
        else {
        cout << "                     LR Factor   = none"                                         << endl;}
        if(offset_isSet){
        cout << "                     OffSet      = " << item.offSet                              << endl;}
        else {
        cout << "                     OffSet      = none"                                         << endl;}
        cout << "                     Key file    = " << item.input_Key_name                      << endl;
        cout << "                     PUF file    = " << item.input_PUF_name                      << endl;
        cout << "                     HD file     = " << item.output_HD_name                      << endl;
        cout << "          Result:                                                              " << endl;
        cout << "                     Calculation progress = " << item.result                     << endl;
        cout << "                                                                               " << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0, 0);
        error = 0;

        while(true){
            cout << endl << "Make a choice by typing in a number (1-8): ";
            if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(menuChoice, '\n');
                if (h) {
                    *h = '\0';
                    switch(menuChoice[0])
                        {
                        case '1':
                            cout << endl << " Processing : Set BCH mode" << endl << endl;
                            ClearScreen();
                            read_p();               /* Read m */
                            generate_gf();          /* Construct the Galois Field GF(2**m) */
                            gen_poly();             /* Compute the generator polynomial of BCH code */
                            BCH << "BCH(" << length << ", " << k << ", " << d << ")";
                            bch_mode = BCH.str();
                            strcpy(item.BCHmode, bch_mode.c_str());
                            BCH.str("");
                            BCH.clear();
                            strcpy(item.result, "none");
                            break;
                        case '2':
                            cout << endl << " Processing : Set Linear Repetition factor" << endl << endl;
                            error = 0;
                            DefineSettings(&item, 2);
                            LR_isSet = 1;
                            strcpy(item.result, "none");
                            break;
                        case '3':
                            cout << endl << " Processing : Set 'offset' for the PUF file" << endl << endl;
                            error = 0;
                            DefineSettings(&item, 1);
                            offset_isSet = 1;
                            strcpy(item.result, "none");
                            break;
                        case '4':
                            cout << endl << " Processing : Set Key file" << endl << endl;
                            error = 0;
                            DefineFilename(&item, 1);
                            strcpy(item.result, "none");
                            break;
                        case '5':
                            cout << endl << " Processing : Set PUF file" << endl << endl;
                            DefineFilename(&item, 2);
                            strcpy(item.result, "none");
                            break;
                        case '6':
                            cout << endl << " Processing : Set HelperData output filename" << endl << endl;
                            DefineFilename(&item, 3);
                            strcpy(item.result, "none");
                            break;
                        case '7':
                            cout << endl << " Processing : Generate HelperData" << endl << endl;
                            if(strcmp(item.BCHmode, "none") == 0) error = 17;
                            else if(!LR_isSet) error = 18;
                            else if(!offset_isSet) error = 19;
                            else if(strcmp(item.input_Key_name, "none") == 0) error = 20;
                            else if(strcmp(item.input_PUF_name, "none") == 0) error = 21;
                            else if(strcmp(item.output_HD_name, "none") == 0) error = 22;
                            if(!error) error = ReadKeyFile(&item);
                            if(!error) error = Calculation(&item);
                            if(!error) strcpy(item.result, "done - HelperData saved");
                            break;
                        case '8':
                            ClearScreen();
                            cout << endl << " Exiting Program -  bye bye" << endl << endl;
                            error = 0;
                            exit = 1;
                            break;
                        default:
                            error = 15;
                            break;
                        }
                    break;
                }
                else {
                    /* newline not found, flush stdin to end of line */
                    while (((ch = getchar()) != '\n')
                             && !feof(stdin)
                             && !ferror(stdin)
                            );
                        error = 2;
                        break;
                }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
                break;
            }
        }
        if(exit) break;
    }
    return 0;
}
