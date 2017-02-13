#include "BCH_Encoder_File.h"

/*
 * BCH-Encoder - File
 *
 * Author: Sebastian Schurig
 *
 * File-System Functions for the BCH-Encoder
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

int
ReadKeyFile(struct Item *item)
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
