/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __desround_h__
#define __desround_h__

#include <systemc.h>

#include "key_gen.h"

SC_MODULE(desround)
{
    sc_in<bool > clk;
    sc_in<bool > reset;

    sc_in<sc_uint<4> > iteration_i;
    sc_in<bool > decrypt_i;
    sc_in<sc_uint<32> > R_i;
    sc_in<sc_uint<32> > L_i;
    sc_in<sc_uint<56> > Key_i;

    sc_out<sc_uint<32> > R_o;
    sc_out<sc_uint<32> > L_o;
    sc_out<sc_uint<56> > Key_o;

    sc_out<sc_uint<6> > s1_o, s2_o, s3_o, s4_o, s5_o, s6_o, s7_o, s8_o;
    sc_in<sc_uint<4> > s1_i, s2_i, s3_i, s4_i, s5_i, s6_i, s7_i, s8_i;

    void registers();
    void round_proc();

    sc_signal<sc_uint<56> > previous_key;
    sc_signal<sc_uint<4> > iteration;
    sc_signal<bool > decrypt;	//When decrypting we rotate rigth instead of left
    sc_signal<sc_uint<56> > non_perm_key;
    sc_signal<sc_uint<48> > new_key;

    sc_signal<sc_uint<32> > next_R;

    ///sc_signal<sc_uint<32> > expanRSig;

    //Round key generator
    key_gen *kg1;

    SC_CTOR(desround)
    {
        kg1 = new key_gen("key_gen");
        kg1->previous_key(previous_key);
        kg1->iteration(iteration);
        kg1->decrypt(decrypt);
        kg1->new_key(new_key);
        kg1->non_perm_key(non_perm_key);

        SC_METHOD(registers);
        sensitive << clk.pos();
        sensitive << reset.neg();

        SC_METHOD(round_proc);
        sensitive << R_i << L_i << Key_i << iteration_i << decrypt_i;
        sensitive << new_key << s1_i << s2_i << s3_i << s4_i << s5_i;
        sensitive << s6_i << s7_i << s8_i;
    }
};

#endif
