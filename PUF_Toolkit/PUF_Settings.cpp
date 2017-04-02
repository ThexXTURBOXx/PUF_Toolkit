#include "PUF_Settings.h"

/*
 * PUF-ToolKit - Settings
 *
 * Author: Sebastian Schurig
 *
 * Setting Functions for the PUF-ToolKit
 *
 */

unsigned int SetInputLen(struct Item *item)
/*
 * Function to calculate the input_length based on the offsets
 * make sure the offsets and it1->input_file_name are initialized before calling
 * this function.
 */
{
    FILE *fd;
    unsigned long filesize;
	// Open the specified file
    if((fd = fopen(item->input_file_name, "rb")) == NULL) return 12;

    // Get the filesize
    fseek(fd, 0, SEEK_END);
    filesize = ftell(fd);
    rewind(fd);

    // Go to the offset_begin position in the file
    fseek(fd, item->offset_begin, SEEK_SET);

    //check if both offset_begin dont exceed the filesize
    do {
        if ((item->offset_begin + item->offset_end) > filesize) {
            printf("offset_begin and offset_end exceeds filesize, set them again\n");
            DefineOffSetLength(item);
        }
    } while ((item->offset_begin + item->offset_end) > filesize);
    //set the length to be read as the filesize - sum of offsets
    item->input_length = filesize - item->offset_begin - item->offset_end;
    printf("input_length : %lu\n", item->input_length);
    // Check if the chosen part of the PUF-Response is valid
    if((item->offset_begin+item->input_length) > filesize) return 13;

    fclose(fd);
    return 0;
}

void DefineMode(struct Item *it1)
/*
 * Function to set the mode to 0 - file or 1 - directory
 */
{
    char oSet[12];
    char *p;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    while (true) {
    	if(error) ErrorMessages(error, i);
    	error = 0;
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
    					// Set the mode
    					ch = atol(oSet);
                        if (ch != 0 && ch != 1)
                            error = 1;
                        else
                            it1->HW_ENTP_mode = ch;
                        printf("mode set to: %u\n", it1->HW_ENTP_mode);
    				}
    
    			} else {
    				/* newline not found, flush stdin to end of line */
    				while (((ch = getchar()) != '\n')
    					&& !feof(stdin)
    					&& !ferror(stdin)
    				);
    				error = 2;
    		    }
        } else {
    		/* fgets failed, handle error */
    		cin.clear();
    		error = 3;
    	}
        if (!error) break;
    }
}

void DefineOffSetLength(struct Item *it1)
/*
 * Function to get the 'offset_begin' and 'length' as user input
 *
 * Inputs:
 * item = pointer to the struct to store the necessary informations
 *
 * offset_begin = the amount of bytes (in decimal) that will be script from the beginning
 * Length = the mount of bytes that will be used (in decimal)
 */
{
    char oSet[12];
    //char iLength[12];
    char *p;
    unsigned int ch, i = 0;
    unsigned int error = 0;

    while(true){
		if (error != 0) ClearScreen();
		cout << "*******************************************************************************" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                  Define the 'offsets' for the PUF-Response:                 *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                   0  = use complete binary PUF-Response                     *" << endl;
		cout << "*                   1  = the first byte will be skipped                       *" << endl;
		cout << "*                  'x' = 'x' bytes from both ends of file will be skipped     *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*******************************************************************************" << endl;

		if(error) ErrorMessages(error, i);
		error = 0;

		cout << endl << "Type in the offset from beginning of file in bytes (as decimal number): ";
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

						// Set the offset_begin
						it1->offset_begin = atol(oSet);

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
        cout << endl << "Type in the offset from end of the file in bytes (as decimal number): ";
        if (fgets(oSet, sizeof(oSet), stdin)) {
            /*fgets succeds scan for newline character*/
            p = strchr(oSet, '\n');
            if (p) {
                *p = '\0';

                //check input if only digits are allowed
                for (i = 0; i < sizeof(oSet) - 1; i++) {
                    if (oSet[i] != '\0' && !isdigit(oSet[i])) {
                        error = 1;
                        break;
                    }
                    if (oSet[i] == '\0') i = sizeof(oSet);
                }
                if (error == 0)
                    it1->offset_end = atol(oSet);
            }
            else {
                /*newline char not found*/
                while (((ch = getchar()) != '\n') && !feof(stdin) &&
                        !ferror(stdin));
                error = 2;
            }
        }
        else {
            /*fgets failed, handle error*/
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
 * 4 = output key filename
 * 5 = input HD filename
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

/*
 * BCH-Encoder - Settings
 *
 * Author: Sebastian Schurig
 *
 * Settings Functions for the BCH-Encoder
 *
 */


void DefineFilename_BCH(struct Item *item, int option)
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
 * 4 = output Key filename 
 * 5 = input HD filename
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
        if(option == 1)
        cout << "*          Set the 'File-Name' of the file in which the Key is stored         *" << endl;
        if(option == 2){
        cout << "*                   Set the 'File-Name' of the PUF-Response                   *" << endl;}
        if(option == 3) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData will be stored    *" << endl;}
        if(option == 4) {
        cout << "*   Set the 'File-Name' of the file in which the decoded Key will be stored   *" << endl;}
        if(option == 5) {
        cout << "*   Set the 'File-Name' of the file in which the HelperData is stored         *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is in the current working directory:                            *" << endl;}
        if(option == 3 || option == 4){
        cout << "* If the file should be stored in the current working directory:              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 2) {
        cout << "*  -> Use only the filename               = Stellaris_PUF1                    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        if(option == 4) {
        cout << "*  -> Use only the filename               = key_file.txt                      *" << endl;}
        if(option == 5) {
        cout << "*  -> Use only the filename               = HelperData_1                      *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is in an sub-folder of the current working directory:           *" << endl;}
        if(option == 3 || option == 4) {
        cout << "* If the file should be stored in an sub-folder of the current directory:     *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the relative path and filename  = device1/Stellaris_PUF1            *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        if(option == 4) {
        cout << "*  -> Use the relative path and filename  = device1/key_file.txt              *" << endl;}
        if(option == 5) {
        cout << "*  -> Use the relative path and filename  = device1/HelperData_1              *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1 || option == 2 || option == 5) {
        cout << "* If the file is not in an sub-folder of the current working directory:       *" << endl;}
        if(option == 3 || option == 4) {
        cout << "* If the file should be stored somewhere else:                                *" << endl;}
        cout << "*                                                                             *" << endl;
        if(option == 1) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/key_file.txt *" << endl;}
        if(option == 2) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/Stellaris_PUF1    *" << endl;}
        if(option == 3) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/HelperData_1      *" << endl;}
        if(option == 4) {
        cout << "*  -> Use the full path and filename      = C:/User/Data/device1/key_file.txt *" << endl;}
        if(option == 5) {
        cout << "*  -> Use the full path and filename      = C:/Data/device1/HelperData_1      *" << endl;}
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) {
            ErrorMessages(error, 0, 0);
            ErrorMessages_decode(error, 0);
        }
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
                            else if (option == 4) strcpy (item->output_Key_name,name);
                            else if (option == 5) strcpy (item->input_HD_name,name);
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
