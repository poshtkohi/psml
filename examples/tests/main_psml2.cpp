// Description : ADPC: Adaptive Differential Pulse-Code | FFmpeg

#include <psml.h>

#include <vector>
#include <iostream>
using namespace std;
std::vector<psml_uint<16> > input_data;
//-----------------------------------------------
class dut : public psml_component
{
public:
    psml_proxy<psml_wire<bool > > terminated;
    psml_proxy<psml_wire<psml_uint<16> > > idata;
    psml_proxy<psml_wire<psml_uint<4> > > odata;

    psml_inport<psml_wire<bool > > terminated_;
    psml_inport<psml_wire<psml_uint<16> > > idata_;
    psml_outport<psml_wire<psml_uint<4> > > odata_;

    psml_process *p;

    class process_state : public psml_process_state
    {
    public:
        struct width_data {
           psml_uint<16> in_data;    // Current input data
           psml_uint<16> pre_data;    // Previous input data
           psml_uint<18> diff_data;
           psml_uint<4> enc_data;
        } width;
        psml_uint<7> index;
        psml_uint<4> index_delta;
        psml_uint<15> divider;
        psml_uint<15> remainder;
        psml_uint<2> remainder1;
        bool neg_flag;
        psml_uint<19> dec_tmp;
        psml_uint<32> temp;
        psml_uint<17> diff;
        process_state()
        {
            temp = 0;
        }
    };

    process_state _process_state;

    dut(const String &name = "")
	{
        set_name(name);

        register_proxy(terminated, terminated_);
        register_proxy(idata, idata_);
        register_proxy(odata, odata_);

        p = register_special_process(process, &_process_state, name + "::process");
        p->register_port(idata_);
        p->register_port(odata_);
        p->register_port(terminated_);
        p->set_delay(psml_time(0, PSML_NS));
	}

