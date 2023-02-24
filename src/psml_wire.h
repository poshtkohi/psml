/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_wire_h__
#define __Parvicursor_psml_psml_wire_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include "psml_globals.h"
#include "psml_port.h"
#include "psml_time.h"
#include "psml_pdes_event.h"
#include "psml_nonshared_interface.h"

#include <iostream>
#include <vector>

using namespace System;

using namespace std;
//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_pdes_lp; ///
		class psml_connector;
		class psml_process;
		//class psml_time;
		class psml_clock;
		class psml_proxy_interface;
		//class psml_pdes_event;
		template <class IF> class psml_inport;
		template <class IF> class psml_outport;
		template <class IF> class psml_proxy;

		template <class T>
		class psml_wire : psml_nonshared_interface
		{
			friend class psml_pdes_lp; ///
			friend class psml_connector;
			friend class psml_proxy_interface;
			friend class psml_clock;
			//template <class IF> friend class psml_nonshared_interface;
			template <class IF> friend class psml_inport;
			template <class IF> friend class psml_outport;
			template <class IF> friend class psml_proxy;
			/*---------------------fields-----------------*/
			private: T _m_cur_val;
			private: T _m_new_val;
			private: bool _is_instance;
			private: String  _objName;
//#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
			//private: bool _is_committed;
//#endif
			/*---------------------methods----------------*/
			// psml_wire Class constructor.
			public: psml_wire(const String &name = "") : _objName(name), _m_cur_val(T()), _m_new_val(T()), /*_owner(null),*/ _is_instance(false)
			{
//#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
                //_is_committed = false;
//#endif
			}
			//----------------------------------------------------
			// psml_wire Class destructor.
			public: ~psml_wire()
			{
				//cout << "~psml_wire _interface: " << this << " connector: " << GetOwnerConnector() << std::endl;
			}
			//----------------------------------------------------
			private: psml_nonshared_interface *CreateInstance()
			{
				psml_wire<T> *_nif = new psml_wire<T>();
				_nif->_m_cur_val = _m_cur_val;
				_nif->_m_new_val = _m_new_val;
				_nif->_is_instance = true;
				_nif->RegisterOwnerConnector(GetOwnerConnector());
				return _nif;
			}
			//----------------------------------------------------
			private: void DestroyInstance(psml_nonshared_interface *instance)
			{
				if(instance == null)
					throw ArgumentNullException("instance", "instance is null");

				psml_wire<T> *w = (psml_wire<T> *)instance;
				delete w;

				//std::cout << "psm_wire::DestroyInstance()" << std::endl;
			}
			//----------------------------------------------------
			private: inline void *CloneValue(Out UInt16 &size)
			{
				T *val = new T();
				//*val = _m_cur_val;
				size = sizeof(T);
				//memmove(val, &_m_cur_val, size);
				memcpy(val, &_m_cur_val, size);
				return (void *)val;
			}
			//----------------------------------------------------
			private: inline void CloneValueToPointer(Out void *p)
			{
				//cout << "clone value " << _m_cur_val << std::endl;
				//T *_p = (T *)((unsigned char *)e + sizeof(class psml_pdes_event));
				//unsigned char *_p = (unsigned char *)e + sizeof(class psml_pdes_event);
				//*_p = _m_cur_val;
				//memmove(_p, &_m_cur_val, sizeof(T));
				//memcpy(_p, &_m_cur_val, sizeof(T));
				memcpy(p, &_m_cur_val, sizeof(T));
                //std::cout << "CloneValueToEvent() _m_cur_val " << _m_cur_val << " _p " << *_p << std::endl;
			}
			//----------------------------------------------------
			private: inline void DeleteValue(In void *buffer)
			{
				//if(buffer == null)
				//	throw ArgumentNullException("buffer", "buffer is null");

				T *val = (T *)buffer;
				delete val;
			}
			//----------------------------------------------------
			private: inline void CopyValue(In void *buffer)
			{
				//memmove(&_m_new_val, buffer, sizeof(T));
				//T *val = (T *)buffer;
				//_m_cur_val = *val;
				//_m_new_val = *val;
				//cout << "_m_new_val " <<*val << std::endl;

				///_m_cur_val = *val;
				//memcpy(&_m_cur_val, buffer, sizeof(T));
				//memmove(&_m_cur_val_m_cur_val, buffer, sizeof(T));
				memcpy(&_m_cur_val, buffer, sizeof(T));
				//std::cout << "CopyValue() _m_cur_val " << _m_cur_val << " buffer " << *(T *)buffer << std::endl;

				if(_m_cur_val)
					SetIsLevelHigh(true);
				else
					SetIsLevelHigh(false);
			}
			//----------------------------------------------------
			private: inline UInt16 ValueSize()
			{
				return sizeof(T);
			}
			//----------------------------------------------------
			/*private: inline void *GetValueReference()
			{
				return (void *)&_m_cur_val;
			}*/
			//----------------------------------------------------
			// This method is not currently implemented.
			private: void Serialize(InOut char **buffer, In Int32 size, Out Int32 &newSize)
			{

			}
			//----------------------------------------------------
			// This method is not currently implemented.
			private: void Deserialize(In char *buffer, In Int32 size)
			{

			}
			//----------------------------------------------------
			private: inline bool GetIsInstance()
			{
				return _is_instance;
			}
			//----------------------------------------------------
			private: inline void SetIsInstance(bool val)
			{
				_is_instance = val;
			}
			//----------------------------------------------------
			// Fetches the data associated with this port.
			public: inline const T &fetch()
			{

				return _m_cur_val;
			}
			//----------------------------------------------------
			// Puts the data associated with this port. This method supports delayed puts.
			public: inline void put(const T &newVal,/* const psml_time &delay = psml_time(0),*/ psml_process *issuer)
			{
				/*//psml_time _delay = psml_time(1, PSML_NS);
				_m_new_val = newVal;

				if(_m_new_val != _m_cur_val)
				{
					_m_cur_val = newVal;

					//if(issuer != null)
						Flush(delay, issuer);
				}*/
#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
				_m_new_val = newVal;

				if(_m_new_val != _m_cur_val)
				{
					if(GetIsInstance())
						_m_cur_val = newVal;
					//else
					//{
					//	if(delay == 0)
					//		_m_cur_val = newVal; // Commit();
					//}

					//Print();///
					//cout << "newVal " << newVal << std::endl;

					//_is_committed = false;

					//if(issuer != null)
                        Offload(issuer);
						//Flush(delay, issuer);

					return;
				}
#else
				_m_new_val = newVal;

				if(_m_new_val != _m_cur_val)
				{
					_m_cur_val = newVal;

					//if(issuer != null)
						Offload(issuer);
				}
#endif
			}
			//----------------------------------------------------
			// Puts the data associated with this port. This method supports delayed puts.
			private: inline void put_clock(const T &newVal, psml_process *issuer, Vector<psml_port *> *subscriber_inports)
			{
				/*_m_new_val = newVal;

				//if(_m_new_val != _m_cur_val)
				{
					//if(GetIsInstance())
						_m_cur_val = newVal;

					//Print();///
					//cout << "newVal " << newVal << std::endl;

					//if(issuer != null)
						FlushClock(delay, issuer, subscriber_inports);
				}*/

#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
				// We expect that the clock value always changes.
				_m_new_val = newVal;

				//if(_m_new_val != _m_cur_val)
				{
					if(GetIsInstance())
						_m_cur_val = newVal;
					//else
					//{
					//	if(delay == 0)
					//		_m_cur_val = newVal; // Commit();
					//}

					//Print();///
					//cout << "newVal " << newVal << std::endl;

					//if(issuer != null)
						FlushClock(issuer, subscriber_inports);

					return;
				}
#else
                _m_new_val = newVal;

				//if(_m_new_val != _m_cur_val)
				{
					//if(GetIsInstance())
						_m_cur_val = newVal;

					//Print();///
					//cout << "newVal " << newVal << std::endl;

					//if(issuer != null)
						FlushClock(issuer, subscriber_inports);
				}
#endif
			}
			//----------------------------------------------------
			/*public: inline psml_wire &operator = (const T &val)
			{
				put(val);
				return *this;
			}
			//----------------------------------------------------
			public: inline psml_wire &operator = (const psml_wire<T> &a)
			{
				put(a.fetch());
				return *this;
			}*/
			//----------------------------------------------------
			/*private: inline psml_connector *GetOwnerConnector()
			{
				return _owner;
			}
			//----------------------------------------------------
			public: inline void RegisterOwnerConnector(psml_connector *conn)
			{
				_owner = conn;
			}*/
			//----------------------------------------------------
//#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
			protected: inline void Commit()
			{
                //if(!_is_committed)
				{
					_m_cur_val = _m_new_val;

                    if(_m_cur_val)
                        SetIsLevelHigh(true);
                    else
                        SetIsLevelHigh(false);

					//_is_committed = true;
				}
			}
            protected: inline void Commit1()
			{
                _m_cur_val = _m_new_val;
			}
//#endif
            //----------------------------------------------------
            /*protected: inline bool GetIsLevelHigh()
			{

                if(_m_cur_val)
                    return true;

                else
                    return false;
			}*/
			//----------------------------------------------------
			public: void Print()
			{
				std::cout << "psml_wire::Print()" << " _m_cur_val: " << _m_cur_val << " _m_new_val: " << _m_new_val << std::endl;
				//std::cout << _m_cur_val << std::endl;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
