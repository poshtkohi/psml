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

#include "fa_comp.h"
#include "monitor_comp.h"
#include "driver_comp.h"

//---------------------------------------
using namespace System;

using namespace Parvicursor::psml;
using namespace Parvicursor::psml::Samples;
using namespace Parvicursor::PS2;
//---------------------------------------
int main(int argc, char **argv)
{
	psml_set_time_resolution(1, PSML_NS);

	try
	{
		psml_connector A3_c("A3_c"), A2_c, A1_c, A0_c, B3_c, B2_c, B1_c, B0_c, S3_c, S2_c, S1_c, S0_c;
		psml_wire<bool> A3_w, A2_w, A1_w, A0_w, B3_w, B2_w, B1_w, B0_w, S3_w, S2_w, S1_w, S0_w;

		psml_connector CIN_s_c, cout1_c, cout2_c, cout3_c, COUT_s_c;
		psml_wire<bool> CIN_s_w, cout1_w, cout2_w, cout3_w, COUT_s_w;

		fa_comp adder1("adder1");
		adder1.a.bind(A0_c, A0_w);
		adder1.b.bind(B0_c, B0_w);
		adder1.cin.bind(CIN_s_c, CIN_s_w);
		adder1.sum.bind(S0_c, S0_w);
		adder1.cout.bind(cout1_c, cout1_w);

		fa_comp adder2("adder2");
		adder2.a.bind(A1_c, A1_w);
		adder2.b.bind(B1_c, B1_w);
		adder2.cin.bind(cout1_c, cout1_w);
		adder2.sum.bind(S1_c, S1_w);
		adder2.cout.bind(cout2_c, cout2_w);

		fa_comp adder3("adder3");
		adder3.a.bind(A2_c, A2_w);
		adder3.b.bind(B2_c, B2_w);
		adder3.cin.bind(cout2_c, cout2_w);
		adder3.sum.bind(S2_c, S2_w);
		adder3.cout.bind(cout3_c, cout3_w);

		fa_comp adder4("adder4");
		adder4.a.bind(A3_c, A3_w);
		adder4.b.bind(B3_c, B3_w);
		adder4.cin.bind(cout3_c, cout3_w);
		adder4.sum.bind(S3_c, S3_w);
		adder4.cout.bind(COUT_s_c, COUT_s_w);

		driver_comp d1("d1");
		d1.B0.bind(B0_c, B0_w);
		d1.B1.bind(B1_c, B1_w);
		d1.B2.bind(B2_c, B2_w);
		d1.B3.bind(B3_c, B3_w);
		d1.A0.bind(A0_c, A0_w);
		d1.A1.bind(A1_c, A1_w);
		d1.A2.bind(A2_c, A2_w);
		d1.A3.bind(A3_c, A3_w);
		d1.CIN_s.bind(CIN_s_c, CIN_s_w);

		monitor_comp m1("m1");
		m1.S0.bind(S0_c, S0_w);
		m1.S1.bind(S1_c, S1_w);
		m1.S2.bind(S2_c, S2_w);
		m1.S3.bind(S3_c, S3_w);
		m1.COUT_s.bind(COUT_s_c, COUT_s_w);

		// Initializes the simulator and time resolution in the entire simulation.
		UInt32 numOfLogicalProcessors = 1;
		psml_simulator sim = psml_simulator(numOfLogicalProcessors);
		// This must be come after psml_simulator.
		psml_time duration(100, PSML_NS);
		//sim.start();
		sim.start(duration);
	}
	catch(Exception &e)
	{
		cout << "Caught exception: " << e.get_Message().get_BaseStream() << endl;
		exit(-1);
	}
	catch(...)
	{
		cout << "Unknown exception caught" << endl;
		exit(-1);
	}

	return 0;
}
//---------------------------------------