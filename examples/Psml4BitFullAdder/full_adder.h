#ifndef __full_adder_h__
#define __full_adder_h__

#include <systemc.h>

//---------------------------------------
SC_MODULE(full_adder)
{
	public: sc_in<bool> a, b, cin;
	public: sc_out<bool> sum, cout;
	//---------------------------------------
	public: SC_CTOR(full_adder)
	{
		SC_METHOD(process);
		sensitive << a << b << cin;
	}
    //---------------------------------------
	private: void process()
	{
		bool aANDb, aXORb, cinANDaXORb;
		aANDb = a.read() & b.read();
		aXORb = a.read() ^ b.read();
		cinANDaXORb = cin.read() & aXORb;

		//Calculate sum and carry out of the 1-BIT adder
		sum = aXORb ^ cin.read();
		cout = aANDb | cinANDaXORb;
		//next_trigger(1, SC_NS);
	 }
	 //---------------------------------------
 };
//---------------------------------------

#endif
