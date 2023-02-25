/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_pe_matmul_h__
#define __Parvicursor_psml_Samples_pe_matmul_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_clock.h>

using namespace Parvicursor::psml;

#include <stdio.h>
#include <string.h>
#include "globals.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class pe_matmul : public psml_component
            {
                /*---------------------process states---------*/
                private: class pe_matmul_proc_process_state : public psml_process_state
                {
                    public: UInt64 base_address;
                    public: Int32 work_item;
                    public: Int32 i;
                    public: pe_matmul_proc_process_state() : base_address(0), work_item(-1), i(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                /*---------------------fields-----------------*/
                public: psml_proxy<psml_wire<bool > > clock;
				private: psml_inport<psml_wire<bool > > _clock;

                /*public: psml_proxy<psml_wire<Int64 > > data_in;
				private: psml_inport<psml_wire<Int64 > > _data_in;*/

                public: psml_proxy<psml_wire<Int32 > > work_sched;
				private: psml_inport<psml_wire<Int32 > > _work_sched;

                /*public: psml_proxy<psml_wire<bool > > re_bus_ctrl;      // Bus Controller Read Enable
				private: psml_outport<psml_wire<bool > > _re_bus_ctrl;

                public: psml_proxy<psml_wire<bool > > we_bus_ctrl;      // Bus Controller Enable
				private: psml_outport<psml_wire<bool > > _we_bus_ctrl;

                public: psml_proxy<psml_wire<bool > > cs_bus_ctrl;      // Bus Controller Chip Select
				private: psml_outport<psml_wire<bool > > _cs_bus_ctrl;*/

                public: psml_proxy<psml_wire<bool > > re_sched;         // Scheduler Read Enable
				private: psml_outport<psml_wire<bool > > _re_sched;

                public: psml_proxy<psml_wire<bool > > cs_sched;         // Scheduler Chip Select
				private: psml_outport<psml_wire<bool > > _cs_sched;

                /*public: psml_proxy<psml_wire<UInt64 > > address;
				private: psml_outport<psml_wire<UInt64 > > _address;

                public: psml_proxy<psml_wire<Int64 > > data_out;
				private: psml_outport<psml_wire<Int64 > > _data_out;*/

                public: psml_process *p;
                private: pe_matmul_proc_process_state _pe_matmul_proc_process_state;

                private: Int64 *local_memory;
                private: Int32 n;
                private: Int64 *A, *B, *C; // The matrices
                //private: const Int32 remote_access_latency = BusAccessLatency;
                /*---------------------methods----------------*/
                public: pe_matmul(const String &name_ = "", Int32 n_ = 1);
                public: ~pe_matmul();
                private: static void pe_matmul_proc(psml_process *ownerProcess);
                private: void multiply();
                private: void print_matrix(const std::string &name, Int64 *x, Int32 n);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif
