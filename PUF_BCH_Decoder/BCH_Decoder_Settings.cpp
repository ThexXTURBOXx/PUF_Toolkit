#include "BCH_Decoder_Settings.h"

/*
 * BCH-Decoder - Settings
 *
 * Author: Sebastian Schurig
 *
 * Setting Function for the BCH-Decoder
 *
 */


void DefineFilename(struct Item *item, int option)
/*
 * Define the filenames
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 * option = definition of the input HelperData and corresponding PUF Response and the name of the output Key file
 *
 * Options:
 * 1 = output Key filename
 * 2 = input PUF filename
 * 3 = input HD filename
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
        cout << "*   Set the 'File-Name' of the file in which the decoded Key will be stored   *" << endl;}
        if(option == 2){
        cout << "*                   Set the 'File-Name' of the PUF-Response                   *" << endl;}
        if(option == 3) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData will is stored    *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 3 || option == 2) {
        cout << "* If the file is in the current working directory:                            *" << endl;}
        if(option == 1){
        cout << "* If the file should be stored in the current working directory:              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 2) {
        cout << "*  -> Use only the filename               = Stellaris_PUF1                    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 3 || option == 2) {
        cout << "* If the file is in an sub-folder of the current working directory:           *" << endl;}
        if(option == 1) {
        cout << "* If the file should be stored in an sub-folder of the current directory:     *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the relative path and filename  = device1/Stellaris_PUF1            *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 3 || option == 2) {
        cout << "* If the file is not in an sub-folder of the current working directory:       *" << endl;}
        if(option == 1) {
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

        if(error) ErrorMessages(error, 0);
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
                            if (option == 1) strcpy (item->output_Key_name ,name);
                            else if (option == 2) strcpy (item->input_PUF_name,name);
                            else if (option == 3) strcpy (item->input_HD_name ,name);
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
