/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef QSORT_H
#define QSORT_H

#include "define_sc.h"


SC_MODULE( quicksort )
{
	int counter;
	sc_in<bool> clk;

	sc_in<sc_uint<32> > indata;
	sc_out<sc_uint<32> > odata;
#if defined __FiberLess_Enabled__
    sc_uint<32>  data[SIZE], data_out[SIZE];
#endif


	/* R */
	void run() ;

	/* S */
	void swap(int *, int *);
	void sort(sc_uint<32> *);


	SC_CTOR( quicksort)
	{
        counter = 0;
#if defined __FiberLess_Enabled__
        memset(data, 0, sizeof(sc_int<32>) * SIZE);
        memset(data_out, 0, sizeof(sc_int<32>) * SIZE);
		SC_METHOD(run);
        ///dont_initialize();
#else
		SC_THREAD(run);
#endif
		sensitive << clk.pos();
	}

   ~quicksort(){}

};

#endif // QOSRT_H

