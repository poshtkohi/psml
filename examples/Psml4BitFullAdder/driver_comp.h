/**
    #define meta ...
    printf("%s\n", meta);
**/

#ifndef __Parvicursor_psml_Samples_driver_comp_h__
#define __Parvicursor_psml_Samples_driver_comp_h__

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
            class driver_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool> > A0, A1, A2, A3, B0, B1, B2, B3, CIN_s;
				private: psml_outport<psml_wire<bool> > _A0, _A1, _A2, _A3, _B0, _B1, _B2, _B3, _CIN_s;
				private: psml_process *p;
                /*---------------------methods----------------*/
                /// driver_comp Class constructor.
                public: driver_comp(const String &name = "");
                /// driver_comp Class destructor.
                public: ~driver_comp();
				public: void process(psml_process *ownerProcess);
				public: static void Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

