/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_time_h__
#define __Parvicursor_psml_psml_time_h__


#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/DateTime/DateTime.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/ArgumentException/ArgumentException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace System;

using namespace std;

//**************************************************************************************************************/

namespace Parvicursor
{
	namespace psml
	{
		enum psml_time_unit
		{
			PSML_FS = 0,
			PSML_PS,
			PSML_NS,
			PSML_US,
			PSML_MS,
			PSML_SEC
		};
		//----------------------------------------------------
		// Forward declarations.
		class psml_time;
		class psml_process;
		class psml_simulator;
		class psml_wait_internal;
		class psml_event;
		class psml_nonshared_interface;
		class psml_clock;
		template <class T> class psml_wire;

		class psml_time : public Object
		{
			friend class psml_simulator;
			friend class psml_wait_internal;
			friend class psml_event;
			friend class psml_nonshared_interface;
			friend class psml_clock;
			friend class psml_process;
			template <class T> friend class psml_wire;
			/*---------------------fields-----------------*/
			private: UInt64 _m_value;
			private: static UInt64 global_default_time_unit;		// default 1 ns
			private: static bool global_time_resolution_specified;
			private: static psml_time_unit global_time_unit; // default in femto seconds
			private: psml_time_unit time_unit;
			private: static Double global_time_resolution;	// in femto seconds
			/*---------------------methods----------------*/
			// psml_time Class constructor.
			public: psml_time(Double v, psml_time_unit tu);
			public: psml_time();
			private: psml_time(UInt64 v);
			public: psml_time(const psml_time &t);
			// psml_time Class destructor.
			public: ~psml_time();
			// Gets the default time resolution to be PSML_FS.
			public: static Double GetDefaultTimeResolution();
			// Gets the time resolution based on tu.
			public: static Double GetTimeResolution(psml_time_unit tu);
			public: static psml_time psml_simulation_time(psml_process *owner);
			public: Double ToDouble() const;  // relative to the time resolution

			// assignment operator

			public: psml_time &operator = (const psml_time &);

			// relational operators

			public: bool operator == (const psml_time &) const;
			public: bool operator != (const psml_time &) const;
			public: bool operator <  (const psml_time &) const;
			public: bool operator <= (const psml_time &) const;
			public: bool operator >  (const psml_time &) const;
			public: bool operator >= (const psml_time &) const;

			// arithmetic operators

			public: psml_time &operator += (const psml_time &);
			public: psml_time &operator -= (const psml_time &);

			public: friend const psml_time operator + (const psml_time &, const psml_time &);
			public: friend const psml_time operator - (const psml_time &, const psml_time &);

			public: psml_time &operator *= (Double);
			public: psml_time& operator /= (Double);
			public: psml_time& operator %= (const psml_time &);

			public: friend const psml_time operator * (const psml_time &, Double );
			public: friend const psml_time operator * (Double, const psml_time&);
			public: friend const psml_time operator / (const psml_time &, Double);
			public: friend Double          operator / (const psml_time &, const psml_time &);
			public: friend const psml_time operator % (const psml_time &, const psml_time &);
			// print function
			public: void Print(::std::ostream& os = std::cout) const;
			public:	const std::string ToString() const;
		};

		// print operator

		inline ::std::ostream& operator << (::std::ostream&, const psml_time &);

		//----------------------------------------------------
		// assignment operator

		inline psml_time& psml_time::operator = (const psml_time &t)
		{
			_m_value = t._m_value;
			return *this;
		}

		inline Double psml_time::ToDouble() const  // relative to the time resolution
		{
			return (Double)_m_value;
		}

		// constructors
		inline psml_time::psml_time() : _m_value(0)
		{}

		inline psml_time::psml_time(const psml_time &t) : _m_value(t._m_value)
		{}


		// relational operators

		inline bool psml_time::operator == (const psml_time &t) const
		{
			return ( _m_value == t._m_value );
		}

		inline bool psml_time::operator != (const psml_time &t) const
		{
			return ( _m_value != t._m_value );
		}

		inline bool psml_time::operator < (const psml_time &t) const
		{
			return ( _m_value < t._m_value );
		}

		inline bool psml_time::operator <= (const psml_time &t) const
		{
			return ( _m_value <= t._m_value );
		}

		inline bool psml_time::operator > (const psml_time &t) const
		{
			return ( _m_value > t._m_value );
		}

		inline bool psml_time::operator >= (const psml_time &t) const
		{
			return ( _m_value >= t._m_value );
		}

		// arithmetic operators

		inline psml_time &psml_time::operator += (const psml_time &t)
		{
			_m_value += t._m_value;
			return *this;
		}

		inline psml_time &psml_time::operator -= ( const psml_time &t)
		{
			_m_value -= t._m_value;
			return *this;
		}


		inline const psml_time operator + (const psml_time &t1, const psml_time &t2)
		{
			return psml_time(t1) += t2;
		}

		inline const psml_time operator -(const psml_time& t1, const psml_time &t2)
		{
			return psml_time(t1) -= t2;
		}


		inline psml_time &psml_time::operator *= (Double d)
		{
			// Linux bug workaround; don't change next two lines
			volatile Double tmp = (Double)(_m_value) * d + 0.5;
			_m_value = static_cast<UInt64>(tmp);
			return *this;
		}

		inline psml_time &psml_time::operator /= (Double d)
		{
			// Linux bug workaround; don't change next two lines
			volatile Double tmp = (Double)(_m_value) / d + 0.5;
			_m_value = static_cast<UInt64>(tmp);
			return *this;
		}

		inline psml_time &psml_time::operator %= (const psml_time &t)
		{
			_m_value %= t._m_value;
			return *this;
		}

		inline const psml_time operator * (const psml_time &t, Double d)
		{
			psml_time tmp(t);
			return tmp *= d;
		}

		inline const psml_time operator * (Double d, const psml_time &t)
		{
			psml_time tmp(t);
			return tmp *= d;
		}

		inline const psml_time operator / (const psml_time &t, Double d)
		{
			psml_time tmp(t);
			return tmp /= d;
		}

		inline Double operator / ( const psml_time &t1, const psml_time &t2)
		{
			return ( t1.ToDouble() / t2.ToDouble() );
		}

		inline const psml_time operator % (const psml_time &t1, const psml_time &t2)
		{
			psml_time tmp(t1);
			return tmp %= t2;
		}

		// print operator

		inline ::std::ostream& operator << (::std::ostream& os, const psml_time &t)
		{
			t.Print(os);
			return os;
		}
		//----------------------------------------------------
		inline static psml_time psml_simulation_time(psml_process *owner) { return psml_time::psml_simulation_time(owner); }
		inline static psml_time psml_time_stamp(psml_process *owner) { return psml_simulation_time(owner); }
		//----------------------------------------------------
	}
};
//**************************************************************************************************************//

#endif


