/*! 

	\page title AFS-3 Programmer's Reference: File Server/Cache Manager
Interface 

\author Edward R. Zayas 
Transarc Corporation 
\version 1.1
\date 20 Aug 1991 9:38 Copyright 1991 Transarc Corporation All Rights Reserved
FS-00-D162 

	\page chap1 Chapter 1: Overview 

	\section sec1-1 Section 1.1: Introduction 

	\subsection sec1-1-1 Section 1.1.1: The AFS 3.1 Distributed File System 

\par
AFS 3.1 is a distributed file system (DFS) designed to meet the following set
of requirements: 
\li Server-client model: Permanent file storage for AFS is maintained by a
collection of file server machines. This centralized storage is accessed by
individuals running on client machines, which also serve as the computational
engines for those users. A single machine may act as both an AFS file server
and client simultaneously. However, file server machines are generally assumed
to be housed in a secure environment, behind locked doors. 
\li Scale: Unlike other existing DFSs, AFS was designed with the specific goal
of supporting a very large user community. Unlike the rule-of-thumb ratio of 20
client machines for every server machine (20:1) used by Sun Microsystem's NFS
distributed file system [4][5], the AFS architecture aims at smoothly
supporting client/server ratios more along the lines of 200:1 within a single
installation. 
\par
AFS also provides another, higher-level notion of scalability. Not only can
each independently-administered AFS site, or cell, grow very large (on the
order of tens of thousands of client machines), but individual cells may easily
collaborate to form a single, unified file space composed of the union of the
individual name spaces. Thus, users have the image of a single unix file system
tree rooted at the /afs directory on their machine. Access to files in this
tree is performed with the standard unix commands, editors, and tools,
regardless of a file's location. 
\par
These cells and the files they export may be geographically dispersed, thus
requiring client machines to access remote file servers across network pathways
varying widely in speed, latency, and reliability. The AFS architecture
encourages this concept of a single, wide-area file system. As of this writing,
the community AFS filespace includes sites spanning the continental United
States and Hawaii, and also reaches overseas to various installations in
Europe, Japan, and Australia. 
\li Performance: This is a critical consideration given the scalability and
connectivity requirements described above. A high-performance system in the
face of high client/server ratios and the existence of low-bandwidth,
high-latency network connections as well as the normal high-speed ones is
achieved by two major mechanisms: 
\li Caching: Client machines make extensive use of caching techniques wherever
possible. One important application of this methodology is that each client is
required to maintain a cache of files it has accessed from AFS file servers,
performing its operations exclusively on these local copies. This file cache is
organized in a least-recently-used (LRU) fashion. Thus, each machine will build
a local working set of objects being referenced by its users. As long as the
cached images remain 'current' (i.e., compatible with the central version
stored at the file servers), operations may be performed on these files without
further communication with the central servers. This results in significant
reductions in network traffic and server loads, paving the way for the target
client/server ratios. 
\par
This file cache is typically located on the client's local hard disk, although
a strictly in-memory cache is also supported. The disk cache has the advantage
that its contents will survive crashes and reboots, with the expectation that
the majority of cached objects will remain current. The local cache parameters,
including the maximum number of blocks it may occupy on the local disk, may be
changed on the fly. In order to avoid having the size of the client file cache
become a limit on the length of an AFS file, caching is actually performed on
chunks of the file. These chunks are typically 64 Kbytes in length, although
the chunk size used by the client is settable when the client starts up. 
\li Callbacks: The use of caches by the file system, as described above, raises
the thorny issue of cache consistency. Each client must efficiently determine
whether its cached file chunks are identical to the corresponding sections of
the file as stored at the server machine before allowing a user to operate on
those chunks. AFS employs the notion of a callback as the backbone of its cache
consistency algorithm. When a server machine delivers one or more chunks of a
file to a client, it also includes a callback 'promise' that the client will be
notified if any modifications are made to the data in the file. Thus, as long
as the client machine is in possession of a callback for a file, it knows it is
correctly synchronized with the centrally-stored version, and allows its users
to operate on it as desired without any further interaction with the server.
Before a file server stores a more recent version of a file on its own disks,
it will first break all outstanding callbacks on this item. A callback will
eventually time out, even if there are no changes to the file or directory it
covers. 
\li Location transparency: The typical AFS user does not know which server or
servers houses any of his or her files. In fact, the user's storage may be
distributed among several servers. This location transparency also allows user
data to be migrated between servers without users having to take corrective
actions, or even becoming aware of the shift. 
\li Reliability: The crash of a server machine in any distributed file system
will cause the information it hosts to become unavailable to the user
community. The same effect is caused when server and client machines are
isolated across a network partition. AFS addresses this situation by allowing
data to be replicated across two or more servers in a read-only fashion. If the
client machine loses contact with a particular server from which it is
attempting to fetch data, it hunts among the remaining machines hosting
replicas, looking for one that is still in operation. This search is performed
without the user's knowledge or intervention, smoothly masking outages whenever
possible. Each client machine will automatically perform periodic probes of
machines on its list of known servers, updating its internal records concerning
their status. Consequently, server machines may enter and exit the pool without
administrator intervention. 
\par
Replication also applies to the various databases employed by the AFS server
processes. These system databases are read/write replicated with a single
synchronization site at any instant. If a synchronization site is lost due to
failure, the remaining database sites elect a new synchronization site
automatically without operator intervention. 
\li Security: A production file system, especially one which allows and
encourages transparent access between administrative domains, must be conscious
of security issues. AFS considers the server machines as 'trusted', being kept
behind locked doors and only directly manipulated by administrators. On the
other hand, client machines are, by definition, assumed to exist in inherently
insecure environments. These client machines are recognized to be fully
accessible to their users, making AFS servers open to attacks mounted by
possibly modified hardware, operating systems, and software from its clients. 
\li To provide credible file system security, AFS employs an authentication
system based on the Kerberos facility developed by Project Athena at MIT
[6][7]. Users operating from client machines are required to interact with
Authentication Server agents running on the secure server machines to generate
secure tokens of identity. These tokens express the user's identity in an
encrypted fashion, and are stored in the kernel of the client machine. When the
user attempts to fetch or store files, the server may challenge the user to
verify his or her identity. This challenge, hidden from the user and handled
entirely by the RPC layer, will transmit this token to the file server involved
in the operation. The server machine, upon decoding the token and thus
discovering the user's true identity, will allow the caller to perform the
operation if permitted. Access control: The standard unix access control
mechanism associates mode bits with every file and directory, applying them
based on the user's numerical identifier and the user's membership in various
groups. AFS has augmented this traditional access control mechanism with Access
Control Lists (ACLs). Every AFS directory has an associated ACL which defines
the principals or parties that may operate on all files contained in the
directory, and which operations these principals may perform. Rights granted by
these ACLs include read, write, delete, lookup, insert (create new files, but
don't overwrite old files), and administer (change the ACL). Principals on
these ACLs include individual users and groups of users. These groups may be
defined by AFS users without administrative intervention. AFS ACLs provide for
much finer-grained access control for its files. 
\li Administrability: Any system with the scaling goals of AFS must pay close
attention to its ease of administration. The task of running an AFS
installation is facilitated via the following mechanisms: 
\li Pervasive RPC interfaces: Access to AFS server agents is performed mostly
via RPC interfaces. Thus, servers may be queried and operated upon regardless
of their location. In combination with the security system outlined above, even
administrative functions such as instigating backups, reconfiguring server
machines, and stopping and restarting servers may be performed by an
administrator sitting in front of any AFS-capable machine, as long as the
administrator holds the proper tokens. 
\li Replication: As AFS supports read-only replication for user data and
read-write replication for system databases, much of the system reconfiguration
work in light of failures is performed transparently and without human
intervention. Administrators thus typically have more time to respond to many
common failure situations. 
\li Data mobility: Improved and balanced utilization of disk resources is
facilitated by the fact that AFS supports transparent relocation of user data
between partitions on a single file server machine or between two different
machines. In a situation where a machine must be brought down for an extended
period, all its storage may be migrated to other servers so that users may
continue their work completely unaffected. 
\li Automated 'nanny' services: Each file server machine runs a BOS Server
process, which assists in the machine's administration. This server is
responsible for monitoring the health of the AFS agents under its care,
bringing them up in the proper order after a system reboot, answering requests
as to their status and restarting them when they fail. It also accepts commands
to start, suspend, or resume these processes, and install new server binaries.
Accessible via an RPC interface, this supervisory process relieves
administrators of some oversight responsibilities and also allows them to
perform their duties from any machine running AFS, regardless of location or
geographic distance from the targeted file server machine. 
\li On-line backup: Backups may be performed on the data stored by the AFS file
server machines without bringing those machines down for the duration.
Copy-on-write 'snapshots' are taken of the data to be preserved, and tape
backup is performed from these clones. One added benefit is that these backup
clones are on-line and accessible by users. Thus, if someone accidentally
deletes a file that is contained in their last snapshot, they may simply copy
its contents as of the time the snapshot was taken back into their active
workspace. This facility also serves to improve the administrability of the
system, greatly reducing the number of requests to restore data from tape. 
\li On-line help: The set of provided program tools used to interact with the
active AFS agents are self-documenting in that they will accept command-line
requests for help, displaying descriptive text in response. 
\li Statistics: Each AFS agent facilitates collection of statistical data on
its performance, configuration, and status via its RPC interface. Thus, the
system is easy to monitor. One tool that takes advantage of this facility is
the scout program. Scout polls file server machines periodically, displaying
usage statistics, current disk capacities, and whether the server is
unavailable. Administrators monitoring this information can thus quickly react
to correct overcrowded disks and machine crashes. 
\li Coexistence: Many organizations currently employ other distributed file
systems, most notably NFS. AFS was designed to run simultaneously with other
DFSs without interfering in their operation. In fact, an NFS-AFS translator
agent exists that allows pure-NFS client machines to transparently access files
in the AFS community. 
\li Portability: Because AFS is implemented using the standard VFS and vnode
interfaces pioneered and advanced by Sun Microsystems, AFS is easily portable
between different platforms from a single vendor or from different vendors. 

	\subsection sec1-1-2 Section 1.1.2: Scope of this Document 

\par
This document is a member of a documentation suite providing specifications of
the operations and interfaces offered by the various AFS servers and agents.
Specifically, this document will focus on two of these system agents: 
\li File Server: This AFS entity is responsible for providing a central disk
repository for a particular set of files and for making these files accessible
to properly-authorized users running on client machines. The File Server is
implemented as a user-space process 
\li Cache Manager: This code, running within the kernel of an AFS client
machine, is a user's representative in communicating with the File Servers,
fetching files back and forth into the local cache as needed. The Cache Manager
also keeps information as to the composition of its own cell as well as the
other AFS cells in existence. It resolves file references and operations,
determining the proper File Server (or group of File Servers) that may satisfy
the request. In addition, it is also a reliable repository for the user's
authentication information, holding on to their tokens and wielding them as
necessary when challenged. 

	\subsection sec1-1-3 Section 1.1.3: Related Documents 

\par
The full AFS specification suite of documents is listed below: 
\li AFS-3 Programmer's Reference: Architectural Overview: This paper provides
an architectual overview of the AFS distributed file system, describing the
full set of servers and agents in a coherent way, illustrating their
relationships to each other and examining their interactions. 
\li AFS-3 Programmer's Reference:Volume Server/Volume Location Server
Interface: This document describes the services through which 'containers' of
related user data are located and managed. 
\li AFS-3 Programmer's Reference: Protection Server Interface: This paper
describes the server responsible for providing two-way mappings between
printable usernames and their internal AFS identifiers. The Protection Server
also allows users to create, destroy, and manipulate 'groups' of users, which
are suitable for placement on ACLs. AFS-3 Programmer's Reference: BOS Server
Interface: This paper explicates the 'nanny' service described above, which
assists in the administrability of the AFS environment. 
\li AFS-3 Programmer's Reference: Specification for the Rx Remote Procedure
Call Facility: This document specifies the design and operation of the remote
procedure call and lightweight process packages used by AFS. 
\par
In addition to these papers, the AFS 3.1 product is delivered with its own
user, administrator, installation, and command reference documents. 

	\section sec1-2 Section 1.2: Basic Concepts 

\par
To properly understand AFS operation, specifically the tasks and objectives of
the File Server and Cache Manager, it is necessary to introduce and explain the
following concepts: 
\li Cell: A cell is the set of server and client machines operated by an
administratively independent organization. The cell administrators make
decisions concerning such issues as server deployment and configuration, user
backup schedules, and replication strategies on their own hardware and disk
storage completely independently from those implemented by other cell
administrators regarding their own domains. Every client machine belongs to
exactly one cell, and uses that information to determine the set of database
servers it uses to locate system resources and generate authentication
information. 
\li Volume: AFS disk partitions do not directly host individual user files or
directories. Rather, connected subtrees of the system's directory structure are
placed into containers called volumes. Volumes vary in size dynamically as
objects are inserted, overwritten, and deleted. Each volume has an associated
quota, or maximum permissible storage. A single unix disk partition may host
one or more volumes, and in fact may host as many volumes as physically fit in
the storage space. However, a practical maximum is 3,500 volumes per disk
partition, since this is the highest number currently handled by the salvager
program. The salvager is run on occasions where the volume structures on disk
are inconsistent, repairing the damage. A compile-time constant within the
salvager imposes the above limit, causing it to refuse to repair any
inconsistent partition with more than 3,500 volumes. Volumes serve many
purposes within AFS. First, they reduce the number of objects with which an
administrator must be concerned, since operations are normally performed on an
entire volume at once (and thus on all files and directories contained within
the volume). In addition, volumes are the unit of replication, data mobility
between servers, and backup. Disk utilization may be balanced by transparently
moving volumes between partitions. 
\li Mount Point: The connected subtrees contained within individual volumes
stored at AFS file server machines are 'glued' to their proper places in the
file space defined by a site, forming a single, apparently seamless unix tree.
These attachment points are referred to as mount points. Mount points are
persistent objects, implemented as symbolic links whose contents obey a
stylized format. Thus, AFS mount points differ from NFS-style mounts. In the
NFS environment, the user dynamically mounts entire remote disk partitions
using any desired name. These mounts do not survive client restarts, and do not
insure a uniform namespace between different machines. 
\par
As a Cache Manager resolves an AFS pathname as part of a file system operation
initiated by a user process, it recognizes mount points and takes special
action to resolve them. The Cache Manager consults the appropriate Volume
Location Server to discover the File Server (or set of File Servers) hosting
the indicated volume. This location information is cached, and the Cache
Manager then proceeds to contact the listed File Server(s) in turn until one is
found that responds with the contents of the volume's root directory. Once
mapped to a real file system object, the pathname resolution proceeds to the
next component. 
\li Database Server: A set of AFS databases is required for the proper
functioning of the system. Each database may be replicated across two or more
file server machines. Access to these databases is mediated by a database
server process running at each replication site. One site is declared to be the
synchronization site, the sole location accepting requests to modify the
databases. All other sites are read-only with respect to the set of AFS users.
When the synchronization site receives an update to its database, it
immediately distributes it to the other sites. Should a synchronization site go
down through either a hard failure or a network partition, the remaining sites
will automatically elect a new synchronization site if they form a quorum, or
majority. This insures that multiple synchronization sites do not become active
in the network partition scenario. 
\par
The classes of AFS database servers are listed below: 
\li Authentication Server: This server maintains the authentication database
used to generate tokens of identity. 
\li Protection Server: This server maintains mappings between human-readable
user account names and their internal numerical AFS identifiers. It also
manages the creation, manipulation, and update of user-defined groups suitable
for use on ACLs. 
\li Volume Location Server: This server exports information concerning the
location of the individual volumes housed within the cell. 

	\section sec1-3 Section 1.3: Document Layout 

\par
Following this introduction and overview, Chapter 2 describes the architecture
of the File Server process design. Similarly, Chapter 3 describes the
architecture of the in-kernel Cache Manager agent. Following these
architectural examinations, Chapter 4 provides a set of basic coding
definitions common to both the AFS File Server and Cache Manager, required to
properly understand the interface specifications which follow. Chapter 5 then
proceeds to specify the various File Server interfaces. The myriad Cache
Manager interfaces are presented in Chapter 6, thus completing the document. 

	\page chap2 Chapter 2: File Server Architecture 

	\section sec2-1 Section 2.1: Overview 

\par
The AFS File Server is a user-level process that presides over the raw disk
partitions on which it supports one or more volumes. It provides 'half' of the
fundamental service of the system, namely exporting and regimenting access to
the user data entrusted to it. The Cache Manager provides the other half,
acting on behalf of its human users to locate and access the files stored on
the file server machines. 
\par
This chapter examines the structure of the File Server process. First, the set
of AFS agents with which it must interact are discussed. Next, the threading
structure of the server is examined. Some details of its handling of the race
conditions created by the callback mechanism are then presented. This is
followed by a discussion of the read-only volume synchronization mechanism.
This functionality is used in each RPC interface call and intended to detect
new releases of read-only volumes. File Servers do not generate callbacks for
objects residing in read-only volumes, so this synchronization information is
used to implement a 'whole-volume' callback. Finally, the fact that the File
Server may drop certain information recorded about the Cache Managers with
which it has communicated and yet guarantee correctness of operation is
explored. 

	\section sec2-2 Section 2.2: Interactions 

\par
By far the most frequent partner in File Server interactions is the set of
Cache Managers actively fetching and storing chunks of data files for which the
File Server provides central storage facilities. The File Server also
periodically probes the Cache Managers recorded in its tables with which it has
recently dealt, determining if they are still active or whether their records
might be garbage-collected. 
\par
There are two other server entities with which the File Server interacts,
namely the Protection Server and the BOS Server. Given a fetch or store request
generated by a Cache Manager, the File Server needs to determine if the caller
is authorized to perform the given operation. An important step in this process
is to determine what is referred to as the caller's Current Protection
Subdomain, or CPS. A user's CPS is a list of principals, beginning with the
user's internal identifier, followed by the the numerical identifiers for all
groups to which the user belongs. Once this CPS information is determined, the
File Server scans the ACL controlling access to the file system object in
question. If it finds that the ACL contains an entry specifying a principal
with the appropriate rights which also appears in the user's CPS, then the
operation is cleared. Otherwise, it is rejected and a protection violation is
reported to the Cache Manager for ultimate reflection back to the caller. 
\par
The BOS Server performs administrative operations on the File Server process.
Thus, their interactions are quite one-sided, and always initiated by the BOS
Server. The BOS Server does not utilize the File Server's RPC interface, but
rather generates unix signals to achieve the desired effect. 

	\section sec2-3 Section 2.3: Threading 

\par
The File Server is organized as a multi-threaded server. Its threaded behavior
within a single unix process is achieved by use of the LWP lightweight process
facility, as described in detail in the companion "AFS-3 Programmer's
Reference: Specification for the Rx Remote Procedure Call Facility" document.
The various threads utilized by the File Server are described below: 
\li WorkerLWP: This lightweight process sleeps until a request to execute one
of the RPC interface functions arrives. It pulls the relevant information out
of the request, including any incoming data delivered as part of the request,
and then executes the server stub routine to carry out the operation. The
thread finishes its current activation by feeding the return code and any
output data back through the RPC channel back to the calling Cache Manager. The
File Server initialization sequence specifies that at least three but no more
than six of these WorkerLWP threads are to exist at any one time. It is
currently not possible to configure the File Server process with a different
number of WorkerLWP threads. 
\li FiveMinuteCheckLWP: This thread runs every five minutes, performing such
housekeeping chores as cleaning up timed-out callbacks, setting disk usage
statistics, and executing the special handling required by certain AIX
implementations. Generally, this thread performs activities that do not take
unbounded time to accomplish and do not block the thread. If reassurance is
required, FiveMinuteCheckLWP can also be told to print out a banner message to
the machine's console every so often, stating that the File Server process is
still running. This is not strictly necessary and an artifact from earlier
versions, as the File Server's status is now easily accessible at any time
through the BOS Server running on its machine. 
\li HostCheckLWP: This thread, also activated every five minutes, performs
periodic checking of the status of Cache Managers that have been previously
contacted and thus appear in this File Server's internal tables. It generates
RXAFSCB Probe() calls from the Cache Manager interface, and may find itself
suspended for an arbitrary amount of time when it enounters unreachable Cache
Managers. 

	\section sec2-4 Section 2.4: Callback Race Conditions 

\par
Callbacks serve to implement the efficient AFS cache consistency mechanism, as
described in Section 1.1.1. Because of the asynchronous nature of callback
generation and the multi-threaded operation and organization of both the File
Server and Cache Manager, race conditions can arise in their use. As an
example, consider the case of a client machine fetching a chunk of file X. The
File Server thread activated to carry out the operation ships the contents of
the chunk and the callback information over to the requesting Cache Manager.
Before the corresponding Cache Manager thread involved in the exchange can be
scheduled, another request arrives at the File Server, this time storing a
modified image of the same chunk from file X. Another worker thread comes to
life and completes processing of this second request, including execution of an
RXAFSCB CallBack() to the Cache Manager who still hasn't picked up on the
results of its fetch operation. If the Cache Manager blindly honors the RXAFSCB
CallBack() operation first and then proceeds to process the fetch, it will wind
up believing it has a callback on X when in reality it is out of sync with the
central copy on the File Server. To resolve the above class of callback race
condition, the Cache Manager effectively doublechecks the callback information
received from File Server calls, making sure they haven't already been
nullified by other file system activity. 

	\section sec2-5 Section 2.5: Read-Only Volume Synchronization 

\par
The File Server issues a callback for each file chunk it delivers from a
read-write volume, thus allowing Cache Managers to efficiently synchronize
their local caches with the authoritative File Server images. However, no
callbacks are issued when data from read-only volumes is delivered to clients.
Thus, it is possible for a new snapshot of the read-only volume to be
propagated to the set of replication sites without Cache Managers becoming
aware of the event and marking the appropriate chunks in their caches as stale.
Although the Cache Manager refreshes its volume version information
periodically (once an hour), there is still a window where a Cache Manager will
fail to notice that it has outdated chunks. 
\par
The volume synchronization mechanism was defined to close this window,
resulting in what is nearly a 'whole-volume' callback device for read-only
volumes. Each File Server RPC interface function handling the transfer of file
data is equipped with a parameter (a volSyncP), which carries this volume
synchronization information. This parameter is set to a non-zero value by the
File Server exclusively when the data being fetched is coming from a read-only
volume. Although the struct AFSVolSync defined in Section 5.1.2.2 passed via a
volSyncP consists of six longwords, only the first one is set. This leading
longword carries the creation date of the read-only volume. The Cache Manager
immediately compares the synchronization value stored in its cached volume
information against the one just received. If they are identical, then the
operation is free to complete, secure in the knowledge that all the information
and files held from that volume are still current. A mismatch, though,
indicates that every file chunk from this volume is potentially out of date,
having come from a previous release of the read-only volume. In this case, the
Cache Manager proceeds to mark every chunk from this volume as suspect. The
next time the Cache Manager considers accessing any of these chunks, it first
checks with the File Server it came from which the chunks were obtained to see
if they are up to date. 

	\section sec2-6 Section 2.6: Disposal of Cache Manager Records 

\par
Every File Server, when first starting up, will, by default, allocate enough
space to record 20,000 callback promises (see Section 5.3 for how to override
this default). Should the File Server fully populate its callback records, it
will not allocate more, allowing its memory image to possibly grow in an
unbounded fashion. Rather, the File Server chooses to break callbacks until it
acquires a free record. All reachable Cache Managers respond by marking their
cache entries appropriately, preserving the consistency guarantee. In fact, a
File Server may arbitrarily and unilaterally purge itself of all records
associated with a particular Cache Manager. Such actions will reduce its
performance (forcing these Cache Managers to revalidate items cached from that
File Server) without sacrificing correctness. 

	\page chap3 Chapter 3: Cache Manager Architecture 

	\section sec3-1 Section 3.1: Overview 

\par
The AFS Cache Manager is a kernel-resident agent with the following duties and
responsibilities: 
\li Users are to be given the illusion that files stored in the AFS distributed
file system are in fact part of the local unix file system of their client
machine. There are several areas in which this illusion is not fully realized: 
\li Semantics: Full unix semantics are not maintained by the set of agents
implementing the AFS distributed file system. The largest deviation involves
the time when changes made to a file are seen by others who also have the file
open. In AFS, modifications made to a cached copy of a file are not necessarily
reflected immediately to the central copy (the one hosted by File Server disk
storage), and thus to other cache sites. Rather, the changes are only
guaranteed to be visible to others who simultaneously have their own cached
copies open when the modifying process executes a unix close() operation on the
file