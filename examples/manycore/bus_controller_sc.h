/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __bus_controller_sc__h__
#define __bus_controller_sc__h__

#include <systemc.h>
#include "globals.h"

//----------------------------------------------------
class bus_controller : public sc_module
{
    /*---------------------fields-----------------*/
    public: sc_in<bool> clock;
    public: sc_in<bool> re_cores[NumOfCores]; // Cores Read Enable
    public: sc_in<bool> we_cores[NumOfCores]; // Cores Write Enable
    public: sc_in<bool> cs_cores[NumOfCores]; // Cores Chip Eelect
    public: sc_in<sc_int<64> > data_in_mem;
    public: sc_in<sc_uint<64> > address_cores[NumOfCores];
    public: sc_in<sc_int<64> > data_in_cores[NumOfCores];
    public: sc_out<sc_int<64> > data_out_cores[NumOfCores];
    public: sc_out<bool> re_mem; // Memory Read Enable
    public: sc_out<bool> we_mem; // Memory Write Enable
    public: sc_out<bool> cs_mem; // Memory Chip Select
    public: sc_out<sc_uint<64> > address_mem;
    public: sc_out<sc_int<64> > data_out_mem;
    //private: const int remote_access_latency = MemoryAccessLatency;
    /*---------------------methods----------------*/
    SC_HAS_PROCESS(bus_controller);
	public: bus_controller(sc_module_name name_);
	public: ~bus_controller();
    private: void bus_controller_proc();
    private: void print_ports(int i);
    /*--------------------------------------------*/
};
//----------------------------------------------------

#endif