    static void process(psml_process *ownerProcess)
    {
        dut *my = (dut *)ownerProcess->get_owner_component();
        process_state *state = (process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        static psml_uint<15> step_table[89] = {
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

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            if(my->terminated_->fetch())
                break;

            state->width.in_data = my->idata_->fetch();
            state->divider = step_table[ state->index ];

            // Encode
            state->diff = (state->width.in_data - state->width.pre_data) & 0x0000ffff;
            if( state->diff[15]==1 ){
             state->width.diff_data = ((state->diff^0xffff) + 1);
             state->neg_flag = true;
            }
            else {
             state->width.diff_data = state->diff;
             state->neg_flag = false;
            }

            state->width.diff_data = (state->width.diff_data<<2);
            state->temp = my->div_mod( state->width.diff_data, state->divider );
            state->width.enc_data = state->temp.range(31,17);
            state->remainder = state->temp.range(14,0);
            state->remainder *=2;
            if( state->remainder >= state->divider ) state->width.enc_data += 1;

            // Decode in the case of overflow
            if( state->width.enc_data > 7 ){
             state->width.enc_data = 7;
             state->dec_tmp = state->width.enc_data * state->divider;
             state->remainder1 = state->dec_tmp.range(1,0);
             if( state->remainder1 >= 2 ){
                state->width.pre_data += (state->dec_tmp >> 2) + 1;
             }
            else {
                state->width.pre_data += (state->dec_tmp >> 2);
             }
            }
            else {
             state->width.pre_data = state->width.in_data;
            }

            // Output encoded data
            if( state->neg_flag == 1 ){
             my->odata_->put(state->width.enc_data + 0x8, ownerProcess);
            }
            else {
             my->odata_->put(state->width.enc_data, ownerProcess);
            }

            // Next step preparation
            state->index_delta = my->get_index_delta( state->width.enc_data );

            if( state->index==0 && state->index_delta==1 ){
             state->index = 0;
            }
            else if( state->index_delta==1 ) {
             state->index -= 1;
            }
            else {
             state->index += state->index_delta;

            }

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }

    psml_uint<4> get_index_delta( psml_uint<4> enc )
    {
       if( enc>=0 && enc<=3 ) return 1;
       else if( enc==4 ) return 2;
       else if( enc==5 ) return 4;
       else if( enc==6 ) return 6;
       else              return 8;
    }

    psml_uint<32> div_mod( psml_uint<18> numerator, psml_uint<15> denominator )
    {
       psml_uint<18> quotient=0;
       psml_uint<15> remainder;
       psml_uint<32> temp;
       psml_uint<18> d=0;
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
class tb : public psml_component
{
public:

    psml_proxy<psml_wire<psml_uint<4> > > odata;
    psml_proxy<psml_wire<psml_uint<16> > > idata;
    psml_proxy<psml_wire<bool > > terminated;

    psml_inport<psml_wire<psml_uint<4> > > odata_;
    psml_outport<psml_wire<psml_uint<16> > > idata_;
    psml_outport<psml_wire<bool > > terminated_;

    psml_connector _terminated_c;
    psml_wire<bool> _terminated_w;
    psml_inport<psml_wire<bool > > terminated_receive;
    psml_outport<psml_wire<bool > > terminated_send;

    psml_process *p1;
    psml_process *p2;

    class send_process_state : public psml_process_state
    {
    public:
        unsigned int counter;
        unsigned int counter1;
        send_process_state()
        {
            counter = 0;
            counter1 = 0;
        }
    };

    class receive_process_state : public psml_process_state
    {
    public:
        unsigned int outdata;
        receive_process_state()
        {
            outdata = 0;
        }
    };

    send_process_state _send_process_state;
    receive_process_state _receive_process_state;

    tb(const String &name = "")
	{
        set_name(name);

        register_proxy(odata, odata_);
        register_proxy(idata, idata_);
        register_proxy(terminated, terminated_);

        p1 = register_special_process(send_process, &_send_process_state, name + "::send_process");
        p1->register_port(terminated_);
        p1->register_port(terminated_send);
        p1->register_port(idata_);
        p1->set_delay(psml_time(0, PSML_NS));

        p2 = register_special_process(receive_process, &_receive_process_state, name + "::receive_process");
        p2->register_port(odata_);
        p2->register_port(terminated_receive);
        p2->set_delay(psml_time(0, PSML_NS));

        terminated_send.bind(_terminated_c, _terminated_w);
        terminated_receive.bind(_terminated_c, _terminated_w);
	}

    static void send_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        send_process_state *state = (send_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        switch(state->get_label())
        {
            case 0: goto L0;
            case 1: goto L1;
            case -2: return;
        }

        my->terminated_->put(false, ownerProcess);
        my->terminated_send->put(false, ownerProcess);

        while(true)
        {
            if(state->counter == input_data.size() )
                state->counter = 0;

            if(state->counter1 >= 1000000)
                break;

            my->idata_->put(input_data[state->counter], ownerProcess);

            state->counter++;
            state->counter1++;


            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        my->terminated_->put(true, ownerProcess);
        my->terminated_send->put(true, ownerProcess);

        psml_wait(delay, ownerProcess); state->set_label(1); return; L1: {}

        state->set_label(-2);
    }

    static void receive_process(psml_process *ownerProcess)
    {
        tb *my = (tb *)ownerProcess->get_owner_component();
        receive_process_state *state = (receive_process_state *)ownerProcess->get_process_state();
        static psml_time delay = psml_time(1, PSML_NS);

        switch(state->get_label())
        {
            case 0: goto L0;
            case -2: return;
        }

        while(true)
        {
            if(my->terminated_receive->fetch())
                break;

            state->outdata = my->odata_->fetch();

            //std::cout << "@time " << psml_time_stamp(ownerProcess) << " " << state->outdata << std::endl;

            psml_wait(delay, ownerProcess); state->set_label(0); return; L0: {}
        }

        state->set_label(-2);
    }
};
//-----------------------------------------------
class top// : public psml_component
{
public:

    dut d;
    tb t;

    psml_connector terminated_c, idata_c, odata_c;

    psml_wire<bool> terminated_w;
    psml_wire<psml_uint<16> > idata_w;
    psml_wire<psml_uint<4> > odata_w;

    top(const String &name = "") : d("d"), t("t")
	{
        //set_name(name);

        //d = new dut("d");
        //t = new tb("t");

        d.terminated.bind(terminated_c, terminated_w);
        d.idata.bind(idata_c, idata_w);
        d.odata.bind(odata_c, odata_w);

        t.terminated.bind(terminated_c, terminated_w);
        t.idata.bind(idata_c, idata_w);
        t.odata.bind(odata_c, odata_w);
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
	int read;
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
int main(int argc, char * argv[])
{
    //for(int i = 0 ; i < 1000 ; i++)
    //    std::cout << "t" << i << ", ";
    //std::cout << std::endl; exit(0);

    build_inputs();

    Int32 retError = 0;

	Int32 numOfCores =  1;  //4096;
	UInt64 simUntil = 99999999999999; //100000;
	UInt32 numOfLogicalProcessors = 1;//132;

    // Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./tests_psml.exe <Integer value>\n");

        Int32 num = atoi(argv[1]);

        if(num <= 0)
        {
            fprintf(stderr, "USAGE: %d must be >= 1\n", num);
            exit(1);
        }
        else
        {
            numOfLogicalProcessors = num;
        }
    }

	psml_set_time_resolution(1, PSML_NS);

	try
	{
        //top tp("tp");
        top tps[120];

		// Initializes the simulator and time resolution in the entire simulation.
		psml_simulator sim = psml_simulator(numOfLogicalProcessors);
		// This must be come after psml_simulator.
		psml_time duration(simUntil, PSML_NS);
		//sim.start();
		sim.start(duration);

		//cout << "\n\n\n_____numberOfActivatedProcesses: " << numberOfActivatedProcesss << endl;
	}
	catch(System::Exception &e)
	{
		cout << "Caught exception: " << e.get_Message().get_BaseStream() << endl;
		retError = -1;
		goto Cleanup;
	}
	catch(...)
	{
		retError = -1;
		cout << "Unknown exception caught" << endl;
		goto Cleanup;
	}

Cleanup:
	return retError;
}
//----------------------------------------------------
