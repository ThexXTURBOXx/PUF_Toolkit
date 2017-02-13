#include "BCH_Decoder.h"

/*
 * BCH-Decoder - Menu
 *
 * Author: Sebastian Schurig
 *
 * Menu for the BCH-Decoder
 *
 */


int Main_Menu()
/*
 * Main Menu for the PUF-BCH-Decoder */
 {

    char menuChoice[3];
    char *h;
    unsigned int ch;
    unsigned int error = 0;
    unsigned int exit = 0;
    struct Item item;


    // Set the initial values for the data struct
    item.offSet = 0;
    item.LR = 0;
    strcpy(item.input_HD_name, "none");
    strcpy(item.input_PUF_name, "none");
    strcpy(item.output_Key_name, "none");
    strcpy(item.BCHmode, "none");
    strcpy(item.result, "none");

    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                         Welcome to the PUF-Toolkit                          *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "********************************* BCH-Decoder *********************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*                        1 : Set HelperData                                   *" << endl;
        cout << "*                        2 : Set PUF file                                     *" << endl;
        cout << "*                        3 : Set Key file                                     *" << endl;
        cout << "*                        4 : Decode HelperData                                *" << endl;
        cout << "*                        5 : View recovered File                              *" << endl;
        cout << "*                        6 : Exit Program                                     *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "    Settings:                                                                  " << endl;
        if(strcmp(item.result, "Done - file saved") == 0) {
        cout << "                                                                               " << endl;}
        cout << "                     HD file     = " << item.input_HD_name                       << endl;
        cout << "                     PUF file    = " << item.input_PUF_name                      << endl;
        cout << "                     Key file    = " << item.output_Key_name                     << endl;
        cout << "                                                                               " << endl;
        cout << "    Result:                                                                    " << endl;
        if(strcmp(item.result, "Done - file saved") == 0) {
        cout << "                                                                               " << endl;}
        cout << "                     BCH mode    = " << item.BCHmode                             << endl;
        if(isSet){
        cout << "                     LR Factor   = " << item.LR                                  << endl;}
        else {
        cout << "                     LR Factor   = none"                                         << endl;}
        if(isSet){
        cout << "                     OffSet      = " << item.offSet                              << endl;}
        else {
        cout << "                     OffSet      = none"                                         << endl;}
        cout << "                                                                               " << endl;
        if(strcmp(item.result, "Done - file saved") == 0) {
        cout << "    Decoding Error summary:                                                    " << endl;
        cout << "                                                                               " << endl;
        cout << "       Error detection           = " << numerr << " errors in total detected"    << endl;
        cout << "       Error correction          = " << corerr << " of " << numerr << " errors corrected" << endl;
        cout << "       Not corrected codeword(s) = " << decerror                                 << endl;
        cout << "                                                                               " << endl;
        cout << "                                                                               " << endl;}
        cout << "            Calculation progress = " << item.result                              << endl;
        cout << "                                                                               " << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        while(true){
            cout << endl << "Make a choice by typing in a number (1-6): ";
            if (fgets(menuChoice, sizeof(menuChoice), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(menuChoice, '\n');
                if (h) {
                    *h = '\0';
                    switch(menuChoice[0])
                        {
                        case '1':
                            cout << endl << " Processing : Set HelperData" << endl << endl;
                            DefineFilename(&item, 3);
                            strcpy(item.result, "none");
                            break;
                        case '2':
                            cout << endl << " Processing : Set PUF file" << endl << endl;
                            error = 0;
                            DefineFilename(&item, 2);
                            strcpy(item.result, "none");
                            break;
                        case '3':
                            cout << endl << " Processing : Set KEY file" << endl << endl;
                            DefineFilename(&item, 1);
                            strcpy(item.result, "none");
                            break;
                        case '4':
                            ClearScreen();
                            cout << endl << " Processing : Decode HelperData" << endl << endl;
                            error = Calculation(&item);
                            if(!error) strcpy(item.result, "Done - file saved");
                            break;
                        case '5':
                            cout << endl << " Processing : View Key file" << endl << endl;
                            if(strcmp(item.output_Key_name, "none") == 0) error = 19;
                            else if(strcmp(item.result, "none") == 0) error = 21;
                            if(error == 0 || error == 21) {
                                if(error == 0) error = ViewFile(&item);
                                else if(error == 21 && !(ViewFile(&item) == 0)) error = 20;
                            }
                            break;
                        case '6':
                            ClearScreen();
                            cout << endl << " Exiting Program -  bye bye" << endl << endl;
                            error = 0;
                            exit = 1;
                            break;
                        default:
                            error = 13;
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
