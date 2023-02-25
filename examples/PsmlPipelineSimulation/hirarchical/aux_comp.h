/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_aux_comp_h__
#define __Parvicursor_psml_Samples_aux_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_event.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>

using namespace Parvicursor::psml;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class aux_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<Int32> > proxy_in1;
				public: psml_proxy<psml_wire<Int32> > proxy_in2;
				private: psml_inport<psml_wire<Int32> > in1;
				private: psml_inport<psml_wire<Int32> > in2;
				private: psml_process *p3;
                /*---------------------methods----------------*/
                /// aux_comp Class constructor.
                public: aux_comp(const String &name = "");
                /// aux_comp Class destructor.
                public: ~aux_comp();
				public: void P3(psml_process *ownerProcess);
				public: static void Wrapper_To_Call_P3(psml_component *owner, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

