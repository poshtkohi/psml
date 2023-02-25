/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __key_gen_h__
#define __key_gen_h__

#include <systemc.h>

SC_MODULE(key_gen)
{
    sc_in<sc_uint<56> > previous_key;
    sc_in<sc_uint<4> > iteration;
    sc_in<bool > decrypt;	//When decrypting we rotate rigth instead of left
    sc_out<sc_uint<56> > non_perm_key;
    sc_out<sc_uint<48> > new_key;

    void generate_key();

    SC_CTOR(key_gen)
    {
        SC_METHOD(generate_key);
        sensitive << previous_key << iteration << decrypt;
    }
};

#endif
