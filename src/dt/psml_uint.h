/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_uint_h__
#define __Parvicursor_psml_psml_uint_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include "psml_dt_utils.h"
#include "../psml_globals.h"

#include <iostream>
#include <stdio.h>
#include <string>

using namespace System;

using namespace std;
//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
        class psml_uint_ref;
        class psml_uint_subref;
        template <Int8 W> class psml_uint;
		class psml_uint_base
		{
            friend class psml_uint_ref;
            friend class psml_uint_subref;
            template <Int8> friend class psml_uint;
			/*---------------------fields-----------------*/
			protected: UInt64 _val;
			protected: Int8 _len;
			protected: Int8 _ulen;
            /*---------------------methods----------------*/
            protected: inline psml_uint_base(UInt64 v, Int8 W)
			{
                initialize(v, W);
			}
            private: inline void initialize(UInt64 v, Int8 W)
			{
				_val = v;
				_len = W;
				_ulen = sizeof(UInt64) * 8 - _len;

				//check_length();
				extend_sign();

				//std::string s = std::bitset<W>(_val).to_string(); // string conversion
				//std::cout << "v " << s << std::endl;
			}
            // Copy constructor
			protected: inline psml_uint_base(const psml_uint_base &a)
			{
                //std::cout << "Hello 6" << std::endl;
                initialize(a._val, a._len);
                //std::cout << "hello 1" << std::endl;
				/*_val = a._val;
				_len = a._len;
				_ulen = sizeof(Int64) * 8 - _len;

				check_length();
				extend_sign();*/
			}
            protected: inline psml_uint_base(const psml_uint_subref &a);
			//----------------------------------------------------
			// assignment operators
            protected: inline psml_uint_base& operator = (const psml_uint_base &a)
			{
                //std::cout << "Hello 5" << std::endl;
				_val = a._val;
				extend_sign();
				return *this;
			}
            /*protected: inline psml_uint_base &operator = (const psml_uint_subref &a)
			{
                //std::cout << "Hello 5" << std::endl;
				//_val = a._val;
				//extend_sign();
				return *this;
			}*/
			//----------------------------------------------------
			protected: inline void check_index(Int8 i) const
			{
				if(i < 0 || i >= _len)
				{
					invalid_index(i);
				}
			}
			//----------------------------------------------------
			protected: void invalid_index(Int8 i) const
			{
				char msg[BUFSIZ];
				sprintf( msg, "sc_int[_base] bit selection: index = %d violates " "0 <= index <= %d", i, _len - 1);
				throw ArgumentOutOfRangeException("i", System::String(msg));
			}
			//----------------------------------------------------
			protected: inline void check_length() const
			{
                //std::cout << "_len " << _len << std::endl;

				if(_len <= 0 || _len > sizeof(UInt64) * 8)
				{
					invalid_length();
				}
			}
			//----------------------------------------------------
			protected: inline void extend_sign()
			{
				///_val = ( _val << _ulen >> _ulen ); /// for psml_int
                _val &= ( ~UINT64_ZERO >> _ulen );   /// for psml_uint
			}
			//----------------------------------------------------
			protected: inline void check_range(Int8 l, Int8 r) const
			{
                //printf("l %d r %d len %d _val %d\n", l, r, _len, _val);
				if(r < 0 || l >= _len || l < r)
				{
					invalid_range( l, r );
				}
			}
			//----------------------------------------------------
            protected: void invalid_length() const
            {
                char msg[BUFSIZ];
                sprintf(msg, "psml_uint[_base] initialization: length = %d violates " "1 <= length <= %d", _len, sizeof(Int64)*8);
                throw ArgumentOutOfRangeException("W", System::String(msg));
            }
            //----------------------------------------------------
			protected: void invalid_range(Int8 l, Int8 r) const
			{
                char msg[BUFSIZ];
                sprintf( msg, "sc_int[_base] part selection: left = %d, right = %d violates " "%d >= left >= right >= 0", l, r, _len - 1);
                throw ArgumentOutOfRangeException("range", System::String(msg));
			}
			//----------------------------------------------------
            // arithmetic operators
            public: inline psml_uint_base operator+ (const psml_uint_base &v)
            {
                return psml_uint_base(_val + v._val, _len);
            }
            /*public: inline UInt64 operator+ (UInt64 v)
            {
                return _val + v;
            }*/
            public: inline psml_uint_base operator- (const psml_uint_base &v)
            {
                return psml_uint_base(_val - v._val, _len);
            }
            /*public: inline UInt64 operator- (UInt64 v)
            {
                return _val - v;
            }*/
            public: inline psml_uint_base operator* (const psml_uint_base &v)
            {
                return psml_uint_base(_val * v._val, _len);
            }
            /*public: inline UInt64 operator* (UInt64 v)
            {
                return _val * v;
            }*/
            public: inline psml_uint_base operator/ (const psml_uint_base &v)
            {
                return psml_uint_base(_val / v._val, _len);
            }
            /*public: inline UInt64 operator/ (UInt64 v)
            {
                return _val / v;
            }*/
            public: inline psml_uint_base operator% (const psml_uint_base &v)
            {
                return psml_uint_base(_val % v._val, _len);
            }
            /*public: inline UInt64 operator% (UInt64 v)
            {
                return _val % v;
            }*/
            public: inline psml_uint_base operator& (const psml_uint_base &v)
            {
                return psml_uint_base(_val & v._val, _len);
            }
            /*public: inline UInt64 operator& (UInt64 v)
            {
                return _val & v;
            }*/
            public: inline psml_uint_base operator| (const psml_uint_base &v)
            {
                return psml_uint_base(_val | v._val, _len);
            }
            /*public: inline UInt64 operator| (UInt64 v)
            {
                return _val | v, _len;
            }*/
            public: inline psml_uint_base operator>> (const psml_uint_base &v)
            {
                return psml_uint_base(_val >> v._val, _len);
            }
            /*public: inline UInt64 operator>> (UInt64 v)
            {
                return _val >> v;
            }*/
            public: inline psml_uint_base operator << (const psml_uint_base &v)
            {
                return psml_uint_base(_val << v._val, _len);
            }
            /*public: inline UInt64 operator<< (UInt64 v)
            {
                return _val << v;
            }*/
            //----------------------------------------------------
			// arithmetic assignment operators
			public: inline psml_uint_base& operator+= (UInt64 v)
			{
				_val += v;
				extend_sign();
				return *this;
			}
			public: inline psml_uint_base& operator-= (UInt64 v)
			{
				_val -= v;
				extend_sign();
				return *this;
			}
			public: inline psml_uint_base& operator*= (UInt64 v)
			{
				_val *= v;
				extend_sign();
				return *this;
			}
			public: inline psml_uint_base& operator/= (UInt64 v)
			{
				_val /= v;
				extend_sign();
				return *this;
			}
			public: inline psml_uint_base& operator%= (UInt64 v)
			{
				_val %= v;
				extend_sign();
				return *this;
			}
            //----------------------------------------------------
			// prefix and postfix increment and decrement operators
            // There is a big difference between postfix and prefix versions of ++.
            // In the prefix version (i.e., ++i), the value of i is incremented, and the value of the expression is the new value of i.
            // In the postfix version (i.e., i++), the value of i is incremented, but the value of the expression is the original value of i.
			public: inline psml_uint_base& operator++ () // prefix
			{
				++_val;
				extend_sign();
				return *this;
			}

			public: inline const psml_uint_base operator++ ( int ) // postfix
			{
                psml_uint_base tmp(*this);
                _val++;
                extend_sign();
                return tmp;
				//return psml_uint_base(_val++, _len);
			}

			public: inline psml_uint_base& operator-- () // prefix
			{
				--_val;
				extend_sign();
				return *this;
			}

			public: inline const psml_uint_base operator-- ( int ) // postfix
			{
                psml_uint_base tmp(*this);
                _val--;
                extend_sign();
                return tmp;
				//return psml_uint_base(_val++, _len);
                //return psml_uint_base(_val--, _len);
			}
			//----------------------------------------------------
			/*public: inline friend bool operator <  (const psml_uint_base &a, const psml_uint_base &b)
			{
                //std::cout << "hello" << std::endl;
				return a._val < b._val;
			}*/
            //----------------------------------------------------
			// reading value operator
			public: inline operator UInt64 () const
			{
                //std::cout << "hello1" << std::endl;
                return _val;
			}
			//----------------------------------------------------
        };
		//----------------------------------------------------
		// Forward declarations
		template <Int8 W> class psml_uint;
		class psml_uint_ref
		{
            template <Int8> friend class psml_uint;
            private: Int8 _index;
            private: psml_uint_base *_ref;
            //----------------------------------------------------
            // reading value operator
			public: inline operator bool () const
			{
                //std::cout << "Hello3" << std::endl;
                //bool bit = psml_dt_utils::GetBits(_ref->_val, _index, 1);
                bool bit_ = _ref->_val & (UINT64_ONE << _index);
                 //eturn (_ref->_val & (1u << _index)) != 0;

                //std::cout << "_ref->_val " << _ref->_val << " bit " << bit_ << std::endl;
                //std::cout << "index " << _index << std::endl;
                //std::cout << bit_;
				return bit_;
			}
            //----------------------------------------------------
			// assignment operators
			public: inline psml_uint_ref& operator= (bool v)
			{
                //it(0);
				//_val = v;
				//std::cout << "Hello4" << std::endl;
				// https://en.wikipedia.org/wiki/Bit_manipulation
				v ? _ref->_val |= (UINT64_ONE << _index) : _ref->_val &= ~(UINT64_ONE << _index);
				//v ? m_val |= (UINT_ONE << i) : m_val &= ~(UINT_ONE << i);
				//_ref->_val ^= (-v ^ _ref->_val) & (1i64 << _index);
				///_ref->extend_sign(); /// for psml_int
				return *this;
			}
            public: inline psml_uint_ref& operator= (const psml_uint_ref &a)
			{
                //bool v = a;
                //v ? _ref->_val |= (UINT64_ONE << _index) : _ref->_val &= ~(UINT64_ONE << _index);
				return ( operator=((bool)a) );
			}
			//----------------------------------------------------
		};
        //----------------------------------------------------
		// Forward declarations
		class psml_uint_base;
		template <Int8 W> class psml_uint;
		class psml_uint_subref
		{
            friend class psml_uint_base;
            template <Int8> friend class psml_uint;
            private: Int8 _right;
            private: Int8 _left;
            private: psml_uint_base *_ref;
            //----------------------------------------------------
            // reading value operator
			/*public: inline operator psml_uint_base () const
			{
                //std::cout << "Hello 1" << std::endl;
				//Int64 new_val = psml_dt_utils::GetBits(_ref->_val, _right, _left - _right + 1);
				//std::cout << "new_val " << psml_dt_utils::Int64toBaseString(new_val, 2) << std::endl;
				Int64 new_val = _ref->_val;
				Int8 offset = _right;
				Int8 n = _left - _right + 1;
                const Int8 max_n = CHAR_BIT * sizeof(Int64);
                if(offset >= max_n)
                    return psml_uint_base(0, n); // value is padded with infinite zeros on the left
                new_val >>= offset; // drop offset bits
                if(n >= max_n)
                    return psml_uint_base(new_val, n); // all  bits requested
                const Int64 mask = (UINT64_ONE << n) - 1; // n '1's
                //std::cout << "new_val " << std::bitset<8>(_ref->_val  & mask).to_string() << std::endl;
                return psml_uint_base(new_val & mask, n);
			}*/
            //----------------------------------------------------
			// assignment operators
			public: inline psml_uint_subref& operator= (const psml_uint_subref &v);
            public: inline psml_uint_subref& operator= (const psml_uint_base &v);
			//----------------------------------------------------
		};
        //----------------------------------------------------
		// Forward declarations

		template <Int8 W>
		class psml_uint : public psml_uint_base
		{
			/*---------------------fields-----------------*/
			//private: Int64 _val;
			//private: Int8 _len;
			//private: Int8 _ulen;
			/*---------------------methods----------------*/
			// psml_uint Class constructor.
			public: inline psml_uint() : psml_uint_base(0, W)
			{
                check_length();
			}
			public: inline psml_uint(UInt64 v) : psml_uint_base(v, W)
			{
                check_length();
			}
			public: inline psml_uint(Double v) : psml_uint_base(v, W)
			{
                check_length();
			}
			public: inline psml_uint(Int32 v) : psml_uint_base(v, W)
			{
                check_length();
			}
            public: inline psml_uint(const std::string &v) : psml_uint_base(psml_dt_utils::BaseStringToUInt64(v, 10), W)
			{
                check_length();
			}
            public: inline psml_uint(const char *v) : psml_uint_base(psml_dt_utils::BaseStringToUInt64(v, 10), W)
			{
                check_length();
			}
			// Copy constructor
			public: inline psml_uint(const psml_uint_base &v) : psml_uint_base(v)
			{
			}
            public: inline psml_uint(const psml_uint_subref &v) : psml_uint_base(v)
			{
                //psml_uint_base new_base = v;
                //psml_uint_base(new_base._val, W);
			}
			//----------------------------------------------------
			// psml_uint Class destructor.
			public: ~psml_uint()
			{
			}
			//----------------------------------------------------
            // assignment operators
			/*public: inline psml_uint<W>& operator = (const psml_uint_base &a)
			{
				_val = a._val;
				extend_sign();
				return *this;
			}*/
            public: inline psml_uint<W>& operator= (const psml_uint_subref &v)
			{
                psml_uint_base new_base = v;
                _val = new_base._val;
                extend_sign();
                return *this;
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
				return (Int64)_val;
			}
			public: inline UInt64 to_uint64() const
			{
				return _val;
			}
			public: inline Double to_double() const
			{
				Double result = static_cast<Double>(_val);
				return result;
			}
			//----------------------------------------------------
			// explicit conversion to character string
			public: std::string to_string(Int8 radix = 10)
			{
                //return psml_dt_utils::Int64toBaseString(_val, radix);
                // http://stackoverflow.com/questions/7349689/how-to-print-using-cout-the-way-a-number-is-stored-in-memory
                if(radix == 2)
                {
                    char ss[64 + 1]; // 3 = 0b0
                    //std::string ss;
                    //ss.reserve(W + 1);
                    size_t nBytes = sizeof(_val);
                    char *rawPtr((char *)(&_val));
                    Int32 i = 0;
                    for(size_t byte = 0 ; byte < nBytes ; byte++)
                    {
                        for(size_t bit = 0 ; bit < CHAR_BIT ; bit++)
                        {
                            if(i == W)
                                goto End;
                            Int32 c = (((rawPtr[byte])>>bit) & 1);
                            //std::cout << "W - i - 1=" << 64 - i - 1 << std::endl;
                            if(c == 0)
                            {
                                ss[W - i - 1] = '0';
                                i++;
                            }
                            else if(c == 1)
                            {
                                ss[W - i - 1] = '1';
                                i++;
                            }
                        }
                    }
                End:

                    for(Int32 j = i ; j < W ; j++)
                       ss[j] = '0';

                    ss[W] = '\0';
                    //::reverse(ss.begin(), ss.end());
                    //std::cout << "i " << i << std::endl;
                    //for(int j = 0 ; j < W ; j++)
                    //   std::cout << ss[j];
                    //std::cout << "\nss " << ss <<std::endl;
                    return ss;
                }
                else if(radix == 10)
                {
                    std::ostringstream ss;
                    ss << std::dec << _val;
                    return ss.str();
                }
                else if(radix == 8)
                {
                    std::ostringstream ss;
                    ss << std::oct << _val;
                    return ss.str();
                }
                else if(radix == 16)
                {
                    std::ostringstream ss;
                    ss << std::hex << _val;
                    return ss.str();
                }
                else
                    return psml_dt_utils::Int64toBaseString(_val, radix);
			}
            //----------------------------------------------------
            // bit selection
			public: inline psml_uint_ref operator[] (Int8 i)
			{
                check_index(i);
                //std::cout << "i " << i << std::endl;

				psml_uint_ref ref_;
				ref_._index = i;
				ref_._ref = this;
				return ref_;
			}
            //----------------------------------------------------
			// part selection
			public: inline psml_uint_subref operator() (Int8 left, Int8 right)
			{
				return range(left, right);
			}
			public: inline psml_uint_subref operator() (Int8 left, Int8 right) const
			{
				return range(left, right);
			}
			public: inline psml_uint_subref range(Int8 left, Int8 right) const
			{
                return range(left, right);
			}
			public: inline psml_uint_subref range(Int8 left, Int8 right)
			{
                //std::cout << "Hello 2 range " << " _val " << _val << std::endl;
                check_range(left, right);
                //std::cout << "i " << i << std::endl;

				psml_uint_subref ref_;
				ref_._left = left;
				ref_._right = right;
				ref_._ref = this;
				return ref_;
			}
            //----------------------------------------------------
            public: inline friend ::std::ostream &operator<<(::std::ostream &stream, psml_uint<W> &a)
            {
                stream << a.to_string();
                return stream;
            }
            public: inline friend ::std::istream &operator>>(::std::istream &stream, psml_uint<W> &a)
            {
                UInt64 v;
                stream >> v;
                psml_uint<W> aa = psml_uint<W>(v);
                a = aa;
                return stream;
            }
			//----------------------------------------------------
		};
		//----------------------------------------------------
		psml_uint_base::psml_uint_base(const psml_uint_subref &a)
		{
            UInt64 new_val = a._ref->_val;
            Int8 offset = a._right;
            Int8 n = a._left - a._right + 1;
            //std::cout << "Hello 1 n " << n << std::endl;
            const Int8 max_n = CHAR_BIT * sizeof(UInt64);
            if(offset >= max_n)
            {
                initialize(UINT64_ZERO, n); // value is padded with infinite zeros on the left
                return;
            }
            new_val >>= offset; // drop offset bits
            if(n >= max_n)
            {
                initialize(new_val, n); // all  bits requested
                return;
            }
            const UInt64 mask = (UINT64_ONE << n) - 1; // n '1's
            //std::cout << "new_val " << std::bitset<8>(_ref->_val  & mask).to_string() << std::endl;
            initialize(new_val & mask, n);
		}
		//----------------------------------------------------
        psml_uint_subref& psml_uint_subref::operator= (const psml_uint_subref &v)
        {
            //std::cout << "Hello 2 _len" << _ref->_len << std::endl;

            if((_left - _right) != (v._left - v._right))
                throw ArgumentOutOfRangeException("v", "The two values have not proper length");

            // Prepares X in [r1,l1]
            /*psml_uint_base X = v;

            // Left shift x to the amount of offset
            UInt64 X_val = X._val;
            UInt64 Y_val = _ref->_val;
            Int8 offsetY = _right;
            Int8 nY = _left - _right + 1;
            const Int8 max_nY = CHAR_BIT * sizeof(Int64);
            if(offsetY >= max_nY)
                Y_val = UINT64_ZERO; // value is padded with infinite zeros on the left
            X_val <<= offsetY; // drop offset bits
            const UInt64 maskY = (UINT64_ONE << nY) - 1; // n '1's

            // Clear the bits of y in [r2, l2]
            if(offsetY != 0)
                Y_val &= ~(maskY << nY);
            else
                Y_val &= (maskY << nY);
            UInt64 new_val = X_val | Y_val;
            _ref->_val = new_val; // Updates Y

            _ref->extend_sign();*/

            UInt64 Y_val = _ref->_val;
            //std::cout << "Y " << psml_uint<64>(Y_val).to_string(2) << std::endl;
            const UInt64 maskY1 = (UINT64_ONE << _right) - 1;
            const UInt64 maskY2 = ( ( UINT64_ONE << (_ref->_len - _left - 1) ) - 1) << (_left + 1);
            const UInt64 maskY = maskY1 | maskY2;
            Y_val &= maskY;

            psml_uint_base X = v;
            UInt64 X_val = X._val;
            X_val <<= _right;

            Y_val |= X_val;
            _ref->_val = Y_val;
            _ref->extend_sign();

            //std::cout << "maskY1 " << psml_uint<64>(maskY1).to_string(2) << std::endl;
            //std::cout << "maskY2 " << psml_uint<64>(maskY2).to_string(2) << std::endl;
            //std::cout << "maskY " << psml_uint<64>(maskY).to_string(2) << std::endl;
            //std::cout << "Y_val " << psml_uint<64>(Y_val).to_string(2) << std::endl;

            return *this;
        }
        //----------------------------------------------------
        psml_uint_subref& psml_uint_subref::operator= (const psml_uint_base &v)
        {
            //std::cout << "Hello 3 _len " << _ref->_len << " right " << _right << " left " << _left << " v " << v << std::endl;

            if((_left - _right + 1) != v._len)
                throw ArgumentOutOfRangeException("v", "The two values have not proper length");

            UInt64 Y_val = _ref->_val;
            const UInt64 maskY1 = (UINT64_ONE << _right) - 1;
            const UInt64 maskY2 = ( ( UINT64_ONE << (_ref->_len - _left - 1) ) - 1) << (_left + 1);
            const UInt64 maskY = maskY1 | maskY2;
            Y_val &= maskY;

            UInt64 X_val = v._val;
            X_val <<= _right;

            Y_val |= X_val;
            _ref->_val = Y_val;
            _ref->extend_sign();

            return *this;
        }
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
