#include "BCH_Decoder_File.h"

/*
 * BCH-Decoder - File
 *
 * Author: Sebastian Schurig
 *
 * File-System Functions for the BCH-Decoder
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
SaveFile(struct Item *item)
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
