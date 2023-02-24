/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#ifndef __Parvicursor_psml_psml_proxy_h__
#define __Parvicursor_psml_psml_proxy_h__

#include <general.h>
#include <System/BasicTypes/BasicTypes.h>
#include <System/Object/Object.h>
#include <System/String/String.h>
#include <System/Guid/Guid.h>
#include <System/ObjectDisposedException/ObjectDisposedException.h>
#include <System/ArgumentException/ArgumentOutOfRangeException.h>
#include <System/ArgumentException/ArgumentNullException.h>
#include <System/InvalidOperationException/InvalidOperationException.h>

#include "psml_proxy_interface.h"
#include "psml_connector.h" ///

#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace System;

using namespace std;

//**************************************************************************************************************//
namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		// Forward declarations
		class psml_component;
		class psml_port;
		class psml_interface;
		class psml_connector;

		template <class IF>
		class psml_proxy : public psml_proxy_interface
		{
			friend class psml_component;
			/*---------------------fields-----------------*/
			private: bool _isMainSingelton;
			private: bool _isSingeltonNull;
			private: psml_port *_ownerPort;
			private: psml_component *_ownerComponent;
			// A singleton pattern is used to maintain the hierarchy through proxies.
			private: psml_connector *_singeltonConnector;
			private: IF *_singeltonInterface;
			// Bound proxies to this proxy.
			private: std::map<psml_proxy_interface *, psml_proxy_interface *> _boundProxies;
			// Child ports those are bound from within the component.
			private: std::map<psml_port *, psml_port *> _boundPorts;
			/// Structural Casuality
			private: std::vector<psml_port *> _boundPortsVector;
			private: String _objName;
			/*---------------------methods----------------*/
			public: psml_proxy(const String &name = "") : _ownerPort(null), _singeltonConnector(null), _singeltonInterface(null), _isMainSingelton(false), _isSingeltonNull(true), _ownerComponent(null)
			{
				if(name == "")
					_objName = Guid().NewGuid();
				else
					_objName = name;
			}
			//----------------------------------------------------
			public: ~psml_proxy()
			{
				if(_isMainSingelton)
				{
					delete _singeltonConnector;
					delete _singeltonInterface;
					//std::cout << "~psml_proxy::singleton" << std::endl;
				}
				//_boundProxies.clear();
				//_boundPorts.clear();
			}
			//----------------------------------------------------
			private: inline void SetOwnerPort(psml_port *port)
			{
				if(port == null)
					throw ArgumentNullException("port", "port is null");

				_ownerPort = port;
			}
			//----------------------------------------------------
			private: inline psml_port *GetOwnerPort()
			{
				return _ownerPort;
			}
			//----------------------------------------------------
			// Recursive proxy binding to support hierarchical components.
			public: void bind(psml_proxy<IF> *proxy)
			{
				if(proxy == null)
					throw ArgumentNullException("proxy", "proxy is null");

				if(_boundProxies.count(proxy) != 0 || proxy->_boundProxies.count(this) != 0)
					throw InvalidOperationException("These two proxies have already been bound to each other");

				// Creates the singleton.
				/*if(_isSingeltonNull && proxy->_isSingeltonNull)
				{
					// Creates the singletons to be shared by all proxies bound.
					_isMainSingelton = true;
					_singeltonConnector = new psml_connector();
					_singeltonInterface = new IF();
					//psml_interface *_if = (psml_interface *)_singeltonInterface;
					//_if->RegisterOwnerConnector(_singeltonConnector);
					_singeltonInterface->RegisterOwnerConnector(_singeltonConnector);
					_singeltonConnector->RegisterOwnerInterface((psml_interface*)_singeltonInterface);
					proxy->_singeltonConnector = _singeltonConnector;
					proxy->_singeltonInterface = _singeltonInterface;
					_isSingeltonNull = false;
					proxy->_isSingeltonNull = false;
				}
				else if(_isSingeltonNull)
				{
					_singeltonConnector = proxy->_singeltonConnector;
					_singeltonInterface = proxy->_singeltonInterface;
					_isSingeltonNull = false;
				}
				else if(proxy->_isSingeltonNull)
				{
					proxy->_singeltonConnector = _singeltonConnector;
					proxy->_singeltonInterface = _singeltonInterface;
					proxy->_isSingeltonNull = false;
				}*/

				//_boundProxies[proxy] = proxy;
				proxy->_boundProxies[this] = this;
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_proxy<IF> *proxy)
			{
				bind(proxy);
			}
			//----------------------------------------------------
			public: inline void bind(psml_proxy<IF> &proxy)
			{
				bind(&proxy);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_proxy<IF> &proxy)
			{
				bind(proxy);
			}
			//----------------------------------------------------
			// Child port binding.
			public: void bind(psml_port *port)
			{
				if(port == null)
					throw ArgumentNullException("proxy", "proxy is null");

				if(port->GetOwnerComponent() != _ownerPort->GetOwnerComponent())
					throw InvalidOperationException("The port doesn't belong to that component which this proxy has been bound");

				if(_boundPorts.count(port) != 0)
					throw InvalidOperationException("The port has already been bound to this proxy");

				_boundPorts[port] = port;
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_port *port)
			{
				bind(port);
			}
			//----------------------------------------------------
			public: inline void bind(psml_port &port)
			{
				bind(&port);
			}
			//----------------------------------------------------
			// Operator overloading for bind().
			public: inline void operator () (psml_port &port)
			{
				bind(port);
			}
			//----------------------------------------------------
			public: void bind(psml_connector *conn, IF *_if)
			{
				if(conn == null)
					throw ArgumentNullException("conn", "conn is null");

				if(_if == null)
					throw ArgumentNullException("_if", "_if is null");

				if(_ownerComponent == null)
					throw ArgumentNullException("ownerComponent", "ownerComponent1 is null. Set it through the SetOwnerComponent");

				if(_singeltonConnector != null)
					throw InvalidOperationException("The proxy has already been bound to a connector and interface");

				if(_singeltonInterface == _if && _singeltonConnector == conn)
					throw InvalidOperationException("The proxy has already been bound to the connector and interface");

				// Creates the singleton.
				/*if(_isSingeltonNull)
				{
					//std::cout << "bind _isSingeltonNull" << std::endl;
					_singeltonConnector = conn;
					_singeltonInterface = _if;
					_singeltonInterface->RegisterOwnerConnector(_singeltonConnector);
					_singeltonConnector->RegisterOwnerInterface((psml_interface*)_singeltonInterface);
					_isSingeltonNull = false;
				}
				else
					throw InvalidOperationException("The proxy has already been bound to the connector and interface");

				// The first time that conn and _if are registered to each other.
				if(conn->_interface == null && _if->GetOwnerConnector() == null)
				{
					conn->_interface = (psml_interface *)_if;
					_if->RegisterOwnerConnector(conn);
				}
				else if(conn->_interface != (psml_interface *)_if)
					throw InvalidOperationException("_if has already been registered with another psml_port");*/

				// The first time that conn and _if are registered to each other.
				/*if(conn->_interface == null && _if->GetOwnerConnector() == null)
				{
					conn->_interface = (psml_interface *)_if;
					_if->RegisterOwnerConnector(conn);
				}
				else if(conn->_interface != (psml_interface *)_if)
					throw InvalidOperationException("_if has already been registered with another psml_port");

				_singeltonInterface = _if;
				_singeltonConnector = conn;*/

				//_singeltonInterface->RegisterOwnerConnector(_singeltonConnector);
				//_singeltonConnector->RegisterOwnerInterface((psml_interface*)_singeltonInterface);

				// The first time that conn and _if are registered to each other.
				if(conn->GetOwnerInterface() == null)
					conn->RegisterOwnerInterface(_if);
				if(_if->GetOwnerConnector() == null)
					_if->RegisterOwnerConnector(conn);
				if(conn->GetOwnerInterface() != _if)
					throw InvalidOperationException("_if has already been registered with another psml_outport");

				_singeltonInterface = _if;
				_singeltonConnector = conn;

				//std::cout << "psml_proxy::bind() Component " << GetOwnerComponent()->GetName().get_BaseStream() << " proxy " << this << " boundPorts " << GetBoundPorts()->size() << " boundProxies " << GetBoundProxies()->size() << std::endl;

				Traverse(this, conn, _if);
			}
			//----------------------------------------------------
			public: inline void bind(psml_connector &conn, IF &_if)
			{
				bind(&conn, &_if);
			}
			//----------------------------------------------------
			private: inline std::map<psml_port *, psml_port *> *GetBoundPorts()
			{
				return &_boundPorts;
			}
            //----------------------------------------------------
            /// Structural Casuality
			private: inline std::vector<psml_port *> *GetBoundPortsVector()
			{
				return &_boundPortsVector;
			}
			//----------------------------------------------------
			private: inline std::map<psml_proxy_interface *, psml_proxy_interface *> *GetBoundProxies()
			{
				return &_boundProxies;
			}
			//----------------------------------------------------
			private: inline psml_interface *GetBoundInterface()
			{
				return (psml_interface *)_singeltonInterface;
			}
			//----------------------------------------------------
			private: inline void SetBoundInterface(psml_interface *_if)
			{
				_singeltonInterface = (IF *)_if;
			}
			//----------------------------------------------------
			private: inline psml_connector *GetBoundConnector()
			{
				return _singeltonConnector;
			}
			//----------------------------------------------------
			private: inline void SetBoundConnector(psml_connector *conn)
			{
				_singeltonConnector = conn;
			}
			//----------------------------------------------------
			public: inline const String &GetName()
			{
				return _objName;
			}
			//----------------------------------------------------
            private: void SetOwnerComponent(psml_component *ownerComponent)
			{
                if(ownerComponent == null)
					throw ArgumentNullException("ownerComponent", "ownerComponent is null");

                if(_ownerComponent != null)
                    throw InvalidOperationException("This proxy has already been registered with another psml_component instance");

                _ownerComponent = ownerComponent;
			}
            //----------------------------------------------------
            private: psml_component *GetOwnerComponent()
			{
                return _ownerComponent;
			}
			//----------------------------------------------------
		};
		//----------------------------------------------------
	};
};

//**************************************************************************************************************//

#endif
