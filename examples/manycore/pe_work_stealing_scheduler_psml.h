/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_pe_work_stealing_scheduler_h__
#define __Parvicursor_psml_Samples_pe_work_stealing_scheduler_h__

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
            class pe_work_stealing_scheduler : public psml_component
            {
                /*---------------------process states---------*/
                private: class pe_work_stealing_scheduler_proc_process_state : public psml_process_state
                {
                    public: Int32 current_work;
                    public: Int32 i;
                    public: pe_work_stealing_scheduler_proc_process_state() : current_work(0), i(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
                /*---------------------fields-----------------*/
                public: psml_proxy<psml_wire<bool > > clock;
				private: psml_inport<psml_wire<bool > > _clock;

                public: psml_proxy<psml_wire<bool > > re_cores[NumOfCores]; // Read Enable
				private: psml_inport<psml_wire<bool > > _re_cores[NumOfCores];

                public: psml_proxy<psml_wire<bool > > cs_cores[NumOfCores]; // Chip Select
				private: psml_inport<psml_wire<bool > > _cs_cores[NumOfCores];

                public: psml_proxy<psml_wire<Int32 > > work_cores[NumOfCores];
				private: psml_outport<psml_wire<Int32 > > _work_cores[NumOfCores];

                public: psml_process *p;
                private: pe_work_stealing_scheduler_proc_process_state _pe_work_stealing_scheduler_proc_process_state;

                private: const Int32 numOfWorks;
                /*---------------------methods----------------*/
                public: pe_work_stealing_scheduler(const String &name_ = "", Int32 numOfWorks_ = 0);
                public: ~pe_work_stealing_scheduler();
                private: static void pe_work_stealing_scheduler_proc(psml_process *ownerProcess);
                private: void print_ports(Int32 i, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif
