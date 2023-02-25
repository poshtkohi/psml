/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __memory_sc__h__
#define __memory_sc__h__

#include <systemc.h>
#include "globals.h"

//----------------------------------------------------
// A 64-bit memory that is arranged in 64-bit words.
class memory : public sc_module
{
    /*---------------------fields-----------------*/
    public: sc_in<bool> clock;
    public: sc_in<bool> re; // Read enable
    public: sc_in<bool> we; // Write enable
    public: sc_in<bool> cs; // Chip select
    public: sc_in<sc_uint<64> > address;
    public: sc_in<sc_int<64> > data_in;
    public: sc_out<sc_int<64> > data_out;
    private: Int64 *array;
    private: int size;
    /*---------------------methods----------------*/
    SC_HAS_PROCESS(memory);
	public: memory(sc_module_name name_, int size_);
	public: ~memory();
    private: void memory_proc();
    private: void print_memory();
    private: void print_ports();
    /*--------------------------------------------*/
};
//----------------------------------------------------

#endif
