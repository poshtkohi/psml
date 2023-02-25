// Description : ADPC: Adaptive Differential Pulse-Code | FFmpeg

#include <systemc.h>

#include <vector>
#include <iostream>
using namespace std;
std::vector<sc_uint<16> > input_data;
//-----------------------------------------------
class dut : sc_module
{
public:
    SC_HAS_PROCESS(dut);

    sc_in<bool> terminated;
    sc_in<sc_uint<16> > idata;
    sc_out<sc_uint<4> > odata;

    dut(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(process);
	}

	void process()
	{
        struct width_data {
           sc_uint<16> in_data;    // Current input data
           sc_uint<16> pre_data;    // Previous input data
           sc_uint<18> diff_data;
           sc_uint<4> enc_data;
        } width;

        sc_uint<15> step_table[89] = {
          7, 8, 9, 10, 11, 12, 13, 14, 16, 17,
          19, 21, 23, 25, 28, 31, 34, 37, 41, 45,
          50, 55, 60, 66, 73, 80, 88, 97, 107, 118,
          130, 143, 157, 173, 190, 209, 230, 253, 279, 307,
          337, 371, 408, 449, 494, 544, 598, 658, 724, 796,
          876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066,
          2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358,
          5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899,
          15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767
        };

        sc_uint<7> index;
        sc_uint<4> index_delta;
        sc_uint<15> divider;
        sc_uint<15> remainder;
        sc_uint<2> remainder1;
        bool neg_flag;
        sc_uint<19> dec_tmp;
        sc_uint<32> temp = 0;
        sc_uint<17> diff;
		while(true)
		{
            if(terminated.read())
                break;

            width.in_data = idata.read();
            divider = step_table[ index ];

            // Encode
            diff = (width.in_data - width.pre_data) & 0x0000ffff;
            if( diff[15]==1 ){
             width.diff_data = ((diff^0xffff) + 1);
             neg_flag = true;
            }
            else {
             width.diff_data = diff;
             neg_flag = false;
            }

            width.diff_data = (width.diff_data<<2);
            temp = div_mod( width.diff_data, divider );
            width.enc_data = temp.range(31,17);
            remainder = temp.range(14,0);
            remainder *=2;
            if( remainder >= divider ) width.enc_data += 1;

            // Decode in the case of overflow
            if( width.enc_data > 7 ){
             width.enc_data = 7;
             dec_tmp = width.enc_data * divider;
             remainder1 = dec_tmp.range(1,0);
             if( remainder1 >= 2 ){
                width.pre_data += (dec_tmp >> 2) + 1;
             }
            else {
                width.pre_data += (dec_tmp >> 2);
             }
            }
            else {
             width.pre_data = width.in_data;
            }

            // Output encoded data
            if( neg_flag == 1 ){
             odata = width.enc_data + 0x8;
            }
            else {
             odata = width.enc_data;
            }

            // Next step preparation
            index_delta = get_index_delta( width.enc_data );

            if( index==0 && index_delta==1 ){
             index = 0;
            }
            else if( index_delta==1 ) {
             index -= 1;
            }
            else {
             index += index_delta;

            }

			wait(1, SC_NS);
		}
	}

	sc_uint<4> get_index_delta( sc_uint<4> enc )
    {
       if( enc>=0 && enc<=3 ) return 1;
       else if( enc==4 ) return 2;
       else if( enc==5 ) return 4;
       else if( enc==6 ) return 6;
       else              return 8;
    }


    sc_uint<32> div_mod( sc_uint<18> numerator, sc_uint<15> denominator )
    {
       sc_uint<18> quotient=0;
       sc_uint<15> remainder;
       sc_uint<32> temp;
       sc_uint<18> d=0;
       int i;

       for ( i=17; i>=0; i-- ) {

         d = (d<<1) + numerator[i];

         if( d >= denominator ){
           d -= denominator;
           quotient += (0x1<<i);
         }
       }
       remainder = d;
       temp = (quotient<<17) + remainder;
       return temp;
    }
};
//-----------------------------------------------
class tb : sc_module
{
public:
    SC_HAS_PROCESS(tb);

    sc_out<bool> terminated;
    sc_signal<bool> _terminated;
    sc_in<sc_uint<4> > odata;
    sc_out<sc_uint<16> > idata;

    tb(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(send_process);
		SC_THREAD(receive_process);
	}

	void send_process()
	{
        unsigned int counter = 0;
        unsigned int counter1 = 0;

		while(true)
		{
            if(counter == input_data.size() )
                counter = 0;

            if(counter1 >= 1000000)
                break;

            idata.write(input_data[counter]);

            counter++;
            counter1++;

			wait(1, SC_NS);
		}

        terminated.write(true);
        _terminated.write(true);
        //wait(1, SC_NS);
        //std::cout << "send_process was terminated" << std::endl;
		//sc_stop();
	}

    void receive_process()
	{
        unsigned int outdata;

		while(true)
		{
            if(terminated.read())
                break;

            outdata = odata.read();

            //std::cout << "@time " << sc_time_stamp() << " " << outdata << std::endl;

			wait(1, SC_NS);
		}

        //std::cout << "receive_process was terminated" << std::endl;
		//sc_stop();
	}
};
//-----------------------------------------------
class top : sc_module
{
public:
    SC_HAS_PROCESS(top);

    dut d;
    tb t;
    sc_signal<bool> terminated;
    sc_signal<sc_uint<16> > idata;
    sc_signal<sc_uint<4> > odata;

    top(sc_module_name mn = ""): sc_module(mn), d("d"), t("t")
	{
	}

    void before_end_of_elaboration()
    {
        d.terminated.bind(terminated);
        d.idata.bind(idata);
        d.odata.bind(odata);

        t.terminated.bind(terminated);
        t.idata.bind(idata);
        t.odata.bind(odata);
    }
};
//-----------------------------------------------
void build_inputs()
{
	ifstream *reader = new ifstream("adpcm_input.txt", ios::in);
	if(!reader->is_open())
	{
		std::cout << "Could not open the file fir_in_data.txt" << std::endl;
		abort();
    }
	//string line;
	unsigned int read;
	while(!reader->eof())
	{
        *reader >> read;
        input_data.push_back(read);
        //std::cout << read << std::endl;
	}

	reader->close();
	delete reader;
}
//----------------------------------------------------
int sc_main(int argc, char * argv[])
{
    //for(int i = 0 ; i < 1000 ; i++)
    //    std::cout << "t" << i << ", ";
    //std::cout << std::endl; exit(0);

    build_inputs();

	sc_set_time_resolution(1, SC_NS);

	//top tp("tp");
	top tps[120];

	//sc_start(100, SC_NS);
	sc_start();

	return 0;
}
//----------------------------------------------------
