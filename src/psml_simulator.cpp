/**
	#define meta ...
	printf("%s\n", meta);
**/


/*
	All rights reserved to Alireza Poshtkohi (c) 1999-2023.
	Email: arp@poshtkohi.info
	Website: http://www.poshtkohi.info
*/

#include "psml_simulator.h"
#include "psml_pdes_event.h"
#include "psml_pdes_memory_manager.h"
#include "psml_nonshared_interface.h"

//**************************************************************************************************************//

namespace Parvicursor
{
	namespace psml
	{
		//----------------------------------------------------
		bool psml_simulator::__parallelExecution = false;
		SimulationManagerType psml_simulator::_simulationManagerType = Sequential;
		bool __simulation_terminated = false;
		//----------------------------------------------------
        void psml_simulator::Stop(psml_process *issuer)
        {
            if(issuer == null)
				throw ArgumentNullException("issuer", "issuer is null");

            if(__simulation_terminated)
            {
                issuer->_stop_requested = true;
                return;
                //throw InvalidOperationException("Simulation has already been stopped");
            }

            issuer->_stop_requested = true;
            issuer->_lp->Terminate();
            __simulation_terminated = true;
        }
		//----------------------------------------------------
		psml_simulator::psml_simulator(UInt32 numOfLogicalProcessors, Partitioner *partitioner) : _manager(null), _started(false), _simulateUntil(0), _channelCapacity(128)
		{
			//psml_time::global_time_resolution = psml_time::GetDefaultTimeResolution();
			//psml_time::global_time_unit = PSML_FS;
			if(numOfLogicalProcessors == 0)
				throw ArgumentOutOfRangeException("numOfLogicalProcessors", "numOfLogicalProcessors is zero");

			///numOfLogicalProcessors = 1;

			//cout << "sizeof(psml_pdes_event) " << sizeof(psml_pdes_event) << std::endl;

			if(numOfLogicalProcessors > 1)
			{
				__parallelExecution = true;
				_parallelExecution = true;
				//_simulationManagerType = DeadlockAvoidance;
				_simulationManagerType = DeadlockDetection;
			}
			else
			{
				__parallelExecution = false;
				_parallelExecution = false;
				_simulationManagerType = Sequential;
			}

			_partitioner = partitioner;
			//_partitioner = null;

			_numOfLogicalProcessors = numOfLogicalProcessors;
		}
		//----------------------------------------------------
		/*psml_simulator::psml_simulator(UInt32 numOfLogicalProcessors, Double v, psml_time_unit tu) : _manager(null), _started(false), _simulateUntil(0), _channelCapacity(128)
		{
		if(numOfLogicalProcessors == 0)
		throw ArgumentOutOfRangeException("numOfLogicalProcessors", "numOfLogicalProcessors is zero");

		if(numOfLogicalProcessors > 1)
		{
		__parallelExecution = true;
		_parallelExecution = true;
		}
		else
		{
		__parallelExecution = false;
		_parallelExecution = false;
		}

		_numOfLogicalProcessors = numOfLogicalProcessors;
		SetTimeResolution(v, tu);
		}*/
		//----------------------------------------------------
		psml_simulator::~psml_simulator()
		{
			for(std::vector<psml_process *>::iterator it = psml_process::__processes->begin() ; it != psml_process::__processes->end() ; ++it)
			{
				psml_process *p = *it;
				delete p;
			}

			if(_manager != null)
			{
				if(_parallelExecution)
				{
					//std::cout << "_psml_nonshared_interfaces: " << _psml_nonshared_interfaces.size() << std::endl;
					for(std::vector<psml_nonshared_interface *>::iterator it = _psml_nonshared_interfaces.begin() ; it != _psml_nonshared_interfaces.end() ; ++it)
					{
						psml_nonshared_interface *_nif = *it;
						_nif->DestroyInstance(_nif);
					}

					_psml_nonshared_interfaces.clear();

					if(_partitioner != null)
					{
						switch(_partitionerType)
						{
                            case RoundRobin:
                                delete (RoundRobinPartitioner *)_partitioner;
                                break;
                            case DepthFirstSearch:
                                delete (DepthFirstSearchPartitioner *)_partitioner;
                                break;
                            case TopologicalSort:
                                delete (TopologicalSortPartitioner *)_partitioner;
                                break;
                            //case Metis:\
                                delete (MetisPartitioner *)_partitioner;\
                                break;
                            case StronglyConnectedComponent:
                                delete (StronglyConnectedComponentPartitioner *)_partitioner;
                                break;
                            case Levelized:
                                delete (LevelizedPartitioner *)_partitioner;
                                break;
                            case Customized:
                                break;
                            default:
                                delete _partitioner;
                                break;
						}
					}
				}

				switch(_simulationManagerType)
				{
                    case Sequential:
                        delete (SequentialSimulationManager *)_manager;
                        break;
                    case DeadlockAvoidance:
                        delete (DeadlockAvoidanceSimulationManager *)_manager;
                        break;
                    case DeadlockDetection:
                        delete (DeadlockDetectionSimulationManager *)_manager;
                        break;
                    default:
                        delete _manager;
                        break;
				}

				psml_pdes_memory_manager::end_of_simulation(this); ///
			}

            if(psml_component::__components != null)
			{
                psml_component::__components->clear();
                delete psml_component::__components;
			}
			if(psml_connector::__connectors != null)
			{
                psml_connector::__connectors->clear();
                delete psml_connector::__connectors;
			}
            if(psml_event::__static_events != null)
			{
                psml_event::__static_events->clear();
                delete psml_event::__static_events;
            }
            if(psml_interface::__interfaces != null)
			{
                psml_interface::__interfaces->clear();
                delete psml_interface::__interfaces;
            }
            if(psml_port::__ports != null)
			{
                psml_port::__ports->clear();
                delete psml_port::__ports;
			}
            if(psml_process::__processes != null)
            {
                psml_process::__processes->clear();
                delete psml_process::__processes;
			}
#if defined __cycles_enabled__
            if(psml_process::__cycles != null)
            {
                //psml_process::__cycles->Clear();
                delete psml_process::__cycles;
			}
            if(psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces != null)
            {
                //psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces->Clear();
                for(register UInt32 i = 0 ; i < psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces->Size() ; i++)
                    delete (*psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces)[i];
                delete psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces;
			}
#endif
            if(psml_proxy_interface::__proxies != null)
			{
                psml_proxy_interface::__proxies->clear();
                delete psml_proxy_interface::__proxies;
			}
		}
		//----------------------------------------------------
		void psml_simulator::start()
		{
			psml_time duration = psml_time(LLONG_MAX, psml_time::global_time_unit);
			start(duration);
		}
		//----------------------------------------------------
		void psml_simulator::start(Int32 duration, psml_time_unit unit)
		{
			start((double)duration, unit);
		}
		//----------------------------------------------------
		void psml_simulator::start(Double duration, psml_time_unit unit)
		{
			psml_time duration_ = psml_time(duration, unit);
			start(duration_);
		}
		//----------------------------------------------------
		void psml_simulator::start(const psml_time &duration)
		{
			if(!_started)
				_started = true;
			else
				throw ThreadStateException("The simulation has already been _started");

			//SetTimeResolution(_v, _tu);

			/**------------------Global Settings------------**/
			//_numOfLogicalProcessors = Environment::get_ProcessorCount() * 2;
			_simulateUntil = duration._m_value;
			//_simulateUntil = 10000;
			_channelCapacity = 1;
			//_parallelExecution = true;
			//const bool optimisticExecution = false;
			if(_partitioner == null)
			{
				//_partitionerType = DepthFirstSearch;
				//_partitionerType = Levelized;
				//_partitionerType = RoundRobin;
				//_partitionerType = TopologicalSort;
				_partitionerType = StronglyConnectedComponent;
				////_partitionerType = Metis;
			}
			else
				_partitionerType = Customized;

			/**------------------Simulation----------------**/
			//PipelineTandemQueuePartitioner *partitioner = new PipelineTandemQueuePartitioner();
			//RoundRobinPartitioner *partitioner = new RoundRobinPartitioner();

			if(_parallelExecution)
			{
				if(_partitioner == null)
				{
					switch(_partitionerType)
					{
                        case RoundRobin:
                            //std::cout << "PartitionerType: RoundRobin" << std::endl;
                            _partitioner = new RoundRobinPartitioner();
                            break;
                            /*case DepthFirstSearch:
                            //std::cout << "PartitionerType: DepthFirstSearch" << std::endl;
                            _partitioner = new DepthFirstSearchPartitioner(driverLp);
                            break;*/
                        case TopologicalSort:
                            //std::cout << "PartitionerType: TopologicalSort" << std::endl;
                            _partitioner = new TopologicalSortPartitioner();
                            break;
                        case StronglyConnectedComponent:
                            //std::cout << "PartitionerType: TopologicalSort" << std::endl;
                            _partitioner = new StronglyConnectedComponentPartitioner();
                            break;
                        //case Metis:\
                            _partitioner = new MetisPartitioner();\
                            break;
                            /*case Levelized:
                            //std::cout << "PartitionerType: Levelized" << std::endl;
                            _partitioner = new LevelizedPartitioner(driverLp);
                            break;*/
                        default:
                            //std::cout << "PartitionerType: RoundRobin" << std::endl;
                            _partitioner = new RoundRobinPartitioner();
                            break;
					}
				}
			}

			switch(_simulationManagerType)
			{
                case Sequential:
                    _manager = new SequentialSimulationManager(_channelCapacity);
                    break;
                case DeadlockAvoidance:
                    _manager = new DeadlockAvoidanceSimulationManager(_numOfLogicalProcessors, _channelCapacity, _partitioner);
                    break;
                case DeadlockDetection:
                    _manager = new DeadlockDetectionSimulationManager(_numOfLogicalProcessors, _channelCapacity, _partitioner);
                    break;
                default:
                    _manager = new SequentialSimulationManager(_channelCapacity);
                    break;
			}

			//cout << "psml_process::__fiber_clusters.size(): " << psml_process::__fiber_clusters.size() << std::endl;

			Elaborate();

			//exit(0);///

			PrintDesignStatistics();

			_manager->Run(_simulateUntil);
			_manager->WaitOnManagerCompletion();

			//PrintDesignStatistics();

			///psml_pdes_memory_manager::end_of_simulation(this);
		}
		//----------------------------------------------------
		void psml_simulator::Elaborate()
		{
			PrintCopyright();

			cout << "Primary PSML elaboration phase ..." << std::endl;
			//cout << "__processes.size(): " << psml_process::__processes.size() << std::endl;

			_manager->RegisterOnPartitioningCompletionCallback(OnPartitioningCompletion, this);

            if(psml_process::__processes == null)
                throw ArgumentOutOfRangeException("Simulation could not be started because you have not registered any process with the PSML framework");
			if(psml_process::__processes->size() == 0)
				throw ArgumentOutOfRangeException("Simulation could not be started because you have not registered any process with the PSML framework");

			/**------ Check Connectivity based on connector and interface binding through a port -----**/
			if(psml_connector::__connectors != null)
                for(std::vector<psml_connector *>::iterator it = psml_connector::__connectors->begin() ; it != psml_connector::__connectors->end() ; ++it)
                {
                    psml_connector *connector = *it;
                    psml_interface *_interface = connector->_interface;

                    if(_interface == null)
                        throw ArgumentNullException("connector", "Connector (" + String(to_string(connector).c_str()) + ", " + connector->GetName() + ") has not been bound to an interface through a port");
                }
			/**------ Check Connectivity based on binding a proxy to other proxies or an owner port --**/
			/*for(std::vector<psml_proxy_interface *>::iterator it = psml_proxy_interface::__proxies.begin() ; it != psml_proxy_interface::__proxies.end() ; ++it)
			{
			psml_proxy_interface *proxy = *it;
			if(proxy->GetOwnerPort() == null && proxy->GetBoundProxies()->size() == 0)
			throw ArgumentNullException("Proxy", "Proxy (" + String(to_string(proxy).c_str()) + ", " + proxy->GetName() + ") has not an owner port or has not been bound to another proxy");
			}*/
			if(psml_proxy_interface::__proxies != null)
                for(std::vector<psml_proxy_interface *>::iterator it = psml_proxy_interface::__proxies->begin() ; it != psml_proxy_interface::__proxies->end() ; ++it)
                {
                    psml_proxy_interface *proxy = *it;
                    if(proxy->GetBoundPorts()->size() == 0 && proxy->GetBoundProxies()->size() == 0)
                        throw ArgumentNullException("Proxy", "Proxy (" + String(to_string(proxy).c_str()) + ", " + proxy->GetName() + ") has not an owner port or has not been bound to another proxy");
                    ///std::cout << "Component " << proxy->GetOwnerComponent()->GetName().get_BaseStream() << " proxy " << proxy << " boundPorts " << proxy->GetBoundPorts()->size() << " boundProxies " << proxy->GetBoundProxies()->size() << std::endl;
                }
			/**------ Registering the Logical Processes with the Simulation Manager Instance --------**/
			//std::vector<psml_process *> clockProcesses;
			for(std::vector<psml_process *>::iterator it = psml_process::__processes->begin() ; it != psml_process::__processes->end() ; ++it)
			{
				psml_process *p = *it;
				//std::cout << "p " << p->GetName().get_BaseStream() << std::endl; ///
				psml_pdes_lp *lp = p->_lp;
				//if(lp->GetIsClockLogicalProcess())
                //    clockProcesses.push_back(p);
				//else
                    _manager->RegisterLogicalProcess(lp);
			}
			// We should register psml_clock instances at the end of the LP list to comply with SystemC and other SLD
			// semantics becuase they run colck generators after executing other processes in the initialization phase.
			// PS2 simulation manager calls the Initialize methods of the processes before starting the parallel simulation phase.
            /*for(std::vector<psml_process *>::iterator it = clockProcesses.begin() ; it != clockProcesses.end() ; ++it)
			{
				psml_process *p = *it;
				psml_pdes_lp *lp = p->_lp;
                _manager->RegisterLogicalProcess(lp);
			}*/
			//std::cout << "hello " << clockProcesses.size() << std::endl;
			/**------ Elaborating the Structure Hierarchy based on Proxies among Components --------**/
			//std::cout << "Hello 1 "  << std::endl;
			for(std::vector<psml_component *>::iterator it1 = psml_component::__components->begin() ; it1 != psml_component::__components->end() ; ++it1)
			{
				psml_component *component = *it1;
				//std::cout << "---------------------------------------------------" << std::endl; ///
				//std::cout << "component: " << component->GetName().get_BaseStream() << std::endl; ///
				for(std::map<psml_proxy_interface *, psml_proxy_interface *>::iterator it2 = component->_proxies.begin() ; it2 != component->_proxies.end() ; ++it2)
				{
					psml_proxy_interface *proxy = it2->second;
					//std::cout << "proxy connector: " << proxy->GetBoundConnector() << " interface: " << proxy->GetBoundInterface() << std::endl; ///
					/*psml_port *ownerPort = proxy->GetOwnerPort();
					// If the proxy is bound to an owner port.
					if(ownerPort != null)
					{
					psml_connector *singeltonConnector = proxy->GetBoundConnector();
					psml_interface *singeltonInterface = proxy->GetBoundInterface();

					if(singeltonConnector == null || singeltonInterface == null)
					throw ArgumentNullException("Proxy", "Proxy (" + String(to_string(proxy).c_str()) + ", " + proxy->GetName() + ") has not has not been bound to another proxy yet");

					if(ownerPort->GetPortType() == __InPort)
					{
					//std::cout << "ownerPort is __InPort" << std::endl; ///
					psml_process *ownerProcess = ownerPort->GetOwnerProcess();
					if(ownerProcess == null)
					throw ArgumentNullException("port", "InPort (" + String(to_string(ownerPort).c_str()) + ", " + ownerPort->GetName() + ") has not been bound to a process method");
					singeltonConnector->_inports->PushBack(ownerPort);
					singeltonConnector->_dest_processes[ownerProcess] = ownerProcess;
					ownerPort->SetBoundConnector(singeltonConnector);
					ownerPort->SetBoundInterface(singeltonInterface);
					//std::cout << "ownerPort connector " << ownerPort->GetBoundConnector() << std::endl;
					}
					else if(ownerPort->GetPortType() == __OutPort)
					{
					//std::cout << "ownerPort is __OutPort" << std::endl; ///
					psml_process *ownerProcess = ownerPort->GetOwnerProcess();
					if(ownerProcess == null)
					throw ArgumentNullException("ownerProcess", "OutPort (" + String(to_string(ownerPort).c_str()) + ", " + ownerPort->GetName() + ") has not been bound to a process method");
					singeltonConnector->_outports.push_back(ownerPort);
					ownerPort->SetBoundConnector(singeltonConnector);
					ownerPort->SetBoundInterface(singeltonInterface);
					//std::cout << "ownerPort connector " << ownerPort->GetBoundConnector() << std::endl;
					}
					else if(ownerPort->GetPortType() == __GeneralPort)
					{
					//std::cout << "ownerPort is __GeneralPort" << std::endl;
					psml_process *ownerProcess = ownerPort->GetOwnerProcess();
					if(ownerProcess == null)
					throw ArgumentNullException("port", "GPort (" + String(to_string(ownerPort).c_str()) + ", " + ownerPort->GetName() + ") has not been bound to a process method");
					singeltonConnector->_gports.push_back(ownerPort);
					ownerPort->SetBoundConnector(singeltonConnector);
					ownerPort->SetBoundInterface(singeltonInterface);
					//std::cout << "ownerPort connector " << ownerPort->GetBoundConnector() << std::endl;
					}
					else
					throw InvalidOperationException("Port (" + String(to_string(ownerPort).c_str()) + ") has an unknown type");
					}*/

                    /*// If there are any structural casuality errors in the simulation, we have to
                    // suspect this fast for loop. Then, replace it as you have seen such behaviour.
					// If the proxy has child ports.
					std::map<psml_port *, psml_port *> *boundPorts = proxy->GetBoundPorts();
					//std::cout << "psml_simulator boundPorts->size() " << boundPorts->size() << std::endl;
					for(std::map<psml_port *, psml_port *>::iterator it3 = boundPorts->begin() ; it3 != boundPorts->end() ; ++it3)
					{
						//std::cout << "boundPorts2" << std::endl;
						psml_port *child = it3->second;
						psml_connector *singeltonConnector = proxy->GetBoundConnector();
						psml_interface *singeltonInterface = proxy->GetBoundInterface();

						if(singeltonConnector == null || singeltonInterface == null)
							throw ArgumentNullException("Proxy", "Proxy (" + String(to_string(proxy).c_str()) + ", " + proxy->GetName()  + ") has not been bound yet to either directly a (connector,interface) or a (connector,interface) through a proxy");

						if(child->GetPortType() == __InPort)
						{
							//std::cout << "child ownerPort is __InPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "InPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_inports->PushBack(child);
							singeltonConnector->_dest_processes[ownerProcess] = ownerProcess;
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
						}
						else if(child->GetPortType() == __OutPort)
						{
							//std::cout << "child ownerPort is __OutPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "OutPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_outports.push_back(child);
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
						}
						else if(child->GetPortType() == __GeneralPort)
						{
							//std::cout << "child ownerPort is __GeneralPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "GPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_gports.push_back(child);
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
						}
					}*/

                    /// Structural Casuality
					// If the proxy has child ports.
					std::vector<psml_port *> *boundPorts = proxy->GetBoundPortsVector();
					//std::cout << "psml_simulator boundPorts->size() " << boundPorts->size() << std::endl;
					//for(std::map<psml_port *, psml_port *>::iterator it3 = boundPorts->begin() ; it3 != boundPorts->end() ; ++it3)
                    for(std::vector<psml_port *>::iterator it3 = boundPorts->begin() ; it3 != boundPorts->end() ; ++it3)
                    //for(std::vector<psml_port *>::reverse_iterator it3 = boundPorts->rbegin() ; it3 != boundPorts->rend() ; ++it3)
					{
						//std::cout << "boundPorts3" << std::endl;
						psml_port *child = *it3;
						psml_connector *singeltonConnector = proxy->GetBoundConnector();
						psml_interface *singeltonInterface = proxy->GetBoundInterface();

						if(singeltonConnector == null || singeltonInterface == null)
							throw ArgumentNullException("Proxy", "Proxy (" + String(to_string(proxy).c_str()) + ", " + proxy->GetName()  + ") has not been bound yet to either directly a (connector,interface) or a (connector,interface) through a proxy");

						if(child->GetPortType() == __InPort)
						{
							//std::cout << "child ownerPort is __InPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "InPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_inports->PushBack(child);
							singeltonConnector->_dest_processes[ownerProcess] = ownerProcess;
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
							//std::cout << "child ownerPort is __InPort " << ownerProcess->GetName().get_BaseStream() << " singeltonInterface " << singeltonInterface << " singeltonConnector " << singeltonConnector << std::endl;
						}
						else if(child->GetPortType() == __OutPort)
						{
							//std::cout << "child ownerPort is __OutPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "OutPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_outports.push_back(child);
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
							//std::cout << "child ownerPort is __OutPort " << ownerProcess->GetName().get_BaseStream() << " singeltonInterface " << singeltonInterface << " singeltonConnector " << singeltonConnector << std::endl;
						}
						else if(child->GetPortType() == __GeneralPort)
						{
							//std::cout << "child ownerPort is __GeneralPort" << std::endl;
							psml_process *ownerProcess = child->GetOwnerProcess();
							if(ownerProcess == null)
								throw ArgumentNullException("port", "GPort (" + String(to_string(child).c_str()) + ", " + child->GetName() + ") has not been bound to a process method");
							singeltonConnector->_gports.push_back(child);
							child->SetBoundConnector(singeltonConnector);
							child->SetBoundInterface(singeltonInterface);
						}
					}
				}
			}

			/*for(std::vector<psml_connector *>::iterator it = psml_connector::__connectors.begin() ; it != psml_connector::__connectors.end() ; ++it)
			{
				psml_connector *connector = *it;
				psml_interface *_interface = connector->_interface;

				Vector<psml_port *> *inports = connector->_inports;
				vector<psml_port *> outports = connector->_outports;
				vector<psml_port *> gports = connector->_gports;

				std::cout << "Connector (" << to_string(connector).c_str() <<  ", " << connector->GetName().get_BaseStream() << ") have inPorts " << inports->Size() << " outPorts " << outports.size() << " gPorts " << gports.size() << " inPortProcesses: ";

				for(register Int32 i = 0 ; i < inports->Size() ; i++)
				{
					psml_port *inport = (*inports)[i];
					std:: cout << "(" << inport->GetOwnerProcess()->GetName().get_BaseStream() << ", " << inport->GetOwnerProcess() << "), ";
				}

				std::cout << " outPortProcesses: ";
				for(register Int32 i = 0 ; i < outports.size() ; i++)
				{
					psml_port *outport = outports[i];
					std:: cout << "(" << outport->GetOwnerProcess()->GetName().get_BaseStream() << ", " << outport->GetOwnerProcess() << "), ";
				}

				std::cout << " gPortProcesses: ";
				for(register Int32 i = 0 ; i < gports.size() ; i++)
				{
					psml_port *gport = gports[i];
					std:: cout << "(" << gport->GetOwnerProcess()->GetName().get_BaseStream() << ", " << gport->GetOwnerProcess() << "), ";
				}

				std::cout << std::endl;
			}*/
			/**------ Check Connectivity based on binding a pair of (connector,interface) to ports --**/
			if(psml_port::__ports != null)
                for(std::vector<psml_port *>::iterator it = psml_port::__ports->begin() ; it != psml_port::__ports->end() ; ++it)
                {
                    psml_port *port = *it;

                    if(port->GetBoundConnector() == null)
                        throw ArgumentNullException("port", "Port (" + String(to_string(port).c_str()) + ", " + port->GetName() + ") has not been bound yet to either directly a (connector,interface) or a (connector,interface) through a proxy");
                }
			//exit(0);
			//std::cout << "psml_simulator:: Hello 2 "  << std::endl;
			///exit(0);
			/**------------------ Elaborating the Static Port-based Process Connectivity ------------**/
			// Check psml_ports to see if it is connected to a port or proxy or not.
			/*for(std::vector<psml_port *>::iterator it = psml_port::__ports.begin() ; it != psml_port::__ports.end() ; ++it)
			{
			psml_port *port = *it;
			psml_connector *connector = port->GetBoundConnector();
			if(connector == null)
			{
			psml_proxy_interface *proxy = port->GetBoundProxy();
			if(proxy == null)
			throw ArgumentNullException("port", "Port " + port->GetName() + " has not been bound to a connector or proxy");
			}
			}*/

			if(_parallelExecution)
			{
				for(std::vector<psml_process *>::iterator it1 = psml_process::__processes->begin() ; it1 != psml_process::__processes->end() ; ++it1)
				{
					psml_process *target = *it1;
					for(std::map<psml_port *, psml_port *>::iterator it2 = target->_outports.begin() ; it2 != target->_outports.end() ; ++it2)
					{
						psml_port *key = it2->first; // key
						psml_port *outport = it2->second; // val
						psml_connector *connector = outport->GetBoundConnector();
						if(connector == null)
							throw ArgumentNullException("connector", "Port (" + String(to_string(outport).c_str()) + ", " + outport->GetName() + ") has not been bound to a connector");
						// Input ports with respect to the connector are destination logical processes.
						//for(std::vector<psml_port *>::iterator it3 = connector->_inports.begin() ; it3 != connector->_inports.end() ; ++it3)
						for(UInt32 it3 = 0 ; it3 < connector->_inports->Size() ; ++it3)
						{
							psml_port *inport =  (*connector->_inports)[it3];//*it3;
							//if(!inport->GetSensitive()) continue; ///
							psml_process *destination = inport->GetOwnerProcess();
							if(destination == null)
								throw ArgumentNullException("connector", "Port (" + String(to_string(inport).c_str()) + ", " + inport->GetName() + ") has not registered with a process method");
							target->_lp->AddOutputLP(destination->_lp->GetID());
						}
					}
				}
			}
			/**------------------ Elaborating the Static Event-based Process Connectivity ------------**/
			BeforeStaticEventElaboration();
			//cout << "hello0 __static_events.size() " << psml_event::__static_events.size() << std::endl;
			if(_parallelExecution)
			{
                if(psml_event::__static_events != null)
                    for(std::vector<psml_event *>::iterator it1 = psml_event::__static_events->begin() ; it1 != psml_event::__static_events->end() ; ++it1)
                    {
                        //cout << "hello1" << std::endl;
                        psml_event *e = *it1;
                        for(std::vector<psml_process *>::iterator it2 = e->_initiators.begin() ; it2 != e->_initiators.end() ; ++it2)
                        {
                            //std::cout << "hello2" << std::endl;
                            psml_process *initiator = *it2;
                            psml_pdes_lp *target = initiator->_lp;
                            /*for(std::map<psml_port *, psml_port *>::iterator it3 = initiator->_outports.begin() ; it3 != initiator->_outports.end() ; ++it3)
                            {
                            //cout << "hello3" << std::endl;
                            psml_port *key = it3->first; // key
                            psml_port *outport = it3->second; // val
                            psml_connector *connector = outport->GetBoundConnector();
                            for(std::vector<psml_process *>::iterator it4 = e->_subscribers.begin() ; it4 != e->_subscribers.end() ; ++it4)
                            {
                            //cout << "hello4" << std::endl;
                            psml_process *subscriber = *it4;
                            // Now we are safe to connect the destination (subscriber) to the target (initiator)
                            if( connector->_dest_processes.count(subscriber) == 0)
                            {
                            //cout << "hello5" << std::endl;
                            psml_pdes_lp *destination = subscriber->_lp;
                            target->AddOutputLP(destination->GetID());
                            }

                            }
                            }*/

                            for(std::vector<psml_process *>::iterator it3 = e->_subscribers.begin() ; it3 != e->_subscribers.end() ; ++it3)
                            {
                                //std::cout << "hello4" << std::endl;
                                psml_process *subscriber = *it3;
                                psml_pdes_lp *destination = subscriber->_lp;
                                // Now we are safe to connect the destination (subscriber) to the target (initiator)
                                if( !target->ContainOutputLogicalProcess(destination->GetID()) )
                                    target->AddOutputLP(destination->GetID());
                            }
                        }
                    }
			}
			/**------------------ End of Elaboration--------------------------------------------------**/
			for(std::vector<psml_component *>::iterator it = psml_component::__components->begin() ; it != psml_component::__components->end() ; ++it)
			{
				psml_component *component = *it;
				component->before_end_of_elaboration();
			}
			for(std::vector<psml_process *>::iterator it = psml_process::__processes->begin() ; it != psml_process::__processes->end() ; ++it)
			{
                psml_process *process = *it;
                process->before_end_of_elaboration();
			}

			//return ;
			/*for(std::vector<psml_component *>::iterator it1 = psml_component::__components.begin() ; it1 != psml_component::__components.end() ; ++it1)
			{
			psml_component *component = *it1;

			for(std::vector<psml_port *>::iterator it2 = component->_outports.begin() ; it2 != component->_outports.end() ; ++it2)
			{
			psml_port *outport = *it2;
			psml_process *target = outport->GetOwnerProcess();
			psml_connector *connector = outport->GetBoundConnector();
			Vector<psml_port *> *inports = connector->_inports;
			// Consider Here for optimistic execution !
			for(register UInt32 it3 = 0 ; it3 < inports->Size() ; ++it3)
			{
			psml_port *inport = (*inports)[it3];
			psml_process *destination = inport->GetOwnerProcess();
			// Intra-cluster communication.
			if(target->_lp->GetOwnerClusterID() == destination->_lp->GetOwnerClusterID())
			{
			psml_pdes_event *_e = new psml_pdes_event(-1, -1, target->_lp, destination->_lp);
			_e->deletable = false;
			inport->SetEvent(_e);
			//cout << target->_lp->GetOwnerClusterID() << std::endl;
			//exit(0);
			}
			}
			}
			}*/
			/**---------------------------------------------------------------------------------------**/
		}
		//----------------------------------------------------
		void psml_simulator::OnPartitioningCompletion(Object *state)
		{
			//std::cout << "psml_simulator::OnPartitioningCompletion()" << std::endl;
			std::cout << "Secondary PSML elaboration phase after clustering ..." << std::endl;

			psml_simulator *sim = (psml_simulator *)state;

			for(register UInt32 i = 0 ; i < sim->_manager->GetClusterCount() ; i++)
				psml_process::__fiber_clusters.PushBack(null);

#if defined __cycles_enabled__
            psml_process::__cycles = new Vector<UInt32>(sim->_numOfLogicalProcessors);
            for(register UInt32 i = 0 ; i < sim->_manager->GetClusterCount() ; i++)
				psml_process::__cycles->PushBack(0);

            psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces = new Vector<Vector<psml_nonshared_interface *>* >(sim->_numOfLogicalProcessors);
            for(register UInt32 i = 0 ; i < sim->_manager->GetClusterCount() ; i++)
				psml_nonshared_interface::__uncommitted_insensitive_nonshared_interfaces->PushBack(new Vector<psml_nonshared_interface *>(1));
#endif

            //for(std::vector<psml_interface *>::iterator it = psml_interface::__interfaces.begin() ; it != psml_interface::__interfaces.end() ; ++it)
            //{
                //std::cout << "XXX " << (psml_nonshared_interface *)*it << std::endl;//
            //}

			/**------------------ Elaborating Interfaces -----------------**/
			//if(sim->_parallelExecution)//
			{
            if(psml_connector::__connectors != null)
				for(std::vector<psml_connector *>::iterator it1 = psml_connector::__connectors->begin() ; it1 != psml_connector::__connectors->end() ; ++it1)
				{
					psml_connector *connector = *it1;
					psml_interface *_interface = connector->_interface;

					//std::cout << "connector " << connector << " _interface " << _interface << std::endl;
					if(_interface == null)
						throw ArgumentNullException("connector", "Connector (" + String(to_string(connector).c_str()) + ", " + connector->GetName() + ") has not been bound to an interface through a port");

					if(_interface->GetInterfaceType() == __NonSharedInterface)
					{
						// Elaborates non-shared interfaces for intra and inter cluster communications.
						//bool connector_is_on_one_cluster = true;
						//UInt32 firstClusterID = -1;

						/*psml_process *currentProcess = null;
						Vector<psml_port *> *inports = connector->_inports;
						std::vector<psml_port *> outports = connector->_outports;
						for(register UInt32 it2 = 0 ; it2 < inports->Size() ; ++it2)
						{
							psml_port *inport = (*inports)[it2];//*it2;
							psml_nonshared_interface *_if = (psml_nonshared_interface *)inport->GetBoundInterface();
							if(_if->GetIsInstance())
							{
								std::cout << "The interface has already been re-created" << std::endl;
								continue;
							}
							psml_nonshared_interface *_nif = _if->CreateInstance();
							_nif->SetIsInstance(true);
							_nif->RegisterOwnerConnector(connector);
							inport->SetBoundInterface(_nif);
							sim->_psml_nonshared_interfaces.push_back(_nif);
						}
						for(std::vector<psml_port *>::iterator it2 = outports.begin() ; it2 != outports.end() ; ++it2)
						{
							psml_port *outport = *it2;
							psml_nonshared_interface *_if = (psml_nonshared_interface *)outport->GetBoundInterface();
							if(_if->GetIsInstance())
							{
								std::cout << "The interface has already been re-created" << std::endl;
								continue;
							}
							psml_nonshared_interface *_nif = _if->CreateInstance();
							_nif->SetIsInstance(true);
							_nif->RegisterOwnerConnector(connector);
							outport->SetBoundInterface(_nif);
							sim->_psml_nonshared_interfaces.push_back(_nif);
						}*/
#if defined __zero_copy_enabled_for_psml_nonshared_interfaces__
                        //std::cout << "XXXXXXXXXXXXXXXXXX 1" << std::endl;//
						Vector<psml_port *> *inports = connector->_inports;
						std::vector<psml_port *> outports = connector->_outports;
						psml_process *target = null;
						psml_process *destination = null;
						bool inter_cluster_communication_found = false;

						for(register UInt32 it2 = 0 ; it2 < inports->Size() ; ++it2)
						{
							psml_port *inport = (*inports)[it2];
							destination = inport->GetOwnerProcess();

							for(std::vector<psml_port *>::iterator it3 = outports.begin() ; it3 != outports.end() ; ++it3)
							{
								psml_port *outport = *it3;
								target = outport->GetOwnerProcess();

								if(destination->_lp->GetOwnerClusterID() != target->_lp->GetOwnerClusterID())
								{
									inter_cluster_communication_found = true;
									break;
								}
							}

							if(inter_cluster_communication_found)
								break;
						}

						if(inter_cluster_communication_found)
						{
							//std::cout << "inter_cluster_communication_found " << inter_cluster_communication_found << std::endl;

							for(register UInt32 it2 = 0 ; it2 < inports->Size() ; ++it2)
							{
								psml_port *inport = (*inports)[it2];
								psml_nonshared_interface *_if = (psml_nonshared_interface *)inport->GetBoundInterface();

								if(_if->GetIsInstance())
								{
									std::cout << "The interface has already been re-created" << std::endl;
									continue;
								}
								psml_nonshared_interface *_nif = _if->CreateInstance();
								_nif->SetIsInstance(true);
								_nif->RegisterOwnerConnector(connector);
								inport->SetBoundInterface(_nif);
								sim->_psml_nonshared_interfaces.push_back(_nif);
							}

							for(std::vector<psml_port *>::iterator it3 = outports.begin() ; it3 != outports.end() ; ++it3)
							{
								psml_port *outport = *it3;
								psml_nonshared_interface *_if = (psml_nonshared_interface *)outport->GetBoundInterface();

								if(_if->GetIsInstance())
								{
									std::cout << "The interface has already been re-created" << std::endl;
									continue;
								}
								psml_nonshared_interface *_nif = _if->CreateInstance();
								_nif->SetIsInstance(true);
								_nif->RegisterOwnerConnector(connector);
								outport->SetBoundInterface(_nif);
								sim->_psml_nonshared_interfaces.push_back(_nif);
							}
						}
#else
                        //std::cout << "XXXXXXXXXXXXXXXXXX 2" << std::endl;//
                        //std::cout << "XXX " << (psml_nonshared_interface *)_interface << std::endl;//
                        psml_process *currentProcess = null;
						Vector<psml_port *> *inports = connector->_inports;
						std::vector<psml_port *> outports = connector->_outports;
						for(register UInt32 it2 = 0 ; it2 < inports->Size() ; ++it2)
						{
							psml_port *inport = (*inports)[it2];//*it2;
							psml_nonshared_interface *_if = (psml_nonshared_interface *)inport->GetBoundInterface();
							if(_if->GetIsInstance())
							{
								std::cout << "The interface has already been re-created" << std::endl;
								continue;
							}
							psml_nonshared_interface *_nif = _if->CreateInstance();
							_nif->SetIsInstance(true);
							_nif->RegisterOwnerConnector(connector);
							inport->SetBoundInterface(_nif);
							sim->_psml_nonshared_interfaces.push_back(_nif);
							//std::cout << "YYY " << _nif << std::endl;//
							//std::cout << "inport " << inport->GetOwnerProcess()->GetName().get_BaseStream() << " _if " << _if << " _nif " << _nif << std::endl;//
							//std::cout << "inport " << inport->GetOwnerProcess()->GetName().get_BaseStream() << " _if " << _if << std::endl;//
						}
						for(std::vector<psml_port *>::iterator it2 = outports.begin() ; it2 != outports.end() ; ++it2)
						{
							psml_port *outport = *it2;
							psml_nonshared_interface *_if = (psml_nonshared_interface *)outport->GetBoundInterface();
							if(_if->GetIsInstance())
							{
								std::cout << "The interface has already been re-created" << std::endl;
								continue;
							}
							psml_nonshared_interface *_nif = _if->CreateInstance();
							_nif->SetIsInstance(true);
							_nif->RegisterOwnerConnector(connector);
							outport->SetBoundInterface(_nif);
							sim->_psml_nonshared_interfaces.push_back(_nif);
							//std::cout << "ZZZ " << _nif << std::endl;//
							//std::cout << "outport " << outport->GetOwnerProcess()->GetName().get_BaseStream() << " _if " << _if << " _nif " << _nif << std::endl;//
							//std::cout << "outport " << outport->GetOwnerProcess()->GetName().get_BaseStream() << " _if " << _if << std::endl;//
						}
#endif
					}
					else if(_interface->GetInterfaceType() == __SharedInterface)
					{
						// We must determine which interfaces are shared among multiple clusters on a multi-core host.
						// This will be used o automatically protect the interface.
						bool connector_is_on_one_cluster = true;
						Int32 firstClusterID = -1;
						psml_process *currentProcess = null;
						std::vector<psml_port *> gports = connector->_gports;
						UInt32 i = 0;
						for(std::vector<psml_port *>::iterator it2 = gports.begin() ; it2 != gports.end() ; ++it2, i++)
						{
							currentProcess = (*it2)->GetOwnerProcess();
							if(i == 0)
							{
								firstClusterID = currentProcess->_lp->GetOwnerClusterID();
								continue;
							}
							if(firstClusterID != currentProcess->_lp->GetOwnerClusterID())
							{
								connector_is_on_one_cluster = false;
								break;
							}
						}
						if(!connector_is_on_one_cluster)
						{
							psml_shared_interface *_sif = (psml_shared_interface *)_interface;
							_sif->SetMustProtect(true);
							//std::cout << "connector_is_on_one_cluster: " << (connector_is_on_one_cluster ? "true" : "false") << std::endl;
						}
					}
					else
						throw InvalidOperationException("Unknown interface type");
				}
			}

			/**------------------ Mutex Protection -----------------**/
			/*if(sim->_mutexProtection)
			{
			for(std::vector<psml_connector *>::iterator it1 = psml_connector::__connectors.begin() ; it1 != psml_connector::__connectors.end() ; ++it1)
			{
			psml_connector *connector = *it1;
			psml_interface *_interface = connector->_interface;

			if(_interface != null && _interface->GetInterfaceType() == __NonSharedInterface)
			{
			bool connector_is_on_one_cluster = true;
			Int32 firstClusterID = -1;
			psml_process *currentProcess = null;
			std::vector<psml_port *> inports = connector->_inports;
			std::vector<psml_port *> outports = connector->_outports;
			UInt32 i;
			for(std::vector<psml_port *>::iterator it2 = inports.begin(), i = 0; it2 != inports.end() ; ++it2, i++)
			{
			currentProcess = (*it2)->GetOwnerProcess();
			if(i == 0)
			{
			firstClusterID = currentProcess->_lp->GetOwnerClusterID();
			continue;
			}
			if(firstClusterID != currentProcess->_lp->GetOwnerClusterID())
			{
			connector_is_on_one_cluster = false;
			break;
			}
			}
			if(connector_is_on_one_cluster)
			{
			for(std::vector<psml_port *>::iterator it2 = outports.begin() ; it2 != outports.end() ; ++it2)
			{
			currentProcess = (*it2)->GetOwnerProcess();
			if(firstClusterID != currentProcess->_lp->GetOwnerClusterID())
			{
			connector_is_on_one_cluster = false;
			break;
			}
			}
			}

			// Instructs the code to protect the interface based on mutexes.
			if(!connector_is_on_one_cluster)
			{
			cout << "Inter-cluster communication was detected on the connector " << connector->GetName().get_BaseStream() << std::endl;
			psml_nonshared_interface *_nif = (psml_nonshared_interface *)_interface;
			_nif->SetHasMutexProtection(true);
			//Mutex *mmtx = new Mutex();
			_nif->SetMutex(null);
			}
			}
			}
			}*/
			/**------------------ End of Elaboration--------------------------------------------------**/
			/*printf("\n\n---------------------------------------------\n");
			for(std::vector<psml_process *>::iterator it1 = psml_process::__processes.begin() ; it1 != psml_process::__processes.end() ; ++it1)
			{
			psml_process *process = *it1;
			std::cout << "process: " << process->GetName().get_BaseStream() << std::endl;
			for(std::map<psml_port *, psml_port *>::iterator it2 = process->_inports.begin() ; it2 != process->_inports.end() ; ++it2)
			{
			psml_port *inport = it2->second;
			printf("inport %p connector %p interface %p\n", inport, inport->GetBoundConnector(), inport->GetBoundInterface());
			}
			for(std::map<psml_port *, psml_port *>::iterator it2 = process->_outports.begin() ; it2 != process->_outports.end() ; ++it2)
			{
			psml_port *outport = it2->second;
			printf("outport %p connector %p interface %p\n", outport, outport->GetBoundConnector(), outport->GetBoundInterface());
			}

			printf("---------------------------------------------\n");

			}
			cout << "sim->_psml_nonshared_interfaces.size(): "  << sim->_psml_nonshared_interfaces.size() << std::endl;*/

			//return ;
			/*for(std::vector<psml_component *>::iterator it1 = psml_component::__components.begin() ; it1 != psml_component::__components.end() ; ++it1)
			{
			psml_component *component = *it1;

			for(std::vector<psml_port *>::iterator it2 = component->_outports.begin() ; it2 != component->_outports.end() ; ++it2)
			{
			psml_port *outport = *it2;
			psml_process *target = outport->GetOwnerProcess();
			psml_connector *connector = outport->GetBoundConnector();
			Vector<psml_port *> *inports = connector->_inports;
			// Consider Here for optimistic execution !
			for(register UInt32 it3 = 0 ; it3 < inports->Size() ; ++it3)
			{
			psml_port *inport = (*inports)[it3];
			psml_process *destination = inport->GetOwnerProcess();
			// Intra-cluster communication.
			if(target->_lp->GetOwnerClusterID() == destination->_lp->GetOwnerClusterID())
			{
			psml_pdes_event *_e = new psml_pdes_event(-1, -1, target->_lp, destination->_lp);
			_e->deletable = false;
			inport->SetEvent(_e);
			//cout << target->_lp->GetOwnerClusterID() << std::endl;
			//exit(0);
			}
			}
			}
			}*/

            psml_connector::before_end_of_elaboration(sim);
			psml_pdes_memory_manager::before_end_of_elaboration(sim);

			/*for(std::vector<psml_component *>::iterator it1 = psml_component::__components.begin() ; it1 != psml_component::__components.end() ; ++it1)
			{
			psml_component *component = *it1;
			Vector<psml_port*> *inports = component->_inports;

			if(inports->Size() != inports->Capacity())
			{
			Vector<psml_port*> *_inports = new Vector<psml_port*>(inports->Size());
			for(register UInt32 i = 0 ; i < inports->Size() ; i++)
			_inports->PushBack((*inports)[i]);
			component->_inports = _inports;
			delete inports;

			cout << component->GetName().get_BaseStream() << " has " << _inports->Size() << " _inports and " << _inports->Capacity() << " cpacity." << std::endl;
			}

			//inports = component->_inports;

			//cout << component->GetName().get_BaseStream() << " has " << inports.Size() << " inports and " << inports.Capacity() << " cpacity." << std::endl;
			}*/

			//exit(0);
		}
		//----------------------------------------------------
		void psml_simulator::BeforeStaticEventElaboration()
		{
            if(psml_interface::__interfaces != null)
                for(std::vector<psml_interface *>::iterator it = psml_interface::__interfaces->begin() ; it != psml_interface::__interfaces->end() ; ++it)
                {
                    psml_interface *_if = *it;
                    _if->before_static_event_elaboration();
                }

            if(psml_component::__components != null)
                for(std::vector<psml_component *>::iterator it = psml_component::__components->begin() ; it != psml_component::__components->end() ; ++it)
                {
                    psml_component *component = *it;
                    component->before_static_event_elaboration();
                }

			/**------ Check Static Events ------------------------------------------------------------**/
			if(psml_event::__static_events != null)
                for(std::vector<psml_event *>::iterator it = psml_event::__static_events->begin() ; it != psml_event::__static_events->end() ; ++it)
                {
                    //std::cout << "event" << std::endl;
                    psml_event *e = *it;
                    if(e->_initiators.size() == 0)
                        throw ArgumentOutOfRangeException("Event", "Event (" + String(to_string(e).c_str()) + ", " + e->GetName() + ", " + Convert::ToString((Int64)e->_eventID) + ") has not at least one initiator registered");
                    if(e->_subscribers.size() == 0)
                        throw ArgumentOutOfRangeException("Event", "Event (" + String(to_string(e).c_str()) + ", " + e->GetName() + ", " + Convert::ToString((Int64)e->_eventID) + ") has not at least one subscriber registered");
                }
		}
		//----------------------------------------------------
		Double psml_simulator::GetTimeResolution()
		{
			return psml_time::global_time_resolution;
		}
		//----------------------------------------------------
		void psml_simulator::SetTimeResolution(Double v, psml_time_unit tu)
		{
			// first perform the necessary checks

			// must be positive
			if( v < 0.0 )
				throw ArgumentOutOfRangeException("v", "value is not positive" );

			// must be a power of ten
			double dummy;
			if( modf( log10( v ), &dummy ) != 0.0 )
				throw ArgumentOutOfRangeException("v", "value not a power of ten" );

			// can be specified only once
			if(psml_time::global_time_resolution_specified)
				throw InvalidOperationException("The time resolution has already been specified");

			// must be larger than or equal to 1 fs
			volatile double resolution = v * psml_time::GetTimeResolution(tu);
			if( resolution < 1.0 )
				throw ArgumentOutOfRangeException("v", "value smaller than 1 fs" );

			// recalculate the default time unit
			volatile double time_unit = Double(psml_time::global_default_time_unit ) * ( psml_time::global_time_resolution / resolution );
			if( time_unit < 1.0 )
			{
				cout << "Default time unit changed" << std::endl;
				psml_time::global_default_time_unit = 1;
			}
			else
				psml_time::global_default_time_unit = static_cast<UInt64>(time_unit);

			psml_time::global_time_unit = tu;

			psml_time::global_time_resolution = resolution;
			psml_time::global_default_time_unit = resolution;

			psml_time::global_time_resolution_specified = true;

		}
		//----------------------------------------------------
		void psml_simulator::PrintCopyright()
		{
			std::cout << "\n\t PDES-Compliant System Modeling Language - PSML v2.0.1 \n\t PSML Runtime Framework v2.0.1 \n";
			std::cout << "\t Current Release: Full Support for GL/RTL/TLM/Mixed\n";
			std::cout << "\t Modeling and Simulation.\n";
			std::cout << "\t Copyright (c) 2014-" << DateTime::get_Now().get_Year() << " by Shahed University,\n";
			std::cout << "\t ALL RIGHTS RESERVED\n\n" << std::endl;
			std::cout << "Preparing system-level simulation runtime environment ..." << std::endl;
		}
		//----------------------------------------------------
		void psml_simulator::PrintDesignStatistics()
		{
			std::cout << "\n\n********** Model and Pre-Simulation Statistics **********\n";
			if(psml_component::__components != null)
                std::cout << "\t Number of Components: " << psml_component::__components->size() << "\n";
            else
                std::cout << "\t Number of Components: 0\n";
            if(psml_process::__processes != null)
                std::cout << "\t Number of Processes: " << psml_process::__processes->size() << "\n";
            else
                std::cout << "\t Number of Processes: 0\n";
            if(psml_port::__ports != null)
                std::cout << "\t Number of Ports: " << psml_port::__ports->size() << "\n";
            else
                std::cout << "\t Number of Ports: 0\n";
            if(psml_connector::__connectors != null)
                std::cout << "\t Number of Connectors: " << psml_connector::__connectors->size() << "\n";
            else
                std::cout << "\t Number of Connectors: 0\n";
            if(psml_interface::__interfaces != null)
                std::cout << "\t Number of Interfaces: " << psml_interface::__interfaces->size() << "\n";
            else
                std::cout << "\t Number of Interfaces: 0\n";
            if(psml_event::__static_events != null)
                std::cout << "\t Number of Static Events: " << psml_event::__static_events->size() << "\n";
            else
                std::cout << "\t Number of Static Events: 0\n";
			std::cout << "\t Simulation Until: " << psml_time(_simulateUntil) << "\n";
			if(_parallelExecution)
			{
				std::cout << "\t Execution Mode: Parallel DES\n";
				std::cout << "\t Number of Logical Processors: " << _numOfLogicalProcessors << "\n";
				switch(_partitionerType)
				{
                    case RoundRobin:
                        std::cout << "\t Partitioner: RoundRobin" << std::endl;
                        break;
                        std::cout << "\t Partitioner: DepthFirstSearch" << std::endl;
                    case TopologicalSort:
                        std::cout << "\t Partitioner: TopologicalSort" << std::endl;
                        break;
                    //case Metis:\
                        std::cout << "\t Partitioner: Metis" << std::endl;\
                        break;
                    case StronglyConnectedComponent:
                        std::cout << "\t Partitioner: StronglyConnectedComponent" << std::endl;
                        break;
                    case Levelized:
                        std::cout << "\t Partitioner: Levelized" << std::endl;
                        break;
                    case Customized:
                        std::cout << "\t Partitioner: Customized" << std::endl;
                        break;
                    default:
                        std::cout << "\t Partitioner: RoundRobin" << std::endl;
                        break;
				}
			}
			else
				std::cout << "\t Execution Mode: Sequential DES\n";
			std::cout << "\t Initial Capacity of Queues: " << _channelCapacity << "\n";
			std::cout << "********** End Of Statistics ****************************\n";
			std::cout << std::endl;
		}
		//----------------------------------------------------
	};
};
//**************************************************************************************************************//
