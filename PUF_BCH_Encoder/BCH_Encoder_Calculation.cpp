/*
 * File:    bch3.c
 * Title:   Encoder/decoder for binary BCH codes in C (Version 3.1)
 * Author:  Robert Morelos-Zaragoza
 * Date:    August 1994
 * Revised: June 13, 1997
 *
 * ===============  Encoder/Decoder for binary BCH codes in C =================
 *
 * Version 1:   Original program. The user provides the generator polynomial
 *              of the code (cumbersome!).
 * Version 2:   Computes the generator polynomial of the code.
 * Version 3:   No need to input the coefficients of a primitive polynomial of
 *              degree m, used to construct the Galois Field GF(2**m). The
 *              program now works for any binary BCH code of length such that:
 *              2**(m-1) - 1 < length <= 2**m - 1
 *
 * Note:        You may have to change the size of the arrays to make it work.
 *
 * The encoding and decoding methods used in this program are based on the
 * book "Error Control Coding: Fundamentals and Applications", by Lin and
 * Costello, Prentice Hall, 1983.
 *
 * Thanks to Patrick Boyle (pboyle@era.com) for his observation that 'bch2.c'
 * did not work for lengths other than 2**m-1 which led to this new version.
 * Portions of this program are from 'rs.c', a Reed-Solomon encoder/decoder
 * in C, written by Simon Rockliff (simon@augean.ua.oz.au) on 21/9/89. The
 * previous version of the BCH encoder/decoder in C, 'bch2.c', was written by
 * Robert Morelos-Zaragoza (robert@spectra.eng.hawaii.edu) on 5/19/92.
 *
 * NOTE:
 *          The author is not responsible for any malfunctioning of
 *          this program, nor for any damage caused by it. Please include the
 *          original program along with these comments in any redistribution.
 *
 *  For more information, suggestions, or other ideas on implementing error
 *  correcting codes, please contact me at:
 *
 *                           Robert Morelos-Zaragoza
 *                           5120 Woodway, Suite 7036
 *                           Houston, Texas 77056
 *
 *                    email: r.morelos-zaragoza@ieee.org
 *
 * COPYRIGHT NOTICE: This computer program is free for non-commercial purposes.
 * You may implement this program for any non-commercial application. You may
 * also implement this program for commercial purposes, provided that you
 * obtain my written permission. Any modification of this program is covered
 * by this copyright.
 *
 * == Copyright (c) 1994-7,  Robert Morelos-Zaragoza. All rights reserved.  ==
 *
 * m = order of the Galois field GF(2**m)
 * n = 2**m - 1 = size of the multiplicative group of GF(2**m)
 * length = length of the BCH code
 * t = error correcting capability (max. no. of errors the code corrects)
 * d = 2*t + 1 = designed min. distance = no. of consecutive roots of g(x) + 1
 * k = n - deg(g(x)) = dimension (no. of information bits/codeword) of the code
 * p[] = coefficients of a primitive polynomial used to generate GF(2**m)
 * g[] = coefficients of the generator polynomial, g(x)
 * alpha_to [] = log table of GF(2**m)
 * index_of[] = antilog table of GF(2**m)
 * bdata[] = information bits = coefficients of data polynomial, i(x)
 * bb[] = coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)
 * numerr = number of errors
 * errpos[] = error positions
 * recd[] = coefficients of the received polynomial
 * decerror = number of decoding errors (in _message_ positions)
 *
 * =============================================================================
 *
 *  This is a BCH Encoder for PUF Responses to generate secure HelperData for
 *  the realization of a SecureKeyStorage.
 *
 *  The main BCH encoder and decoder code are from:
 *
 * File:    bch3.c
 * Title:   Encoder/decoder for binary BCH codes in C (Version 3.1)
 * Author:  Robert Morelos-Zaragoza
 * Date:    August 1994
 * Revised: June 13, 1997
 *
 *  The encoder and decoder functionality was fixed, due the base implementation
 *  was not running properly. Additional the functionality for the SecureKeyStorage
 *  and HelperData generation / reconstruction was added.
 *
 *  This code is written / modified by:      Sebastian Schurig
 */

