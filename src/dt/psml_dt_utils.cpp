/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_dt_utils.h"
#include <string.h>
#include <bitset>


//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// See: http://stackoverflow.com/questions/19072348/converting-a-decimal-to-any-radix-base-number
		std::string psml_dt_utils::Int64toBaseString(Int64 num, Int16 rad)
		{
			static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
			Int32 i;
			char buf[66];   /* enough space for any 64-bit in base 2 */

			/* bounds check for radix */
			if (rad < 2 || rad > 62)
			{
				throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");
				//return NULL;
			}
			/* if num is zero */
			if (!num)
				return std::string("0"/*strdup("0")*/);

			/* null terminate buf, and set i at end */
			buf[65] = '\0';
			i = 65;

			if (num > 0) {  /* if positive... */
				while (num) { /* until num is 0... */
					/* go left 1 digit, divide by radix, and set digit to remainder */
					buf[--i] = digits[num % rad];
					num /= rad;
				}
			} else {    /* same for negative, but negate the modulus and prefix a '-' */
				while (num) {
					buf[--i] = digits[-(num % rad)];
					num /= rad;
				}
				buf[--i] = '-';
			}
			/* return a duplicate of the used portion of buf */
			return std::string(buf + i/*strdup(buf + i)*/);
		}
		//----------------------------------------------------
		// See: http://www.cplusplus.com/reference/string/stoll/
		Int64 psml_dt_utils::BaseStringToInt64(const std::string &num, Int16 rad)
		{
			/* bounds check for radix */
			if (rad < 2 || rad > 62)
				throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");

			Int64 ll;

#if defined WIN32 || WIN64
			ll = _strtoi64(num.c_str(), null, rad);
#else
			//ll = std::stoll(num, null, rad);
			ll = strtoll(num.c_str(), null, rad);
#endif
            return ll;
		}
		//----------------------------------------------------
        Int64 psml_dt_utils::BaseStringToInt64(const char *num, Int16 rad)
		{
            if(num == NULL)
                throw ArgumentNullException("num", "num is null");
			/* bounds check for radix */
			if (rad < 2 || rad > 62)
				throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");

			Int64 ll;

#if defined WIN32 || WIN64
			ll = _strtoi64(num, null, rad);
#else
			//ll = std::stoll(num, null, rad);
			ll = strtoll(num, null, rad);
#endif
            return ll;
		}
        //----------------------------------------------------
		// See: http://www.cplusplus.com/reference/string/stoll/
		UInt64 psml_dt_utils::BaseStringToUInt64(const std::string &num, Int16 rad)
		{
			/* bounds check for radix */
			if (rad < 2 || rad > 62)
				throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");

			UInt64 ull;

#if defined WIN32 || WIN64
			ull = _strtoui64(num.c_str(), null, rad);
#else
			ull = strtoull(num.c_str(), null, rad);
#endif
            return ull;
		}
		//----------------------------------------------------
        UInt64 psml_dt_utils::BaseStringToUInt64(const char *num, Int16 rad)
		{
            if(num == NULL)
                throw ArgumentNullException("num", "num is null");
			/* bounds check for radix */
			if (rad < 2 || rad > 62)
				throw ArgumentOutOfRangeException("radix", "radix must be between 2 and 62");

			UInt64 ull;

#if defined WIN32 || WIN64
			ull = _strtoui64(num, null, rad);
#else
			ull = strtoull(num, null, rad);
#endif
            return ull;
		}
		//----------------------------------------------------
		// There is a single BEXTR (Bit field extract (with register)) x86 instruction on Intel and AMD CPUs and UBFX on ARM. There are intrinsic functions such as _bextr_u32() (link requires sign-in) that allow to invoke this instruction explicitly.
		// They implement (source >> offset) & ((1 << n) - 1) C code: get n continuous bits from source starting at the offset bit. Here's a complete function definition that handles edge cases:
		// See: http://stackoverflow.com/questions/8011700/how-do-i-extract-specific-n-bits-of-a-32-bit-unsigned-integer-in-c
		// see also: http://icfun.blogspot.com/2009/04/get-n-th-bit-value-of-any-integer.html
		Int64 psml_dt_utils::GetBits(Int64 value, unsigned offset, unsigned n)
		{
			const Int64 max_n = CHAR_BIT * sizeof(Int64);
			if (offset >= max_n)
				return 0; /* value is padded with infinite zeros on the left */
			value >>= offset; /* drop offset bits */
			if (n >= max_n)
				return value; /* all  bits requested */
			const Int64 mask = (1u << n) - 1; /* n '1's */
			return value & mask;
		}

		const CryptoPP::Integer psml_dt_utils::GetBits(const CryptoPP::Integer &value, unsigned offset, unsigned n)
		{
			const unsigned max_n = value.BitCount();
			if (offset >= max_n)
				return 0; /* value is padded with infinite zeros on the left */
			CryptoPP::Integer _value = value;
			_value >>= offset; /* drop offset bits */
			if (n >= max_n)
				return _value; /* all  bits requested */
			const CryptoPP::Integer mask = (1u << n) - 1; /* n '1's */
			return _value & mask;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
