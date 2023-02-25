/**
    #define meta ...
    printf("%s\n", meta);
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

#include <Parvicursor/xSim2/priority_queue.h>

#include "source_comp.h"
#include "sink_comp.h"

#include "pipeline_fifo_example.h"
//---------------------------------------
using namespace System;

using namespace Parvicursor::psml;
using namespace Parvicursor::psml::Samples;
using namespace Parvicursor::PS2;
//---------------------------------------
int main(int argc, char **argv)
{
	//priq_test();
	//return 0;
	//try
	{
		source_comp source("source");
		sink_comp sink("sink");

		//source.proxy_out(sink.proxy_in1);
		source.proxy_out.bind(sink.proxy_in1);

		//pipeline_fifo_example example("example");

		// Initializes the simulator and time resolution in the entire simulation.
		bool parallelExecution = true;
		psml_simulator sim = psml_simulator(parallelExecution, 1, PSML_NS);
		// This must be come after psml_simulator.
		psml_time duration(100, PSML_NS);
		//sim.start();
		sim.start(duration);
	}
	/*catch(Exception &e)
	{
		cout << "Caught exception: " << e.get_Message().get_BaseStream() << endl;
		exit(-1);
	}
	catch(...)
	{
		cout << "Unknown exception caught" << endl;
		exit(-1);
	}*/

	return 0;
}
//---------------------------------------