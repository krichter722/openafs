/*!
 * \addtogroup rxrpc-spec RX RPC Specification
 * RX RPC Specification
 * @{
 *
 * \mainpage AFS-3 Programmer's Reference: Specification for the Rx Remote
 * Procedure Call Facility
 *
 * AFS-3 Programmer's Reference:  
 * 
 * Specification for the Rx Remote Procedure Call Facility 
 * \author Edward R. Zayas 
 * Transarc Corporation 
 * \version 1.2
 * \date 28 August 1991 10:11 .cCopyright 1991 Transarc Corporation All Rights
 * Reserved FS-00-D164 
 * 
 * 	\page chap1 Chapter 1 -- Overview of the Rx RPC system                                
 *                                                                                 
 * 	\section sec1-1 Section 1.1: Introduction to Rx                                      
 * 
 * \par 
 * The Rx package provides a high-performance, multi-threaded, and secure
 * mechanism by which 
 * remote procedure calls (RPCs) may be performed between programs executing
 * anywhere in a 
 * network of computers. The Rx protocol is adaptive, conforming itself to
 * widely varying 
 * network communication media. It allows user applications to define and
 * insert their own 
 * security modules, allowing them to execute the precise end-to-end
 * authentication algorithms 
 * required to suit their needs and goals. Although pervasive throughout the
 * AFS distributed 
 * file system, all of its agents, and many of its standard application
 * programs, Rx is entirely 
 * separable from AFS and does not depend on any of its features. In fact, Rx
 * can be used to build applications engaging in RPC-style communication under
 * a variety of unix-style file systems. There are in-kernel and user-space
 * implementations of the Rx facility, with both sharing the same interface.
 * \par 
 * This document provides a comprehensive and detailed treatment of the Rx RPC
 * package. 
 * 
 * 	\section sec1-2 Section 1.2: Basic Concepts 
 * 
 * \par 
 * The Rx design operates on the set of basic concepts described in this
 * section. 
 * 
 * 	\subsection sec1-2-1 Section 1.2.1: Security
 * 
 * \par 
 * The Rx architecture provides for tight integration between the RPC mechanism
 * and methods for making this communication medium secure. As elaborated in
 * Section 5.3.1.3 and illustrated by the built-in rxkad security system
 * described in Chapter 3, Rx defines the format for a generic security module,
 * and then allows application programmers to define and activate
 * instantiations of these modules. Rx itself knows nothing about the internal
 * details of any particular security model, or the module-specific state it
 * requires. It does, however, know when to call the generic security
 * operations, and so can easily execute the security algorithm defined. Rx
 * does maintain basic state per connection on behalf of any given security
 * class. 
 * 
 * 	\subsection sec1-2-2 Section 1.2.2: Services 
 * 
 * \par 
 * An Rx-based server exports services, or specific RPC interfaces that
 * accomplish certain tasks. Services are identified by (host-address,
 * UDP-port, serviceID) triples. An Rx service is installed and initialized on
 * a given host through the use of the rx NewService() routine (See Section
 * 5.6.3). Incoming calls are stamped with the Rx service type, and must match
 * an installed service to be accepted. Internally, Rx services also carry
 * string names which identify them, which is useful for remote debugging and
 * statistics-gathering programs. The use of a service ID allows a single
 * server process to export multiple, independently-specified Rx RPC services. 
 * \par 
 * Each Rx service contains one or more security classes, as implemented by
 * individual security objects. These security objects implement end-to-end
 * security protocols. Individual peer-to-peer connections established on
 * behalf of an Rx service will select exactly one of the supported security
 * objects to define the authentication procedures followed by all calls
 * associated with the connection. Applications are not limited to using only
 * the core set of built-in security objects offered by Rx. They are free to
 * define their own security objects in order to execute the specific protocols
 * they require. 
 * \par 
 * It is possible to specify both the minimum and maximum number of lightweight
 * processes available to handle simultaneous calls directed to an Rx service.
 * In addition, certain procedures may be registered with the service and
 * called at specific times in the course of handling an RPC request. 
 * 
 * 	\subsection sec1-2-3 Section 1.2.3: Connections 
 * 
 * \par 
 * An Rx connection represents an authenticated communication path, allowing a
 * sequence of multiple asynchronous conversations (calls). Each connection is
 * identified by a connection ID. The low-order bits of the connection ID are
 * reserved so that they may be stamped with the index of a particular call
 * channel. With up to RX MAXCALLS concurrent calls (set to 4 in this
 * implementation), the bottom two bits are set aside for this purpose. The
 * connection ID is not sufficient to uniquely identify an Rx connection by
 * itself. Should a client crash and restart, it may reuse a connection ID,
 * causing inconsistent results. Included with the connection ID is the epoch,
 * or start time for the client side of the connection. After a crash, the next
 * incarnation of the client will choose a different epoch value. This will
 * differentiate the new incarnation from the orphaned connection record on the
 * server side. 
 * \par 
 * Each connection is associated with a parent service, which defines a set of
 * supported security models. At creation time, an Rx connection selects the
 * particular security protocol it will implement, referencing the associated
 * service. The connection structure maintains state for each individual call
 * simultaneously handled. 
 * 
 * 	\subsection sec1-2-4 Section 1.2.4: Peers 
 * 
 * \par 
 * For each connection, Rx maintains information describing the entity, or
 * peer, on the other side of the wire. A peer is identified by a (host,
 * UDP-port) pair, with an IP address used to identify the host. Included in
 * the information kept on this remote communication endpoint are such network
 * parameters as the maximum packet size supported by the host, current
 * readings on round trip time and retransmission delays, and packet skew (see
 * Section 1.2.7). There are also congestion control fields, including
 * retransmission statistics and descriptions of the maximum number of packets
 * that may be sent to the peer without pausing. Peer structures are shared
 * between connections whenever possible, and, hence, are reference-counted. A
 * peer object may be garbage-collected if it is not actively referenced by any
 * connection structure and a sufficient period of time has lapsed since the
 * reference count dropped to zero. 
 * 
 * 	\subsection sec1-2-5 Section 1.2.5: Calls 
 * 
 * \par 
 * An Rx call represents an individual RPC being executed on a given
 * connection. As described above, each connection may have up to RX MAXCALLS
 * calls active at any one instant. The information contained in each call
 * structure is specific to the given call. 
 * \par 
 * "Permanent" call state, such as the call number, is maintained in the
 * connection structure itself. 
 * 
 * 	\subsection sec1-2-6 Section 1.2.6: Quotas 
 * 
 * \par 
 * Each attached server thread must be able to make progress to avoid system
 * deadlock. The Rx facility ensures that it can always handle the arrival of
 * the next unacknowledged data packet for an attached call with its system of
 * packet quotas. A certain number of packets are reserved per server thread
 * for this purpose, allowing the server threads to queue up an entire window
 * full of data for an active call and still have packet buffers left over to
 * be able to read its input without blocking. 
 * 
 * 	\subsection sec1-2-7 Section 1.2.7: Packet Skew 
 * 
 * \par 
 * If a packet is received n packets later than expected (based on packet
 * serial numbers), then we define it to have a skew of n. The maximum skew
 * values allow us to decide when a packet hasn't been received yet because it
 * is out of order, as opposed to when it is likely to have been dropped. 
 * 
 * 	\subsection sec1-2-8 Section 1.2.8: Multicasting 
 * 
 * \par 
 * The rx multi.c module provides for multicast abilities, sending an RPC to
 * several targets simultaneously. While true multicasting is not achieved, it
 * is simulated by a rapid succession of packet transmissions and a collection
 * algorithm for the replies. A client program, though, may be programmed as if
 * multicasting were truly taking place. Thus, Rx is poised to take full
 * advantage of a system supporting true multicasting with minimal disruption
 * to the existing client code base. 
 * 
 * 	\section sec1-3 Section 1.3: Scope 
 * 
 * \par 
 * This paper is a member of a documentation suite providing specifications as
 * to the operation and interfaces offered by the various AFS servers and
 * agents. Rx is an integral part of the AFS environment, as it provides the
 * high-performance, secure pathway by which these system components
 * communicate across the network. Although AFS is dependent on Rx's services,
 * the reverse is not true. Rx is a fully independent RPC package, standing on
 * its own and usable in other environments. 
 * \par 
 * The intent of this work is to provide readers with a sufficiently detailed
 * description of Rx that they may proceed to write their own applications on
 * top of it. In fact, code for a sample Rx server and client are provided. 
 * \par 
 * One topic related to Rx will not be covered by this document, namely the
 * Rxgen stub generator. Rather, rxgen is addressed in a separate document. 
 * 
 * 	\section sec1-4 Section 1.4: Document Layout 
 * 
 * \par 
 * After this introduction, Chapter 2 will introduce and describe various
 * facilities and tools that support Rx. In particular, the threading and
 * locking packages used by Rx will be examined, along with a set of timer and
 * preemption tools. Chapter 3 proceeds to examine the details of one of the
 * built-in security modules offered by Rx. Based on the Kerberos system
 * developed by MIT's Project Athena, this rxkad module allows secure, ecrypted
 * communication between the server and client ends of the RPC. Chapter 5 then
 * provides the full Rx programming interface, and Chapter 6 illustrates the
 * use of this programming interface by providing a fully-operational
 * programming example employing Rx. This rxdemo suite is examined in detail,
 * ranging all the way from a step-by-step analysis of the human-authored
 * files, and the Rxgen-generated files upon which they are based, to the
 * workings of the associated Makefile. Output from the example rxdemo server
 * and client is also provided. 
 * 
 * 	\section sec1-5 Section 1.5: Related Documents 
 * 
 * \par 
 * Titles for the full suite of AFS specification documents are listed below.
 * All of the servers and agents making up the AFS computing environment,
 * whether running in the unix kernel or in user space, utilize an Rx RPC
 * interface through which they export their services. 
 * \par
 * \li 	AFS-3 Programmer's Reference: Architectural Overview: This paper
 * provides an architectual overview of the AFS distributed file system,
 * describing the full set of servers and agents in a coherent way,
 * illustrating their relationships to each other and examining their
 * interactions. 
 * \li 	AFS-3 Programmer's Reference: file Server/Cache Manager Interface: This
 * document describes the workings and interfaces of the two primary AFS
 * agents, the file Server and Cache Manager. The file Server provides a
 * centralized disk repository for sets of files, regulating access to them.
 * End users sitting on client machines rely on the Cache Manager agent,
 * running in their kernel, to act as their agent in accessing the data stored
 * on file Server machines, making those files appear as if they were really
 * housed locally. 
 * \li 	AFS-3 Programmer's Reference:Volume Server/Volume Location Server
 * Interface: This document describes the services through which "containers"
 * of related user data are located and managed. 
 * \li 	AFS-3 Programmer's Reference: Protection Server Interface: This paper
 * describes the server responsible for mapping printable user names to and
 * from their internal AFS identifiers. The Protection Server also allows users
 * to create, destroy, and manipulate "groups" of users, which are suitable for
 * placement on access control lists (ACLs). 
 * \li 	AFS-3 Programmer's Reference: BOS Server Interface: This paper
 * explicates the "nanny" service which assists in the administrability of the
 * AFS environment. 
 * \par 
 * In addition to these papers, the AFS 3.1 product is delivered with its own
 * user, system administrator, installation, and command reference documents. 
 * 
 * 	\page chap2 Chapter 2 -- The LWP Lightweight Process Package 
 * 
 * 	\section sec2-1 Section 2.1: Introduction 
 * \par 
 * This chapter describes a package allowing multiple threads of control to
 * coexist and cooperate within one unix process. Each such thread of control
 * is also referred to as a lightweight process, in contrast to the traditional
 * unix (heavyweight) process. Except for the limitations of a fixed stack size
 * and non-preemptive scheduling, these lightweight processes possess all the
 * properties usually associated with full-fledged processes in typical
 * operating systems. For the purposes of this document, the terms lightweight
 * process, LWP, and thread are completely interchangeable, and they appear
 * intermixed in this chapter. Included in this lightweight process facility
 * are various sub-packages, including services for locking, I/O control,
 * timers, fast time determination, and preemption. 
 * \par 
 * The Rx facility is not the only client of the LWP package. Other LWP clients
 * within AFS include the file Server, Protection Server, BOS Server, Volume
 * Server, Volume Location Server, and the Authentication Server, along with
 * many of the AFS application programs. 
 * 
 * 	\section sec2-2 Section 2.2: Description 
 * 
 * 	\subsection Section 2.2.1: sec2-2-1 LWP Overview 
 * 
 * \par 
 * The LWP package implements primitive functions that provide the basic
 * facilities required to enable procedures written in C to execute
 * concurrently and asynchronously. The LWP package is meant to be
 * general-purpose (note the applications mentioned above), with a heavy
 * emphasis on simplicity. Interprocess communication facilities can be built
 * on top of this basic mechanism and in fact, many different IPC mechanisms
 * could be implemented. 
 * \par 
 * In order to set up the threading support environment, a one-time invocation
 * of the LWP InitializeProcessSupport() function must precede the use of the
 * facilities described here. This initialization function carves an initial
 * process out of the currently executing C procedure and returns its thread
 * ID. For symmetry, an LWP TerminateProcessSupport() function may be used
 * explicitly to release any storage allocated by its counterpart. If this
 * function is used, it must be issued from the thread created by the original
 * LWP InitializeProcessSupport() invocation. 
 * \par 
 * When any of the lightweight process functions completes, an integer value is
 * returned to indicate whether an error condition was encountered. By
 * convention, a return value of zero indicates that the operation succeeded. 
 * \par 
 * Macros, typedefs, and manifest constants for error codes needed by the
 * threading mechanism are exported by the lwp.h include file. A lightweight
 * process is identified by an object of type PROCESS, which is defined in the
 * include file. 
 * \par 
 * The process model supported by the LWP operations is based on a
 * non-preemptive priority dispatching scheme. A priority is an integer in the
 * range [0..LWP MAX PRIORITY], where 0 is the lowest priority. Once a given
 * thread is selected and dispatched, it remains in control until it
 * voluntarily relinquishes its claim on the CPU. Control may be relinquished
 * by either explicit means (LWP_DispatchProcess()) or implicit means (through
 * the use of certain other LWP operations with this side effect). In general,
 * all LWP operations that may cause a higher-priority process to become ready
 * for dispatching preempt the process requesting the service. When this
 * occurs, the dispatcher mechanism takes over and automatically schedules the
 * highest-priority runnable process. Routines in this category, where the
 * scheduler is guaranteed to be invoked in the absence of errors, are: 
 * \li LWP_WaitProcess() 
 * \li LWP_MwaitProcess() 
 * \li LWP_SignalProcess() 
 * \li LWP_DispatchProcess() 
 * \li LWP_DestroyProcess() 
 * \par 
 * The following functions are guaranteed not to cause preemption, and so may
 * be issued with no fear of losing control to another thread: 
 * \li LWP_InitializeProcessSupport() 
 * \li LWP_NoYieldSignal() 
 * \li LWP_CurrentProcess() 
 * \li LWP_ActiveProcess() 
 * \li LWP_StackUsed() 
 * \li LWP_NewRock() 
 * \li LWP_GetRock() 
 * \par 
 * The symbol LWP NORMAL PRIORITY, whose value is (LWP MAX PRIORITY-2),
 * provides a reasonable default value to use for process priorities. 
 * \par 
 * The lwp debug global variable can be set to activate or deactivate debugging
 * messages tracing the flow of control within the LWP routines. To activate
 * debugging messages, set lwp debug to a non-zero value. To deactivate, reset
 * it to zero. All debugging output from the LWP routines is sent to stdout. 
 * \par 
 * The LWP package checks for stack overflows at each context switch. The
 * variable that controls the action of the package when an overflow occurs is
 * lwp overflowAction. If it is set to LWP SOMESSAGE, then a message will be
 * printed on stderr announcing the overflow. If lwp overflowAction is set to
 * LWP SOABORT, the abort() LWP routine will be called. finally, if lwp
 * overflowAction is set to LWP SOQUIET, the LWP facility will ignore the
 * errors. By default, the LWP SOABORT setting is used. 
 * \par 
 * Here is a sketch of a simple program (using some psuedocode) demonstrating
 * the high-level use of the LWP facility. The opening #include line brings in
 * the exported LWP definitions. Following this, a routine is defined to wait
 * on a "queue" object until something is deposited in it, calling the
 * scheduler as soon as something arrives. Please note that various LWP
 * routines are introduced here. Their definitions will appear later, in
 * Section 2.3.1. 
 * 
 * \code
 * #include <afs/lwp.h> 
 * static read_process(id) 
 * int *id; 
 * {  /* Just relinquish control for now */
 * 	LWP_DispatchProcess(); 
 * 	for 	(;;) 
 * 	{  
 * 		/* Wait until there is something in the queue */
 * 		while 	(empty(q)) LWP_WaitProcess(q); 
 * 		/* Process the newly-arrived queue entry */
 * 		LWP_DispatchProcess(); 
 * 	} 
 * } 
 * \endcode
 * 
 * \par
 * The next routine, write process(), sits in a loop, putting messages on the
 * shared queue and signalling the reader, which is waiting for activity on the
 * queue. Signalling a thread is accomplished via the LWP SignalProcess()
 * library routine. 
 * 
 * \code
 * static write_process() 
 * { ... 
 * 	/* Loop, writing data to the shared queue.  */
 * 	for 	(mesg = messages; *mesg != 0; mesg++) 
 * 	{ 
 * 		insert(q, *mesg); 
 * 		LWP_SignalProcess(q); 
 * 	} 
 * } 
 * \endcode
 * 
 * \par
 * finally, here is the main routine for this demo pseudocode. It starts by
 * calling the LWP initialization routine. Next, it creates some number of
 * reader threads with calls to LWP CreateProcess() in addition to the single
 * writer thread. When all threads terminate, they will signal the main routine
 * on the done variable. Once signalled, the main routine will reap all the
 * threads with the help of the LWP DestroyProcess() function. 
 * 
 * \code
 * main(argc, argv) 
 * int argc; 
 * char **argv; 
 * { 
 * 	PROCESS *id;  /* Initial thread ID */
 * 	/* Set up the LWP package, create the initial thread ID. */
 * 	LWP_InitializeProcessSupport(0, &id); 
 * 	/* Create a set of reader threads.  */
 * 	for (i = 0; i < nreaders; i++) 
 * 		LWP_CreateProcess(read_process, STACK_SIZE, 0, i, "Reader",
 * 		&readers[i]); 
 * 
 * 	/* Create a single writer thread.  */
 * 	LWP_CreateProcess(write_process, STACK_SIZE, 1, 0, "Writer", &writer); 
 * 	/* Wait for all the above threads to terminate.  */
 * 	for (i = 0; i <= nreaders; i++) 
 * 		LWP_WaitProcess(&done); 
 * 
 * 	/* All threads are done. Destroy them all.  */
 * 	for (i = nreaders-1; i >= 0; i--) 
 * 		LWP_DestroyProcess(readers[i]); 
 * } 
 * \endcode
 * 
 * 	\subsection sec2-2-2 Section 2.2.2: Locking 
 * \par
 * The LWP locking facility exports a number of routines and macros that allow
 * a C programmer using LWP threading to place read and write locks on shared
 * data structures.  This locking facility was also written with simplicity in
 * mind. 
 * \par
 * In order to invoke the locking mechanism, an object of type struct Lock must
 * be associated with the object. After being initialized with a call to
 * LockInit(), the lock object is used in invocations of various macros,
 * including ObtainReadLock(), ObtainWriteLock(), ReleaseReadLock(),
 * ReleaseWriteLock(), ObtainSharedLock(), ReleaseSharedLock(), and
 * BoostSharedLock(). 
 * \par
 * Lock semantics specify that any number of readers may hold a lock in the
 * absence of a writer. Only a single writer may acquire a lock at any given
 * time. The lock package guarantees fairness, legislating that each reader and
 * writer will eventually obtain a given lock. However, this fairness is only
 * guaranteed if the priorities of the competing processes are identical. Note
 * that ordering is not guaranteed by this package. 
 * \par
 * Shared locks are read locks that can be "boosted" into write locks. These
 * shared locks have an unusual locking matrix. Unboosted shared locks are
 * compatible with read locks, yet incompatible with write locks and other
 * shared locks. In essence, a thread holding a shared lock on an object has
 * effectively read-locked it, and has the option to promote it to a write lock
 * without allowing any other writer to enter the critical region during the
 * boost operation itself. 
 * \par
 * It is illegal for a process to request a particular lock more than once
 * without first releasing it. Failure to obey this restriction will cause
 * deadlock. This restriction is not enforced by the LWP code. 
 * \par
 * Here is a simple pseudocode fragment serving as an example of the available
 * locking operations. It defines a struct Vnode object, which contains a lock
 * object. The get vnode() routine will look up a struct Vnode object by name,
 * and then either read-lock or write-lock it. 
 * \par
 * As with the high-level LWP example above, the locking routines introduced
 * here will be fully defined later, in Section 2.3.2. 
 * 
 * \code
 * #include <afs/lock.h> 
 * 
 * struct Vnode { 
 * 	... 
 * 	struct Lock lock;  Used to lock this vnode  
 * ... }; 
 * 
 * #define READ 0 
 * #define WRITE 1 
 * 
 * struct Vnode *get_vnode(name, how) char *name; 
 * int how; 
 * { 
 * 	struct Vnode *v; 
 * 	v = lookup(name); 
 * 	if (how == READ) 
 * 		ObtainReadLock(&v->lock); 
 * 	else 
 * 		ObtainWriteLock(&v->lock); 
 * } 
 * \endcode
 * 
 * 
 * 	\subsection sec2-2-3 Section 2.2.3: IOMGR 
 * 
 * \par
 * The IOMGR facility associated with the LWP service allows threads to wait on
 * various unix events. The exported IOMGR Select() routine allows a thread to
 * wait on the same set of events as the unix select() call. The parameters to
 * these two routines are identical. IOMGR Select() puts the calling LWP to
 * sleep until no threads are active. At this point, the built-in IOMGR thread,
 * which runs at the lowest priority, wakes up and coalesces all of the select
 * requests together. It then performs a single select() and wakes up all
 * threads affected by the result. 
 * \par
 * The IOMGR Signal() routine allows an LWP to wait on the delivery of a unix
 * signal. The IOMGR thread installs a signal handler to catch all deliveries
 * of the unix signal. This signal handler posts information about the signal
 * delivery to a global data structure. The next time that the IOMGR thread
 * runs, it delivers the signal to any waiting LWP. 
 * \par
 * Here is a pseudocode example of the use of the IOMGR facility, providing the
 * blueprint for an implemention a thread-level socket listener. 
 * 
 * \code
 * void rpc_SocketListener() 
 * { 
 * 	int ReadfdMask, WritefdMask, ExceptfdMask, rc; 
 * 	struct timeval *tvp; 
 * 	while(TRUE) 
 * 	{ ... 
 * 		ExceptfdMask = ReadfdMask = (1 << rpc_RequestSocket); 
 * 		WritefdMask = 0; 
 * 
 * 		rc = IOMGR_Select(8*sizeof(int), &ReadfdMask, &WritefdMask,
 * 		&ExceptfdMask, tvp); 
 * 
 * 		switch(rc) 
 * 		{ 
 * 			case 0: /* Timeout */ continue; 
 * 			/* Main while loop */
 * 
 * 			case -1: /* Error */ 
 * 			SystemError("IOMGR_Select"); 
 * 			exit(-1); 
 * 
 * 			case 1: /* RPC packet arrived! */ ... 
 * 			process packet ... 
 * 			break; 
 * 
 * 			default: Should never occur 
 * 		} 
 * 	} 
 * } 
 * \endcode
 * 
 * 	\subsection sec2-2-4 Section 2.2.4: Timer 
 * \par
 * The timer package exports a number of routines that assist in manipulating
 * lists of objects of type struct TM Elem. These struct TM Elem timers are
 * assigned a timeout value by the user and inserted in a package-maintained
 * list. The time remaining to each timer's timeout is kept up to date by the
 * package under user control. There are routines to remove a timer from its
 * list, to return an expired timer from a list, and to return the next timer
 * to expire. 
 * \par
 * A timer is commonly used by inserting a field of type struct TM Elem into a
 * structure. After setting the desired timeout value, the structure is
 * inserted into a list by means of its timer field. 
 * \par
 * Here is a simple pseudocode example of how the timer package may be used.
 * After calling the package initialization function, TM Init(), the pseudocode
 * spins in a loop. first, it updates all the timers via TM Rescan() calls.
 * Then, it pulls out the first expired timer object with TM GetExpired() (if
 * any), and processes it. 
 * 
 * \code
 * static struct TM_Elem *requests; 
 * ... 
 * TM_Init(&requests); /* Initialize timer list */ ... 
 * for (;;) { 
 * 	TM_Rescan(requests);  /* Update the timers */
 * 	expired = TM_GetExpired(requests); 
 * 	if (expired == 0) 
 * 	break; 
 * 	. . . process expired element . . . 
 * 	} 
 * \endcode
 * 
 * 	\subsection sec2-2-5 Section 2.2.5: Fast Time 
 * 
 * \par
 * The fast time routines allows a caller to determine the current time of day
 * without incurring the expense of a kernel call. It works by mapping the page
 * of the kernel that holds the time-of-day variable and examining it directly.
 * Currently, this package only works on Suns. The routines may be called on
 * other architectures, but they will run more slowly. 
 * \par
 * The initialization routine for this package is fairly expensive, since it
 * does a lookup of a kernel symbol via nlist(). If the client application
 * program only runs for only a short time, it may wish to call FT Init() with
 * the notReally parameter set to TRUE in order to prevent the lookup from
 * taking place. This is useful if you are using another package that uses the
 * fast time facility. 
 * 
 * 	\subsection sec2-2-6 Section 2.2.6: Preemption 
 * 
 * \par
 * The preemption package provides a mechanism by which control can pass
 * between lightweight processes without the need for explicit calls to LWP
 * DispatchProcess(). This effect is achieved by periodically interrupting the
 * normal flow of control to check if other (higher priority) procesess are
 * ready to run. 
 * \par
 * The package makes use of the BSD interval timer facilities, and so will
 * cause programs that make their own use of these facilities to malfunction.
 * In particular, use of alarm(3) or explicit handling of SIGALRM is
 * disallowed. Also, calls to sleep(3) may return prematurely. 
 * \par
 * Care should be taken that routines are re-entrant where necessary. In
 * particular, note that stdio(3) is not re-entrant in general, and hence
 * multiple threads performing I/O on the same fiLE structure may function
 * incorrectly. 
 * \par
 * An example pseudocode routine illustrating the use of this preemption
 * facility appears below. 
 * 
 * \code
 * #include <sys/time.h> 
 * #include "preempt.h" 
 * 	...  struct timeval tv; 
 * 	LWP_InitializeProcessSupport( ... ); 
 * 	tv.tv_sec = 10; 
 * 	tv.tv_usec = 0; 
 * 	PRE_InitPreempt(&tv); 
 * 	PRE_PreemptMe(); ... 
 * 	PRE_BeginCritical(); ... 
 * 	PRE_EndCritical(); ... 
 * 	PRE_EndPreempt(); 
 * \endcode
 * 
 * 	\section sec2-3 Section 2.3: Interface Specifications 
 * 
 * 	\subsection sec2-3-1 Section 2.3.1: LWP 
 * 
 * \par
 * This section covers the calling interfaces to the LWP package. Please note
 * that LWP macros (e.g., ActiveProcess) are also included here, rather than
 * being relegated to a different section. 
 * 
 * 	\subsubsection sec2-3-1-1 Section 2.3.1.1: LWP_InitializeProcessSupport
 * 	_ Initialize the LWP package 
 * 
 * \par
 * int LWP_InitializeProcessSupport(IN int priority; OUT PROCESS *pid) 
 * \par Description 
 * This function initializes the LWP package. In addition, it turns the current
 * thread of control into the initial process with the specified priority. The
 * process ID of this initial thread is returned in the pid parameter. This
 * routine must be called before any other routine in the LWP library. The
 * scheduler will NOT be invoked as a result of calling
 * LWP_InitializeProcessSupport(). 
 * \par Error Codes 
 * LWP EBADPRI The given priority is invalid, either negative or too large. 
 * 
 * 	 \subsubsection sec2-3-1-2 Section 2.3.1.2: LWP_TerminateProcessSupport
 * 	 _ End process support, perform cleanup 
 * 
 * \par
 * int LWP_TerminateProcessSupport() 
 * \par Description 
 * This routine terminates the LWP threading support and cleans up after it by
 * freeing any auxiliary storage used. This routine must be called from within
 * the process that invoked LWP InitializeProcessSupport(). After LWP
 * TerminateProcessSupport() has been called, it is acceptable to call LWP
 * InitializeProcessSupport() again in order to restart LWP process support. 
 * \par Error Codes 
 * ---Always succeeds, or performs an abort(). 
 * 
 * 	\subsubsection sec2-3-1-3 Section 2.3.1.3: LWP_CreateProcess _ Create a
 * 	new thread 
 * 
 * \par
 * int LWP_CreateProcess(IN int (*ep)(); IN int stacksize; IN int priority; IN
 * char *parm; IN char *name; OUT PROCESS *pid) 
 * \par Description 
 * This function is used to create a new lightweight process with a given
 * printable name. The ep argument identifies the function to be used as the
 * body of the thread. The argument to be passed to this function is contained
 * in parm. The new thread's stack size in bytes is specified in stacksize, and
 * its execution priority in priority. The pid parameter is used to return the
 * process ID of the new thread. 
 * \par
 * If the thread is successfully created, it will be marked as runnable. The
 * scheduler is called before the LWP CreateProcess() call completes, so the
 * new thread may indeed begin its execution before the completion. Note that
 * the new thread is guaranteed NOT to run before the call completes if the
 * specified priority is lower than the caller's. On the other hand, if the new
 * thread's priority is higher than the caller's, then it is guaranteed to run
 * before the creation call completes. 
 * \par Error Codes 
 * LWP EBADPRI The given priority is invalid, either negative or too large. 
 * \n LWP NOMEM Could not allocate memory to satisfy the creation request. 
 * 
 * 	\subsubsection sec2-3-1-4 Section: 2.3.1.4: LWP_DestroyProcess _ Create
 * 	a new thread 
 * 
 * \par
 * int LWP_DestroyProcess(IN PROCESS pid) 
 * \par Description 
 * This routine destroys the thread identified by pid. It will be terminated
 * immediately, and its internal storage will be reclaimed. A thread is allowed
 * to destroy itself. In this case, of course, it will only get to see the
 * return code if the operation fails. Note that a thread may also destroy
 * itself by returning from the parent C routine. 
 * \par
 * The scheduler is called by this operation