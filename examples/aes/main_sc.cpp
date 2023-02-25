/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "aes_cipher_sc.h"
#include "tb_aes_cipher_sc.h"

#include <vector>
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
std::vector<std::vector<sc_uint<8> > *> __inputs__;
sc_uint<8> __input_key__[SIZE];
//---------------------------------------
class Core
{
	private: sc_clock *clk;
    private: sc_signal<sc_uint<8> >  in_data[SIZE];
    private: sc_signal<sc_uint<8> >  in_key[SIZE];
    private: sc_signal<sc_uint<8> >  out_data[SIZE];
	private: aes_cipher *u_AES;
	private: test_aes_cipher *test;
	public: Core()
	{
		sc_time period(2, SC_NS);
		double duty_cycle = 0.5;
		sc_time start_time(0, SC_NS);
		bool posedge_first = true;

		clk = new sc_clock("", period, duty_cycle, start_time, posedge_first);

		u_AES = new aes_cipher("");
        //connect to AES CIPHER
        u_AES->clk(*clk);
        for(int x=0; x < SIZE; x++)
        {
            u_AES->idata[x](in_data[x]);
            u_AES->ikey[x]( in_key[x]);
            u_AES->odata[x](out_data[x]);
        }

		test = new test_aes_cipher("");
        // connect to Test Bench
        test->clk(*clk);
        for(int x=0; x < SIZE; x++)
        {
            test->idata[x](in_data[x]);
            test->ikey[x](in_key[x]);
            test->odata[x](out_data[x]);

            //test->idata[x](in_data[x]);
            //test->ikey[x](in_key[x]);
            //test->odata[x](in_data[x]);
        }
	}
	public: ~Core()
	{
		delete clk;
		//delete u_AES;
        delete test;
	}
};
//---------------------------------------
void BuildInputs()
{
	ifstream *reader = new ifstream(INFILENAME, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME");
	//string line;
	int read;
	while(!reader->eof()/*getline(*reader, line)*/)
	{
		std::vector<sc_uint<8> > *input = new std::vector<sc_uint<8> >;
		for(int i = 0 ; i < SIZE ; i++)
		{
			*reader >> read;
			input->push_back(read);
			//cout << read << endl;
		}
		__inputs__.push_back(input);
	}

	reader->close();
	delete reader;

	reader = new ifstream(INFILENAME_KEY, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME_KEY");

	int i = 0;
	while(!reader->eof()/*getline(*reader, line)*/)
	{
		*reader >> read;
		__input_key__[i] = read;
		//cout << read << endl;
		i++;
	}

	reader->close();
	delete reader;

	//cout << __inputs__.size() << endl;
}
//---------------------------------------
int sc_main(int argc, char** argv)
{

	BuildInputs();

	struct timeval start; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&start, null);


	int numOfCores = 1; // 1200
	int simUntil = 100; // 100000

	// psml_set_time_resolution... should be called before psml_time variable declaration
	sc_set_time_resolution(1, SC_NS);

	std::vector<Core *> cores;

	for(int i = 0 ; i < numOfCores ; i++)
		cores.push_back(new Core());

	sc_start(simUntil, SC_NS);

	for(int i = 0 ; i < numOfCores ; i++)
		delete cores[i];

	struct timeval stop; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&stop, null);

	double _d1, _d2;
	_d1 = (double)start.tv_sec + 1e-6*((double)start.tv_usec);
	_d2 = (double)stop.tv_sec + 1e-6*((double)stop.tv_usec);
	// return result in seconds
	double totalSimulationTime = _d2 - _d1;

	cout << "Simulation completed in " << totalSimulationTime << " secs." << endl;

	cout << "sc_delta_count(): " << sc_delta_count() << endl; // returns the absolute number of delta cycles that have occurred during simulation,
	cout << "Total events change_stamp(): " << sc_get_curr_simcontext()->change_stamp() << endl;
	cout << "Timed events: " << sc_get_curr_simcontext()->change_stamp() -  sc_delta_count() << endl;
	cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	cout << "\n---------------- Runtime Statistics ----------------\n\n";

	usage u;
	ResourceProfiler::GetResourceUsage(&u);
	ResourceProfiler::PrintResourceUsage(&u);

	return 0;

};
//---------------------------------------
