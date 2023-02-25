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

#include "define_psml.h"
#include "aes_cipher_psml.h"
#include "tb_aes_psml.h"

#include "CorePartitioner.h"

#include <vector>
using namespace std;

//---------------------------------------
using namespace System;

using namespace Parvicursor::psml;
using namespace Parvicursor::psml::Samples;
using namespace Parvicursor::PS2;
//---------------------------------------
extern Int64 numberOfActivatedProcesss;
std::vector<psml_core *> *__psml_cores__ = new std::vector<psml_core *>;
Vector<Vector<UInt8> *> __inputs__;
UInt8 __input_key__[SIZE];
//---------------------------------------
class CoreModel
{
	private: psml_connector _clk_c;
	private: psml_wire<bool> _clk_w;
	private: psml_wire<UInt8> _idata_w[SIZE], _odata_w[SIZE], _ikey_w[SIZE];
	private: psml_connector _idata_c[SIZE], _odata_c[SIZE], _ikey_c[SIZE];
	private: psml_clock *clkcomp;
	private: aes_comp *acomp;
	private: test_aes_comp *tcomp;
	public: CoreModel()
	{
		psml_time period(2, PSML_NS);
		Double duty_cycle = 0.5;
		psml_time start_time(0, PSML_NS);
		bool posedge_first = true;

		clkcomp = new psml_clock("clock_comp", period, duty_cycle, start_time, posedge_first);
		clkcomp->out.bind(_clk_c, _clk_w);

		acomp = new aes_comp("acomp");
		acomp->clk.bind(_clk_c, _clk_w);
		for(register Int32 i = 0 ; i < SIZE ; i++)
		{
			acomp->idata[i].bind(_idata_c[i], _idata_w[i]);
            acomp->ikey[i].bind(_ikey_c[i], _ikey_w[i]);
			acomp->odata[i].bind(_odata_c[i], _odata_w[i]);
		}

		tcomp = new test_aes_comp("tcomp");
		tcomp->clk.bind(_clk_c, _clk_w);
		for(register Int32 i = 0 ; i < SIZE ; i++)
		{
			tcomp->idata[i].bind(_idata_c[i], _idata_w[i]);
            tcomp->ikey[i].bind(_ikey_c[i], _ikey_w[i]);
			tcomp->odata[i].bind(_odata_c[i], _odata_w[i]);

            //tcomp->idata[i].bind(_idata_c[i], _idata_w[i]);
            //tcomp->ikey[i].bind(_ikey_c[i], _ikey_w[i]);
			//tcomp->odata[i].bind(_idata_c[i], _idata_w[i]);
		}

		psml_core *newCore = new psml_core();
		newCore->add_process(clkcomp->get_process());
		newCore->add_process(acomp->p);
		newCore->add_process(tcomp->p1);
		newCore->add_process(tcomp->p2);
		__psml_cores__->push_back(newCore);
	}
	public: ~CoreModel()
	{
		delete clkcomp;
		delete acomp;
		delete tcomp;
	}
};
//---------------------------------------
void BuildInputs()
{
	ifstream *reader = new ifstream(INFILENAME, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME");
	//string line;
	Int32 read;
	while(!reader->eof()/*getline(*reader, line)*/)
	{
		Vector<UInt8> *input = new Vector<UInt8>;
		for(register UInt32 i = 0 ; i < SIZE ; i++)
		{
			*reader >> read;
			input->PushBack(read);
			//cout << read << endl;
		}
		__inputs__.PushBack(input);
	}

	reader->close();
	delete reader;

	reader = new ifstream(INFILENAME_KEY, ios::in);
	if(!reader->is_open())
		throw IOException("Could not open the file INFILENAME_KEY");

	UInt32 i = 0;
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
Int32 main(Int32 argc, char **argv)
{
	BuildInputs();

	//std::cout << __inputs__.size() << endl;
	//std::cout << (UInt32)__input_key__[15] << endl;

	Int32 retError = 0;

	UInt32 numOfCores = 1200; // 1200
	UInt64 simUntil = 10000; // 100000
	UInt32 numOfLogicalProcessors = 1; // 120

    // Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./aes_psml.exe <Integer value>\n");

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

	for(UInt32 i = 0 ; i < numOfCores ; i++)
		cores.push_back(new CoreModel());

	CorePartitioner *partitioner = null;
	partitioner = new CorePartitioner(__psml_cores__);

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

	for(UInt32 i = 0 ; i < numOfCores ; i++)
	{
		delete cores[i];
		delete (*__psml_cores__)[i];
	}

	cores.clear();

	delete __psml_cores__;

	if(partitioner != null)
		delete partitioner;

	for(UInt32 i = 0 ; i < __inputs__.Size() ; i++)
		delete __inputs__[i];

	return retError;
}
//---------------------------------------
