# PSML: parallel system modeling and simulation language for electronic system level


PSML is a parallel system-level simulation language for fast modeling and simulation of computers systems, in particular, digital electronic systems. This language, along with the collection of introduced tools, provides the ability to simulate models at different levels of abstraction by using a wide range of PDES synchronisation protocols in order for the designer to leverage the massive parallelism of today’s multi-core and many-core platforms. One of the primary goals of PSML language is to enable asynchronous, explicitly parallel system-level modeling—that is, real parallel modeling of systems above the RTL that might be implemented in hardware, software or a combination of the two. Of course, RTL and GL modeling are also possible in PSML. PSML framework is completely built on top of the Parvicursor. PSML compiler emits native Parvicursor-compliant codes that can be compiled by any existing C++ compiler. This complier generate an intermediate C++ code which is called native PSML.

This repository contains a conservatively parallel implemenation of the Native PSML with several examples. I wrote it while working as a researcher in the High-Performance Computing Centre (HPCC) at Shahed University as part of the following journal paper


[1] A. Poshtkohi, M. B. Ghaznavi-Ghoushchi, K. Saghafi, _PSML: Parallel System Modeling and Simulation Language for Electronic System Level_, Journal of Supercomputing, 75:5 (2019), 2691–2724. doi: 10.1007/s11227-018-2682-1

**Note:** to make use of this project, xSim simulation engine from this repository https://github.com/poshtkohi/pdes must be first installed.

**Licence:** Note that the code can be changed and reused as long as you keep the copyright inside and at the beginning of source files in the source directory unchanged. If this ;anguage  is used everywhere, it should be cited as _A. Poshtkohi, M. B. Ghaznavi-Ghoushchi, K. Saghafi, PSML: Parallel System Modeling and Simulation Language for Electronic System Level, Journal of Supercomputing, 75:5 (2019), 2691–2724. doi: 10.1007/s11227-018-2682-1_.
