/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "aes_cipher_psml.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		namespace Samples
		{
			//----------------------------------------------------
			aes_comp::aes_comp(const String &name)
			{
				set_name(name);

				register_proxy(clk, _clk);
				for(register Int32 i = 0 ; i < SIZE ; i++)
				{
					register_proxy(idata[i], _idata[i]);
					register_proxy(ikey[i], _ikey[i]);
					register_proxy(odata[i], _odata[i]);
				}

#ifdef __FiberLess_Enabled__
                p = register_process(run, false, "aes_comp::run");
#else
                p = register_special_process(run, &_run_process_state, "aes_comp::run");
#endif
				p->register_port(_clk);
				for(register Int32 i = 0 ; i < SIZE ; i++)
				{
					p->register_port(_idata[i]);
					p->register_port(_ikey[i]);
					p->register_port(_odata[i]);
				}
				p->register_sensitivity(_clk, PSML_POS_EDGE);
                p->set_delay(psml_time(0, PSML_NS));
				///p->dont_initialize();///

				counter = 0;
				//memset(data, 0, sizeof(Int32) * SIZE);
				//memset(data_out, 0, sizeof(Int32) * SIZE);
			}
			//----------------------------------------------------
			aes_comp::~aes_comp()
			{
			}
			//----------------------------------------------------
			void aes_comp::run(psml_process *ownerProcess)
			{
                aes_comp *my = (aes_comp *)ownerProcess->get_owner_component();

				//  const  unsigned char keys[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
				//                      0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
				//const unsigned char init[]={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
				//                      0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};

				const Int32  Sbox[256] = {
					0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
						0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
						0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
						0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
						0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
						0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
						0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
						0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
						0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
						0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
						0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
						0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
						0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
						0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
						0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
						0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
				};

#ifdef __FiberLess_Enabled__
				UInt8 key[32];
				Int32 i, j;
				Int32 wrk;

				/**** Read in Key  ****/
				for(i = 0 ; i < SIZE ; i++)
				{
					key[i] = my->_ikey[i]->fetch();
				}

				/**** Read in data to be encrypted  ****/
				for(i = 0 ; i < SIZE ; i++)
				{
					my->init[i] = my->_idata[i]->fetch();
				}

				/**** Key expansion****/
				my->KeyExpansion(key, Sbox);

				/**** Data conversion  *****/
				for( i = 0 ; i < NBb/4 ; i ++)
				{
					wrk = 0;
					for(j = 3 ; j >= 0 ; j--)
					{
						wrk = (wrk << 8) | my->init[i*4+j];
					}
					my->data[i] = wrk;
				}


				/**** Encrypt data   *****/
				my->Cipher(my->data, Sbox);


				/*** Output data ****/
				for(i = 0 ; i < NB ; i++)
				{
					for(j = 0 ; j < NB ; j++)
					{
						my->_odata[(i*NB)+j]->put(my->data[i] >> (j * 8), ownerProcess);
					}
				}
#else
                aes_comp::run_process_state *state = (aes_comp::run_process_state *)ownerProcess->get_process_state();
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
                    /**** Read in Key  ****/
                    for(state->i = 0 ; state->i < SIZE ; state->i++)
                    {
                        state->key[state->i] = my->_ikey[state->i]->fetch();
                    }

                    /**** Read in data to be encrypted  ****/
                    //std::cout << "@time " << psml_time_stamp(ownerProcess);\
                    std::cout << " in run() ";
                    for(state->i = 0 ; state->i < SIZE ; state->i++)
                    {
                        my->init[state->i] = my->_idata[state->i]->fetch();
                        //std::cout << (UInt32)my->init[state->i];
                    }
                    //std::cout << std::endl;

                    /**** Key expansion****/
                    my->KeyExpansion(state->key, Sbox);

                    /**** Data conversion  *****/
                    for(state->i = 0 ; state->i < NBb/4 ; state->i++)
                    {
                        state->wrk = 0;
                        for(state->j = 3 ; state->j >= 0 ; state->j--)
                        {
                            state->wrk = (state->wrk << 8) | my->init[state->i*4+state->j];
                        }
                        my->data[state->i] = state->wrk;
                    }


                    /**** Encrypt data   *****/
                    my->Cipher(my->data, Sbox);


                    /*** Output data ****/
                    for(state->i = 0 ; state->i < NB ; state->i++)
                    {
                        for(state->j = 0 ; state->j < NB ; state->j++)
                        {
                            my->_odata[(state->i*NB)+state->j]->put(my->data[state->i] >> (state->j * 8), ownerProcess);
                        }
                    }

                    psml_wait(ownerProcess); state->set_label(1); return; L1: {}
                }
#endif
			}
			//----------------------------------------------------
			Int32 aes_comp::Cipher(Int32 *data, const Int32 Sbox[256])
			{
				Int32 i;

				AddRoundKey(data,0);

				for(i=1;i<nr;i++)
				{
					SubBytes(data, Sbox);
					ShiftRows(data);
					MixColumns(data);
					AddRoundKey(data,i);
				}

				SubBytes(data, Sbox);
				ShiftRows(data);
				AddRoundKey(data,i);
				return(i);
			}
			//----------------------------------------------------
			void aes_comp::SubBytes(Int32 *data, const Int32 Sbox[256])
			{
				Int32 i,j;
				unsigned char cb[4];
				//  unsigned char *cb=(unsigned char*)data;
				Int32 wrk;

				for(i=0;i<NBb;i+=4)
				{
					wrk = data[i/4];

					for(j=0;j<4;j++)
					{
						cb[j] = Sbox[(wrk >> ((3-j)*8)) & 0xff];

					}

					for(j=0;j<4;j++) {
						wrk = (wrk << 8) | cb[j];

					}

					data[i/4] = wrk;
				}
			}
			//----------------------------------------------------
			void aes_comp::ShiftRows(Int32 *data)
			{
				Int32 i, j;
				Int32 cb[NB];

				for (i = 0; i < NB; i ++) {
					cb[i] = 0;
					for (j = 3; j >= 0; j --) {
						cb[i] = (cb[i] << 8) | ((data[(i+j)&3] >> (j*8)) & 0xff);
					}

				}

				for (i = 0; i < NB; i ++) {
					data[i] = cb[i];
				}
			}
			//----------------------------------------------------
			Int32 aes_comp::mul(Int32 dt,Int32 n)
			{
				Int32 i,x=0;
				for(i=8;i>0;i>>=1)
				{
					x <<= 1;
					if(x&0x100)
						x = (x ^ 0x1b) & 0xff;
					if((n & i))
						x ^= dt;
				}
				return(x);
			}
			//----------------------------------------------------
			Int32 aes_comp::dataget(Int32 *data,Int32 n)
			{
				Int32 ret;

				ret = (data[(n>>2)] >> ((n & 0x3) * 8)) & 0xff;
				return (ret);
			}
			//----------------------------------------------------
			void aes_comp::MixColumns(Int32 *data)
			{
				Int32 i,i4,x;

				for(i=0;i<NB;i++)
				{
					i4 = i*4;
					x  =  mul(dataget(data,i4+0),2) ^
						mul(dataget(data,i4+1),3) ^
						mul(dataget(data,i4+2),1) ^
						mul(dataget(data,i4+3),1);
					x |= (mul(dataget(data,i4+1),2) ^
						mul(dataget(data,i4+2),3) ^
						mul(dataget(data,i4+3),1) ^
						mul(dataget(data,i4+0),1)) << 8;
					x |= (mul(dataget(data,i4+2),2) ^
						mul(dataget(data,i4+3),3) ^
						mul(dataget(data,i4+0),1) ^
						mul(dataget(data,i4+1),1)) << 16;
					x |= (mul(dataget(data,i4+3),2) ^
						mul(dataget(data,i4+0),3) ^
						mul(dataget(data,i4+1),1) ^
						mul(dataget(data,i4+2),1)) << 24;
					data[i] = x;
				}
			}
			//----------------------------------------------------
			void aes_comp::AddRoundKey(Int32 *data, Int32 n)
			{
				Int32 i;

				for(i=0;i<NB/2;i++)
				{
					data[i*2] ^= w[i*2  +NB*n];
					data[i*2+1] ^= w[i*2+1+NB*n];

				}
			}
			//----------------------------------------------------
			Int32 aes_comp::SubWord(Int32 word, const Int32 Sbox[256])
			{
				Int32 inw=word;
				Int32 i;


				for (i = 3; i >= 0; i--) {
					inw = (inw<<8) | Sbox[(word>>(8*i)) & 0xff];
				}

				return(inw);
			}

			//----------------------------------------------------
			Int32 aes_comp::RotWord(Int32 word)
			{
				Int32 inw=word,inw2=0;

				inw2 = ((inw & 0xff) << 24) | ((inw >> 8) & 0x00ffffff);
				return(inw2);
			}
			//----------------------------------------------------
			void aes_comp::KeyExpansion(UInt8 *key, const Int32 Sbox[256])
			{
				Int32 Rcon[10]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};
				Int32 i,temp;
				Int32 j;

				for (i = 0; i < nk*4/4; i ++) {
					temp = 0;
					for (j = 3; j >= 0; j --) {
						temp = (temp << 8) | key[i*4+j];
					}
					w[i] = temp;
				}

				for(i=nk;i<NB*(nr+1);i++)
				{
					temp = w[i-1];
					if((i%nk) == 0)
						temp = SubWord(RotWord(temp), Sbox) ^ Rcon[(i/nk)-1];
					w[i] = w[i-nk] ^ temp;
				}
			}
			//----------------------------------------------------
		}
	}
};
//**************************************************************************************************************//