#include "BCH_Encoder_Calculation.h"

/*
 * BCH-Encoder - Calculation
 *
 * Author: Sebastian Schurig
 *
 * Calculation Functions for the BCH-Encoder
 *
 */


void
read_p()
/*
 *	Read m, the degree of a primitive polynomial p(x) used to compute the
 *	Galois field GF(2**m). Get precomputed coefficients p[] of p(x). Read
 *	the code length.
 */
{
	int			    i, ninf;
	char            *h;
    unsigned int    ch;
    char            user_input[7];
    int             error;


	while(true){
        if (error != 0) ClearScreen();
        cout << "*******************************************************************************" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                  Set the relevant parameter to define the BCH mode:         *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*                   'm'  = such that the code length is in between            *" << endl;
		cout << "*                                                                             *" << endl;
        cout << "*                        (2^(m-1) - 1) < length <= (2^m - 1)                  *" << endl;
		cout << "*                                                                             *" << endl;
		cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, i, 0);
        error = 0;

        // Get "m" as user input
        cout << endl << "Enter m (between 4 and 14): ";
        if (fgets(user_input, sizeof(user_input), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(user_input, '\n');
                if (h) {
                    if(user_input[0] == '\n') error = 6;
                    else {
                        *h = '\0';
                        //check input if only digits are used
                        for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                            if(user_input[i] != '\0' && !isdigit(user_input[i])){
                                error = 1;
                                break;
                            }
                            if(user_input[i] == '\0') i = sizeof(user_input);
                        }
                        if(!error){
                            if(atoi(user_input) >= 4 && atoi(user_input) <= 14){
                                m = atoi(user_input);
                                break;
                            }
                            else error = 7;
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

    // Generate coefficients of a primitive polynomial used to generate GF(2**m)
    for (i=1; i<m; i++)
            p[i] = 0;

    p[0] = p[m] = 1;

    if 		(m == 4)	p[1] = 1;
    else if (m == 5)	p[2] = 1;
    else if (m == 6)	p[1] = 1;
    else if (m == 7)	p[1] = 1;
    else if (m == 8)	p[4] = p[5] = p[6] = 1;
    else if (m == 9)	p[4] = 1;
    else if (m == 10)	p[3] = 1;
    else if (m == 11)	p[2] = 1;
    else if (m == 12)	p[3] = p[4] = p[7] = 1;
    else if (m == 13)	p[1] = p[3] = p[4] = 1;
    else if (m == 14)	p[1] = p[11] = p[12] = 1;

    // Calculate 'n'
    n = 1;
    for (i = 0; i <= m; i++) {
        n *= 2;
    }

    // Calculate the supported BCH code length, limited by "m"
    n = n / 2 - 1;
    ninf = (n + 1) / 2 - 1;

    // Get the code length from user input
    while(true){
        ClearScreen();
        cout << "*******************************************************************************" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*             Set the relevant parameter to define the BCH mode:              *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*              length  = the length of each BCH encoded codeword              *" << endl;
        cout << "*                                                                             *" << endl;
        cout << "*******************************************************************************" << endl;

        if(error) ErrorMessages(error, i, n);
        error = 0;

        cout << endl << "Enter the code length between (" << ninf << " < length <= " << n << "): ";
        if (fgets(user_input, sizeof(user_input), stdin)) {
            /* fgets succeeds, scan for newline character */
            h = strchr(user_input, '\n');
            if (h) {
                if(user_input[0] == '\n') error = 6;
                else {
                    *h = '\0';
                    //check input if only digits are used
                    for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                        if(user_input[i] != '\0' && !isdigit(user_input[i])){
                            error = 1;
                            break;
                        }
                        if(user_input[i] == '\0') i = sizeof(user_input);
                    }
                    if(!error){
                        if(atoi(user_input) > ninf && atoi(user_input) <= n){
                            length = atoi(user_input);
                            break;
                        }
                        else {
                            error = 8;
                            i = ninf;
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
    }
}

void
generate_gf()
/*
 * Generate field GF(2**m) from the irreducible polynomial p(X) with
 * coefficients in p[0]..p[m].
 *
 * Lookup tables:
 *   index->polynomial form: alpha_to[] contains j=alpha^i;
 *   polynomial form -> index form:	index_of[j=alpha^i] = i
 *
 * alpha=2 is the primitive element of GF(2**m)
 */
{
	int    i, mask;


	mask = 1;
	alpha_to[m] = 0;
	for (i = 0; i < m; i++) {
		alpha_to[i] = mask;
		index_of[alpha_to[i]] = i;
		if (p[i] != 0)
			alpha_to[m] ^= mask;
		mask <<= 1;

	}
	index_of[alpha_to[m]] = m;
	mask >>= 1;
	for (i = m + 1; i < n; i++) {
		if (alpha_to[i - 1] >= mask)
		  alpha_to[i] = alpha_to[m] ^ ((alpha_to[i - 1] ^ mask) << 1);
		else
		  alpha_to[i] = alpha_to[i - 1] << 1;
		index_of[alpha_to[i]] = i;
	}
	index_of[0] = -1;
}

void
gen_poly()
/*
 * Compute the generator polynomial of a binary BCH code. Fist generate the
 * cycle sets modulo 2**m - 1, cycle[][] =  (i, 2*i, 4*i, ..., 2^l*i). Then
 * determine those cycle sets that contain integers in the set of (d-1)
 * consecutive integers {1..(d-1)}. The generator polynomial is calculated
 * as the product of linear factors of the form (x+alpha^i), for every i in
 * the above cycle sets.
 */
{
	int	ii, jj, ll, kaux, zero;
	int	test, aux, nocycles, root, noterms, rdncy;
	int      cycle[8192][21];
	std::vector<int> size(8192);
    std::vector<int> min(2048);
    std::vector<int> zeros(16385);
    char        *h;
    unsigned int ch;
    char user_input[6];
    int error = 0, i = 0, done = 0;
    char processing[3] = {' ',' ','\0'};
    double max_error = 0;


	/* Generate cycle sets modulo n, n = 2**m - 1 */
	cycle[0][0] = 0;
	size[0] = 1;
	cycle[1][0] = 1;
	size[1] = 1;
	jj = 1;			/* cycle set index */

	if (m > 9)  {

		printf("\nComputing cycle sets modulo %d\n", n);
		printf("(This may take some time)...\n\n");
	}
	do {
		/* Generate the jj-th cycle set */
		ii = 0;
		do {
			ii++;
			cycle[jj][ii] = (cycle[jj][ii - 1] * 2) % n;
			size[jj]++;
			aux = (cycle[jj][ii] * 2) % n;
		} while (aux != cycle[jj][0]);

		/* Next cycle set representative */
		ll = 0;
		do {
			ll++;
			test = 0;
			for (ii = 1; ((ii <= jj) && (!test)); ii++)
			/* Examine previous cycle sets */
			  for (kaux = 0; ((kaux < size[ii]) && (!test)); kaux++)
			     if (ll == cycle[ii][kaux])
			        test = 1;
		} while ((test) && (ll < (n - 1)));
		if (!(test)) {
			jj++;	/* next cycle set index */
			cycle[jj][0] = ll;
			size[jj] = 1;
		}

    zero = ((n-1)/100);

    // Status progress
    if(zero != 0 && (ll/zero) <= 100) cout << "\r" << setw(-20) << StatusProgress((ll/zero)) << " " << (ll/zero) << "% completed." << processing << flush;
        else if(zero != 0 && (ll/zero) >= 100) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;

    if(processing[0] == ' ' && processing[1] == ' '){
            processing[0] = '.';
    }
    else if(processing[0] == '.' && processing[1] == ' '){
        processing[1] = '.';
    }
    else if(processing[0] == '.' && processing[1] == '.') {
       processing[0] = ' ';
       processing[1] = ' ';
    }

	} while (ll < (n - 1));
	nocycles = jj;		/* number of cycle sets modulo n */

	while(!done){
        while(true){
            ClearScreen();
            cout << "*******************************************************************************" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*             Set the relevant parameter to define the BCH mode:              *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*    't'  = the error correcting capability for each BCH encoded codeword     *" << endl;
            cout << "*                                                                             *" << endl;
            cout << "*******************************************************************************" << endl;

            if(error) ErrorMessages(error, i, (int)max_error);
            error = 0;

            // Get "t" as user input
            cout << endl << "Enter the error correcting capability, t: ";
            if (fgets(user_input, sizeof(user_input), stdin)) {
                /* fgets succeeds, scan for newline character */
                h = strchr(user_input, '\n');
                if (h) {
                    if(user_input[0] == '\n') error = 6;
                    else {
                        *h = '\0';
                        //check input if only digits are used
                        for(i = 0; i < (signed)sizeof(user_input)-1; i++){
                            if(user_input[i] != '\0' && !isdigit(user_input[i])){
                                error = 1;
                                break;
                            }
                            if(user_input[i] == '\0') i = sizeof(user_input);
                        }
                        if(!error){
                            t = atoi(user_input);
                            break;
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

        d = 2 * t + 1;

        /* Search for roots 1, 2, ..., d-1 in cycle sets */
        kaux = 0;
        rdncy = 0;
        max_error = 0;
        for (ii = 1; ii <= nocycles; ii++) {
            min[kaux] = 0;
            test = 0;
            for (jj = 0; ((jj < size[ii]) && (!test)); jj++)
                for (root = 1; ((root < d) && (!test)); root++)
                    if (root == cycle[ii][jj])  {
                        test = 1;
                        min[kaux] = ii;
                    }
            if (min[kaux]) {
                rdncy += size[min[kaux]];
                kaux++;
            }
        }
        noterms = kaux;
        kaux = 1;
        for (ii = 0; ii < noterms; ii++)
            for (jj = 0; jj < size[min[ii]]; jj++) {
                zeros[kaux] = cycle[min[ii]][jj];
                kaux++;
            }

        k = length - rdncy;

        if (k<2){
             i = (int)(exp2((double(m))) - double(1));
             max_error = exp2((double(m-2))) - double(1);
             error = 9;
        }
        else done = 1;
    }

	/* Compute the generator polynomial */
	g[0] = alpha_to[zeros[1]];
	g[1] = 1;		/* g(x) = (X + zeros[1]) initially */
	for (ii = 2; ii <= rdncy; ii++) {
	  g[ii] = 1;
	  for (jj = ii - 1; jj > 0; jj--)
	    if (g[jj] != 0)
	      g[jj] = g[jj - 1] ^ alpha_to[(index_of[g[jj]] + zeros[ii]) % n];
	    else
	      g[jj] = g[jj - 1];
	  g[0] = alpha_to[(index_of[g[0]] + zeros[ii]) % n];
	}
}

void
encode_bch()
/*
 * Compute redundacy bb[], the coefficients of b(x). The redundancy
 * polynomial b(x) is the remainder after dividing x^(length-k)*data(x)
 * by the generator polynomial g(x).
 */
{
	int    i, j;
	int    feedback;


	for (i = 0; i < length - k; i++)
		bb[i] = 0;
	for (i = k - 1; i >= 0; i--) {
		feedback = bdata[i] ^ bb[length - k - 1];
		if (feedback != 0) {
			for (j = length - k - 1; j > 0; j--)
				if (g[j] != 0)
					bb[j] = bb[j - 1] ^ feedback;
				else
					bb[j] = bb[j - 1];
			bb[0] = g[0] && feedback;
		} else {
			for (j = length - k - 1; j > 0; j--)
				bb[j] = bb[j - 1];
			bb[0] = 0;
		}
	}
}

int
Calculation(struct Item *item)
/*
 * Function to handle the actual calculation:
 *
 * -> Generate BCH encoded codewords.
 * -> Perform the linear repetition.
 * -> XOR codewords with the PUF Response.
 * -> Store the helper-data into a file.
 * -> Append the Pattern to the end of the file
 *
 * Inputs:
 * item  = pointer to the struct to get/store the necessary informations
 */
{
	FILE *fd;
    long puf_filesize;
    struct Pattern infos;
	int    i = 0, j = 0, z = 0;
	int    len = 0, u = 0, encode_binary_length = 0;
    char processing[3] = {' ',' ','\0'};


	// Check if PUF file is a file
    if(!IsFile(item->input_PUF_name)){
        return 12;
    }

	// Get the file size in binary int representation
    u = (item->input_Key_length*8);

    // Check if the file size is a multiplicative to the chosen factor 'k' of the BCH mode
    if( u%k != 0){
        // If it is not a multiplicative to the chosen factor 'k' of the BCH mode, determine the new length
        len = (((item->input_Key_length*8)/k)+1)*k;
    }
    else {
        // If it is a multiplicative to the chosen factor 'k' of the BCH mode use it
        len = (item->input_Key_length*8);
    }

    // Check if the encoded key is byte block aligned
    if (((len/k)*length)%8 != 0){
        encode_binary_length = (((len/k)*length) / 8)+1;
    }
    else {
        encode_binary_length = ((len/k)*length) / 8;
    }

    // Responsible for correct status progress visualization
    u = (len-1) / 100;

    cout << endl << "Calculating BCH Encoding:" << endl << "(This may take some time)..." << endl << endl;

    // Process the actual BCH encoding for the complete Key file
    for(j = 0; j < len ; j= j+k){

        // Fill the data vector with the input values
        for(i = 0; i < k; i++){
            if( i + j < (signed)input.size()){
                bdata[i] = input[i+j];
            }
            else {
                bdata[i] = 0;
            }
        }

        encode_bch();           /* encode data block wise */

		// Take the calculated redundancy and store it in the recd vector
        for (i = 0; i < length - k; i++){
            recd[i] = bb[i];
        }

		// Take the corresponding original input and store it in the recd vector at the corresponding position
        for (i = 0; i < k; i++){
            recd[i + (length - k)] = bdata[i];
        }

        // Store the BCH codewords in the output vector
        for (i = 0; i < length; i++){
            output.push_back(recd[i]);
        }

        // Increment the iteration counter
        z = z + 1;

        // Processing status visualization of the BCH encoding process
        if ((j+k) >= len) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;
        else cout << "\r" << setw(-20) << StatusProgress((j/u)) << " " << (j/u) << "% completed." << processing << flush;

        // Visualization of the working process
        if(processing[0] == ' ' && processing[1] == ' ') processing[0] = '.';
        else if(processing[0] == '.' && processing[1] == ' ') processing[1] = '.';
        else if(processing[0] == '.' && processing[1] == '.') {
           processing[0] = ' ';
           processing[1] = ' ';
        }
	}

	// Allocate space for the LR encoded BCH code words, the binary encoded codeword representation and the PUF Response
	unsigned char *codeWordArrayBeforLR = (unsigned char *) malloc(sizeof(unsigned char) * encode_binary_length);
	unsigned char *encodeLR = (unsigned char *) malloc(sizeof(unsigned char) * (item->LR*encode_binary_length));
	unsigned char *puf = (unsigned char *) malloc(sizeof(unsigned char) * (item->LR*encode_binary_length));

    // Try to open the PUF file
    if ((fd = fopen(item->input_PUF_name, "rb")) == NULL) {
		fclose(fd);
        return 11;
    }

    // Get the file size
    fseek(fd, 0, SEEK_END);
    puf_filesize = ftell(fd);
    rewind(fd);

    // Go to the offset position in the file
    fseek(fd, item->offSet, SEEK_SET);

    // Check if the chosen part of the PUF-Response is valid
    if((item->LR*encode_binary_length) > puf_filesize) {
            fclose(fd);
            return 13;
    }

    // Read the PUF Response into the allocated space puf[]
    if ((signed)fread(&puf[0], sizeof(char), (item->LR*encode_binary_length), fd) != (item->LR*encode_binary_length)) {
        fclose(fd);
        return 14;
    }

    // Close PUF file
    fclose(fd);

	// Generate from the binary bit stream (a integer vector) each byte block for the encoded codewords
	for(j = 0; j < encode_binary_length; j++){
        codeWordArrayBeforLR[j] = 0;
        if(7+(j * 8) < (signed) output.size()){
            if(output.at(7+(j*8)) ==  1) codeWordArrayBeforLR[j]^=1;
            if(output.at(7+(j*8)-1) == 1) codeWordArrayBeforLR[j]^=2;
            if(output.at(7+(j*8)-2) == 1) codeWordArrayBeforLR[j]^=4;
            if(output.at(7+(j*8)-3) == 1) codeWordArrayBeforLR[j]^=8;
            if(output.at(7+(j*8)-4) == 1) codeWordArrayBeforLR[j]^=16;
            if(output.at(7+(j*8)-5) == 1) codeWordArrayBeforLR[j]^=32;
            if(output.at(7+(j*8)-6) == 1) codeWordArrayBeforLR[j]^=64;
            if(output.at(7+(j*8)-7) == 1) codeWordArrayBeforLR[j]^=128;
        }
	}

	// Cleanup - erase the input/output vectors
	input.erase(input.begin(), input.begin() + input.size());
	output.erase(output.begin(), output.begin() + output.size());

	i = 0;

	// Try to open the output file for the helperdata
	if ((fd = fopen(item->output_HD_name, "wb")) == NULL) {
		fclose(fd);
        return 11;
    }

	// Perform Linear Repetition and XOR with PUF Response
    if(item->LR == 7){
        while( i < encode_binary_length){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                       // MSB (bit at position 8 == 0)
                encodeLR[(i*item->LR)] = 0x00;
            }
            else {                                                              // MSB (bit at position 8 == 1)
                encodeLR[(i*item->LR)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                       // (bit at position 7 == 0)
                encodeLR[(i*item->LR)+1] = 0x00;
            } else {                                                            // (bit at position 7 == 1)
                encodeLR[(i*item->LR)] = encodeLR[(i*item->LR)] ^ 0x01;
                encodeLR[(i*item->LR)+1] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                       // (bit at position 6 == 0)
                encodeLR[(i*item->LR)+2] = 0x00;
            } else {                                                            // (bit at position 6 == 1)
                encodeLR[(i*item->LR)+1] = encodeLR[(i*item->LR)+1] ^ 0x03;
                encodeLR[(i*item->LR)+2] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                       // (bit at position 5 == 0)
                encodeLR[(i*item->LR)+3] = 0x00;
            } else {                                                            // (bit at position 5 == 1)
                encodeLR[(i*item->LR)+2] = encodeLR[(i*item->LR)+2] ^ 0x07;
                encodeLR[(i*item->LR)+3] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                       // (bit at position 4 == 0)
                encodeLR[(i*item->LR)+4] = 0x00;
            } else {                                                            // (bit at position 4 == 1)
                encodeLR[(i*item->LR)+3] = encodeLR[(i*item->LR)+3] ^ 0x0F;
                encodeLR[(i*item->LR)+4] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                       // (bit at position 3 == 0)
                encodeLR[(i*item->LR)+5] = 0x00;
            } else {                                                            // (bit at position 3 == 1)
                encodeLR[(i*item->LR)+4] = encodeLR[(i*item->LR)+4] ^ 0x1F;
                encodeLR[(i*item->LR)+5] = 0xC0;
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                       // (bit at position 2 == 0)
                encodeLR[(i*item->LR)+6] = 0x00;
            } else {                                                            // (bit at position 2 == 1)
                encodeLR[(i*item->LR)+5] = encodeLR[(i*item->LR)+5] ^ 0x3F;
                encodeLR[(i*item->LR)+6] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                       // (bit at position 1 == 0)
                encodeLR[(i*item->LR)+6] = encodeLR[(i*item->LR)+6] ^ 0x00;
            } else {                                                            // (bit at position 1 == 1
                encodeLR[(i*item->LR)+6] = encodeLR[(i*item->LR)+6] ^ 0x7F;
            }
            encodeLR[(i*item->LR)] = (encodeLR[(i*item->LR)] ^ puf[(i*item->LR)]);
            encodeLR[(i*item->LR)+1] = (encodeLR[(i*item->LR)+1] ^ puf[(i*item->LR)+1]);
            encodeLR[(i*item->LR)+2] = (encodeLR[(i*item->LR)+2] ^ puf[(i*item->LR)+2]);
            encodeLR[(i*item->LR)+3] = (encodeLR[(i*item->LR)+3] ^ puf[(i*item->LR)+3]);
            encodeLR[(i*item->LR)+4] = (encodeLR[(i*item->LR)+4] ^ puf[(i*item->LR)+4]);
            encodeLR[(i*item->LR)+5] = (encodeLR[(i*item->LR)+5] ^ puf[(i*item->LR)+5]);
            encodeLR[(i*item->LR)+6] = (encodeLR[(i*item->LR)+6] ^ puf[(i*item->LR)+6]);

            // Write each byte into the output file
            for(j = 0; j < item->LR; j++){
               fwrite(&encodeLR[(i*item->LR)+j], sizeof(char), 1, fd);
            }

            i++;
        }
    }
    else if(item->LR == 15){
		while( i < encode_binary_length){
            if((codeWordArrayBeforLR[i] & 0x80) == 0x00){                        // MSB (bit at position 8 == 0)
                encodeLR[(i*item->LR)] = 0x00;
                encodeLR[((i*item->LR)+1)] = 0x00;
            }
            else {                                                               // MSB (bit at position 8 == 1)
                encodeLR[(i*item->LR)] = 0xFF;
                encodeLR[((i*item->LR)+1)] = 0xFE;
            }
            if((codeWordArrayBeforLR[i] & 0x40) == 0x00){                        // (bit at position 7 == 0)
                encodeLR[((i*item->LR)+2)] = 0x00;
                encodeLR[((i*item->LR)+3)] = 0x00;
            } else {                                                             // (bit at position 7 == 1)
                encodeLR[((i*item->LR)+1)] = encodeLR[((i*item->LR)+1)] ^ 0x01;
                encodeLR[((i*item->LR)+2)] = 0xFF;
                encodeLR[((i*item->LR)+3)] = 0xFC;
            }
            if((codeWordArrayBeforLR[i] & 0x20) == 0x00){                        // (bit at position 6 == 0)
                encodeLR[((i*item->LR)+4)] = 0x00;
                encodeLR[((i*item->LR)+5)] = 0x00;
            } else {                                                             // (bit at position 6 == 1)
                encodeLR[((i*item->LR)+3)] = encodeLR[((i*item->LR)+3)] ^ 0x03;
                encodeLR[((i*item->LR)+4)] = 0xFF;
                encodeLR[((i*item->LR)+5)] = 0xF8;
            }
            if((codeWordArrayBeforLR[i] & 0x10) == 0x00){                        // (bit at position 5 == 0)
                encodeLR[((i*item->LR)+6)] = 0x00;
                encodeLR[((i*item->LR)+7)] = 0x00;
            } else {                                                             // (bit at position 5 == 1)
                encodeLR[((i*item->LR)+5)] = encodeLR[((i*item->LR)+5)] ^ 0x07;
                encodeLR[((i*item->LR)+6)] = 0xFF;
                encodeLR[((i*item->LR)+7)] = 0xF0;
            }
            if((codeWordArrayBeforLR[i] & 0x08) == 0x00){                        // (bit at position 4 == 0)
                encodeLR[((i*item->LR)+8)] = 0x00;
                encodeLR[((i*item->LR)+9)] = 0x00;
            } else {                                                             // (bit at position 4 == 1)
                encodeLR[((i*item->LR)+7)] = encodeLR[((i*item->LR)+7)] ^ 0x0F;
                encodeLR[((i*item->LR)+8)] = 0xFF;
                encodeLR[((i*item->LR)+9)] = 0xE0;
            }
            if((codeWordArrayBeforLR[i] & 0x04) == 0x00){                        // (bit at position 3 == 0)
                encodeLR[((i*item->LR)+10)] = 0x00;
                encodeLR[((i*item->LR)+11)] = 0x00;
            } else {                                                             // (bit at position 3 == 1)
                encodeLR[((i*item->LR)+9)] = encodeLR[((i*item->LR)+9)] ^ 0x1F;
                encodeLR[((i*item->LR)+10)] = 0xFF;
                encodeLR[((i*item->LR)+11)] = 0xC0;
            }
            if((codeWordArrayBeforLR[i] & 0x02) == 0x00){                        // (bit at position 2 == 0)
                encodeLR[((i*item->LR)+12)] = 0x00;
                encodeLR[((i*item->LR)+13)] = 0x00;
            } else {                                                           	 // (bit at position 2 == 1)
                encodeLR[((i*item->LR)+11)] = encodeLR[((i*item->LR)+11)] ^ 0x3F;
                encodeLR[((i*item->LR)+12)] = 0xFF;
                encodeLR[((i*item->LR)+13)] = 0x80;
            }
            if((codeWordArrayBeforLR[i] & 0x01) == 0x00){                        // (bit at position 1 == 0)
                encodeLR[((i*item->LR)+13)] = encodeLR[((i*item->LR)+13)] ^ 0x00;
                encodeLR[((i*item->LR)+14)] = 0x00;
            } else {                                                             // (bit at position 1 == 1
                encodeLR[((i*item->LR)+13)] = encodeLR[((i*item->LR)+13)] ^ 0x7F;
                encodeLR[((i*item->LR)+14)] = 0xFF;
            }

            encodeLR[(i*item->LR)] = (encodeLR[(i*item->LR)] ^ puf[(i*item->LR)]);
            encodeLR[(i*item->LR)+1] = (encodeLR[(i*item->LR)+1] ^ puf[(i*item->LR)+1]);
            encodeLR[(i*item->LR)+2] = (encodeLR[(i*item->LR)+2] ^ puf[(i*item->LR)+2]);
            encodeLR[(i*item->LR)+3] = (encodeLR[(i*item->LR)+3] ^ puf[(i*item->LR)+3]);
            encodeLR[(i*item->LR)+4] = (encodeLR[(i*item->LR)+4] ^ puf[(i*item->LR)+4]);
            encodeLR[(i*item->LR)+5] = (encodeLR[(i*item->LR)+5] ^ puf[(i*item->LR)+5]);
            encodeLR[(i*item->LR)+6] = (encodeLR[(i*item->LR)+6] ^ puf[(i*item->LR)+6]);
            encodeLR[(i*item->LR)+7] = (encodeLR[(i*item->LR)+7] ^ puf[(i*item->LR)+7]);
            encodeLR[(i*item->LR)+8] = (encodeLR[(i*item->LR)+8] ^ puf[(i*item->LR)+8]);
            encodeLR[(i*item->LR)+9] = (encodeLR[(i*item->LR)+9] ^ puf[(i*item->LR)+9]);
            encodeLR[(i*item->LR)+10] = (encodeLR[(i*item->LR)+10] ^ puf[(i*item->LR)+10]);
            encodeLR[(i*item->LR)+11] = (encodeLR[(i*item->LR)+11] ^ puf[(i*item->LR)+11]);
            encodeLR[(i*item->LR)+12] = (encodeLR[(i*item->LR)+12] ^ puf[(i*item->LR)+12]);
            encodeLR[(i*item->LR)+13] = (encodeLR[(i*item->LR)+13] ^ puf[(i*item->LR)+13]);
            encodeLR[(i*item->LR)+14] = (encodeLR[(i*item->LR)+14] ^ puf[(i*item->LR)+14]);

            // Write each byte into the output file
            for(j = 0; j < item->LR; j++){
               fwrite(&encodeLR[(i*item->LR)+j], sizeof(char), 1, fd);
            }

			i++;
        }
    }

    // Prepare the pattern with essential informations for the decoding
    infos.errorCode = 1;
    infos.Golay_k_BCH_m = m;
    infos.Golay_BCH_length = length;
    infos.Golay_d_BCH_t = t;
    infos.linearRep = item->LR;
    infos.puf_offSet = item->offSet;
    infos.original_filesize = filesize;

    // Add the pattern to the output file
    fwrite(&infos, sizeof(infos), 1, fd);

    // Cleanup
    fclose(fd);
    free(puf);
    free(encodeLR);
    free(codeWordArrayBeforLR);

	return 0;
}
