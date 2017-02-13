#include "BCH_Encoder_Settings.h"

/*
 * BCH-Encoder - Settings
 *
 * Author: Sebastian Schurig
 *
 * Settings Functions for the BCH-Encoder
 *
 */


void DefineFilename(struct Item *item, int option)
/*
 * Define the filename
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 * option = definition if the input Key / PUF or output HD file will be specified
 *
 * Options:
 * 1 = input Key filename
 * 2 = input PUF filename
 * 3 = output HD filename
 */
{
    char *h;
    unsigned int ch;
    char name[52];
    unsigned int error = 0;


    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*          Set the 'File-Name' of the file in which the Key is stored         *" << endl;}
        if(option == 2){
        cout << "*                   Set the 'File-Name' of the PUF-Response                   *" << endl;}
        if(option == 3) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData will be stored    *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2) {
        cout << "* If the file is in the current working directory:                            *" << endl;}
        if(option == 3){
        cout << "* If the file should be stored in the current working directory:              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 2) {
        cout << "*  -> Use only the filename               = Stellaris_PUF1                    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2) {
        cout << "* If the file is in an sub-folder of the current working directory:           *" << endl;}
        if(option == 3) {
        cout << "* If the file should be stored in an sub-folder of the current directory:     *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the relative path and filename  = device1/Stellaris_PUF1            *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2) {
        cout << "* If the file is not in an sub-folder of the current working directory:       *" << endl;}
        if(option == 3) {
        cout << "* If the file should be stored somewhere else:                                *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/key_file.txt *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/Stellaris_PUF1    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/HelperData_1      *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0, 0);
        error = 0;

        cout << endl << "Type in the (Path and) Filename : ";
        if (fgets(name, sizeof(name), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(name, '\n');
                if (h) {
                    if(name[0] == '\n') error = 6;
                    else {
                            *h = '\0';
							// Set input/ouput filename
                            if (option == 1) strcpy (item->input_Key_name ,name);
                            else if (option == 2) strcpy (item->input_PUF_name,name);
                            else if (option == 3) strcpy (item->output_HD_name,name);
                    }
                }
                else {
                    /* newline not found, flush stdin to end of line */
                        while (((ch = getchar()) != '\n')
                            && !feof(stdin)
                            && !ferror(stdin)
                        );
                        error = 5;
                }
          }
          else {
            /* fgets failed, handle error */
            cin.clear();
            error = 3;
          }
          if(!error) break;
    }
}

void DefineSettings(struct Item *item, int option)
/*
 * Function to get the 'offset' and 'Linear Repetition factor' as user input
 *
 * Inputs:
 * item  = pointer to the struct to store the necessary informations
 * option = definition if the offset or LR factor will be specified
 *
 * Options:
 * 1 = Offset, the amount of bytes (in decimal) that will be skipped from the beginning
 * 2 = Linear Repetition factor should be 7 or 15
 */
{
    char oSet[12];
    char lr[4];
    char *h;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    if(option == 1){
        while(true){
            ClearScreen();
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                  Define the 'offset' for the PUF-Response:                  *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                   0  = use the PUF-Response from beginning on               *" << endl;
            cout << "*                   1  = the first byte will be skipped                       *" << endl;
            cout << "*                  'x' = the first 'x' bytes will be skipped                  *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, 0);
            error = 0;

            cout << endl << "Type in the offset in bytes (as decimal number): ";
            if (fgets(oSet, sizeof(oSet), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(oSet, '\n');
                if (h) {
                    *h = '\0';
                    //check input if only digits are used
                    for(i = 0; i < sizeof(oSet)-1; i++){
                        if(oSet[i] != '\0' && !isdigit(oSet[i])){
                            error = 1;
                            break;
                        }
                            if(oSet[i] == '\0') i = sizeof(oSet);
                    }
                    if(error == 0){

                        // Set the offSet
                        item->offSet = atol(oSet);
                        break;
                    }
                }
                else {
                    /* newline not found, flush stdin to end of line */
                    while (((ch = getchar()) != '\n')
                            && !feof(stdin)
                            && !ferror(stdin)
                            );
                    error = 2;
                }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
            }
            if(!error) break;
        }
    }
    else if(option == 2){
        while(true){
            ClearScreen();
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                  Define the factor for the Linear Repetition:               *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*                   7   = each bit will be repeated seven times               *" << endl;
            cout << "*                   15  = each bit will be repeated fifteen times             *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, 0);
            error = 0;

            // Get "LR factor" as user input
            cout << endl << "Enter the Linear Repetition factor (7 or 15): ";
            if (fgets(lr, sizeof(lr), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(lr, '\n');
                    if (h) {
                        if(lr[0] == '\n') error = 6;
                        else {
                            *h = '\0';
                            //check input if only digits are used
                            for(i = 0; i < (signed)sizeof(lr)-1; i++){
                                if(lr[i] != '\0' && !isdigit(lr[i])){
                                    error = 1;
                                    break;
                                }
                                if(lr[i] == '\0') i = sizeof(lr);
                            }
                            if(!error){
                                if(atoi(lr) == 7 || atoi(lr) == 15){
                                    item->LR = atoi(lr);
                                    break;
                                }
                                else error = 4;
                            }
                        }
                    }
                    else {
                        /* newline not found, flush stdin to end of line */
                            while (((ch = getchar()) != '\n')
                                    && !feof(stdin)
                                    && !ferror(stdin)
                                );
                            error = 2;
                    }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
            }
        }
    }
}
