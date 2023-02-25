# PSML: parallel system modelling and simulation language for electronic system level


Fast and accurate simulation is highly desirable for efficient and effective system design due to the ever-increasing complexity of embedded and cyber physical systems. Parallel discrete-event simulation (PDES) is the main technique to solve this problem for large-scale system-level models. PDES works based on state space partitioning by using the so-called logical process worldview. This repository includes the parallel system modeling and simulation language (PSML), along with its formalized distributed parallel simulation kernel, that provides execution of hardware models in order to improve simulation speed significantly.

PSML is a parallel system-level simulation language for fast modelling and simulation of computers systems, in particular, digital electronic systems. This language, along with the collection of introduced tools, provides the ability to simulate models at different levels of abstraction by using a wide range of PDES synchronisation protocols in order for the designer to leverage the massive parallelism of today’s multi-core and many-core platforms. One of the primary goals of PSML language is to enable asynchronous, explicitly parallel system-level modelling—that is, real parallel modelling of systems above the RTL that might be implemented in hardware, software or a combination of the two. Of course, RTL and GL modelling are also possible in PSML. The PSML conceptual framework is shown below:

![PSML conceptual framework](/assets/images/psml.png)

PSML framework is completely built on top of the Parvicursor infrastracture (see https://github.com/poshtkohi/pads). PSML compiler emits native Parvicursor-compliant codes that can be compiled by any existing C++ compiler. This compiler generates an intermediate C++ code which is called Native PSML. The parallel algorithm is shown below:

![The PSML tool flow: PSML compiler suite and parallel execution framework](/assets/images/psml-compiler.png)

It shows the PSML tool flow and parallel execution toolset. A PSML model is first compiled and then executed on top of the parallel computing platform. There are several tools, components and runtime libraries, which resides in the source code directory.

This repository contains a conservatively parallel implementation of the Native PSML with several examples. I wrote it while working as a researcher in the High-Performance Computing Centre (HPCC) at Shahed University as part of the following journal paper:

[1] A. Poshtkohi, M. B. Ghaznavi-Ghoushchi, K. Saghafi, _PSML: Parallel System Modeling and Simulation Language for Electronic System Level_, Journal of Supercomputing, 75:5 (2019), 2691–2724. doi: 10.1007/s11227-018-2682-1

**Note 1:** To make use of this project, xSim simulation engine from this repository https://github.com/poshtkohi/pdes must be first installed.
**Note 2:** There is currently no plan to make the PSML compiler open-source, but the Native PSML language can be seamlessly be used to develop complex system level models similar to the SystemC language (SystemC is a set of C++ classes and macros which provide an event-driven simulation interface).

**Licence:** Note that the code can be changed and reused as long as you keep the copyright inside and at the beginning of source files in the source directory unchanged. If this language  is used everywhere, it should be cited as _A. Poshtkohi, M. B. Ghaznavi-Ghoushchi, K. Saghafi, PSML: Parallel System Modeling and Simulation Language for Electronic System Level, Journal of Supercomputing, 75:5 (2019), 2691–2724. doi: 10.1007/s11227-018-2682-1_.
