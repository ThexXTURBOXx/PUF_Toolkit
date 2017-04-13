#include "PUF_File.h"

/*
 * PUF-ToolKit - File
 *
 * Author: Sebastian Schurig
 *
 * File-System Functions for the PUF-ToolKit
 *
 */


bool IsFile(const char* input)
/*
 * Check if the input is a file */
{
    struct stat test;
    stat(input, &test);
    return S_ISREG(test.st_mode);
}

bool IsDir(const char* input)
/*
 * Check if the input is a directory */
{
    struct stat test;
    stat(input, &test);
    return S_ISDIR(test.st_mode);
}

int ViewFile(struct Item *item, int option)
/*
 * View a text file */
{
    string text;
    ifstream view;
    char buf[2];
    unsigned int ch;
    char *p;

    view.open(item->output_file_name);

    if(view.fail()) return 11;

		// Output of the file content
        while(!view.eof())
        {
            getline(view,text);
            cout << "" << text << endl;
        }

    view.close();

	// Show output until 'return' is pressed, workaround to skip all other input
    cout << "To continue press 'return'";
    if (fgets(buf, sizeof(buf), stdin)) {
        /* fgets succeeds, scan for newline character */
        p = strchr(buf, '\n');
        if (p) {
        *p = '\0';
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

int SaveFile(struct Item *item, int option)
/*
 * Save the result of the calculation into a text file
 *
 * Inputs:
 * item = pointer to the struct with necessary informations
 * option = definition of the output format
 *
 * Options:
 * 1 = output for the Hamming Weight
 * 2 = output for the (Shannon) Entropy
 * 3 = output for the Min-Entropy (CTW)
 */
{
    ofstream file;

    DefineFilename(item, 2);

    file.open (item->output_file_name);

    if(file.fail()) return 11;

    if (file.is_open())
      {
                        file << "*******************************************************************************" << endl;
                        file << "                                                                               " << endl;
        if(option == 1) file << "                                 Hamming Weight                                " << endl;
        if(option == 2) file << "                               (Shannon) Entropy                               " << endl;
        if(option == 3) file << "                               Min-Entropy (CTW)                               " << endl;
                        file << "                                                                               " << endl;
                        file << "                          Used Settings:                                       " << endl;
                        file << "                                                                               " << endl;
        if(option == 3) file << "                          Input-Path:     '" << item->input_path_name.at(0).c_str() <<"'"      << endl;
        else            file << "                          Input file:     '" << item->input_file_name <<"'"      << endl;
                        file << "                          OffSet:         " << item->offset_begin                      << endl;
                        file << "                          Length:         " << item->input_length << " byte (" << item->input_length*8 << " bit)" << endl;
                        file << "                                                                               " << endl;
        if(option == 1) file << "                          Result:         " << item->ones                        << endl;
        if(option == 2) file << "                          Result:         " << item->entrp                       << endl;
        if(option == 3){file << "                          Result:                                              " << endl;
                        file << "                          Min-Entropy (CTW)      = " << item->entrp              << endl;
                        file << "                          Fractional Min-Entropy = " << ((item->entrp) / double(item->input_length*8)) << " (" << (((item->entrp) / double(item->input_length*8)) * 100) << "%)" << endl;}
                        file << "                                                                               " << endl;
                        file << "*******************************************************************************" << endl << endl;
        file.close();
      }
    else return 14;

    file.close();

	return 0;
 }


/*
 * BCH-Encoder - File
 *
 * Author: Sebastian Schurig
 *
 * File-System Functions for the BCH-Encoder
 *
 */


int ReadKeyFile(struct Item *item)
/*
 * read in the Key file */
{
    FILE *fd;
    unsigned char hex;
    unsigned char * sramData;
    int j = 0;
    int bit0 = 0,  bit1 = 0,  bit2 = 0,  bit3 = 0,  bit4 = 0,  bit5 = 0,  bit6 = 0,  bit7 = 0;


    // Check if Input file is a file
    if(!IsFile(item->input_Key_name)){
        return 10;
    }

	// Try to open the Key file
    if ((fd = fopen(item->input_Key_name, "rb")) == NULL) {
		fclose(fd);
        return 11;
    }

    // Get the file size
    fseek(fd, 0, SEEK_END);
    filesize = ftell(fd);
    rewind(fd);

    // Store the original file size
    item->input_Key_length = filesize;

    // Allocate space for the Key to read it in
    sramData = (unsigned char *) malloc(sizeof(char) * filesize);

    if ((signed)fread(&sramData[0], sizeof(char), filesize, fd) != filesize) {
		fclose(fd);
        return 14;
    }

    // Generate a binary bit stream (for a integer array) out of each byte block of the input file and store it in the data vector
    for(j = 0; j < filesize; j++){
        hex = sramData[j];

        // Get the binary value for each bit position of an byte and insert it in the correct order into the input vector
        bit7 = hex&1;
        hex >>= 1;
        bit6 = hex&1;
        hex >>= 1;
        bit5 = hex&1;
        hex >>= 1;
        bit4 = hex&1;
        hex >>= 1;
        bit3 = hex&1;
        hex >>= 1;
        bit2 = hex&1;
        hex >>= 1;
        bit1 = hex&1;
        hex >>= 1;
        bit0 = hex&1;
        input.push_back(bit0);
        input.push_back(bit1);
        input.push_back(bit2);
        input.push_back(bit3);
        input.push_back(bit4);
        input.push_back(bit5);
        input.push_back(bit6);
        input.push_back(bit7);
    }

    // Cleanup
    free(sramData);
    fclose(fd);

    return 0;
}

//decode savefile
int SaveFile_bch(struct Item *item)
/*
 * Save in the data vector information into a file */
{
    FILE *fd;
    unsigned char * sData;
    int j = 0;


	if ((fd = fopen(item->output_Key_name, "wb")) == NULL) {
        fclose(fd);
        return 4;
    }

    // Allocate space for the output
    sData = (unsigned char *) malloc(sizeof(char) * filesize);

	// Generate from the binary bit stream (a integer array) each byte block for the output file
	for(j = 0; j < filesize; j++){
        sData[j] = 0;
        if(7+(j * 8) < (signed) output.size()){
            if(output[7+(j*8)] ==  1) sData[j]^=1;
            if(output[7+(j*8)-1] == 1) sData[j]^=2;
            if(output[7+(j*8)-2] == 1) sData[j]^=4;
            if(output[7+(j*8)-3] == 1) sData[j]^=8;
            if(output[7+(j*8)-4] == 1) sData[j]^=16;
            if(output[7+(j*8)-5] == 1) sData[j]^=32;
            if(output[7+(j*8)-6] == 1) sData[j]^=64;
            if(output[7+(j*8)-7] == 1) sData[j]^=128;
        }
    }

    // Write the recovered data into a file
    fwrite(&sData[0], sizeof(char), filesize, fd);
    fclose(fd);
    free(sData);

    return 0;
}
