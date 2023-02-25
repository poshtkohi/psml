/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_memory_h__
#define __Parvicursor_psml_Samples_memory_h__

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
            // A 64-bit memory that is arranged in 64-bit words.
            class memory : public psml_component
            {
                /*---------------------process states---------*/
                private: class memory_proc_process_state : public psml_process_state
                {
                    public: memory_proc_process_state() {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                /*---------------------fields-----------------*/
                public: psml_proxy<psml_wire<bool > > clock;
				private: psml_inport<psml_wire<bool > > _clock;

                public: psml_proxy<psml_wire<bool > > re; // Read enable
				private: psml_inport<psml_wire<bool > > _re;

                public: psml_proxy<psml_wire<bool > > we; // Write enable
				private: psml_inport<psml_wire<bool > > _we;

                public: psml_proxy<psml_wire<bool > > cs; // Chip select
				private: psml_inport<psml_wire<bool > > _cs;

                public: psml_proxy<psml_wire<UInt64 > > address;
				private: psml_inport<psml_wire<UInt64 > > _address;

                public: psml_proxy<psml_wire<Int64 > > data_in;
				private: psml_inport<psml_wire<Int64 > > _data_in;

                public: psml_proxy<psml_wire<Int64 > > data_out;
				private: psml_outport<psml_wire<Int64 > > _data_out;

				public: psml_process *p;
                private: memory_proc_process_state _memory_proc_process_state;

                private: Int64 *array;
                private: Int32 size;
                /*---------------------methods----------------*/
                public: memory(const String &name_ = "", Int32 size_ = MemorySize);
                public: ~memory();
                private: static void memory_proc(psml_process *ownerProcess);
                private: void print_memory();
                private: void print_ports(psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

