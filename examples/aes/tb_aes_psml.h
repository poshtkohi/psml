/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_test_aes_comp_h__
#define __Parvicursor_psml_Samples_test_aes_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_clock.h>

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
            class test_aes_comp : public psml_component
            {
                /*---------------------process states---------*/
#ifndef __FiberLess_Enabled__
                private: class send_process_state : public psml_process_state
                {
                    public: Int32 i;
                    public: Int32 next_input;
                    public: Vector<UInt8> *input;
                    public: send_process_state() : next_input(0), i(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                private: class recv_process_state : public psml_process_state
                {
                    public: Int32 i;
                    public: UInt8 out_write[SIZE];
                    public: recv_process_state() : i(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
#endif
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool > > clk;
				private: psml_inport<psml_wire<bool > > _clk_revc;
				private: psml_inport<psml_wire<bool > > _clk_send;

				public: psml_proxy<psml_wire<UInt8 > > odata[SIZE];
				private: psml_inport<psml_wire<UInt8 > > _odata[SIZE];

				public: psml_proxy<psml_wire<UInt8 > > ikey[SIZE];
				private: psml_outport<psml_wire<UInt8 > > _ikey[SIZE];

				public: psml_proxy<psml_wire<UInt8 > > idata[SIZE];
				private: psml_outport<psml_wire<UInt8 > > _idata[SIZE];

				public: psml_process *p1;
				public: psml_process *p2;

#ifdef __FiberLess_Enabled__
				private: Int32 next_input;
#else
                private: send_process_state _send_process_state;
                private: recv_process_state _recv_process_state;
#endif
                /*---------------------methods----------------*/
                /// test_aes_comp Class constructor.
                public: test_aes_comp(const String &name = "");
                /// test_aes_comp Class destructor.
                public: ~test_aes_comp();
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

