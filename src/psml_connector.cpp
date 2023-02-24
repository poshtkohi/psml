/**
    #define meta ...
    printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/
#include "psml_connector.h"
#include "psml_port.h"
#include "psml_interface.h"
#include "psml_nonshared_interface.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		std::vector<psml_connector *> *psml_connector::__connectors = null;//
		//----------------------------------------------------
		psml_connector::psml_connector(const String &name) : _interface(null)
		{
			if(name == "")
				_objName = Guid().NewGuid();
			else
				_objName = name;

            if(__connectors == null)
                __connectors = new std::vector<psml_connector *>;

			__connectors->push_back(this);

			_inports = new Vector<psml_port *>;

			//std::cout << "psml_connector::psml_connector() " << this << " __connectors " << __connectors->size() << std::endl;
		}
		//----------------------------------------------------
		psml_connector::~psml_connector()
		{
			delete _inports;
			//cout << "~psml_connector _inports.size(): " << _inports.size() << " _outports.size(): " << _outports.size() << std::endl;
			//cout << "~psml_connector _interface: " << _interface << " connector: " << this << std::endl;
		}
		//----------------------------------------------------
		const String &psml_connector::GetName()
		{
			return _objName;
		}
		//----------------------------------------------------
		void psml_connector::SetName(const String &n)
		{
			_objName = n;
		}
		//----------------------------------------------------
		psml_interface *psml_connector::GetOwnerInterface()
		{
			return _interface;
		}
		//----------------------------------------------------
		void psml_connector::RegisterOwnerInterface(psml_interface *_if)
		{
			if(_if == null)
				throw ArgumentNullException("_if", "_if is null");

			_interface = _if;
		}
		//----------------------------------------------------
		void psml_connector::before_end_of_elaboration(const psml_simulator *sim)
		{
			//std::cout << "psml_connector::before_end_of_elaboration()" << std::endl;

			if(__connectors == null)
                return;

			for(register UInt32 i = 0 ; i < __connectors->size() ; i++)
			{
                psml_connector *connector = (*__connectors)[i];
                psml_interface *if_ = connector->GetOwnerInterface();
                if(if_ != null && if_->GetInterfaceType() == __NonSharedInterface)
                {
                    psml_nonshared_interface *nif = (psml_nonshared_interface *)if_;
                    bool is_intra_cluster_communication = ! nif->GetIsInstance();
                    if(!is_intra_cluster_communication)
                        continue;
                    Vector<psml_port *> *inports = connector->_inports;
                    for(register Int32 j = 0 ; j < inports->Size() ; j++)
                    {
                        //std::cout << "inports->Size() " << inports->Size() << std::endl;
                        psml_port *inport = (*inports)[j];
                        if(inport->GetSensitive())
                            nif->_numOfSensitivePorts++;
                    }
                    //std::cout << "nif->_numOfSensitivePorts " << nif->_numOfSensitivePorts++ << std::endl;
                }
			}
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
