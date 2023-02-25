/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "fa_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            fa_comp::fa_comp(const String &name)
            {
				SetName(name);

				register_proxy(a, _a);
				register_proxy(b, _b);
				register_proxy(cin, _cin);
				register_proxy(sum, _sum);
				register_proxy(cout, _cout);

				p = register_process(Wrapper_To_Call_process, false, "process");
				p->register_port(_a);
				p->register_port(_b);
				p->register_port(_cin);
				p->register_sensitivity(_a);
				p->register_sensitivity(_b);
				p->register_sensitivity(_cin);
				p->register_port(_sum);
				p->register_port(_cout);
				//_sum.bind(test_c, test_w);

				/*test_p = register_process(Wrapper_To_Call_test_process, false, "test_process");
				test_p->register_port(test_in);
				test_p->register_sensitivity(test_in);
				test_in.bind(test_c, test_w);*/
            }
            //----------------------------------------------------
            fa_comp::~fa_comp()
            {
            }
			//----------------------------------------------------
			void fa_comp::process(psml_process *ownerProcess)
			{
				//std::cout << "fa_comp::p()" << endl;
				
				psml_time wire_delay(0, PSML_NS);

				bool aANDb, aXORb, cinANDaXORb;
				aANDb = _a->fetch() & _b->fetch();
				aXORb = _a->fetch() ^ _b->fetch();
				cinANDaXORb = _cin->fetch() & aXORb;

				//Calculate sum and carry out of the 1-BIT adder
				bool sum_ = aXORb ^ _cin->fetch();
				bool cout_ = aANDb | cinANDaXORb;
				_sum->put(sum_, wire_delay, ownerProcess);
				_cout->put(cout_, wire_delay, ownerProcess);

				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " in fa_comp::p received in1: " << _in1 << endl;// " g1.get_interface() " << g1.get_interface() << endl;

			}
			//----------------------------------------------------
			void fa_comp::Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to fa_comp
				fa_comp *mySelf = (fa_comp *)owner;
				// call member
				mySelf->process(ownerProcess);
			}
			//----------------------------------------------------
			/*void fa_comp::test_process(psml_process *ownerProcess)
			{
				//std::cout << "@time " << psml_time_stamp(ownerProcess) \
				<< " in fa_comp::p received in1: " << _in1 << endl;// " g1.get_interface() " << g1.get_interface() << endl;

			}
			//----------------------------------------------------
			void fa_comp::Wrapper_To_Call_test_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to fa_comp
				fa_comp *mySelf = (fa_comp *)owner;
				// call member
				mySelf->test_process(ownerProcess);
			}*/
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
