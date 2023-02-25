/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef DEFINE_H
#define DEFINE_H

//#include <iostream>
#include "stdio.h"

//#define __FiberLess_Enabled__


#define SIZE 16

#define NB 4
#define NBb 16
#define  nk 4
#define  nr 10


#define INFILENAME                "aes_cipher_input.txt"
#define INFILENAME_KEY            "aes_cipher_key.txt"

#define OUTFILENAME_GOLDEN       "aes_cipher_output_golden.txt"
#define OUTFILENAME              "aes_cipher_output.txt"
#define DIFFFILENAME             "diff.txt"

//#define WAVE_DUMP          // set do dump waveform or set as compile option -DWAVE_DUMP

#endif  // DEFINE_H

