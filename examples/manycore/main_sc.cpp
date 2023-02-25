/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

//#include "memory_sc.h"
#include "pe_work_stealing_scheduler_sc.h"
#include "pe_matmul_sc.h"
//#include "bus_controller_sc.h"
#include "globals.h"

#include <string>

using namespace std;

#include <general.h>
#include <StaticFunctions/StaticFunctions.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/DateTime/DateTime.h>
#include <Parvicursor/Profiler/ResourceProfiler.h>
#include <System.IO/IOException/IOException.h>

using namespace System;
using namespace System::IO;
using namespace Parvicursor::Profiler;

//----------------------------------------------------
extern long long int numberOfActivatedProcesss;
Int64 *__mem__;
//----------------------------------------------------
int sc_main(int argc, char * argv[])
{
    double simUntil = SimulationUntil; // 100000

	struct timeval start; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&start, null);

	sc_set_time_resolution(1, SC_NS);

	int numOfWorkItems = NumOfWorks;
    std::cout << "numOfWorkItems " << numOfWorkItems << std::endl;

    __mem__ = new Int64[NumOfWorks * 3 * N * N];
    for(register Int32 i = 0 ; i < NumOfWorks * 3 * N * N ; i++)
        __mem__[i] = i;

    // Clock definition
    sc_time period(2, SC_NS);
    double duty_cycle = 0.5;
    sc_time start_time(0, SC_NS);
    bool posedge_first = true;
	sc_clock clock("clock", period, duty_cycle, start_time, posedge_first);

    // Bus definitions
    /*sc_signal<sc_uint<64> > address_mem;
    sc_signal<sc_int<64> > data_in_mem;
    sc_signal<sc_int<64> > data_out_mem;
    sc_signal<sc_uint<64> > address_bus_ctrl[NumOfCores];
    sc_signal<sc_int<64> > data_in_bus_ctrl[NumOfCores];
    sc_signal<sc_int<64> > data_out_bus_ctrl[NumOfCores];*/
    sc_signal<sc_int<32> > work_cores[NumOfCores];
    /*sc_signal<bool > re_mem;     // Memory Read Enable
    sc_signal<bool > we_mem;     // Memory Write Enable
    sc_signal<bool > cs_mem;     // Memory Chip Select
    sc_signal<bool > re_bus_ctrl[NumOfCores];     // Bus Controller Read Enable
    sc_signal<bool > we_bus_ctrl[NumOfCores];     // Bus Controller Write Enable
    sc_signal<bool > cs_bus_ctrl[NumOfCores];     // Bus Controller Chip Select*/
    sc_signal<bool > re_sched_cores[NumOfCores];   // Scheduler Read Enable
    sc_signal<bool > cs_sched_cores[NumOfCores];   // Scheduler Chip Select
    //enable.write(false);

    // Module definitions
	/*memory mem("mem", MemorySize);
	bus_controller bus_ctrl("bus_ctrl");*/
	pe_work_stealing_scheduler sched("pe_sched", numOfWorkItems);
	//pe_matmul mul0("pe_matmul0", N);
	//pe_matmul mul1("pe_matmul1", N);

	std::vector<pe_matmul *> mul_cores;
	for(int i = 0 ; i < NumOfCores ; i++)
	{
        //std::string name = std::string("pe_matmul") + std::to_string(i);
        std::string name = std::string("pe_matmul") + to_string(i);
        mul_cores.push_back(new pe_matmul(sc_core::sc_module_name(name.c_str()), N));
    }

    // Memory port binding
	/*mem.clock(clock);
	mem.re(re_mem);
	mem.we(we_mem);
	mem.cs(cs_mem);
	mem.address(address_mem);
	mem.data_in(data_in_mem);
	mem.data_out(data_out_mem);*/

    // Bus controller port binding
	/*bus_ctrl.clock(clock);
    for(int i = 0 ; i < NumOfCores ; i++)
    {
        bus_ctrl.re_cores[i](re_bus_ctrl[i]);
        bus_ctrl.we_cores[i](we_bus_ctrl[i]);
        bus_ctrl.cs_cores[i](cs_bus_ctrl[i]);
        bus_ctrl.address_cores[i](address_bus_ctrl[i]);
        bus_ctrl.data_in_cores[i](data_in_bus_ctrl[i]);
        bus_ctrl.data_out_cores[i](data_out_bus_ctrl[i]);
    }
    bus_ctrl.re_mem(re_mem);
    bus_ctrl.we_mem(we_mem);
    bus_ctrl.cs_mem(cs_mem);
    bus_ctrl.address_mem(address_mem);
    bus_ctrl.data_in_mem(data_out_mem);
    bus_ctrl.data_out_mem(data_in_mem);*/

    // Scheduler PE port binding
	sched.clock(clock);
	for(int i = 0 ; i < NumOfCores ; i++)
	{
        sched.re_cores[i](re_sched_cores[i]);
        sched.cs_cores[i](cs_sched_cores[i]);
        sched.work_cores[i](work_cores[i]);
	}

	// matmul PE port binding
	for(int i = 0 ; i < NumOfCores ; i++)
	{
        mul_cores[i]->clock(clock);
        //mul_cores[i]->data_in(data_out_bus_ctrl[i]);
        mul_cores[i]->work_sched(work_cores[i]);
        /*mul_cores[i]->re_bus_ctrl(re_bus_ctrl[i]);
        mul_cores[i]->we_bus_ctrl(we_bus_ctrl[i]);
        mul_cores[i]->cs_bus_ctrl(cs_bus_ctrl[i]);*/
        mul_cores[i]->re_sched(re_sched_cores[i]);
        mul_cores[i]->cs_sched(cs_sched_cores[i]);
        /*mul_cores[i]->address(address_bus_ctrl[i]);
        mul_cores[i]->data_out(data_in_bus_ctrl[i]);*/
    }

    /*mul0.clock(clock);
    mul0.data_in(data_out);
    mul0.work(work);
    mul0.re_mem(re_mem);
    mul0.we_mem(we_mem);
    mul0.cs_mem(cs_mem);
    mul0.re_sched(re_sched);
    mul0.cs_sched(cs_sched);
    mul0.address(address);
    mul0.data_out(data_in);

    mul1.clock(clock);
    mul1.data_in(data_out);
    mul1.work(work);
    mul1.re_mem(re_mem);
    mul1.we_mem(we_mem);
    mul1.cs_mem(cs_mem);
    mul1.re_sched(re_sched);
    mul1.cs_sched(cs_sched);
    mul1.address(address);
    mul1.data_out(data_in);*/

    // Simulation
    sc_time d(simUntil, SC_NS);
	sc_start(d);

    for(int i = 0 ; i < NumOfCores ; i++)
        delete mul_cores[i];

    struct timeval stop; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&stop, null);

	double _d1, _d2;
	_d1 = (double)start.tv_sec + 1e-6*((double)start.tv_usec);
	_d2 = (double)stop.tv_sec + 1e-6*((double)stop.tv_usec);
	// return result in seconds
	double totalSimulationTime = _d2 - _d1;

	std::cout << "Simulation completed in " << totalSimulationTime << " secs." << std::endl;
	std::cout << "Simulated time " << sc_time_stamp() << std::endl;

	std::cout << "sc_delta_count(): " << sc_delta_count() << std::endl; // returns the absolute number of delta cycles that have occurred during simulation,
	std::cout << "Total events change_stamp(): " << sc_get_curr_simcontext()->change_stamp() << std::endl;
	std::cout << "Timed events: " << sc_get_curr_simcontext()->change_stamp() -  sc_delta_count() << std::endl;
	std::cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << std::endl;
	std::cout << "\n---------------- Runtime Statistics ----------------\n\n";

	usage u;
	ResourceProfiler::GetResourceUsage(&u);
	ResourceProfiler::PrintResourceUsage(&u);

	return 0;
}
//----------------------------------------------------
