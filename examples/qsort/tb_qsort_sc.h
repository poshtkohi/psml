/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "define_sc.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using std::string;
using namespace std;

SC_MODULE (test_qsort)
{

	// Inputs
	sc_in<bool>          clk;

	sc_in<sc_uint<32> > odata;


	//  Outputs
	sc_out<sc_uint<32> >  indata;
#if defined __FiberLess_Enabled__
	int next_input;
#endif


	//For data feeding
	//ifstream *reader;

	/* R */
	void recv();

	/* S */
	void send();



	SC_CTOR ( test_qsort )
	{
#if defined __FiberLess_Enabled__
        next_input = 0;
		SC_METHOD(send);
        ///dont_initialize();
#else
		SC_THREAD(send);
#endif
		sensitive << clk.pos();

#if defined __FiberLess_Enabled__
		SC_METHOD(recv);
        ///dont_initialize();
#else
		SC_THREAD(recv);
#endif
		sensitive << clk.pos();

		/*reader = new ifstream(INFILENAME, ios::in);

		if(!reader->is_open())
		{
			cout << "Could not open the file INFILENAME" << endl;
			exit(-1);
		}*/
	}

	~test_qsort()
	{
		/*reader->close();
		delete reader;*/
	}

};
