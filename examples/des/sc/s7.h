/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __s7_h__
#define __s7_h__

#include <systemc.h>

SC_MODULE(s7)
{
    sc_in<sc_uint<6> > stage1_input;
    sc_out<sc_uint<4> > stage1_output;

    void s7_box();

    SC_CTOR(s7)
    {
        SC_METHOD(s7_box);
        sensitive << stage1_input;
    }
};

#endif
