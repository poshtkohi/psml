/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_bigint_h__
#define __Parvicursor_psml_psml_bigint_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include "psml_dt_utils.h"

#include <cryptopp/integer.h>
#include <iostream>

using namespace System;
using namespace std;
using namespace CryptoPP;
//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations

		template <Int32 W>
		class psml_bigint
		{
			/*---------------------fields-----------------*/
			private: CryptoPP::Integer _val;
			private: Int32 _len;
			/*---------------------methods----------------*/
			// psml_bigint Class constructor.
			public: inline psml_bigint()
			{
				_val = 0;
				_len = W;
			}
			private: inline psml_bigint(/*const*/ CryptoPP::Integer &v)
			{
				_val = v;
				_len = W;
			}
			public: inline psml_bigint(Int64 v)
			{
				_val = v;
				_len = W;
			}
			public: inline psml_bigint(Double v)
			{
				_val = v;
				_len = W;
			}
			public: inline psml_bigint(Int32 v)
			{
				_val = v;
				_len = W;
			}

			// Copy constructor
			public: inline psml_bigint(const psml_bigint<W> &a)
			{
				_val = a._val;
				_len = W;
			}
			//----------------------------------------------------
			// psml_bigint Class destructor.
			public: ~psml_bigint()
			{
			}
			//----------------------------------------------------
			// assignment operators
			/*public: inline psml_bigint<W>& operator = (Int64 v)
			{
				_val = v;
				return *this;
			}*/

			public: template <typename T> inline psml_bigint<W>& operator = (T v)
			{
				_val = v;
				//extend_sign();
				return *this;
			}

			public: inline psml_bigint<W>& operator = (const psml_bigint<W> &a)
			{
				_val = a._val;
				//extend_sign();
				return *this;
			}
			//----------------------------------------------------
			// reading value operator
			public: inline operator const CryptoPP::Integer& () const
			{
				return _val;
			}
			//----------------------------------------------------
			// print operator
			/*public: inline ::std::ostream& operator << (::std::ostream& os)
			{
				os << _val;
				return os;
			}*/
			//----------------------------------------------------
			// arithmetic assignment operators
			public: inline psml_bigint<W>& operator += (Int64 v)
			{
				_val += v;
				//extend_sign();
				return *this;
			}

			public: inline psml_bigint<W>& operator -= (Int64 v)
			{
				_val -= v;
				//extend_sign();
				return *this;
			}

			public: inline psml_bigint<W>& operator *= (Int64 v)
			{
				_val *= v;
				//extend_sign();
				return *this;
			}

			public: inline psml_bigint<W>& operator /= (Int64 v)
			{
				_val /= v;
				//extend_sign();
				return *this;
			}

			public: inline psml_bigint<W>& operator %= (Int64 v)
			{
				_val %= v;
				//extend_sign();
				return *this;
			}
			//----------------------------------------------------
			// ADDition, SUBtraction, MULtiplication, DIVision and MODulo operators
			public: inline friend psml_bigint<W> operator + (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val + v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator - (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val - v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator / (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val / v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator * (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val * v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator % (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val % v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			//----------------------------------------------------
			// BITWISE OPERATORS:
			// Bitwise AND, OR, XOR, LEFT SHIFT, RIGHT SHIFT, operators
			public: inline friend psml_bigint<W> operator & (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val & (v._val);
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator | (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val | v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator ^ (const psml_bigint<W> &u, const psml_bigint<W> &v)
			{
				CryptoPP::Integer result = u._val ^ v._val;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator << (const psml_bigint<W> &u, Int32 n)
			{
				CryptoPP::Integer result = u._val << n;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			public: inline friend psml_bigint<W> operator >> (const psml_bigint<W> &u, Int32 n)
			{
				CryptoPP::Integer result = u._val >> n;
				psml_bigint<W> ret = psml_bigint<W>(result);
				//extend_sign();
				return ret;
			}
			//----------------------------------------------------
			// relational operators
			public: inline friend bool operator == (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val == b._val;
			}
			public: template <typename T> inline friend bool operator == (const psml_bigint<W> &a, T b)
			{
				return a._val == b;
			}
			public: template <typename T> inline friend bool operator == (T a, const psml_bigint<W> &b)
			{
				return a == b._val;
			}

			public: inline friend bool operator != (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val != b._val;
			}
			public: template <typename T> inline friend bool operator != (const psml_bigint<W> &a, T b)
			{
				return a._val != b;
			}
			public: template <typename T> inline friend bool operator != (T a, const psml_bigint<W> &b)
			{
				return a != b._val;
			}

			public: inline friend bool operator <  (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val < b._val;
			}
			public: template <typename T> inline friend bool operator <  (const psml_bigint<W> &a, T b)
			{
				return a._val < b;
			}
			public: template <typename T> inline friend bool operator <  (T a, const psml_bigint<W> &b)
			{
				return a < b._val;
			}


			public: inline friend bool operator <= (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val <= b._val;
			}
			public: template <typename T> inline friend bool operator <= (const psml_bigint<W> &a, T b)
			{
				return a._val <= b;
			}
			public: template <typename T> inline friend bool operator <= (T a, const psml_bigint<W> &b)
			{
				return a <= b._val;
			}

			public: inline friend bool operator >  (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val > b._val;
			}
			public: template <typename T> inline friend bool operator >  (const psml_bigint<W> &a, T b)
			{
				return a._val > b;
			}
			public: template <typename T> inline friend bool operator >  (T a, const psml_bigint<W> &b)
			{
				return a > b._val;
			}

			public: inline friend bool operator >= (const psml_bigint<W> &a, const psml_bigint<W> &b)
			{
				return a._val >= b._val;
			}
			public: template <typename T> inline friend bool operator >= (const psml_bigint<W> &a, T b)
			{
				return a._val >= b;
			}
			public: template <typename T> inline friend bool operator >= (T a, const psml_bigint<W> &b)
			{
				return a >= b._val;
			}
			//----------------------------------------------------
			// bitwise assignment operators
			psml_bigint<W>& operator &= (Int64 v)
			{
				_val &= v;
				//extend_sign();
				return *this;
			}

			psml_bigint<W>& operator |= (Int64 v)
			{
				_val |= v;
				//extend_sign();
				return *this;
			}

			psml_bigint<W>& operator ^= (Int64 v)
			{
				_val ^= v;
				//extend_sign();
				return *this;
			}

			psml_bigint<W>& operator <<= (Int64 v)
			{
				_val <<= v;
				//extend_sign();
				return *this;
			}

			psml_bigint<W>& operator >>= (Int64 v)
			{
				_val >>= v;
				//extend_sign();
				return *this;
			}
			//----------------------------------------------------
			// prefix and postfix increment and decrement operators
			public: inline psml_bigint<W>& operator ++ () // prefix
			{
				++_val;
				//extend_sign();
				return *this;
			}

			public: inline const psml_bigint<W> operator ++ ( int ) // postfix
			{
				psml_bigint<W> ret = psml_bigint<W>(_val++);
				//extend_sign();
				return ret;
			}

			public: inline psml_bigint<W>& operator -- () // prefix
			{
				--_val;
				//extend_sign();
				return *this;
			}

			public: inline const psml_bigint<W> operator -- ( int ) // postfix
			{
				psml_bigint<W> ret = psml_bigint<W>(_val++);
				//extend_sign();
				return ret;
			}
			//----------------------------------------------------
			// explicit conversions
			public: inline Int32 to_int() const
			{
				Int32 result = static_cast<Int32>(_val);
				return result;
			}

			public: inline UInt32 to_uint() const
			{
				UInt32 result = static_cast<UInt32>(_val);
				return result;
			}

			public: inline long to_long() const
			{
				long result = static_cast<long>(_val);
				return result;
			}

			public: inline unsigned long to_ulong() const
			{
				unsigned long result = static_cast<unsigned long>(_val);
				return result;
			}

			public: inline Int64 to_int64() const
			{
				return _val;
			}

			public: inline UInt64 to_uint64() const
			{
				return (UInt64)_val;
			}

			public: inline Double to_double() const
			{
				Double result = static_cast<Double>(_val);
				return result;
			}
			// explicit conversion to character string
			public: inline const std::string to_string(Int32 radix = 10) const
			{
				// bounds check for radix
				if (radix < 2 || radix > 62)
					throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");

				std::string str = CryptoPP::IntToString<Integer>(_val, radix);
				return str;
			}
			//----------------------------------------------------
			public: inline bool operator [] (Int32 i)
			{
				check_index(i);
				bool bit = _val.GetBit(i);
				return bit;
			}
			//----------------------------------------------------
			private: void check_index(Int32 i) const
			{
				if(i < 0 || i >= _len)
				{
					invalid_index(i);
				}
			}
			//----------------------------------------------------
			private: void invalid_index(Int32 i) const
			{
				char msg[BUFSIZ];
				std::sprintf( msg, "sc_int[_base] bit selection: index = %d violates " "0 <= index <= %d", i, _len - 1);
				throw ArgumentOutOfRangeException("i", System::String(msg));
			}
			//----------------------------------------------------
			/*private: void check_length() const
			{
				if(_len <= 0 || _len > sizeof(Int64) * 8)
				{
					invalid_length();
				}
			}
			//----------------------------------------------------
			private: void invalid_length() const
			{
				char msg[BUFSIZ];
				std::sprintf(msg, "psml_bigint[_base] initialization: length = %d violates " "1 <= length <= %d", _len, sizeof(Int64)*8);
				throw ArgumentOutOfRangeException("W", System::String(msg));
			}
			//----------------------------------------------------
			private: inline void extend_sign()
			{
				_val = ( _val << _ulen >> _ulen );
			}*/
			//----------------------------------------------------
			private: void check_range(Int32 l, Int32 r) const
			{
				if(r < 0 || l >= _len || l < r)
				{
					invalid_range( l, r );
				}
			}
			//----------------------------------------------------
			private: void invalid_range(Int32 l, Int32 r) const
			{
				char msg[BUFSIZ];
				std::sprintf( msg, "sc_int[_base] part selection: left = %d, right = %d violates " "%d >= left >= right >= 0", l, r, _len - 1);
				throw ArgumentOutOfRangeException("range", System::String(msg));
			}
			//----------------------------------------------------
			// part selection
			public: inline const psml_bigint<W> operator () (Int32 left, Int32 right)
			{
				return range(left, right);
			}

			public: inline const psml_bigint<W> operator () (Int32 left, Int32 right) const
			{
				return range(left, right);
			}

			public: inline const psml_bigint<W> range(Int32 left, Int32 right)
			{
				check_range(left, right);

				CryptoPP::Integer new_val = psml_dt_utils::GetBits(_val, right, left - right + 1);
				return psml_bigint<W>(new_val);
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
		// print operator
		/*template <Int32 W>
		inline ::std::ostream& operator << (::std::ostream& os, const psml_bigint<W> &a)
		{
			os << a;
			return os;
		}*/
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//
//#include "psml_bigint.cpp"
#endif
