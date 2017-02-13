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
 * data[] = information bits = coefficients of data polynomial, i(x)
 * bb[] = coefficients of redundancy polynomial x^(length-k) i(x) modulo g(x)
 * numerr = number of errors
 * errpos[] = error positions
 * recd[] = coefficients of the received polynomial
 * decerror = number of decoding errors (in _message_ positions)
 *
 * =============================================================================
 *
 *  This is a BCH Decoder for PUF Responses to recover data from HelperData for
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

#include "BCH_Decoder_Calculation.h"

/*
 * BCH-Decoder - Calculation
 *
 * Author: Sebastian Schurig
 *
 * Calculation Functions for the BCH-Decoder
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
	int			    i;


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
	register int    i, mask;


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
	register int	ii, jj, ll, kaux, zero;
	register int	test, aux, nocycles, root, noterms, rdncy;
	int      cycle[8192][21];
	std::vector<int> size(8192);
    std::vector<int> min(2048);
    std::vector<int> zeros(16385);
    char processing[3] = {' ',' ','\0'};


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

    d = 2 * t + 1;

    /* Search for roots 1, 2, ..., d-1 in cycle sets */
    kaux = 0;
    rdncy = 0;
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
decode_bch()
/*
 * Simon Rockliff's implementation of Berlekamp's algorithm.
 *
 * Assume we have received bits in recd[i], i=0..(n-1).
 *
 * Compute the 2*t syndromes by substituting alpha^i into rec(X) and
 * evaluating, storing the syndromes in s[i], i=1..2t (leave s[0] zero) .
 * Then we use the Berlekamp algorithm to find the error location polynomial
 * elp[i].
 *
 * If the degree of the elp is >t, then we cannot correct all the errors, and
 * we have detected an uncorrectable error pattern. We output the information
 * bits uncorrected.
 *
 * If the degree of elp is <=t, we substitute alpha^i , i=1..n into the elp
 * to get the roots, hence the inverse roots, the error location numbers.
 * This step is usually called "Chien's search".
 *
 * If the number of errors located is not equal the degree of the elp, then
 * the decoder assumes that there are more than t errors and cannot correct
 * them, only detect them. We output the information bits uncorrected.
 */
{
  	register int    i, j;
	register int    u, q;
	register int    t2;
	register int    count, syn_error;
	static int      elp[8194][8192];

    std::vector<int> d(8194);
    std::vector<int> l(8194);
	std::vector<int> u_lu(8194);
	std::vector<int> s(8192);
	std::vector<int> root(4096);
	std::vector<int> loc(4096);
	std::vector<int> err(8192);
	std::vector<int> reg(4097);


	count = 0;
	syn_error = 0;
	t2 = 2 * t;

	/* first form the syndromes */
	for (i = 1; i <= t2; i++) {
		s[i] = 0;
		for (j = 0; j < length; j++)
			if (recd[j] != 0)
				s[i] ^= alpha_to[(i * j) % n];
		if (s[i] != 0)
			syn_error = 1; /* set error flag if non-zero syndrome */
/*
 * Note:    If the code is used only for ERROR DETECTION, then
 *          exit program here indicating the presence of errors.
 */
		/* convert syndrome from polynomial form to index form  */
		s[i] = index_of[s[i]];
	}

	if (syn_error) {	/* if there are errors, try to correct them */
		/*
		 * Compute the error location polynomial via the Berlekamp
		 * iterative algorithm. Following the terminology of Lin and
		 * Costello's book :   d[u] is the 'mu'th discrepancy, where
		 * u='mu'+1 and 'mu' (the Greek letter!) is the step number
		 * ranging from -1 to 2*t (see L&C),  l[u] is the degree of
		 * the elp at that step, and u_l[u] is the difference between
		 * the step number and the degree of the elp.
		 */
		/* initialise table entries */
		d[0] = 0;			/* index form */
		d[1] = s[1];		/* index form */
		elp[0][0] = 0;		/* index form */
		elp[1][0] = 1;		/* polynomial form */
		for (i = 1; i < t2; i++) {
			elp[0][i] = -1;	/* index form */
			elp[1][i] = 0;	/* polynomial form */
		}
		l[0] = 0;
		l[1] = 0;
		u_lu[0] = -1;
		u_lu[1] = 0;
		u = 0;

		do {
			u++;
			if (d[u] == -1) {
				l[u + 1] = l[u];
				for (i = 0; i <= l[u]; i++) {
					elp[u + 1][i] = elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			} else
				/*
				 * search for words with greatest u_lu[q] for
				 * which d[q]!=0
				 */
			{
				q = u - 1;
				while ((d[q] == -1) && (q > 0))
					q--;
				/* have found first non-zero d[q]  */
				if (q > 0) {
				  j = q;
				  do {
				    j--;
				    if ((d[j] != -1) && (u_lu[q] < u_lu[j]))
				      q = j;
				  } while (j > 0);
				}

				/*
				 * have now found q such that d[u]!=0 and
				 * u_lu[q] is maximum
				 */
				/* store degree of new elp polynomial */
				if (l[u] > l[q] + u - q)
					l[u + 1] = l[u];
				else
					l[u + 1] = l[q] + u - q;

				/* form new elp(x) */
				for (i = 0; i < t2; i++)
					elp[u + 1][i] = 0;
				for (i = 0; i <= l[q]; i++)
					if (elp[q][i] != -1)
						elp[u + 1][i + u - q] =
                                   alpha_to[(d[u] + n - d[q] + elp[q][i]) % n];
				for (i = 0; i <= l[u]; i++) {
					elp[u + 1][i] ^= elp[u][i];
					elp[u][i] = index_of[elp[u][i]];
				}
			}
			u_lu[u + 1] = u - l[u + 1];

			/* form (u+1)th discrepancy */
			if (u < t2) {
			/* no discrepancy computed on last iteration */
			  if (s[u + 1] != -1)
			    d[u + 1] = alpha_to[s[u + 1]];
			  else
			    d[u + 1] = 0;
			    for (i = 1; i <= l[u + 1]; i++)
			      if ((s[u + 1 - i] != -1) && (elp[u + 1][i] != 0))
			        d[u + 1] ^= alpha_to[(s[u + 1 - i]
			                      + index_of[elp[u + 1][i]]) % n];
			  /* put d[u+1] into index form */
			  d[u + 1] = index_of[d[u + 1]];
			}
		} while ((u < t2) && (l[u + 1] <= t));

		u++;

		if (l[u] <= t) {/* Can correct errors */
			/* put elp into index form */
			for (i = 0; i <= l[u]; i++)
				elp[u][i] = index_of[elp[u][i]];

			// Detected errors and correction
			for (i = 0; i <= l[u]; i++){
                if(elp[u][i] > 0) numerr++;
			}

			/* Chien search: find roots of the error location polynomial */
			for (i = 1; i <= l[u]; i++)
				reg[i] = elp[u][i];
			count = 0;
			for (i = 1; i <= n; i++) {
				q = 1;
				for (j = 1; j <= l[u]; j++)
					if (reg[j] != -1) {
						reg[j] = (reg[j] + j) % n;
						q ^= alpha_to[reg[j]];
					}
				if (!q) {	/* store root and error
						 * location number indices */
					root[count] = i;
					loc[count] = n - i;
					count++;
				}
			}
			if (count == l[u]){
			/* no. roots = degree of elp hence <= t errors */
				for (i = 0; i < l[u]; i++){
                    recd[loc[i]] ^= 1;
                    corerr++;
				}
            }
			else {
                /* elp has degree >t hence cannot solve */
				decerror++;
			}
		}
	}
}

int MajorityVoting(unsigned long pos, struct Item *item)
/*
 * Majority Vote
 *
 * Inputs:
 * pos = the actual bit position which will be checked
 * item = pointer to the struct to store the necessary informations
 *
 */
{
	int c = 0;
	int zero = 0;
	int one = 0;
	unsigned long posi = pos;


    while(c < item->LR){
       if( ((posi >> ((item->LR-1)-c)) & 0x00000001) == 1){
           one++;
           }
        else{
            zero++;
        }
        c++;
    }

    if(zero > one) return 0;
    else return 1;
}

int
Calculation(struct Item *item)
/*
 * Function to handle the actual calculation:
 *
 * -> Read in the HelperData
 * -> Extract the Settings (BCHmode, LR factor, encoded length and original Key filesize
 * -> Perform the XOR of the PUF with the HelperData
 * -> Majority vote to recover encoded codewords
 * -> Perform the decoding
 * -> Store the recovered Key into a file.
 *
 * Inputs:
 * item  = pointer to the struct to get/store the necessary informations
 */
{
    FILE *fd;
    long HD_filesize;
    long len;
    long encode_binary_length;
    unsigned char hex;
    unsigned char recLR;
    unsigned long pos8 = 0, pos7 = 0 ,pos6 = 0 , pos5 = 0, pos4 = 0, pos3 = 0, pos2 = 0, pos1 = 0;
    int bit0 = 0,  bit1 = 0,  bit2 = 0,  bit3 = 0,  bit4 = 0,  bit5 = 0,  bit6 = 0,  bit7 = 0;
    int i = 0, j = 0, u = 0, z = 0;
    struct Pattern infos;
    int error = 0;
    stringstream BCH;
    string bch_mode;
    char processing[3] = {' ',' ','\0'};

    // Check if HD Input file is a file
    if(!IsFile(item->input_HD_name)){
        return 10;
    }

    // Check if PUF Input file is a file
    if(!IsFile(item->input_PUF_name)){
        return 9;
    }

	// Try to open the HelperData file
    if ((fd = fopen(item->input_HD_name, "rb")) == NULL) {
		fclose(fd);
        return 11;
    }

    // Get the file size
    fseek(fd, 0, SEEK_END);
    HD_filesize = ftell(fd);
    rewind(fd);

    // Allocate space for the helperData to read it in
    unsigned char *helperData = (unsigned char *) malloc(sizeof(char) * (HD_filesize - sizeof(infos)));

    // Read the complete HelperData
    if (fread(&helperData[0], sizeof(char), (HD_filesize - sizeof(infos)), fd) != (HD_filesize - sizeof(infos))) {
		fclose(fd);
        return 12;
    }

    // Go to the end of the HelperData, at the start position of the Pattern struct
    fseek(fd, (HD_filesize - sizeof(infos)), SEEK_SET);

    // Read the coding informations (pattern)
    if ((signed)fread(&infos, sizeof(infos), 1, fd) != 1) {
		fclose(fd);
        return 12;
    }

    // Close the HelperData file
    fclose(fd);

    // Set the relevant informations for the decoding
    m = infos.Golay_k_BCH_m;
    t = infos.Golay_d_BCH_t;
    length = infos.Golay_BCH_length;
    item->LR = infos.linearRep;
    item->offSet = infos.puf_offSet;
    filesize = infos.original_filesize;

    // Check if all settings and informations are valid
    if(!(m >= 4 && m <= 14) ) return 14;
    else if(!(t >= 0 && t <= 4095))  return 15;
    else if(!(length >= 7 && length <= 16383))  return 16;
    else if(!(item->LR == 7 || item->LR == 15))  return 17;
    else if((item->offSet < 0 || item->offSet >= (signed)(HD_filesize - sizeof(infos))))  return 18;
    else if((filesize < 0 || filesize >= (signed)(HD_filesize - sizeof(infos))))  return 19;

    // Signal that all Settings are set and look valid
    isSet = 1;

    // BCH Mode calculation
    read_p();               /* Set primitive polynomial p(x) */
	generate_gf();          /* Construct the Galois Field GF(2**m) */
	gen_poly();             /* Compute the generator polynomial of BCH code */

    BCH << "BCH(" << length << ", " << k << ", " << d << ")";
    bch_mode = BCH.str();
    strcpy(item->BCHmode, bch_mode.c_str());
    BCH.str("");
    BCH.clear();

    /* Calculate length for the processing
     * Check if the original file size is a multiplicative to the chosen factor 'k' of the BCH mode
     *
     * len = length of the input before the bch coding (as integer [0 or 1] vector)
     *
     */
    if( (filesize * 8) % k != 0){

        // If it is not a multiplicative to the chosen factor 'k' of the BCH mode, determine the new length
        len = (((filesize * 8)/k)+1)*k;
    }
    else {

        // If it is a multiplicative to the chosen factor 'k' of the BCH mode use it
        len = (filesize * 8);
    }

    /* Check if the encoded key is byte block aligned
     *
     * encode_binary_length = length of bch encoded representation as byte blocks
     *
     */
    if (((len/k)*length)%8 != 0){

        // If not set the correct length
        encode_binary_length = (((len/k)*length) / 8)+1;
    }
    else {

        // If use the corresponding length
        encode_binary_length = ((len/k)*length) / 8;
    }

    // Try to open the PUF file
    if ((fd = fopen(item->input_PUF_name, "rb")) == NULL) {
		fclose(fd);
        return 8;
    }

    // Go to the offset position in the file
    fseek(fd, item->offSet, SEEK_SET);

    // Allocate space for the PUF
    unsigned char *sramData = (unsigned char *) malloc(sizeof(char) * (encode_binary_length * item->LR));

    // Allocate space for the BCH encoded byte stream
    unsigned char *recoverdFromLR = (unsigned char *) malloc(sizeof(char) * encode_binary_length);

    // Read the needed PUF Data
    if ((signed)fread(&sramData[0], sizeof(char), (encode_binary_length * item->LR), fd) != (encode_binary_length * item->LR)) {
		fclose(fd);
        return 7;
    }

    // Close file
    fclose(fd);

    // Perform the XOR operation of the HelperData and the PUF sramData and do the Majority Vote (LR = 7)to recover the BCH encoded byte stream
    if(item->LR == 7){
        while (j < encode_binary_length){
			recLR = 0x00;
			pos8 = (helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) >> 1;
                    if(MajorityVoting(pos8, item)) {
                        recLR = 0x80;
                    }
            pos7 = ((helperData[((j*item->LR)+1)] ^ sramData[(j*item->LR)+1]) >> 2) ^ (((helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) & 0x01) << 6);
                    if(MajorityVoting(pos7, item)){
                        recLR = (recLR ^ 0x40);
                    }
            pos6 = ((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) >> 3) ^ (((helperData[((j*item->LR)+1)] ^ sramData[(j*item->LR)+1]) & 0x03) << 5);
                    if(MajorityVoting(pos6, item)){
                        recLR = (recLR ^ 0x20);
                    }
            pos5 = ((helperData[((j*item->LR)+3)] ^ sramData[(j*item->LR)+3]) >> 4) ^ (((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) & 0x07) << 4);
                    if(MajorityVoting(pos5, item)){
                        recLR = (recLR ^ 0x10);
                    }
            pos4 = ((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) >> 5) ^ (((helperData[((j*item->LR)+3)] ^ sramData[(j*item->LR)+3]) & 0x0F) << 3);
                    if(MajorityVoting(pos4, item)){
                        recLR = (recLR ^ 0x08);
                    }
            pos3 = ((helperData[((j*item->LR)+5)] ^ sramData[(j*item->LR)+5]) >> 6) ^ (((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) & 0x1F) << 2);
                    if(MajorityVoting(pos3, item)){
                        recLR = (recLR ^ 0x04);
                    }
            pos2 = ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) >> 7) ^ (((helperData[((j*item->LR)+5)] ^ sramData[(j*item->LR)+5]) & 0x3F) << 1);
                    if(MajorityVoting(pos2, item)){
                        recLR = (recLR ^ 0x02);
                    }
            pos1 = ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) & 0x7F);
                    if(MajorityVoting(pos1, item)) {
                        recLR = (recLR ^ 0x01);
                    }
            // Set the recovered bits
            recoverdFromLR[j] = recLR;

            j++;
        }
    }
    // Perform the XOR operation of the HelperData and the PUF sramData and do the Majority Vote (LR = 15)to recover the BCH encoded byte stream
    else if(item->LR == 15){
        while (j < encode_binary_length){
            recLR = 0x00;
            pos8 = ((helperData[(j*item->LR)] ^ sramData[(j*item->LR)]) << 7) ^ ((helperData[(j*item->LR)+1] ^ sramData[(j*item->LR)+1]) >> 1);
                if(MajorityVoting(pos8, item)) {
                    recLR = 0x80;
                }
            pos7 = ((((helperData[(j*item->LR)+1] ^ sramData[(j*item->LR)+1]) & 0x01) << 14) ^ ((helperData[((j*item->LR)+2)] ^ sramData[(j*item->LR)+2]) << 6) ^ ((helperData[(j*item->LR)+3] ^ sramData[(j*item->LR)+3]) >> 2));
                if(MajorityVoting(pos7, item)){
                    recLR = (recLR ^ 0x40);
                }
            pos6 = ((((helperData[(j*item->LR)+3] ^ sramData[(j*item->LR)+3]) & 0x03) << 13) ^ ((helperData[((j*item->LR)+4)] ^ sramData[(j*item->LR)+4]) << 5) ^ ((helperData[(j*item->LR)+5] ^ sramData[(j*item->LR)+5]) >> 3));
                if(MajorityVoting(pos6, item)){
                    recLR = (recLR ^ 0x20);
                }
            pos5 = ((((helperData[(j*item->LR)+5] ^ sramData[(j*item->LR)+5]) & 0x07) << 12) ^ ((helperData[((j*item->LR)+6)] ^ sramData[(j*item->LR)+6]) << 4) ^ ((helperData[(j*item->LR)+7] ^ sramData[(j*item->LR)+7]) >> 4));
                if(MajorityVoting(pos5, item)){
                    recLR = (recLR ^ 0x10);
                }
            pos4 = ((((helperData[(j*item->LR)+7] ^ sramData[(j*item->LR)+7]) & 0x0F) << 11) ^ ((helperData[((j*item->LR)+8)] ^ sramData[(j*item->LR)+8]) << 3) ^ ((helperData[(j*item->LR)+9] ^ sramData[(j*item->LR)+9]) >> 5));
                if(MajorityVoting(pos4, item)){
                    recLR = (recLR ^ 0x08);
                }
            pos3 = ((((helperData[(j*item->LR)+9] ^ sramData[(j*item->LR)+9]) & 0x1F) << 10) ^ ((helperData[((j*item->LR)+10)] ^ sramData[(j*item->LR)+10]) << 2) ^ ((helperData[(j*item->LR)+11] ^ sramData[(j*item->LR)+11]) >> 6));
                if(MajorityVoting(pos3, item)){
                    recLR = (recLR ^ 0x04);
                }
            pos2 = ((((helperData[(j*item->LR)+11] ^ sramData[(j*item->LR)+11]) & 0x3F) << 9) ^ ((helperData[((j*item->LR)+12)] ^ sramData[(j*item->LR)+12]) << 1) ^ ((helperData[(j*item->LR)+13] ^ sramData[(j*item->LR)+13]) >> 7));
                if(MajorityVoting(pos2, item)){
                    recLR = (recLR ^ 0x02);
                }
            pos1 = ((((helperData[(j*item->LR)+13] ^ sramData[(j*item->LR)+13]) & 0x7F) << 8) ^ (helperData[(j*item->LR)+14] ^ sramData[(j*item->LR)+14])) ;
                if(MajorityVoting(pos1, item)) {
                    recLR = (recLR ^ 0x01);
                }
            // Set the recovered bits
            recoverdFromLR[j] = recLR;
            j++;
        }
    }

    // Generate a binary bit stream (for a integer vector) out of each byte block of the input file and store it in the data vector
    for(j = 0; j < encode_binary_length; j++){
        hex = recoverdFromLR[j];

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
    free(helperData);
    free(sramData);
    free(recoverdFromLR);

	// Calculate the length of the binary representation (integer array with binary values) based on the file size of the encoded file
    len = (encode_binary_length * 8);
    u = (len / 100);

    // Perform the actual DECODING
    cout << endl << endl << "Calculating BCH Decoding:" << endl << "(This may take some time - depending on the used BCH(mode) and file size)..." << endl << endl;

	// Process a complete file
    for(j = 0; j < len; j = j+length){

        // Fill the recd vector with the input values
        for(i = 0; i < length; i++){
            if( i + j < (signed)input.size()){
                recd[i] = input[i+j];
            }
            else {
                recd[i] = 0;
            }
        }

		// Decoding of one codeword (placed in the beginning of the recv vector)
		decode_bch();             /* DECODE received codeword recv[] */

        // Store the recovered original into the data vector
		for (i = 0; i < k; i++) {
            output.push_back(recd[i+(length-k)]);
        }

		z = z + 1;

		// Processing status
        if ((j+length) >= len) cout << "\r" << setw(-20) << StatusProgress(100) << " " << 100 << "% completed.  " << flush;
        else cout << "\r" << setw(-20) << StatusProgress((j/u)) << " " << (j/u) << "% completed." << processing << flush;

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
	}

    // Save the complete decoded data (block wise) into a binary file
    error = SaveFile(item);

    // Cleanup - erase the input/output vectors
	input.erase(input.begin(), input.begin() + input.size());
	output.erase(output.begin(), output.begin() + output.size());

    return error;
}
