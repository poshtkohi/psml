/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_bus_controller_h__
#define __Parvicursor_psml_Samples_bus_controller_h__

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
            class bus_controller : public psml_component
            {
                /*---------------------process states---------*/
                private: class bus_controller_proc_process_state : public psml_process_state
                {
                    public: Int32 i;
                    public: bus_controller_proc_process_state() : i(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                /*---------------------fields-----------------*/
                public: psml_proxy<psml_wire<bool > > clock;
				private: psml_inport<psml_wire<bool > > _clock;

                public: psml_proxy<psml_wire<bool > > re_cores[NumOfCores]; // Cores Read Enable
				private: psml_inport<psml_wire<bool > > _re_cores[NumOfCores];

                public: psml_proxy<psml_wire<bool > > we_cores[NumOfCores]; // Cores Write Enable
				private: psml_inport<psml_wire<bool > > _we_cores[NumOfCores];

                public: psml_proxy<psml_wire<bool > > cs_cores[NumOfCores]; // Cores Chip Eelect
				private: psml_inport<psml_wire<bool > > _cs_cores[NumOfCores];

                public: psml_proxy<psml_wire<Int64 > > data_in_mem;
				private: psml_inport<psml_wire<Int64 > > _data_in_mem;

                public: psml_proxy<psml_wire<UInt64 > > address_cores[NumOfCores];
				private: psml_inport<psml_wire<UInt64 > > _address_cores[NumOfCores];

                public: psml_proxy<psml_wire<Int64 > > data_in_cores[NumOfCores];
				private: psml_inport<psml_wire<Int64 > > _data_in_cores[NumOfCores];

                public: psml_proxy<psml_wire<Int64 > > data_out_cores[NumOfCores];
				private: psml_outport<psml_wire<Int64 > > _data_out_cores[NumOfCores];

                public: psml_proxy<psml_wire<bool > > re_mem;   // Memory Read Enable
				private: psml_outport<psml_wire<bool > > _re_mem;

                public: psml_proxy<psml_wire<bool > > we_mem;   // Memory Write Enable
				private: psml_outport<psml_wire<bool > > _we_mem;

                public: psml_proxy<psml_wire<bool > > cs_mem;   // Memory Chip Select;
				private: psml_outport<psml_wire<bool > > _cs_mem;

                public: psml_proxy<psml_wire<UInt64 > > address_mem;
				private: psml_outport<psml_wire<UInt64 > > _address_mem;

                public: psml_proxy<psml_wire<Int64 > > data_out_mem;
				private: psml_outport<psml_wire<Int64 > > _data_out_mem;

                public: psml_process *p;
                private: bus_controller_proc_process_state _bus_controller_proc_process_state;

                //private: const int remote_access_latency = MemoryAccessLatency;
                /*---------------------methods----------------*/
                public: bus_controller(const String &name_ = "");
                public: ~bus_controller();
                private: static void bus_controller_proc(psml_process *ownerProcess);
                private: void print_ports(Int32 i, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif
