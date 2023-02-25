/**
    #define meta ...
    prInt32f("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __globals_h__
#define __globals_h__

#define NumOfWorks 1000 // 30000
#define NumOfCores 48
#define N 100 // 100

#define __cores__ 2
#define SimulationUntil 99999999999
//#define DebugMode
//typedef signed long long int Int64;

#include <string>
#include <iostream>
using namespace std;

template <typename T> static std::string to_string(const T &n)
{
    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

#include <System/BasicTypes/BasicTypes.h>
using namespace System;

//static Int64 *__mem__;


#endif
