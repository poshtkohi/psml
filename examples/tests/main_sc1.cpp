#define FILTER_TAPS 10

#include <systemc.h>

#include <vector>
#include <iostream>
using namespace std;
std::vector<sc_uint<8> > input_data;
sc_uint<16> input_coef[FILTER_TAPS] = {12, 34, 65, 3, 2, 4, 56, 4, 3, 34};
//-----------------------------------------------
class dut : sc_module
{
public:
    SC_HAS_PROCESS(dut);

    sc_in<bool> terminated;
    sc_in<sc_uint<8> > in_data[FILTER_TAPS - 1];
    sc_out< sc_uint<8> > filter_output ;

    dut(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(process);
	}

	void process()
	{
        // Variables declaration
        sc_uint<8> filter_output_function;
        sc_uint<8> in_data_read[9];
        sc_uint<16> coeff_read[9] ;
        int i;

		while(true)
		{
            if(terminated.read())
                break;

            for(i = 0 ; i < FILTER_TAPS - 1 ; i++)
            {
                in_data_read[i] = in_data[i].read();
                coeff_read[i] = input_coef[i];
            }

            // Filter function
            filter_output_function = fir_filter(in_data_read, coeff_read);

            filter_output.write(filter_output_function);

			wait(1, SC_NS);
		}
	}

    // Filter function
    sc_uint<8> fir_filter( sc_uint<8>  *ary, sc_uint<16>  *coeff)
    {
        int sop=0;
        sc_uint <8> filter_result ;
        int i ;

        // Sum of product (SOP) generation
        for(i = 0 ; i< FILTER_TAPS - 1 ; i++)
        {
            sop += ary[i] * coeff[i] ;
        }

        // Sign adjustment and rounding to sc_unit <8>)
        if ( sop < 0 ){
            sop = 0 ;
        }

        filter_result = sc_uint<8>(sop);

        return filter_result;
    }
};
//-----------------------------------------------
class tb : sc_module
{
public:
    SC_HAS_PROCESS(tb);

    sc_in<sc_uint<8> > filter_output;
    sc_out<sc_uint<8> > in_data[FILTER_TAPS - 1];
    sc_out<bool> terminated;
    sc_signal<bool> _terminated;

    tb(sc_module_name mn): sc_module(mn)
	{
		SC_THREAD(send_process);
		SC_THREAD(receive_process);
	}

	void send_process()
	{
        int i;
        unsigned int counter = 0;
        unsigned int counter1 = 0;
        terminated.write(false);
        _terminated.write(false);

		while(true)
		{
            //if(counter >= input_data.size())
            //    break;

            if(counter1 >= 10000000)
				break;
            if(counter == input_data.size())
				counter = 0;

            for(i = 0 ; i < FILTER_TAPS - 1 ; i++)
            {
                //in_data[i].write(input_data[counter + i]);
                //counter++;
                in_data[i].write(input_data[i] & counter);
            }

            counter1++;
            counter++;

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
        unsigned int filter_out_write = 0;

		while(true)
		{
            if(terminated.read())
                break;

            filter_out_write = filter_output.read();

            ///std::cout << "@time " << sc_time_stamp() << " " << filter_out_write << std::endl;

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
    sc_signal<sc_uint<8> >  in_data[FILTER_TAPS - 1];
    sc_signal<sc_uint<8> > filter_output;

    top(sc_module_name mn = ""): sc_module(mn), d("d"), t("t")
	{
	}

    void before_end_of_elaboration()
    {
        for(int x = 0 ; x < FILTER_TAPS - 1 ; x++)
        {
            d.in_data[x].bind(in_data[x]);
            t.in_data[x].bind(in_data[x]);
        }

        d.terminated.bind(terminated);
        d.filter_output.bind(filter_output);

        t.terminated.bind(terminated);
        t.filter_output.bind(filter_output);
    }
};
//-----------------------------------------------
void build_inputs()
{
	ifstream *reader = new ifstream("fir_in_data.txt", ios::in);
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
	top tps[96];

	//sc_start(100, SC_NS);
	sc_start();

	return 0;
}
//----------------------------------------------------
