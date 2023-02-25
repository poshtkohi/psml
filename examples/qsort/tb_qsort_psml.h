/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_test_qsort_comp_h__
#define __Parvicursor_psml_Samples_test_qsort_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_clock.h>
#include <Parvicursor/psml/dt/psml_uint.h>

#include "define_psml.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace Parvicursor::psml;

using std::string;
using namespace std;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class test_qsort_comp : public psml_component
            {
                /*---------------------process states---------*/
#ifndef __FiberLess_Enabled__
                private: class send_process_state : public psml_process_state
                {
                    public: UInt32 in_read;
                    public: UInt32 next_input;
                    public: send_process_state() : next_input(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                private: class recv_process_state : public psml_process_state
                {
                    public: UInt32 out_write;
                    public: recv_process_state() {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
#endif
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool > > clk;
                private: psml_inport<psml_wire<bool > > _clk_send; // 1
				private: psml_inport<psml_wire<bool > > _clk_revc; // 0

				public: psml_proxy<psml_wire<UInt32 > > odata;
				private: psml_inport<psml_wire<UInt32 > > _odata; // 2

				public: psml_proxy<psml_wire<UInt32 > > indata;
				private: psml_outport<psml_wire<UInt32 > > _indata; // 3

				public: psml_process *p1;
				public: psml_process *p2;

#ifdef __FiberLess_Enabled__
				private: Int32 next_input;
#else
                private: send_process_state _send_process_state;
                private: recv_process_state _recv_process_state;
#endif

				//For data feeding
				//private: ifstream *reader;
                /*---------------------methods----------------*/
                /// test_qsort_comp Class constructor.
                public: test_qsort_comp(const String &name = "");
                /// test_qsort_comp Class destructor.
                public: ~test_qsort_comp();
				/*private: void recv(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_recv(psml_process *ownerProcess);
				private: void send(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_send(psml_process *ownerProcess);*/
				private: static void recv(psml_process *ownerProcess);
				private: static void send(psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

