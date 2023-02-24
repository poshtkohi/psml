/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_inport_h__
#define __Parvicursor_psml_psml_inport_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include "psml_port.h"
#include "psml_connector.h"
#include "psml_component.h"
#include "psml_process.h"
#include "psml_nonshared_interface.h"
#include "psml_proxy_interface.h"
#include "psml_pdes_event.h"

using namespace System;


//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		//class psml_component;
		//template <class T> psml_wire;
		template <class IF> class psml_outport;
		enum psml_edge_trigger_type;
		class psml_clock;
		//class psml_pdes_event;

		// The port must be associated with the owner component instance through the psml_inport constructor
		// or the SetOwnerComponent() method by the user.
		// This port type is only used for none shared interfaces.
		template <class IF>
		class psml_inport : public psml_port
		{
			friend class psml_clock;
			/*---------------------fields-----------------*/
			private: psml_edge_trigger_type _edgeTriggerType;
			private: psml_component *_ownerComponent;
			private: psml_proxy_interface *_boundProxy;
			private: psml_pdes_event *_e;
			private: String _objName;
			/*---------------------methods----------------*/
			public: psml_inport(const String &name = "") : _ownerComponent(null), _edgeTriggerType(PSML_UNKNOW_EDGE), _boundProxy(null), _e(null)
			{
				if(name == "")
					_objName = Guid().NewGuid();
				else
					_objName = name;

				SetOwnerProcess(null);
				SetBoundConnector(null);
				SetBoundInterface(null);
			}
			//----------------------------------------------------
			public: ~psml_inport()
			{
				/*if(_e != null)
				{
					_e->GetReceiverReference()->DeallocateEvent(_e);
					_e = null;
					//delete _e;
				}*/
				//cout << "~psml_inport() GetPortID(): " << GetPortID() << std::endl;
			}
			//----------------------------------------------------
			//public: void bind(psml_connector *conn, psml_interface *_if)
			public: void bind(psml_connector *conn, IF *_if)
			{
				if(conn == null)
					throw ArgumentNullException("conn", "conn is null");

				if(_if == null)
					throw ArgumentNullException("_if", "_if is null");

				if(GetBoundConnector() != null)
					throw InvalidOperationException("This port has already been bound");

				if(_ownerComponent == null)
					throw ArgumentNullException("ownerComponent", "ownerComponent is null. Set it through the SetOwnerComponent");

				if(_if->GetInterfaceType() != __NonSharedInterface)
					throw InvalidOperationException("psml_inport can only be used for psml_nonshared_interface instances");

				// The first time that conn and _if are registered to each other.
				if(conn->GetOwnerInterface() == null)
					conn->RegisterOwnerInterface(_if);
				if(_if->GetOwnerConnector() == null)
					_if->RegisterOwnerConnector(conn);
				if(conn->GetOwnerInterface() != _if)
					throw InvalidOperationException("_if has already been registered with another psml_inport");

				SetBoundInterface(_if);
				SetBoundConnector(conn);
				GetBoundConnector()->_inports->PushBack(this);
				GetBoundConnector()->_dest_processes[_ownerProcess] = GetOwnerProcess();
			}
			//----------------------------------------------------
			public: inline void bind(psml_connector &conn, IF &_if)
			{
				bind(&conn, &_if);
			}
			//----------------------------------------------------
			/*public: void bind(psml_proxy<IF> *proxy)
			{
				if(proxy == null)
					throw ArgumentNullException("proxy", "proxy is null");

				if(GetOwnerProcess() == null || _ownerComponent == null)
					throw InvalidOperationException("Port" + GetName() + " has not already has registered with a process");

				_ownerComponent->register_proxy_child(proxy, this);
			}*/
			//----------------------------------------------------
			public: inline void bind(psml_proxy<IF> &proxy)
			{
				bind(&proxy);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_proxy<IF> *proxy)
			{
				bind(proxy);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_proxy<IF> &proxy)
			{
				bind(proxy);
			}
			//----------------------------------------------------
			/*public: void bind(psml_outport<IF> *port)
			{
				//bind(port->get_connector(), port->get_interface());
				_boundPort = port;
			}
			//----------------------------------------------------
			public: void bind(psml_outport<IF> &port)
			{
				bind(&port);
			}
			//----------------------------------------------------
			public: void bind(psml_inport<IF> *port)
			{
				//bind(port->get_connector(), port->get_interface());
				_boundPort = port;
			}
			//----------------------------------------------------
			public: void bind(psml_inport<IF> &port)
			{
				bind(&port);
			}
			//----------------------------------------------------
			public: psml_port *GetBoundPort()
			{
				return _boundPort;
			}*/
			//----------------------------------------------------
			private: inline void SetBoundProxy(psml_proxy_interface *proxy)
			{
				_boundProxy = proxy;
			}
			//----------------------------------------------------
			private: inline psml_proxy_interface *GetBoundProxy()
			{
				return _boundProxy;
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_connector *conn, IF *_if)
			{
				bind(conn, _if);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_connector &conn, IF &_if)
			{
				bind(conn, _if);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			/*public: inline void operator () (psml_outport<IF> *port)
			{
				bind(port);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_outport<IF> &port)
			{
				bind(port);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_inport<IF> *port)
			{
				bind(port);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_inport<IF> &port)
			{
				bind(port);
			}*/
			//----------------------------------------------------
			// Allows to call methods provided by the bound interface.
			public: inline IF* operator -> ()
			{
				return get_interface();
			}
			//----------------------------------------------------
			// Allows to call methods provided by the bound interface.
			public: inline const IF* operator -> () const
			{
				return get_interface();
			}
			//----------------------------------------------------
			// Gets the interface bound to this port.
			public: inline IF *get_interface()
			{
				//if(_interface == null)
				//	throw ArgumentNullException("interface", "Port " + GetName() + " has not already been bound to an interface");

				return (IF *)GetBoundInterface();
			}
			//----------------------------------------------------
			// Gets the connector bound to this port.
			public: inline psml_connector *get_connector()
			{
				return GetBoundConnector();
			}
			//----------------------------------------------------
			private: void SetOwnerComponent(psml_component *owner)
			{
				if(_ownerComponent != null)
					throw InvalidOperationException("This port has already been associated with an owner component");

				_ownerComponent = owner;
 				//SetPortID((Int32)_ownerComponent->_inports.size());
				//_ownerComponent->_inports.push_back(this);
				//owner->_inports.push_back(this);
			}
			//----------------------------------------------------
			private: inline psml_component *GetOwnerComponent()
			{
				return _ownerComponent;
			}
			//----------------------------------------------------
			// Fetches the data associated with this port. This method
			// can be directly called by the user.
			/*public: const T &Fetch()
			{
				return *_m_cur_val;
			}
			//----------------------------------------------------
			// Puts the data associated with this port. This method
			// can be only called by the psml framework.
			public: void Put(const T &newVal)
			{
				*_m_new_val = newVal;

				if(*_m_new_val != *_m_cur_val)
					*_m_cur_val = newVal;
			}*/
			//----------------------------------------------------
			public: inline const String &GetName() const
			{
				return _objName;
			}
			//----------------------------------------------------
			private: inline psml_port_type GetPortType()
			{
				return __InPort;
			}
			//----------------------------------------------------
			private: inline void SetEdgeTriggerType(psml_edge_trigger_type type)
			{
				_edgeTriggerType = type;
			}
			//----------------------------------------------------
			private: inline psml_edge_trigger_type GetEdgeTriggerType()
			{
				return _edgeTriggerType;
			}
			//----------------------------------------------------
			protected: psml_pdes_event *GetEvent()
			{
				return _e;
			}
			//----------------------------------------------------
			protected: void SetEvent(psml_pdes_event *e)
			{
				if(e == null)
					throw ArgumentNullException("e", "e is null");

				_e = e;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
