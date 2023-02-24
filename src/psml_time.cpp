/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_time.h"
#include "psml_process.h"
#include "psml_simulator.h"

#if !defined(PRIu64)
#   if defined(_MSC_VER) || defined(__MINGW32__)
#       define PRIu64 "I64u"
#   else
#       define PRIu64 "llu"
#   endif
#endif // PRIu64

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		static Double time_values[] =
		{
			1,       // fs
			1e3,     // ps
			1e6,     // ns
			1e9,     // us
			1e12,    // ms
			1e15     // s
		};

		static const char *time_units[] =
		{
			"fs",
			"ps",
			"ns",
			"us",
			"ms",
			"s"
		};
		Double psml_time::global_time_resolution = 1000.0;// default 1 ps
		psml_time_unit psml_time::global_time_unit = PSML_FS;
		UInt64 psml_time::global_default_time_unit = 1000;
		bool psml_time::global_time_resolution_specified = false;
		//----------------------------------------------------
		psml_time::psml_time(Double v, psml_time_unit tu) : _m_value(0)
		{
			if( v != 0 )
			{
				Double scale_fac = time_values[tu] / global_time_resolution;
				// Linux bug workaround; don't change next two lines
				volatile Double tmp = v * scale_fac + 0.5;
				_m_value = static_cast<UInt64>(tmp);
			}
		}
		//----------------------------------------------------
		psml_time::psml_time(UInt64 v) : _m_value(0)
		{
			if( v != 0 )
			{
				_m_value = v;
			}
		}
		//----------------------------------------------------
		psml_time::~psml_time()
		{
		}
		//----------------------------------------------------
		Double psml_time::GetDefaultTimeResolution()
		{
			return time_values[0];
		}
		//----------------------------------------------------
		Double psml_time::GetTimeResolution(psml_time_unit tu)
		{
			return time_values[(UInt32)tu];
		}
		//----------------------------------------------------
		void psml_time::Print(::std::ostream& os) const
		{
			os << ToString();
		}
		//----------------------------------------------------
		const std::string psml_time::ToString() const
		{
			UInt64 val = _m_value;
			if( val == 0 )
				return std::string( "0 s" );

			UInt64 tr = static_cast<UInt64>(global_time_resolution);
			int n = 0;
			while( ( tr % 10 ) == 0 ) {
				tr /= 10;
				n ++;
			}
			assert( tr == 1 );
			while( ( val % 10 ) == 0 ) {
				val /= 10;
				n ++;
			}

            char *buf = new char[BUFSIZ];
            std::sprintf(buf, "%" PRIu64, val );
            std::string result(buf);
            delete buf;

            /*char buf[BUFSIZ];
            std::sprintf(buf, "%" PRIu64, val );
            std::string result(buf);*/

			if( n >= 15 ) {
				for( UInt32 i = n - 15; i > 0; -- i ) {
					result += "0";
				}
				result += " s";
			} else {
				for( UInt32 i = n % 3; i > 0; -- i ) {
					result += "0";
				}
				result += " ";
				result += time_units[n / 3];
			}
			return result;
		}
		//----------------------------------------------------
		psml_time psml_time::psml_simulation_time(psml_process *owner)
		{
			UInt64 lvt = owner->_lp->GetSimulationTime();
			psml_time t(lvt);
			return t;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
