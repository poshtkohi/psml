
/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include <systemc.h>
#include <iostream>
#include "des.h"
#include "tb.h"

#include <vector>
#include <iostream>
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
//---------------------------------------
extern long long int numberOfActivatedProcesss;
std::vector<sc_uint<64> > __patterns__;
sc_uint<64> __input_key__ = "17855376605625100923";
//---------------------------------------
void BuildInputs()
{
	ifstream *reader = new ifstream("patterns.txt", ios::in);
	if(!reader->is_open())
	{
        //std::cout << "Could not open the file patterns.txt" << std::endl;
        //abort();
        throw IOException("Could not open the file patterns.txt");
	}
	std::string line;
	sc_uint<64> read;
	while(getline(*reader, line))
	{
		*reader >> read;
		__patterns__.push_back(read);
	}

	reader->close();
	delete reader;

	//cout << __inputs__.size() << endl;
}
//---------------------------------------
//sc_clock *clk;
class Core
{
    sc_signal<bool > reset;
    sc_signal<bool > rt_load;
    sc_signal<bool > rt_decrypt;
    sc_signal<sc_uint<64> > rt_data_i;
    sc_signal<sc_uint<64> > rt_key;
    sc_signal<sc_uint<64> > rt_data_o;
    sc_signal<bool > rt_ready;
    sc_clock *clk;
    des *de1;
    tb *tb1;
    public: Core()
    {
        sc_time period(2, SC_NS);
        double duty_cycle = 0.5;
        sc_time start_time(0, SC_NS);
        bool posedge_first = true;
        clk  = new sc_clock("", period, duty_cycle, start_time, posedge_first);

        de1 = new des("des");
        tb1 = new tb("tb");

        de1->clk(*clk);
        de1->reset(reset);
        de1->load_i(rt_load);
        de1->decrypt_i(rt_decrypt);
        de1->data_i(rt_data_i);
        de1->key_i(rt_key);
        de1->data_o(rt_data_o);
        de1->ready_o(rt_ready);

        tb1->clk(*clk);
        tb1->rt_des_data_i(rt_data_o);
        tb1->rt_des_ready_i(rt_ready);
        tb1->rt_load_o(rt_load);
        tb1->rt_des_data_o(rt_data_i);
        tb1->rt_des_key_o(rt_key);
        tb1->rt_decrypt_o(rt_decrypt);
        tb1->rt_reset(reset);
    }
};
void Model()
{
    new Core();
}
//---------------------------------------
int sc_main(int argc, char *argv[])
{
    BuildInputs();

    //for(Int32 i = 0 ; i < 10 ; i++)
    //    std::cout << __patterns__[i].to_string(SC_BIN) << std::endl;

    //return 0;

	struct timeval start; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&start, null);

    int numOfCores = 1; // 1200
    int simUntil = 100; // 100000

    // sc_set_time_resolution... should be called before starting the simulation.
    sc_set_time_resolution(1, SC_NS);

    /*sc_time period(2, SC_NS);
    double duty_cycle = 0.5;
    sc_time start_time(0, SC_NS);
    bool posedge_first = true;
    clk  = new sc_clock("", period, duty_cycle, start_time, posedge_first);*/

    for(register UInt32 i = 0 ; i < numOfCores ; i++)
        Model();


    sc_start(simUntil, SC_NS);

	struct timeval stop; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&stop, null);

	double _d1, _d2;
	_d1 = (double)start.tv_sec + 1e-6*((double)start.tv_usec);
	_d2 = (double)stop.tv_sec + 1e-6*((double)stop.tv_usec);
	// return result in seconds
	double totalSimulationTime = _d2 - _d1;

	std::cout << "Simulation completed in " << totalSimulationTime << " secs." << std::endl;

	std::cout << "sc_delta_count(): " << sc_delta_count() << std::endl; // returns the absolute number of delta cycles that have occurred during simulation,
	std::cout << "Total events change_stamp(): " << sc_get_curr_simcontext()->change_stamp() << std::endl;
	std::cout << "Timed events: " << sc_get_curr_simcontext()->change_stamp() -  sc_delta_count() << std::endl;
	std::cout << "\nnumberOfActivatedProcesses: " << numberOfActivatedProcesss << std::endl;
	std::cout << "\n---------------- Runtime Statistics ----------------\n\n";

	usage u;
	ResourceProfiler::GetResourceUsage(&u);
	ResourceProfiler::PrintResourceUsage(&u);

    return 0;
}
//---------------------------------------
