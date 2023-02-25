/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_pipeline_fifo_example_h__
#define __Parvicursor_psml_Samples_pipeline_fifo_example_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_event.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_fifo.h>
#include <Parvicursor/psml/psml_gport.h>

using namespace Parvicursor::psml;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class pipeline_fifo_example : public psml_component
            {
                /*---------------------fields-----------------*/
				private: psml_process *source_p;
				private: psml_gport<psml_fifo<Int32> > source_out;
			    
				private: psml_process *stage_p;
				private: psml_gport<psml_fifo<Int32> > stage_in;
				private: psml_gport<psml_fifo<Int32> > stage_out;
			    
				private: psml_process *sink_p;
				private: psml_gport<psml_fifo<Int32> > sink_in;

				private: psml_fifo<Int32> fifo1;
				private: psml_connector fifo1_c;

				private: psml_fifo<Int32> fifo2;
				private: psml_connector fifo2_c;
                /*---------------------methods----------------*/
                /// pipeline_fifo_example Class constructor.
                public: pipeline_fifo_example(const String &name = "");
                /// pipeline_fifo_example Class destructor.
                public: ~pipeline_fifo_example();
				private: void source_process(psml_process *ownerProcess);
				private: void stage_process(psml_process *ownerProcess);
				private: void sink_process(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_source_process(psml_component *owner, psml_process *ownerProcess);
				private: static void Wrapper_To_Call_stage_process(psml_component *owner, psml_process *ownerProcess);
				private: static void Wrapper_To_Call_sink_process(psml_component *owner, psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

