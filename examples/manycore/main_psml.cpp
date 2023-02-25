/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

//---------------------------------------
//#include "memory_psml.h"
#include "pe_work_stealing_scheduler_psml.h"
#include "pe_matmul_psml.h"
//#include "bus_controller_psml.h"
#include "core_partitioner.h"
#include "globals.h"

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/String/String.h>
#include <System/Convert/Convert.h>

#include <Parvicursor/psml/psml_connector.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_simulator.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_clock.h>

#include <vector>
using namespace std;

//---------------------------------------
using namespace System;

using namespace Parvicursor::psml;
using namespace Parvicursor::psml::Samples;
using namespace Parvicursor::PS2;
//---------------------------------------
extern Int64 numberOfActivatedProcesss;
Int64 *__mem__;
//---------------------------------------
Int32 main(Int32 argc, char **argv)
{
	Int32 retError = 0;

	UInt64 simUntil = SimulationUntil;
	UInt32 numOfLogicalProcessors = __cores__;
	UInt32 numOfLogicalProcessors_read = __cores__;

    //Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./manycore_psml.exe <Integer value>\n");

        Int32 num = atoi(argv[1]);

        if(num <= 0)
        {
        fprintf(stderr, "USAGE: %d must be >= 1\n", num);
        exit(1);
        }
        else
        {
            numOfLogicalProcessors_read = num;
            numOfLogicalProcessors = num;
        }
    }

    __mem__ = new Int64[NumOfWorks * 3 * N * N];
    for(register Int32 i = 0 ; i < NumOfWorks * 3 * N * N ; i++)
        __mem__[i] = i;

	if(numOfLogicalProcessors > 1)
        numOfLogicalProcessors += 1;//3

	std::vector<psml_core *> *psml_cores = new std::vector<psml_core *>;

	// psml_set_time_resolution... should be called before psml_time variable declaration
	psml_set_time_resolution(1, PSML_NS);

    Int32 numOfWorkItems = NumOfWorks;
    std::cout << "numOfWorkItems " << numOfWorkItems << std::endl;

    // Clock definition
    psml_time period(2, PSML_NS);
    Double duty_cycle = 0.5;
    psml_time start_time(0, PSML_NS);
    bool posedge_first = true;
	psml_clock clock("clock", period, duty_cycle, start_time, posedge_first);
    psml_connector clock_c;
	psml_wire<bool> clock_w;
	clock.out.bind(clock_c, clock_w);

    // Bus definitions
    /*psml_wire<UInt64 > address_mem_w;
    psml_wire<Int64 > data_in_mem_w;
    psml_wire<Int64 > data_out_mem_w;
    psml_wire<UInt64 > address_bus_ctrl_w[NumOfCores];
    psml_wire<Int64 > data_in_bus_ctrl_w[NumOfCores];
    psml_wire<Int64 > data_out_bus_ctrl_w[NumOfCores];*/
    psml_wire<Int32 > work_cores_w[NumOfCores];
    /*psml_wire<bool > re_mem_w;     // Memory Read Enable
    psml_wire<bool > we_mem_w;     // Memory Write Enable
    psml_wire<bool > cs_mem_w;     // Memory Chip Select
    psml_wire<bool > re_bus_ctrl_w[NumOfCores];     // Bus Controller Read Enable
    psml_wire<bool > we_bus_ctrl_w[NumOfCores];     // Bus Controller Write Enable
    psml_wire<bool > cs_bus_ctrl_w[NumOfCores];     // Bus Controller Chip Select*/
    psml_wire<bool > re_sched_cores_w[NumOfCores];  // Scheduler Read Enable
    psml_wire<bool > cs_sched_cores_w[NumOfCores];  // Scheduler Chip Select

    /*psml_connector address_mem_c;
    psml_connector data_in_mem_c;
    psml_connector data_out_mem_c;
    psml_connector address_bus_ctrl_c[NumOfCores];
    psml_connector data_in_bus_ctrl_c[NumOfCores];
    psml_connector data_out_bus_ctrl_c[NumOfCores];*/
    psml_connector work_cores_c[NumOfCores];
    /*psml_connector re_mem_c;
    psml_connector we_mem_c;
    psml_connector cs_mem_c;
    psml_connector re_bus_ctrl_c[NumOfCores];
    psml_connector we_bus_ctrl_c[NumOfCores];
    psml_connector cs_bus_ctrl_c[NumOfCores];*/
    psml_connector re_sched_cores_c[NumOfCores];
    psml_connector cs_sched_cores_c[NumOfCores];

    // Component definitions
	/*memory mem("mem", MemorySize);
	bus_controller bus_ctrl("bus_ctrl");*/
	pe_work_stealing_scheduler sched("pe_sched", numOfWorkItems);

    std::vector<pe_matmul *> mul_cores;
    for(register Int32 i = 0 ; i < NumOfCores ; i++)
    {
        //std::string name = std::string("pe_matmul") + std::to_string(i);
        std::string name = std::string("pe_matmul") + to_string(i);
        mul_cores.push_back(new pe_matmul(String(name.c_str()), N));

        //newCore = new psml_core();
        //newCore->add_process(mul_cores[i]->p);
        //psml_cores->push_back(newCore);
    }

    if(numOfLogicalProcessors_read >  1)
    {
        psml_core *newCore = new psml_core();
        newCore->add_process(clock.get_process());
        //psml_cores->push_back(newCore);

        //newCore = new psml_core();
        /*newCore->add_process(mem.p);
        newCore->add_process(bus_ctrl.p);*/
        //psml_cores->push_back(newCore);

        //newCore = new psml_core();
        newCore->add_process(sched.p);
        psml_cores->push_back(newCore);

        UInt32 per = NumOfCores / numOfLogicalProcessors_read;
        UInt32 remainder = NumOfCores % numOfLogicalProcessors_read;

        //std::cout << "per " << per << " remainder " << remainder << std::endl;

        for(register Int32 i = 0 ; i < per * numOfLogicalProcessors_read ; )
        {
            newCore = new psml_core();
            psml_cores->push_back(newCore);
            for(register Int32 j = 0 ; j < per ; j++, i++)
            {
                //std::cout << "i " << i << " j " << j << std::endl;
                pe_matmul *pe = mul_cores[i];
                newCore->add_process(pe->p);
            }
        }

        if(remainder > 0)
        {
            newCore = (*psml_cores)[psml_cores->size() - 1 - 1];//-3

            for(register Int32 i = per * numOfLogicalProcessors_read ; i < NumOfCores ; i++)
            {
                //std::cout << "i " << i << std::endl;
                pe_matmul *pe = mul_cores[i];
                newCore->add_process(pe->p);
            }
        }
    }

    // Memory port binding
	/*mem.clock.bind(clock_c, clock_w);
	mem.re.bind(re_mem_c, re_mem_w);
	mem.we.bind(we_mem_c, we_mem_w);
	mem.cs.bind(cs_mem_c, cs_mem_w);
	mem.address.bind(address_mem_c, address_mem_w);
	mem.data_in.bind(data_in_mem_c, data_in_mem_w);
	mem.data_out.bind(data_out_mem_c, data_out_mem_w);*/

    // Bus controller port binding
	/*bus_ctrl.clock.bind(clock_c, clock_w);
    for(register Int32 i = 0 ; i < NumOfCores ; i++)
    {
        bus_ctrl.re_cores[i].bind(re_bus_ctrl_c[i], re_bus_ctrl_w[i]);
        bus_ctrl.we_cores[i].bind(we_bus_ctrl_c[i], we_bus_ctrl_w[i]);
        bus_ctrl.cs_cores[i].bind(cs_bus_ctrl_c[i], cs_bus_ctrl_w[i]);
        bus_ctrl.address_cores[i].bind(address_bus_ctrl_c[i], address_bus_ctrl_w[i]);
        bus_ctrl.data_in_cores[i].bind(data_in_bus_ctrl_c[i], data_in_bus_ctrl_w[i]);
        bus_ctrl.data_out_cores[i].bind(data_out_bus_ctrl_c[i], data_out_bus_ctrl_w[i]);
    }
    bus_ctrl.re_mem.bind(re_mem_c, re_mem_w);
    bus_ctrl.we_mem.bind(we_mem_c, we_mem_w);
    bus_ctrl.cs_mem.bind(cs_mem_c, cs_mem_w);
    bus_ctrl.address_mem.bind(address_mem_c, address_mem_w);
    bus_ctrl.data_in_mem.bind(data_out_mem_c, data_out_mem_w);
    bus_ctrl.data_out_mem.bind(data_in_mem_c, data_in_mem_w);*/

    // Scheduler PE port binding
	sched.clock.bind(clock_c, clock_w);
	for(register Int32 i = 0 ; i < NumOfCores ; i++)
	{
        sched.re_cores[i].bind(re_sched_cores_c[i], re_sched_cores_w[i]);
        sched.cs_cores[i].bind(cs_sched_cores_c[i], cs_sched_cores_w[i]);
        sched.work_cores[i].bind(work_cores_c[i], work_cores_w[i]);
	}

	// matmul PE port binding
	for(register Int32 i = 0 ; i < NumOfCores ; i++)
	{
        mul_cores[i]->clock.bind(clock_c, clock_w);
        //mul_cores[i]->data_in.bind(data_out_bus_ctrl_c[i], data_out_bus_ctrl_w[i]);
        mul_cores[i]->work_sched.bind(work_cores_c[i], work_cores_w[i]);
        /*mul_cores[i]->re_bus_ctrl.bind(re_bus_ctrl_c[i], re_bus_ctrl_w[i]);
        mul_cores[i]->we_bus_ctrl.bind(we_bus_ctrl_c[i], we_bus_ctrl_w[i]);
        mul_cores[i]->cs_bus_ctrl.bind(cs_bus_ctrl_c[i], cs_bus_ctrl_w[i]);*/
        mul_cores[i]->re_sched.bind(re_sched_cores_c[i], re_sched_cores_w[i]);
        mul_cores[i]->cs_sched.bind(cs_sched_cores_c[i], cs_sched_cores_w[i]);
        /*mul_cores[i]->address.bind(address_bus_ctrl_c[i], address_bus_ctrl_w[i]);
        mul_cores[i]->data_out.bind(data_in_bus_ctrl_c[i], data_in_bus_ctrl_w[i]);*/
    }

	core_partitioner *partitioner = null;
	partitioner = new core_partitioner(psml_cores);

	try
	{
		// Initializes the simulator and time resolution in the entire simulation.
		psml_simulator sim = psml_simulator(numOfLogicalProcessors, partitioner);
		// This must be come after psml_simulator.
		psml_time duration(simUntil, PSML_NS);
		//sim.start();
		sim.start(duration);
		cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	}
	catch(Exception &e)
	{
		cout << "Caught exception: " << e.get_Message().get_BaseStream() << endl;
		retError = -1;
		goto Cleanup;
	}
	catch(...)
	{
		retError = -1;
		cout << "Unknown exception caught" << endl;
		goto Cleanup;
	}

Cleanup:

    for(register Int32 i = 0 ; i < NumOfCores ; i++)
        delete mul_cores[i];

	if(psml_cores != null)
	{
        for(register Int32 i = 0 ; i < psml_cores->size() ; i++)
            delete (*psml_cores)[i];

        delete psml_cores;
    }

	if(partitioner != null)
		delete partitioner;

    delete __mem__;

	return retError;
}
//---------------------------------------
