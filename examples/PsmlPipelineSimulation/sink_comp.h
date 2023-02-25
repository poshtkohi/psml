/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_sink_comp_h__
#define __Parvicursor_psml_Samples_sink_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_event.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_gport.h>
#include <Parvicursor/psml/psml_fifo.h>

#include "aux_comp.h"

using namespace Parvicursor::psml;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class sink_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<Int32> > proxy_in1;
				private: psml_inport<psml_wire<Int32> > in1;
				private: psml_outport<psml_wire<Int32> > out1;
				private: psml_process *p1;

				private: psml_proxy<psml_wire<Int32> > proxy_out2;
				private: psml_inport<psml_wire<Int32> > in2;
			    private: psml_outport<psml_wire<Int32> > out2;
				private: psml_process *p2;
				private: psml_connector c;
				private: psml_wire<Int32> c_w;

				//private: psml_fifo<Int32> fifo;
				//private: psml_connector c_fifo;
				//private: psml_gport<psml_fifo<Int32> > g1;
				//private: psml_gport<psml_fifo<Int32> > g2;

				private: psml_inport<psml_wire<Int32> > in4;
				private: psml_process *p4;


				private: aux_comp *aux;
                /*---------------------methods----------------*/
                /// sink_comp Class constructor.
                public: sink_comp(const String &name = "");
                /// sink_comp Class destructor.
                public: ~sink_comp();
				private: void P1(psml_process *ownerProcess);
				private: void P2(psml_process *ownerProcess);
				private: void P4(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_P1(psml_component *owner, psml_process *ownerProcess);
				private: static void Wrapper_To_Call_P2(psml_component *owner, psml_process *ownerProcess);
				private: static void Wrapper_To_Call_P4(psml_component *owner, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

