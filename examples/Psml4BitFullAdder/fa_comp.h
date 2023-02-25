/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_fa_comp_h__
#define __Parvicursor_psml_Samples_fa_comp_h__

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
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_connector.h>

using namespace Parvicursor::psml;

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class fa_comp : public psml_component
            {
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool > > a, b, cin;
				public: psml_proxy<psml_wire<bool> > sum, cout;
				private: psml_inport<psml_wire<bool > > _a, _b, _cin;
				private: psml_outport<psml_wire<bool> > _sum, _cout;
				private: psml_process *p;

				/*private: psml_process *test_p;
				private: psml_inport<psml_wire<bool > > test_in;
				private: psml_connector test_c;
				private: psml_wire<bool> test_w;*/
                /*---------------------methods----------------*/
                /// fa_comp Class constructor.
                public: fa_comp(const String &name = "");
                /// fa_comp Class destructor.
                public: ~fa_comp();
				private: void process(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess);

				/*private: void test_process(psml_process *ownerProcess);
				private: static void Wrapper_To_Call_test_process(psml_component *owner, psml_process *ownerProcess);*/
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

