/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __tb_h__
#define __tb_h__

#include <systemc.h>
#include <vector>

using namespace std;

SC_MODULE(tb)
{
    // inputs
    sc_in<bool> clk;
    sc_in<sc_uint<64> > rt_des_data_i;
    sc_in<bool> rt_des_ready_i;

    // outputs
    sc_out<bool> rt_load_o;
    sc_out<sc_uint<64> >rt_des_data_o;
    sc_out<sc_uint<64> >rt_des_key_o;
    sc_out<bool> rt_decrypt_o;
    sc_out<bool> rt_reset;

    void send();
    void recv();

    SC_CTOR(tb)
    {
        SC_THREAD(send);
        sensitive << clk.pos();

        SC_THREAD(recv);
        sensitive << rt_des_ready_i.pos();
    }

    ~tb()
    {
    }

};


#endif
