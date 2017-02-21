#include "PUF_Settings.h"

/*
 * PUF-ToolKit - Settings
 *
 * Author: Sebastian Schurig
 *
 * Setting Functions for the PUF-ToolKit
 *
 */


void DefineOffSetLength(struct Item *it1)
/*
 * Function to get the 'offset' and 'length' as user input
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 *
 * Offset = the amount of bytes (in decimal) that will be script from the beginning
 * Length = the mount of bytes that will be used (in decimal)
 */
{
    char oSet[12];
    char iLength[12];
    char *p;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    while(true){
		if (error != 0) ClearScreen();
		cout << "*******************************************************************************" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                  Define the 'offset' for the PUF-Response:                  *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                   0  = use complete binary PUF-Response                     *" << endl;
		cout << "*                   1  = the first byte will be skipped                       *" << endl;
		cout << "*                  'x' = 'x' bytes from both ends of file will be skipped     *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*******************************************************************************" << endl;

		if(error) ErrorMessages(error, i);
		error = 0;

		cout << endl << "Type in the offset in bytes (as decimal number): ";
		if (fgets(oSet, sizeof(oSet), stdin)) {
			/* fgets succeeds, scan for newline character */
			p = strchr(oSet, '\n');
				if (p) {
					*p = '\0';

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
						it1->offSet = atol(oSet);

					while(true){
						ClearScreen();
						cout << "*******************************************************************************" << endl;
						cout << "*                                                                             *" << endl;
						cout << "*                 Define the 'length' of the used PUF-Response                *" << endl;
						cout << "*                                                                             *" << endl;
						cout << "*                 256 = use 256 bytes of the binary PUF-Response              *" << endl;
						cout << "*                 'x' = use 'x' bytes of the binary PUF-Response              *" << endl;
						cout << "*                                                                             *" << endl;
						cout << "*******************************************************************************" << endl;
						if(error) ErrorMessages(error, i);
						error = 0;
						cout << endl << "Type in the length in bytes (as decimal number): ";
							if (fgets(iLength, sizeof(iLength), stdin)) {
								/* fgets succeeds, scan for newline character */
								p = strchr(iLength, '\n');
									if (p) {
										*p = '\0';

										//check input if only digits are used
										for(i = 0; i < sizeof(iLength); i++){
											if(iLength[i] != '\0' && !isdigit(iLength[i])){
											   error = 1;
											   break;
											}
											if(iLength[i] == '\0') i = sizeof(iLength);
										}

										if(error == 0 && atol(iLength) > 0){
											// Set the length
											it1->input_length = atol(iLength);
											break;
										}
										else if(error == 0 && atol(iLength) == 0){
										   error = 4;
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

void DefineFilename(struct Item *item, int option)
/*
 * Define the filename
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 * option = definition if the input or output file will be specified
 *
 * Options:
 * 1 = input filename
 * 2 = output filename
 * 3 = input filename for Median and Average
 */
{
    char *p;
    unsigned int ch;
    char name[52];
    unsigned int error = 0;

    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*              Define the 'Input-File-Name' for the PUF-Response              *" << endl;}
        if(option == 2){
        cout << "*                Define the 'Output-File-Name' for the Results                *" << endl;}
        if(option == 3) {
        cout << "*              Define the 'Input-File-Name' for the Median and Average        *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 3) {
        cout << "* If the file is in the current working directory:                            *" << endl;}
        if(option == 2){
        cout << "* If the file should be stored in the current working directory:              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use only the filename               = input_file1                       *" << endl;}
        if(option == 2) {
        cout << "*  -> Use only the filename               = output_file1.txt                  *" << endl;}
        if(option == 3) {
        cout << "*  -> Use only the filename               = input_file1.txt                   *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 3) {
        cout << "* If the file is in an sub-folder of the current working directory:           *" << endl;}
        if(option == 2) {
        cout << "* If the file should be stored in an sub-folder of the current directory:     *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the relative path and filename  = device1/input_file1               *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the relative path and filename  = device1/output_file1.txt          *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the relative path and filename  = device1/input_file1.txt           *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 3) {
        cout << "* If the file is not in an sub-folder of the current working directory:       *" << endl;}
        if(option == 2) {
        cout << "* If the file should be stored somewhere else:                                *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/input_file1  *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/output_file1.txt  *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/input_file1.txt   *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 3) {
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*   NOTE:       The input file for the Median and Average calculation         *" << endl;
        cout << "*               has to consist of plain numbers separated with a blank.       *" << endl;
        cout << "*               -> Integers and floating point numbers are allowed.           *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*               Example: '1 42 0.739 79 0 ...'                                *" << endl;
        cout << "*                                                                             *" << endl;}
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, 0);
        error = 0;

        cout << endl << "Type in the (Path and) Filename : ";

        if (fgets(name, sizeof(name), stdin)) {
            /* fgets succeeds, scan for newline character */
            p = strchr(name, '\n');
                if (p) {
                    if(name[0] == '\n') error = 6;
                    else {
                            *p = '\0';

							// Set input/ouput filename
                            if (option == 1 || option == 3) strcpy (item->input_file_name,name);
                            else if (option == 2) strcpy (item->output_file_name,name);
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

void DefinePathname(struct Item *item, int option)
/*
 * Define the pathname
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 * option = definition if one or more input paths will be specified
 *
 * Options:
 * 1 = one input path
 * 2 = more input paths
 */
{
    char *p;
    unsigned int ch;
    char name[102];
    char path_numbers[4];
    unsigned int number_of_paths = 1;
    unsigned int i = 0;
    unsigned int error = 0;
    bool amount_set = false;

    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1){
        cout << "*    Define the 'Input-Path' for the PUF-Responses of the desired device      *" << endl;}
        if(option == 2){
        cout << "*    Define the 'Input-Paths' for the PUF-Responses of the desired devices    *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*    If the folder is a sub-folder of the current working directory:          *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*          -> Use the relative path   = device1                               *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*    If the folder is not a sub-folder of the current working directory:      *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*          -> Use the full path       = C:/User/Data/device1                  *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, i);
        error = 0;

        if(option == 2 && !amount_set){
        cout << endl << "Type in how many different devices folder to use (min = 2, max = 99): ";

            if (fgets(path_numbers, sizeof(path_numbers), stdin)) {
                /* fgets succeeds, scan for newline character */
                p = strchr(path_numbers, '\n');
                    if (p) {
                        *p = '\0';

                        //check input if only digits are used
                        for(i = 0; i < sizeof(path_numbers); i++){
                            if(path_numbers[i] != '\0' && !isdigit(path_numbers[i])){
                                error = 1;
                                break;
                            }
                            if(path_numbers[i] == '\0') i = sizeof(path_numbers);
                        }

                        if(error == 0 && atol(path_numbers) > 1){
                            // Set the amount of paths
                            number_of_paths = atol(path_numbers);
                            error = 0;
                        }
                        else if(error == 0 && atol(path_numbers) == 0){
                            error = 7;
                        }
                        else if(error == 0 && atol(path_numbers) == 1){
                            error = 9;
                        }
                    }
                    else {
                        /* newline not found, flush stdin to end of line */
                        while (((ch = getchar()) != '\n')
                            && !feof(stdin)
                            && !ferror(stdin)
                            );
                        error = 8;
                    }
            }
            else {
                /* fgets failed, handle error */
                cin.clear();
                error = 3;
                }
            if(!error) {
                amount_set = true;
                }
        }
        if(!error){

           for(i = 0; i < number_of_paths; i++){

                if(option == 1) cout << endl << "Type in the Path: ";
                if(option == 2) cout << endl << "Type in Path " << i+1 <<": ";

                if (fgets(name, sizeof(name), stdin)) {
                    /* fgets succeeds, scan for newline character */
                    p = strchr(name, '\n');
                    if (p) {
                        if(name[0] == '\n') {
                                error = 6;
                                break;
                        }
                        else {
                            *p = '\0';

                            if (option == 1 && error == 0) {

                                item->input_path_name.at(0) = name;

                            }
                            if (option == 2 && error == 0) {

                                // Set the device paths
                                if(i < item->input_path_name.size()) item->input_path_name.at(i) = name;
                                else if(i >= item->input_path_name.size())  item->input_path_name.push_back(name);

                                // handle the vector size if actual input is shorter then the one before
                                if(i+1 == number_of_paths && item->input_path_name.size() > i+1 ){
                                        item->input_path_name.erase(item->input_path_name.begin() + i+1, item->input_path_name.end());
                                }
                            }
                        }
                    }
                    else {
                        /* newline not found, flush stdin to end of line */
                            while (((ch = getchar()) != '\n')
                                && !feof(stdin)
                                && !ferror(stdin)
                                );
                            error = 10;
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
            if(!error) break;
        }
    }
}
