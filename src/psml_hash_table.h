/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_hash_table_h__
#define __Parvicursor_psml_psml_hash_table_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

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
		// Adopated from the following link
		// https://www.tutorialspoint.com/data_structures_algorithms/hash_data_structure.htm
        template <class T>
        class psml_hash_table : public Object
        {
            /*---------------------fields-----------------*/
            struct DataItem;
            private: DataItem *_hashArray;
            private: UInt32 _length;
            public: UInt32 _capacity;
            private: struct DataItem
            {
                T data;
                UInt32 key;
                bool deleted;
                DataItem() : deleted(true){}
            };
            /*---------------------methods----------------*/
            // psml_hash_table Class constructor.
            public: psml_hash_table(UInt32 capacity)
            {
                if(capacity == 0)
                    throw ArgumentOutOfRangeException("capacity", "capacity can not be zero");

                _capacity = capacity;
                _length = 0;

                _hashArray = new DataItem[_capacity];
            }
            //----------------------------------------------------
            // psml_hash_table Class destructor.
            public: ~psml_hash_table()
            {
                delete[] _hashArray;
            }
            //----------------------------------------------------
            private: inline UInt32 hashCode(UInt32 key)
            {
               return key % _capacity;
            }
            //----------------------------------------------------
            public: void insert(UInt32 key, In T &data)
            {
                //if(_length == _capacity)\
                    throw ArgumentOutOfRangeException("length", "psml_hash_table is full");

                //get the hash
                UInt32 hashIndex = hashCode(key);

                //move in array until an empty or deleted cell
                while(!_hashArray[hashIndex].deleted/* && hashArray[hashIndex]->key != -1*/)
                {
                    //go to next cell
                    ++hashIndex;

                    //wrap around the table
                    hashIndex %= _capacity;
                }

                //hashArray[hashIndex] = item;
                _hashArray[hashIndex].data = data;
                _hashArray[hashIndex].key = key;
                _hashArray[hashIndex].deleted = false;
                _length++;
            }
            //----------------------------------------------------
            public: void remove(UInt32 key)
            {
                if(_length == 0)
                    return;
                //get the hash
                UInt32 hashIndex = hashCode(key);

                //move in array until an empty
                while(!_hashArray[hashIndex].deleted)
                {
                    if(_hashArray[hashIndex].key == key)
                    {
                        _hashArray[hashIndex].deleted = true;
                        _length--;
                        return;
                    }

                    //go to next cell
                    ++hashIndex;

                    //wrap around the table
                    hashIndex %= _capacity;
                }
            }
            //----------------------------------------------------
            public: bool search(UInt32 key, Out T &value)
            {
                if(_length == 0)
                    return false;
                //get the hash
                UInt32 hashIndex = hashCode(key);

                //move in array until an empty
                while(!_hashArray[hashIndex].deleted)
                {
                    if(_hashArray[hashIndex].key == key)
                    {
                        value = _hashArray[hashIndex].data;
                        return true;
                    }

                    //go to next cell
                    ++hashIndex;

                    //wrap around the table
                    hashIndex %= _capacity;
                }

               return false;
            }
            //----------------------------------------------------
            public: bool contains(UInt32 key)
            {
                if(_length == 0)
                    return false;
                //get the hash
                UInt32 hashIndex = hashCode(key);

                //move in array until an empty
                while(!_hashArray[hashIndex].deleted)
                {
                    if(_hashArray[hashIndex].key == key)
                        return true;

                    //go to next cell
                    ++hashIndex;

                    //wrap around the table
                    hashIndex %= _capacity;
                }

               return false;
            }
            //----------------------------------------------------
            // Returns true if the hasttable contains a value at the index.
            public: inline bool findAtIndex(UInt32 index, Out T &val)
            {
                //if(index >= _capacity)\
                    throw ArgumentOutOfRangeException("index", "n is equall or greater than capacity()");

                if(_hashArray[index].deleted)
                    return false;

                val = _hashArray[index].data;
                return true;
            }
            //----------------------------------------------------
            public: inline bool findAtIndex(UInt32 index, Out UInt32 &key, Out T &val)
            {
                //if(index >= _capacity)\
                    throw ArgumentOutOfRangeException("index", "n is equall or greater than capacity()");

                if(_hashArray[index].deleted)
                    return false;

                key = _hashArray[index].key;
                val = _hashArray[index].data;
                return true;
            }
            //----------------------------------------------------
            public: inline void reset()
            {
                _length = 0;

                for(register Int32 i = _capacity - 1 ; i != -1 ; --i)
                //for(register UInt32 i = 0 ; i < _capacity ; i++)
                     _hashArray[i].deleted = true;
            }
            //----------------------------------------------------
            public: inline UInt32 capacity()
            {
                return _capacity;
            }
            //----------------------------------------------------
        };
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif

/*psml_hash_table<Int32, 1000> ht;

int arr[3] = {10, 5, 1024};
for(int i = 0 ; i < 3 ; i++)
    ht.insert(arr[i], arr[i]);

for(int i = 0 ; i < 3 ; i++)
{
    int val;
    if(ht.search(arr[i], val))
        std::cout << val << std::endl;
}*/

/*psml_hash_table<std::vector<int> *> ht(1000);

std::vector<int> *v = new std::vector<int> ;
v->push_back(-100);
v->push_back(10000);
ht.insert(0, v);

std::vector<int> *vv;

if(ht.search(0, vv))
{
    for(int i = 0 ; i < vv->size() ; i++)
        std::cout << "vv " << (*vv)[i] << std::endl;
}

for(UInt32 i = 0 ; i < ht.capacity() ; i++)
{
    if(ht.findAtIndex(i, vv))
        vv->clear();
}

delete v;

//for(int i = 0 ; i < 3 ; i++)
//    std::cout << hash_(arr[i]) << std::endl;
return 0;*/
