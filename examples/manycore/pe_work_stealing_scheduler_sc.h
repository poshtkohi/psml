/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __pe_work_stealing_scheduler_sc__h__
#define __pe_work_stealing_scheduler_sc__h__

#include <systemc.h>
#include "globals.h"

//----------------------------------------------------
class pe_work_stealing_scheduler : public sc_module
{
    /*---------------------fields-----------------*/
    public: sc_in<bool> clock;
    public: sc_in<bool> re_cores[NumOfCores]; // Read Enable
    public: sc_in<bool> cs_cores[NumOfCores]; // Chip Select
    public: sc_out<sc_int<32> > work_cores[NumOfCores];
    private: const int numOfWorks;
    /*---------------------methods----------------*/
    SC_HAS_PROCESS(pe_work_stealing_scheduler);
	public: pe_work_stealing_scheduler(sc_module_name name_, int numOfWorks_);
	public: ~pe_work_stealing_scheduler();
    private: void pe_work_stealing_scheduler_proc();
    private: void print_ports(int i);
    /*--------------------------------------------*/
};
//----------------------------------------------------

#endif
