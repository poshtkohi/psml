/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_qsort_comp_h__
#define __Parvicursor_psml_Samples_qsort_comp_h__

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

using namespace Parvicursor::psml;

#include <stdio.h>
#include <string.h>

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class qsort_comp : public psml_component
            {
                /*---------------------process states---------*/
#ifndef __FiberLess_Enabled__
                private: class run_process_state : public psml_process_state
                {
                    public: Int32 x;
                    public: Int32 counter;
                    public: UInt32 data[SIZE];
                    public: UInt32 data_out[SIZE];
                    public: run_process_state()
                    {
                        x = 0;
                        counter = 0;
                        memset(data, 0, sizeof(UInt32) * SIZE);
                        memset(data_out, 0, sizeof(UInt32) * SIZE);
                    }
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
#endif
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool > > clk;
				private: psml_inport<psml_wire<bool > > _clk;

				public: psml_proxy<psml_wire<UInt32 > > indata;
				private: psml_inport<psml_wire<UInt32 > > _indata;

				public: psml_proxy<psml_wire<UInt32 > > odata;
				private: psml_outport<psml_wire<UInt32 > > _odata;

				public: psml_process *p;


#ifdef __FiberLess_Enabled__
                public: Int32 counter;
				private: Int32 data[SIZE];
				private: Int32 data_out[SIZE];
#else
                private: run_process_state _run_process_state;
#endif
                /*---------------------methods----------------*/
                /// qsort_comp Class constructor.
                public: qsort_comp(const String &name = "");
                /// qsort_comp Class destructor.
                public: ~qsort_comp();
				private: static void swap(int *, int *);
				private: static void sort(UInt32 *);
				/*private: void run(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_run(psml_process *ownerProcess);*/
				private: static void run(psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

