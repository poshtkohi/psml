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

#include <Parvicursor/psml/psml_memory_pool.h>
#include <Parvicursor/psml/dt/psml_int.h>
#include <Parvicursor/psml/dt/psml_uint.h>

#include "qsort_psml.h"
#include "tb_qsort_psml.h"

#include "CorePartitioner.h"

#include <vector>
using namespace std;

//---------------------------------------
using namespace System;

using namespace Parvicursor::psml;
using namespace Parvicursor::psml::Samples;
using namespace Parvicursor::PS2;
//---------------------------------------
std::vector<psml_core *> *__psml_cores__ = new std::vector<psml_core *>;
extern Int64 numberOfActivatedProcesss;
Vector<UInt32 > __inputs__;
//---------------------------------------
class CoreModel
{
	private: psml_connector _clk_c;
	private: psml_wire<bool> _clk_w;
	private: psml_wire<UInt32 > _indata_w, _odata_w;
	private: psml_connector _indata_c, _odata_c;
	private: psml_clock *clkcomp;
	private: qsort_comp *qcomp;
	private: test_qsort_comp *tcomp;
	public: CoreModel()
	{
		psml_time period(2, PSML_NS);
		Double duty_cycle = 0.5;
		psml_time start_time(0, PSML_NS);
		bool posedge_first = true;

        clkcomp = new psml_clock("clock_comp", period, duty_cycle, start_time, posedge_first);\
        clkcomp->out.bind(_clk_c, _clk_w);

		qcomp = new qsort_comp("qcomp");
        qcomp->clk.bind(_clk_c, _clk_w);
		qcomp->indata.bind(_indata_c, _indata_w);
		qcomp->odata.bind(_odata_c, _odata_w);

        tcomp = new test_qsort_comp("tcomp");
		tcomp->clk.bind(_clk_c, _clk_w);
		tcomp->indata.bind(_indata_c, _indata_w);
		tcomp->odata.bind(_odata_c, _odata_w);

        //clkcomp = new psml_clock("clock_comp", period, duty_cycle, start_time, posedge_first);\
        clkcomp->out.bind(_clk_c, _clk_w);

		psml_core *newCore = new psml_core();
		newCore->add_process(clkcomp->get_process());
		newCore->add_process(qcomp->p);
		newCore->add_process(tcomp->p1);
		newCore->add_process(tcomp->p2);
		__psml_cores__->push_back(newCore);
	}
	public: ~CoreModel()
	{
		//cout << "---------------------\ncounter: " << qcomp->counter << endl;
		delete clkcomp;
		delete qcomp;
		delete tcomp;
	}
};
//---------------------------------------
void BuildInputs()
{
	std::ifstream *reader = new std::ifstream(INFILENAME, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME");
	std::string line;
	UInt32 read;
	while(getline(*reader, line))
	{
		*reader >> read;
		__inputs__.PushBack(read);
	}

	reader->close();
	delete reader;

	//cout << __inputs__.size() << endl;
}
//---------------------------------------
class monitor : public psml_component {
	psml_inport<psml_wire<bool > > clk_process;
	psml_inport<psml_wire<int > > in_process;
	psml_process *process_handle;
	public: psml_proxy<psml_wire<int > > in;
	public: psml_proxy<psml_wire<bool > > clk;
	monitor( const String &name = "" ) {
		set_name(name);
		register_proxy(clk, clk_process);
		register_proxy(in, in_process);
		process_handle = register_process(process, true, "monitor_process");
		process_handle->register_port(clk_process);
		process_handle->register_port(in_process);
		process_handle->set_delay(psml_time(0, PSML_NS));
		process_handle->register_sensitivity(clk_process, PSML_POS_EDGE);
	}
	static void process(psml_process *owner) {
		monitor *my = (monitor *)owner->get_owner_component();
		while(true){
			cout << "@time " << psml_time_stamp(owner) << " " << my->in_process->fetch() << endl;
			psml_wait(owner);
		}
	}
};

class driver : public psml_component {
	psml_inport<psml_wire<bool > > clk_process;
	psml_outport<psml_wire<int > > out_process;
	psml_process *process_handle;
	public: psml_proxy<psml_wire<bool > > clk;
	public: psml_proxy<psml_wire<int > > out;
	driver( const String &name = "" ) {
		set_name(name);
		register_proxy(clk, clk_process);
		register_proxy(out, out_process);
		process_handle = register_process(process, true, "driver_process");
		process_handle->register_port(clk_process);
		process_handle->register_port(out_process);
		process_handle->set_delay(psml_time(0, PSML_NS));
		process_handle->register_sensitivity(clk_process, PSML_POS_EDGE);
	}
	static void process(psml_process *owner) {
		driver *my = (driver *)owner->get_owner_component();
		int counter = 0;
		while(true) {
			my->out_process->put(counter, owner);
			counter++;
			psml_wait(owner);
		}
	}
};

int main1(int argc, char *argv[]) {
	psml_set_time_resolution(1, PSML_NS);
	psml_time period(2, PSML_NS);
	psml_time start(0, PSML_NS);
	psml_clock clk_comp("clk", period, 0.5, start, true);
	psml_connector clk_c("clk_c");
	psml_wire<bool> clk("clk");
	clk_comp.out.bind(clk_c, clk);
	psml_wire<int> s;
	psml_connector s_c("s_c");
	driver d("d");
	d.clk.bind(clk_c, clk);
	d.out.bind(s_c, s);
	monitor m("m");
	m.clk.bind(clk_c, clk);
	m.in.bind(s_c, s);
	UInt32 numOfLogicalProcessors = 1;
	psml_simulator sim = psml_simulator(numOfLogicalProcessors);
	sim.start(100, PSML_NS);
	return 0;
}


Int32 main(Int32 argc, char **argv)
{
    /*UInt32 yy = 8317;
    UInt32 xxx;
    xxx = yy.range(31, 0);
    std::cout << xxx << std::endl;
    return 0;*/
    /*psml_uint<8> x = 182;
    psml_uint<8> y = 201;
    y.range(4, 3) = x.range(7, 6);
    std::cout << x.to_string(2) << endl;
    std::cout << y.to_string(2) << endl;

    return 0;*/

    /*UInt32 x = "2797223681";
    UInt32 y = "999999999999";
    UInt32 z = "888888888888";
    y.range(15, 11) = x.range(15, 11);
    z.range(20, 16) = psml_uint<5>(55);
    std::cout << "x " << x << endl;
    std::cout << "y " << y << endl;
    std::cout << "z " << z << endl;
    return 0;*/

    /*UInt32 stage1_R_o = "2797223681";
    UInt32 stage1_L_o = "196708764";
    psml_uint<64> data_o_var_t;
    data_o_var_t.range(63, 32) = stage1_R_o;
    data_o_var_t.range(31, 0) = stage1_L_o;
    psml_uint<10> x = data_o_var_t.range(19, 10);
    x = x << 1;

    std::cout << "stage1_R_o " << stage1_R_o;
    std::cout << " stage1_L_o " << stage1_L_o;
    std::cout << " data_o_var_t " << data_o_var_t << std::endl;
    std::cout << " x " << x << std::endl;
    return 0;*/
    //psml_int<8> x = 182;
    /*psml_uint<64> x = "99999999999999";
    psml_uint<64> y;//  = "99999999999999";
    psml_uint<64> z;
    std::cout << "x " << x.to_string(2) << std::endl;
    std::cout << "x " << x.to_string(10) << std::endl;

    //z.range(63, 0) = x;
    //y = z.range(63, 0);
    //y.range(63, 0) = x(63, 0);
    //psml_wire<psml_int<64> > w;
    //w.put(ww, null);
    //w.put(x.range(63, 0), null);
    //serialize(y.range(63, 0));

    //psml_int<8> y = 201;
    //psml_int<64> y;
    //y = x;
    //y = x.range(31, 0);
    //y.range(63, 0) = x(63, 0);
    //z = x + y;
    //y.range(63, 22) = x.range(63, 22);
    //psml_uint<64> z = x << y;
    //if(z) int ff = 0;
    // check for different sizes !! we must throw error !!
    //y.range(4, 2) = x.range(3, 1);
    //psml_int<3> z = 0;
    //z.range(2, 0) = x.range(3, 1);

    //x.range(63, 0) = y.range(63, 0);

    struct timeval start1; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&start1, null);

	for(Int32 i = 0, j = 0 ; i < 100000000 ; i++)
	{
        z = x + y - x + 1 - x + 10* x * y * z * 10;
        z += x;
        if(z < x)
            z++;
        else
            z--;
        //z = x + y;
        y[j] = x[j];
        //printf("j=%d x[%d]=%d y[%d]=%d\n", j, j, x[j], j, y[j]);
        //bool v = x[10];
        //serialize1(x[10]);
        //printf("x[%d]=%d %d\n", 10, v, x[10]);
        //std::cout << x[10] << std::endl;
        //y = x;
        //y = x.range(63, 0);
        //y.range(63, 22) = x.range(63, 22);
        //bool bit = x[j];
        //y[j] = bit;
        //std::cout << bit;
        if(j == 63)
            j = 0;
        //    break;
        j++;
    }
    //int xx = x;
    //std::cout << x << endl;

    std::cout << std::endl;
    //x.range(63, 0) = y.range(63, 0);

	struct timeval stop1; // has tv_sec and tv_usec elements.
	xParvicursor_gettimeofday(&stop1, null);

    double d1, d2;
	d1 = (double)start1.tv_sec + 1e-6*((double)start1.tv_usec);
	d2 = (double)stop1.tv_sec + 1e-6*((double)stop1.tv_usec);
	// return result in seconds
	double totalSimulationTime1 = d2 - d1;

	cout << "Simulation completed in " << totalSimulationTime1 << " secs." << endl;


    std::cout << "y " << y.to_string(2) << std::endl;
    std::cout << "y " << y.to_string(10) << std::endl;
    std::cout << "z " << z.to_string(2) << std::endl;
    std::cout << "z " << z << std::endl;

    return 0;*/

	BuildInputs();

	Int32 retError = 0;

	Int32 numOfCores =  1;  //4096;
	UInt64 simUntil = 100; //100000;
	UInt32 numOfLogicalProcessors = 1;//132;

    // Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./qsort_psml.exe <Integer value>\n");

        Int32 num = atoi(argv[1]);

        if(num <= 0)
        {
            fprintf(stderr, "USAGE: %d must be >= 1\n", num);
            exit(1);
        }
        else
        {
            numOfLogicalProcessors = num;
        }
    }

	// psml_set_time_resolution... should be called before psml_time variable declaration
	psml_set_time_resolution(1, PSML_NS);

	std::vector<CoreModel *> cores;

	for(Int32 i = 0 ; i < numOfCores ; i++)
		cores.push_back(new CoreModel());

	CorePartitioner *partitioner = null;
	partitioner = new CorePartitioner(__psml_cores__);

	try
	{
		// Initializes the simulator and time resolution in the entire simulation.
		psml_simulator sim = psml_simulator(numOfLogicalProcessors/*, partitioner*/);
		// This must be come after psml_simulator.
		psml_time duration(simUntil, PSML_NS);
		//sim.start();
		sim.start(duration);

		cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	}
	catch(System::Exception &e)
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

	for(Int32 i = 0 ; i < numOfCores ; i++)
	{
		delete cores[i];
		delete (*__psml_cores__)[i];
	}

	cores.clear();

	delete __psml_cores__;

	if(partitioner != null)
		delete partitioner;

	//prInt32f("Goodbye\n");

	return retError;
}
//---------------------------------------
