/**
    #define meta ...
    printf("%s\n", meta);
**/

#ifndef __Parvicursor_psml_Samples_source_comp_h__
#define __Parvicursor_psml_Samples_source_comp_h__

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
            class source_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<Int32> > proxy_out;
				private: psml_outport<psml_wire<Int32> > out;
				//private: psml_connector connector_out;
				//private: psml_connector connector_out;
				private: psml_process *p0;
                /*---------------------methods----------------*/
                /// source_comp Class constructor.
                public: source_comp(const String &name = "");
                /// source_comp Class destructor.
                public: ~source_comp();
				public: void P0(psml_process *ownerProcess);
				public: static void Wrapper_To_Call_P0(psml_component *owner, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

