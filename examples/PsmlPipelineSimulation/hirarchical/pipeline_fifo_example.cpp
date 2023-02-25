/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "pipeline_fifo_example.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            pipeline_fifo_example::pipeline_fifo_example(const String &name)
            {
				SetName(name);

				source_p = register_process(Wrapper_To_Call_source_process, true);	// A stackful process
				source_p->register_port(source_out);
				source_out.bind(fifo1_c, fifo1);

				stage_p = register_process(Wrapper_To_Call_stage_process, true);	// A stackful process
				stage_p->register_port(stage_in);
				stage_p->register_port(stage_out);
				stage_in.bind(fifo1_c, fifo1);
				stage_out.bind(fifo2_c, fifo2);

				sink_p = register_process(Wrapper_To_Call_sink_process, true);		// A stackful process
				sink_p->register_port(sink_in);
				sink_in.bind(fifo2_c, fifo2);

				// psml_event binding.
				psml_event *fifo1_e = fifo1.get_event();
				fifo1_e->register_initiator(source_p);
				fifo1_e->register_subscriber(stage_p);

				psml_event *fifo2_e = fifo2.get_event();
				fifo2_e->register_initiator(stage_p);
				fifo2_e->register_subscriber(sink_p);
            }
            //----------------------------------------------------
            pipeline_fifo_example::~pipeline_fifo_example()
            {
            }
			//----------------------------------------------------
			void pipeline_fifo_example::source_process(psml_process *ownerProcess)
			{
				//cout << "pipeline_fifo_example::source_process()" << endl;
				Int32 counter = 1;
				psml_time delay(2, PSML_NS);
				psml_time write_delay(1, PSML_NS);

				while(true)
				{
					source_out->nb_write(counter, write_delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " sleep in pipeline_fifo_example::source_process() due to wait(" << delay  << ")" << endl;

					psml_wait(delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " wakeup in pipeline_fifo_example::source_process() due to wait(" << delay  << ")" << endl;

					counter++;
				}
			}
			//----------------------------------------------------
			void pipeline_fifo_example::stage_process(psml_process *ownerProcess)
			{
				//cout << "pipeline_fifo_example::stage_process()" << endl;
				Int32 counter = 0;
				psml_time delay(2, PSML_NS);
				psml_time write_delay(1, PSML_NS);

				while(true)
				{
					stage_in->read(counter, ownerProcess);

					// If fifo is not empty
					if(counter != 0)
						stage_out->nb_write(counter, write_delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " sleep in pipeline_fifo_example::stage_process() due to wait(" << delay  << ")" << endl;

					psml_wait(delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " wakeup in pipeline_fifo_example::stage_process() due to wait(" << delay  << ")" << endl;
				}
			}
			//----------------------------------------------------
			void pipeline_fifo_example::sink_process(psml_process *ownerProcess)
			{
				//cout << "pipeline_fifo_example::sink_process()" << endl;
				Int32 counter = 0;
				psml_time delay(1, PSML_NS);

				while(true)
				{
					sink_in->read(counter, ownerProcess);

					// fifo is empty
					//if(counter == 0)\
						std::cout << "@time " << psml_time_stamp(ownerProcess) << "pipeline_fifo_example::sink_process() input fifo is empty" << endl;*/

					if(counter != 0)\
						std::cout << "@time " << psml_time_stamp(ownerProcess) << " counter " << counter << endl;


					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " sleep in pipeline_fifo_example::sink_process() due to wait(" << delay  << ")" << endl;

					psml_wait(delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " wakeup in pipeline_fifo_example::sink_process() due to wait(" << delay  << ")" << endl;
				}
			}
			//----------------------------------------------------
			void pipeline_fifo_example::Wrapper_To_Call_source_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to pipeline_fifo_example
				pipeline_fifo_example *mySelf = (pipeline_fifo_example *)owner;
				// call member
				mySelf->source_process(ownerProcess);
			}
			//----------------------------------------------------
			void pipeline_fifo_example::Wrapper_To_Call_stage_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to pipeline_fifo_example
				pipeline_fifo_example *mySelf = (pipeline_fifo_example *)owner;
				// call member
				mySelf->stage_process(ownerProcess);
			}
			//----------------------------------------------------
			void pipeline_fifo_example::Wrapper_To_Call_sink_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to pipeline_fifo_example
				pipeline_fifo_example *mySelf = (pipeline_fifo_example *)owner;
				// call member
				mySelf->sink_process(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
