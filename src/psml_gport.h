/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_gport_h__
#define __Parvicursor_psml_psml_gport_h__

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
#include "psml_shared_interface.h"
#include "psml_proxy_interface.h"

using namespace System;


//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		//class psml_component;

		// The port must be associated with the owner component instance through the psml_gport constructor
		// or the SetOwnerComponent() method by the user.
		// This port type is only used for none shared interfaces.
		// A general ports that can be used with shared interfaces.
		template <class IF>
		class psml_gport : public psml_port
		{
			/*---------------------fields-----------------*/
			//private: psml_connector *_conn;
			private: psml_component *_ownerComponent;
			//private: psml_process *_ownerProcess;
			//private: IF *_interface;
			/*private: bool _sensitive;*/
			private: psml_proxy_interface *_boundProxy;
			private: String _objName;
			/*---------------------methods----------------*/
			public: psml_gport(const String &name = "") : _ownerComponent(null), _boundProxy(null)
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
			public: ~psml_gport()
			{
				//cout << "~psml_gport() GetPortID(): " << GetPortID() << std::endl;
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

				if(_if->GetInterfaceType() != __SharedInterface)
					throw InvalidOperationException("psml_gport can only be used for psml_shared_interface instances");

				// The first time that conn and _if are registered to each other.
				if(conn->_interface == null && _if->GetOwnerConnector() == null)
				{
					conn->_interface = (psml_interface *)_if;
					_if->RegisterOwnerConnector(conn);
				}
				else if(conn->_interface != (psml_interface *)_if)
					throw InvalidOperationException("_if has already been registered with another psml_gport");

				SetBoundInterface(_if);
				SetBoundConnector(conn);
				GetBoundConnector()->_gports.push_back(this);
				//_conn->_dest_processes[_ownerProcess] = _ownerProcess;
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

				if(_ownerProcess == null || _ownerComponent == null)
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
			/*public: inline psml_connector *get_connector()
			{
				return _conn;
			}*/
			//----------------------------------------------------
			private: void SetOwnerComponent(psml_component *owner)
			{
				if(_ownerComponent != null)
					throw InvalidOperationException("This port has already been associated with an owner component");

				_ownerComponent = owner;
			}
			//----------------------------------------------------
			private: inline psml_component *GetOwnerComponent()
			{
				return _ownerComponent;
			}
			//----------------------------------------------------
			public: inline const String &GetName() const
			{
				return _objName;
			}
			//----------------------------------------------------
			private: inline psml_port_type GetPortType()
			{
				return __GeneralPort;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	}
};

//**************************************************************************************************************//

#endif
