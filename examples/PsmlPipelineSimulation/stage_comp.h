/**
    #define meta ...
    printf("%s\n", meta);
**/

#ifndef __Parvicursor_psml_Samples_stage_comp_h__
#define __Parvicursor_psml_Samples_stage_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_event.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>

using namespace Parvicursor::psml;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class stage_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_inport<psml_wire<Int32> > in_process;
				public: psml_outport<psml_wire<Int32> > out_process;

				private: psml_process *p1;
				//private: psml_process *p2;
                /*---------------------methods----------------*/
                /// stage_comp Class constructor.
                public: stage_comp(const String &name = "");
                /// stage_comp Class destructor.
                public: ~stage_comp();
				public: void process(psml_process *ownerProcess);
				public: static void Wrapper_To_Call_process(psml_component *ownerComponent, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

