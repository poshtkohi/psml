/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "qsort_sc.h"
#include "tb_qsort_sc.h"

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
std::vector<sc_uint<32> > __inputs__;
//---------------------------------------
class Core
{
	private: sc_clock *clk;
	private: sc_signal<sc_uint<32> > indata;
	private: sc_signal<sc_uint<32> > odata;
	private: quicksort *u_QSORT;
	private: test_qsort *u_test;
	public: Core()
	{
		sc_time period(2, SC_NS);
		double duty_cycle = 0.5;
		sc_time start_time(0, SC_NS);
		bool posedge_first = true;

		clk = new sc_clock("", period, duty_cycle, start_time, posedge_first);

		u_QSORT = new quicksort("");
        //connect to bubble sort
		u_QSORT->clk(*clk);
		u_QSORT->indata(indata);
		u_QSORT->odata(odata);

		u_test = new test_qsort("");
		// connect to test bench
		u_test->clk(*clk);
		u_test->indata( indata );
		u_test->odata( odata );
	}
	public: ~Core()
	{
		///cout << "counter: " << u_QSORT->counter << endl;
		delete clk;
		delete u_QSORT;
		delete u_test;
	}
};

//---------------------------------------
void BuildInputs()
{
	ifstream *reader = new ifstream(INFILENAME, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME");
	std::string line;
	Int32 read;
	while(getline(*reader, line))
	{
		*reader >> read;
		__inputs__.push_back(read);
	}

	reader->close();
	delete reader;

	//cout << __inputs__.size() << endl;
}
//---------------------------------------
// Driver program to test above functions
/*int sc_main(int argc, char** argv)
{
    // Create a graph given in the above diagram
    Graph g(16);
    g.addEdge(0, 1);
    g.addEdge(0, 4);
    g.addEdge(0, 14);
    g.addEdge(1, 2);
    g.addEdge(1, 15);
    g.addEdge(2, 1);
    g.addEdge(2, 4);
    g.addEdge(2, 5);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 2);
    g.addEdge(5, 3);
    g.addEdge(5, 4);
    g.addEdge(5, 9);
    g.addEdge(5, 6);
    g.addEdge(5, 7);
    g.addEdge(5, 8);
    g.addEdge(5, 10);
    g.addEdge(5, 11);
    g.addEdge(5, 12);
    g.addEdge(5, 13);
    g.addEdge(6, 5);
    g.addEdge(7, 5);
    g.addEdge(8, 5);
    g.addEdge(9, 5);
    g.addEdge(10, 5);
    g.addEdge(11, 5);
    g.addEdge(12, 5);
    g.addEdge(13, 5);
    g.addEdge(14, 1);
    g.addEdge(14, 2);
    g.addEdge(14, 5);
    g.addEdge(14, 4);

    cout << "Following are strongly connected components in "
            "given graph \n";
    g.SCC();

    return 0;
}*/
SC_MODULE(monitor) {
	sc_in<int> in;
	sc_in<bool> clk;
	SC_CTOR(monitor) {
		SC_THREAD(process);
		sensitive << clk.pos();
	}
	void process() {
		while(true)
		{
			cout << "@time " << sc_time_stamp() << " " << in << endl;
			wait();
		}
	}
};

SC_MODULE(driver) {
	sc_in<bool> clk;
	sc_out<int>  out;
	SC_CTOR(driver) {
		SC_THREAD(process);
		sensitive << clk.pos();
	}
	void process() {
		int counter = 0;
		while(true) {
			out.write(counter);
			counter++;
			wait();
		}
	}
};

int sc_main1(int argc, char *argv[]) {
	sc_set_time_resolution(1, SC_NS);
	sc_time period(2, SC_NS);
	sc_time start(0, SC_NS);
	sc_clock clk("clk", period, 0.5, start, true);
	sc_signal<int> s;
	driver d("d");
	d.clk(clk);
	d.out(s);
	monitor m("m");
	m.clk(clk);
	m.in(s);
	sc_start(100, SC_NS);
	return 0;
}


int sc_main(int argc, char** argv)
{
    /*sc_uint<32> x = "2797223681";
    sc_uint<32> y = "999999999999";
    sc_uint<32> z = "888888888888";
    y.range(15, 11) = x.range(15, 11);
    z.range(20, 16) = sc_uint<5>(55);
    std::cout << "x " << x << endl;
    std::cout << "y " << y << endl;
    std::cout << "z " << z << endl;
    return 0;*/

    /*sc_uint<32> stage1_R_o = "2797223681";
    sc_uint<32> stage1_L_o = "196708764";
    sc_uint<64> data_o_var_t;
    data_o_var_t.range(63, 32) = stage1_R_o;
    data_o_var_t.range(31, 0) = stage1_L_o;
    sc_uint<10> x = data_o_var_t.range(19, 10);
    x = x << 1;

    std::cout << "stage1_R_o " << stage1_R_o;
    std::cout << " stage1_L_o " << stage1_L_o;
    std::cout << " data_o_var_t " << data_o_var_t << std::endl;
    std::cout << " x " << x << std::endl;
    return 0;*/

	BuildInputs();

	struct timeval start; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&start, null);


	int numOfCores = 1;//4096;
	int simUntil = 100;

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

