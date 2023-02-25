#include "des.h"
#include "tb.h"

#include "CorePartitioner.h"
using namespace Parvicursor::psml;

#include <vector>
using namespace std;

//---------------------------------------
extern UInt64 numberOfActivatedProcesss;
std::vector<psml_uint<64> > __patterns__;
psml_uint<64> __input_key__ = "17855376605625100923";
std::vector<psml_core *> *__psml_cores__ = new std::vector<psml_core *>;
//---------------------------------------
void BuildInputs()
{
     ifstream *reader = new ifstream("patterns.txt", ios::in);
     if(!reader->is_open())
     {
            std::cout << "Could not open the file patterns.txt" << std::endl;
            abort();

     }
     std::string line;
     psml_uint<64> read;
     while(getline(*reader, line))
     {
        *reader >> read;
        __patterns__.push_back(read);
     }

     reader->close();
     delete reader;
}
//---------------------------------------
/*psml_connector clk_c;
psml_wire<bool> clk;
psml_clock *clk_comp;*/
class CoreModel
{
    psml_wire<bool > reset;
    psml_connector reset_c;
    psml_wire<bool > rt_load;
    psml_connector rt_load_c;
    psml_wire<bool > rt_decrypt;
    psml_connector rt_decrypt_c;
    psml_wire<psml_uint<64> > rt_data_i;
    psml_connector rt_data_i_c;
    psml_wire<psml_uint<64> > rt_key;
    psml_connector rt_key_c;
    psml_wire<psml_uint<64> > rt_data_o;
    psml_connector rt_data_o_c;
    psml_wire<bool > rt_ready;
    psml_connector rt_ready_c;
    psml_connector clk_c;
    psml_wire<bool> clk;
    des *de1;
    tb *tb1;
    psml_clock *clk_comp;
    public: CoreModel()
    {
        psml_core *newCore = new psml_core();
		__psml_cores__->push_back(newCore);

		//if(__psml_cores__->size() == 1)
        //    newCore->add_process(clk_comp->get_process());

        psml_time period(2, PSML_NS);
        double duty_cycle = 0.5;
        psml_time start_time(0, PSML_NS);
        bool posedge_first = true;
        clk_comp = new psml_clock("clk", period, duty_cycle, start_time, posedge_first);
        clk_comp->out.bind(clk_c, clk);
        newCore->add_process(clk_comp->get_process());

        de1 = new des("des", newCore);
        tb1 = new tb("tb", newCore);

        de1->clk.bind(clk_c, clk);
        de1->reset.bind(reset_c, reset);
        de1->load_i.bind(rt_load_c, rt_load);
        de1->decrypt_i.bind(rt_decrypt_c, rt_decrypt);
        de1->data_i.bind(rt_data_i_c, rt_data_i);
        de1->key_i.bind(rt_key_c, rt_key);
        de1->data_o.bind(rt_data_o_c, rt_data_o);
        de1->ready_o.bind(rt_ready_c, rt_ready);

        tb1->clk.bind(clk_c, clk);
        tb1->rt_des_data_i.bind(rt_data_o_c, rt_data_o);
        tb1->rt_des_ready_i.bind(rt_ready_c, rt_ready);
        tb1->rt_load_o.bind(rt_load_c, rt_load);
        tb1->rt_des_data_o.bind(rt_data_i_c, rt_data_i);
        tb1->rt_des_key_o.bind(rt_key_c, rt_key);
        tb1->rt_decrypt_o.bind(rt_decrypt_c, rt_decrypt);
        tb1->rt_reset.bind(reset_c, reset);
    }
};

static int x;
class Test
{
    public: static int x;
};

