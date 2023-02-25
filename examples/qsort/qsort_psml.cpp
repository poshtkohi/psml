/**
	#define meta ...
	prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "qsort_psml.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
			//----------------------------------------------------
			qsort_comp::qsort_comp(const String &name)
			{

				set_name(name);

				register_proxy(clk, _clk);
				register_proxy(indata, _indata);
				register_proxy(odata, _odata);

#ifdef __FiberLess_Enabled__
                counter = 0;
				memset(data, 0, sizeof(Int32) * SIZE);
				memset(data_out, 0, sizeof(Int32) * SIZE);
#endif

				//p = register_process(Wrapper_To_Call_run, false, "run");
#ifdef __FiberLess_Enabled__
                p = register_process(run, false, "qsort_comp::run");
#else
                p = register_special_process(run, &_run_process_state, "qsort_comp::run");
#endif
				p->register_port(_clk);
				p->register_port(_indata);
				p->register_port(_odata);
				p->register_sensitivity(_clk, PSML_POS_EDGE);
				p->set_delay(psml_time(0, PSML_NS));
				//p->dont_initialize(); ///
			}
			//----------------------------------------------------
			qsort_comp::~qsort_comp()
			{
			}
			//----------------------------------------------------
			void qsort_comp::run(psml_process *ownerProcess)
			{
				qsort_comp *my = (qsort_comp *)ownerProcess->get_owner_component();

#ifdef __FiberLess_Enabled__
				my->data[my->counter] = my->_indata->fetch();

				//std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " in run() read " << my->data[my->counter] << " write " << my->data_out[my->counter] << std::endl;

				my->_odata->put(my->data_out[my->counter], ownerProcess);

				my->counter++;

				if(my->counter == SIZE)
				{
					sort(my->data);

					for(UInt32 x = 0 ; x < SIZE ; x++)
					{
						my->data_out[x] = my->data[x];
					}

					my->counter = 0;
				}
#else
                qsort_comp::run_process_state *state = (qsort_comp::run_process_state *)ownerProcess->get_process_state();
                switch(state->get_label())
                {
                    case 0:
                        goto L0;
                        break;
                    case 1:
                        goto L1;
                        break;
                }

                psml_wait(ownerProcess); state->set_label(0); return; L0: {}

                while(true)
                {
                    for(state->x = 0 ; state->x < SIZE ; state->x++)
                    {
                        state->data[state->x] = my->_indata->fetch();
                        //std::cout << xxx<< std::endl;
                        my->_odata->put(state->data_out[state->x],  ownerProcess);

                        //std::cout << "@time " << psml_time_stamp(ownerProcess);\
                        std::cout << " in run() read " << state->data[state->x] << " write " << state->data_out[state->x] << std::endl;

                        psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                    }


                    // Main quick sort routine
                    sort(state->data);


                    for(state->x = 0 ; state->x < SIZE ; state->x++)
                    {
                        state->data_out[state->x] = state->data[state->x];
                    }
                    //  wait() ;

                    state->counter++;
                }
#endif
			}
			//----------------------------------------------------
			/*void qsort_comp::run(psml_process *ownerProcess)
			{
				psml_time wire_delay(0, PSML_NS);

				data[counter] = _indata->fetch();
				//std::cout << "@time " << psml_time_stamp(ownerProcess);\
				std::cout << " in run() " << data[x] << endl;
				_odata->put(data_out[counter], wire_delay, ownerProcess);

				counter++;

				if(counter == SIZE)
				{
					sort(data);

					for(UInt32 x = 0 ; x < SIZE ; x++)
					{
						data_out[x] = data[x];
					}

					counter = 0;
				}
			}*/
			//----------------------------------------------------
			void qsort_comp::sort(UInt32 *arr)
			{
                  // Variables declaration
                  int  piv, beg[SIZE], end[SIZE], i=0, L, R;

                  beg[0]=0;
                  end[0]=SIZE;

                  while (i>=0) {

                    L=beg[i]; R=end[i]-1;
                    if (L<R) {
                      piv=arr[L];

                      while (L<R) {
                        while (arr[R]>=piv && L<R)
                      R--;
                    if (L<R)
                      arr[L++]=arr[R];

                    while (arr[L]<=piv && L<R)
                      L++;
                    if(L<R)
                      arr[R--]=arr[L];
                      }

                      arr[L]=piv;
                      beg[i+1]=L+1;
                      end[i+1]=end[i];
                      end[i++]=L;

                      if (end[i]-beg[i]>end[i-1]-beg[i-1]){
                    swap(&end[i], &beg[i]);

                      }
                    }

                    else{
                      i--;
                    }
                  } // end while


			}
			//----------------------------------------------------
			void qsort_comp::swap(int *end, int *beg)
			{
                  int swap;

                  swap=*beg;
                  *beg=*(beg-1);
                  *(beg-1)=swap;

                  swap=*end;
                  *end=*(end-1);
                  *(end-1)=swap;
			}
			//----------------------------------------------------
			/*void qsort_comp::Wrapper_To_Call_run(psml_component *owner, psml_process *ownerProcess)
			{
				// explicitly cast to a poInt32er to qsort_comp
				qsort_comp *mySelf = (qsort_comp *)owner;
				// call member
				mySelf->run(ownerProcess);
			}*/
			//----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
