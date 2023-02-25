/**
    #define meta ...
    printf("%s\n", meta);
**/

#ifndef __Parvicursor_psml_Samples_monitor_comp_h__
#define __Parvicursor_psml_Samples_monitor_comp_h__

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
            class monitor_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool> > COUT_s, S0, S1, S2, S3;
				private: psml_inport<psml_wire<bool> > _COUT_s, _S0, _S1, _S2, _S3;
				private: psml_process *p;
                /*---------------------methods----------------*/
                /// monitor_comp Class constructor.
                public: monitor_comp(const String &name = "");
                /// monitor_comp Class destructor.
                public: ~monitor_comp();
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