int Test::x = 0;
//---------------------------------------
int main(int argc, char *argv[])
{
    /*Test t;
    std::cout << &t.x << std::endl;
    return 0;*/
    BuildInputs();

    //for(Int32 i = 0 ; i < 10 ; i++)
    //    std::cout << __patterns__[i].to_string(2) << std::endl;

    //return 0;

    UInt32 numOfCores = 1; // 1024
    UInt32 numOfLogicalProcessors = 1;
    Double simUntil = 100; //100000;

    // Verify two args were passed in
    if(argc == 2)
    {
        fprintf(stderr, "USAGE: ./des_psml.exe <Integer value>\n");

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

    /*psml_time period(2, PSML_NS);
    double duty_cycle = 0.5;
    psml_time start_time(0, PSML_NS);
    bool posedge_first = true;
    clk_comp = new psml_clock("", period, duty_cycle, start_time, posedge_first);
    clk_comp->out.bind(clk_c, clk);*/

    /*psml_core *newCore = new psml_core();
    __psml_cores__->push_back(newCore);
    newCore->add_process(clk_comp->get_process());*/

    /*psml_time period(2, PSML_NS);
    double duty_cycle = 0.5;
    psml_time start_time(0, PSML_NS);
    bool posedge_first = true;
    psml_clock clk_comp("", period, duty_cycle, start_time, posedge_first);
    psml_connector clk_c("clk_c");
    psml_wire<bool> clk("clk");
    clk_comp.out.bind(clk_c, clk);

    des *de1;
    tb *tb1;

    de1 = new des("des");
    tb1 = new tb("tb");

    psml_wire<bool > reset;
    psml_connector reset_c("reset_c");
    psml_wire<bool > rt_load;
    psml_connector rt_load_c("rt_load_c");
    psml_wire<bool > rt_decrypt;
    psml_connector rt_decrypt_c("rt_decrypt_c");
    psml_wire<psml_uint<64> > rt_data_i;
    psml_connector rt_data_i_c("rt_data_i_c");
    psml_wire<psml_uint<64> > rt_key;
    psml_connector rt_key_c("rt_key_c");
    psml_wire<psml_uint<64> > rt_data_o;
    psml_connector rt_data_o_c("rt_data_o_c");
    psml_wire<bool > rt_ready;
    psml_connector rt_ready_c("rt_ready_c");

    de1->clk.bind(clk_c, clk);
    de1->reset.bind(reset_c, reset);
    de1->load_i.bind(rt_load_c, rt_load);
    de1->decrypt_i.bind(rt_decrypt_c, rt_decrypt);
    de1->data_i.bind(rt_data_i_c, rt_data_i);
    de1->key_i.bind(rt_key_c, rt_key);
    de1->data_o.bind(rt_data_o_c, rt_data_o);
    de1->ready_o.bind(rt_ready_c, rt_ready);

    tb1->clk.bind(clk_c, clk);
    tb1->rt_des_data_i.bind(rt_data_o_c, rt_data_o);
    tb1->rt_des_ready_i.bind(rt_ready_c, rt_ready);
    tb1->rt_load_o.bind(rt_load_c, rt_load);
    tb1->rt_des_data_o.bind(rt_data_i_c, rt_data_i);
    tb1->rt_des_key_o.bind(rt_key_c, rt_key);
    tb1->rt_decrypt_o.bind(rt_decrypt_c, rt_decrypt);
    tb1->rt_reset.bind(reset_c, reset);*/


	std::vector<CoreModel *> cores;

	for(Int32 i = 0 ; i < numOfCores ; i++)
		cores.push_back(new CoreModel());
	CorePartitioner *partitioner = null;
	partitioner = new CorePartitioner(__psml_cores__);

	psml_simulator sim = psml_simulator(numOfLogicalProcessors/*, partitioner*/);
    sim.start(simUntil, PSML_NS);

    std::cout << "\nnumberOfActivatedProcesses: " << numberOfActivatedProcesss << std::endl;

	for(Int32 i = 0 ; i < numOfCores ; i++)
	{
		delete cores[i];
		delete (*__psml_cores__)[i];
	}

	delete __psml_cores__;

	if(partitioner != null)
		delete partitioner;

    return 0;
}
//---------------------------------------
