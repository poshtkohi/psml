/**
    #define meta ...
    printf("%s\n", meta);
**/

#include "stage_comp.h"

extern psml_event e1;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            stage_comp::stage_comp(const String &name)
            {
				SetName(name);

				p1 = register_process(Wrapper_To_Call_process, false);
				//p1 = register_process(Wrapper_To_Call_process, false);
				p1->register_port(in_process);
				p1->register_port(out_process);
				p1->register_sensitivity(in_process);
				//e1.register_subscriber(p1);

				//p2 = register_process(Wrapper_To_Call_process1, this);
            }
            //----------------------------------------------------
            stage_comp::~stage_comp()
            {
				// De-allocates in and out ports.
				//delete in_process;
            }
			//----------------------------------------------------
			void stage_comp::process(psml_process *ownerProcess)
			{
				//cout << "stage_comp::process()" << endl;

				Int32 counter = in_process->fetch();

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in stage_comp::process() received " << counter << endl;

				psml_time signal_delay(1, PSML_NS);

				out_process->put(counter, signal_delay, ownerProcess);

				/*while(true)
				{
					std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " sleep in stage_comp::process() due to wait(e1)" << endl;

					std::cout << "in_process.fetch(): " << in_process->fetch() << endl;

					psml_wait(e1, ownerProcess);

					std::cout << "@time " << psml_time_stamp(ownerProcess) \
						<< " wakeup in stage_comp::process() due to wait(e1)" << endl;
				}*/
			}
			//----------------------------------------------------
			void stage_comp::Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to stage_comp
				stage_comp *mySelf = (stage_comp *)owner;
				// call member
				mySelf->process(ownerProcess);
			}
			//----------------------------------------------------
			/*void stage_comp::process1(void)
			{
				cout << "stage_comp::process1()" << endl;
			}
			//----------------------------------------------------
			void stage_comp::Wrapper_To_Call_process1(Object *obj)
			{
				// explicitly cast to a pointer to stage_comp
				stage_comp *mySelf = (stage_comp *)obj;
				// call member
				mySelf->process1();
			}*/
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
