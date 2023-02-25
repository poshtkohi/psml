
/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "tb_qsort_sc.h"

extern std::vector<sc_uint<32> > __inputs__;
//--------------------------
// Send data thread
//-------------------------
void test_qsort::send()
{

  // Variables declaration
	sc_uint<32> in_read;

	//indata.write(0);

#if defined __FiberLess_Enabled__

    in_read = __inputs__[next_input];
    indata.write(in_read);

   // std::cout << "@time " << sc_time_stamp();\
    std::cout << " in send() " << in_read << std::endl;

    next_input++;
    if(next_input == __inputs__.size())
        next_input = 0;
#else
    int next_input = 0;
	wait();
	//wait(1, SC_NS);

	while(true)
	{
		in_read = __inputs__[next_input];
		indata.write(in_read);

        //std::cout << "@time " << sc_time_stamp();\
        std::cout << " in send() " << in_read << std::endl;

		next_input++;
		if(next_input == __inputs__.size())
			next_input = 0;

		wait();
		//wait(1, SC_NS);
	}
#endif
}



//--------------------------
// Receive data thread
//--------------------------
void test_qsort::recv(){

	// Variables declaration
	sc_uint<32> out_write;
#if defined __FiberLess_Enabled__
    out_write = odata.read();
    //std::cout << "@time " << sc_time_stamp();\
    std::cout << " in recv() " << out_write << std::endl;
#else
	wait();
	//wait(1, SC_NS);

	while(true)
	{

		out_write = odata.read();

		std::cout << "@time " << sc_time_stamp();\
		std::cout << " in recv() " << out_write << std::endl;

		wait();
		//wait(1, SC_NS);
	}
#endif
}

