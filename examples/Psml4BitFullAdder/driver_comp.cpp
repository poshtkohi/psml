/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "driver_comp.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            driver_comp::driver_comp(const String &name)
            {
				SetName(name);

				register_proxy(A0, _A0);
				register_proxy(A1, _A1);
				register_proxy(A2, _A2);
				register_proxy(A3, _A3);
				register_proxy(B0, _B0);
				register_proxy(B1, _B1);
				register_proxy(B2, _B2);
				register_proxy(B3, _B3);
				register_proxy(CIN_s, _CIN_s);

				p = register_process(Wrapper_To_Call_process, true, "process");
				p->register_port(_A0);
				p->register_port(_A1);
				p->register_port(_A2);
				p->register_port(_A3);
				p->register_port(_B0);
				p->register_port(_B1);
				p->register_port(_B2);
				p->register_port(_B3);
				p->register_port(_CIN_s);
            }
            //----------------------------------------------------
            driver_comp::~driver_comp()
            {
            }
			//----------------------------------------------------
			inline bool toBool(char c)
			{
				if(c == '1')
					return true;
				else
					return false;
			}
			//----------------------------------------------------
			void driver_comp::process(psml_process *ownerProcess)
			{
				//cout << "driver_comp::P0()" << endl;
				Int32 counter = 0;
				psml_time delay(5, PSML_NS);
				psml_time wire_delay(0, PSML_NS);
				
				const Int32 patternNum = 10;
				static const char patterns[patternNum][9+1] = {"010100011", "101101011", "001111001", "110101000", "000000000", "101001110", "101010101", "100001010", "001100110", "111111111"};

				while(true)
				{
					//cout << "@time " << psml_time_stamp(ownerProcess) << " counter: " << counter << endl;

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " sleep in driver_comp::P0() due to wait(" << delay  << ")" << endl;
					
					_A0->put(toBool(patterns[counter][0]), wire_delay, ownerProcess);
					_A1->put(toBool(patterns[counter][1]), wire_delay, ownerProcess);
					_A2->put(toBool(patterns[counter][2]), wire_delay, ownerProcess);
					_A3->put(toBool(patterns[counter][3]), wire_delay, ownerProcess);
					_B0->put(toBool(patterns[counter][4]), wire_delay, ownerProcess);
					_B1->put(toBool(patterns[counter][5]), wire_delay, ownerProcess);
					_B2->put(toBool(patterns[counter][6]), wire_delay, ownerProcess);
					_B3->put(toBool(patterns[counter][7]), wire_delay, ownerProcess);
					_CIN_s->put(toBool(patterns[counter][8]), wire_delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) << " pattern: " << patterns[counter] << endl;

					//std::cout << "Driver @time " << psml_time_stamp(ownerProcess);\
					std::cout << " A3 A2 A1 A0 " << patterns[counter][3] << patterns[counter][2] << patterns[counter][1] << patterns[counter][0];\
					std::cout << " | B3 B2 B1 B0 " << patterns[counter][7] << patterns[counter][6] << patterns[counter][5] << patterns[counter][4];\
					std::cout << " CIN_s " << patterns[counter][8] << endl;
					//std::cout << " A B " << a << " " << b << endl;

					
					psml_wait(delay, ownerProcess);

					//std::cout << "@time " << psml_time_stamp(ownerProcess) \
					<< " wakeup in driver_comp::P0() due to wait(" << delay  << ")" << endl;

					counter++;
					if(counter >= patternNum)
						counter = 0;
				}
			}
			//----------------------------------------------------
			void driver_comp::Wrapper_To_Call_process(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a pointer to driver_comp
				driver_comp *mySelf = (driver_comp *)owner;
				// call member
				mySelf->process(ownerProcess);
			}
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
