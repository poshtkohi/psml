/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_Samples_aes_comp_h__
#define __Parvicursor_psml_Samples_aes_comp_h__

#include <Parvicursor/psml/psml_component.h>
#include <Parvicursor/psml/psml_inport.h>
#include <Parvicursor/psml/psml_outport.h>
#include <Parvicursor/psml/psml_process.h>
#include <Parvicursor/psml/psml_wire.h>
#include <Parvicursor/psml/psml_time.h>
#include <Parvicursor/psml/psml_wait.h>
#include <Parvicursor/psml/psml_proxy.h>
#include <Parvicursor/psml/psml_clock.h>

#include "define_psml.h"

using namespace Parvicursor::psml;

#include <stdio.h>
#include <string.h>

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
        namespace Samples
        {
            //----------------------------------------------------
            class aes_comp : public psml_component
            {
                /*---------------------process states---------*/
#ifndef __FiberLess_Enabled__
                private: class run_process_state : public psml_process_state
                {
                    public: UInt8 key[32];
                    public: Int32 i, j;
                    public: Int32 wrk;
                    public: run_process_state() : i(0), j(0), wrk(0) {}
                    // Overridden metods
                    //public: void serialize();
                    //public: void deserialize();
                };
#endif
                /*---------------------fields-----------------*/
				public: psml_proxy<psml_wire<bool > > clk;
				private: psml_inport<psml_wire<bool > > _clk;

				public: psml_proxy<psml_wire<UInt8 > > idata[SIZE];
				private: psml_inport<psml_wire<UInt8 > > _idata[SIZE];

				public: psml_proxy<psml_wire<UInt8 > > ikey[SIZE];
				private: psml_inport<psml_wire<UInt8 > > _ikey[SIZE];

				public: psml_proxy<psml_wire<UInt8 > > odata[SIZE];
				private: psml_outport<psml_wire<UInt8 > > _odata[SIZE];

				public: psml_process *p;

				private: Int32 counter;
				private: Int32 w[60];
				private: Int32 data[NB];
				private: UInt8 keys[SIZE];
				private: UInt8 init[SIZE];
#ifdef __FiberLess_Enabled__
#else
                private: run_process_state _run_process_state;
#endif
                /*---------------------methods----------------*/
                /// aes_comp Class constructor.
                public: aes_comp(const String &name = "");
                /// aes_comp Class destructor.
                public: ~aes_comp();
				/* A */
				private: void AddRoundKey(Int32 *,Int32);
				/* C */
				private: Int32 Cipher(Int32 *, const Int32 *);
				/* D */
				private: inline Int32 dataget(Int32 *,Int32);
				/* K */
				private: void KeyExpansion(UInt8 *, const Int32 *);
				/* M */
				private: void MixColumns(Int32 *);
				private: Int32 mul(Int32, Int32);
				/* S */
				private: void ShiftRows(Int32 *);
				private: void SubBytes(Int32 *, const Int32 *);
				private: Int32 SubWord(Int32, const Int32 *);
				/* R */
				private: inline Int32 RotWord(Int32);
				private: static void run(psml_process *ownerProcess);
                /*--------------------------------------------*/
            };
            //----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//

#endif

