/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __pe_matmul_sc__h__
#define __pe_matmul_sc__h__

#include <systemc.h>
#include "globals.h"

//----------------------------------------------------
class pe_matmul : public sc_module
{
    /*---------------------fields-----------------*/
    public: sc_in<bool> clock;
    //public: sc_in<sc_int<64> > data_in;
    public: sc_in<sc_int<32> > work_sched;
    /*public: sc_out<bool> re_bus_ctrl;    // Bus Controller Read Enable
    public: sc_out<bool> we_bus_ctrl;    // Bus Controller Enable
    public: sc_out<bool> cs_bus_ctrl;    // Bus Controller Chip Select*/
    public: sc_out<bool> re_sched;  // Scheduler Read Enable
    public: sc_out<bool> cs_sched;  // Scheduler Chip Select
    /*public: sc_out<sc_uint<64> > address;
    public: sc_out<sc_int<64> > data_out;*/
    private: Int64 *local_memory;
    private: int n;
    private: Int64 *A, *B, *C; // The matrices
    //private: const int remote_access_latency = BusAccessLatency;
    /*---------------------methods----------------*/
    SC_HAS_PROCESS(pe_matmul);
	public: pe_matmul(sc_module_name name_, int n_);
	public: ~pe_matmul();
    private: void pe_matmul_proc();
    private: void multiply();
    private: void print_matrix(const std::string &name, Int64 *x, int n);
    /*--------------------------------------------*/
};
//----------------------------------------------------

#endif
