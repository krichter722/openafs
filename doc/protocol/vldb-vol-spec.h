/*!
  \addtogroup vldb-spec VLDB Server Interface
  @{

	\page title AFS-3 Programmer's Reference: Volume Server/Volume Location
Server  Interface 

	\author Edward R. Zayas 
Transarc Corporation 
\version 1.0
\date 29 August 1991 14:48 Copyright 1991 Transarc Corporation All Rights
Reserved FS-00-D165 


	\page chap1 Chapter 1: Overview 

	\section sec1-1 Section 1.1: Introduction 

\par
This document describes the architecture and interfaces for two of the
important agents of the AFS distributed file system, the Volume Server and the
Volume Location Server. The Volume Server allows operations affecting entire
AFS volumes to be executed, while the Volume Location Server provides a lookup
service for volumes, identifying the server or set of servers on which volume
instances reside. 

	\section sec1-2 Section 1.2: Volumes 

	\subsection sec1-2-1 Section 1.2.1: Definition 

\par
The underlying concept manipulated by the two AFS servers examined by this
document is the volume. Volumes are the basic mechanism for organizing the data
stored within the file system. They provide the foundation for addressing,
storing, and accessing file data, along with serving as the administrative
units for replication, backup, quotas, and data motion between File Servers. 
\par
Specifically, a volume is a container for a hierarchy of files, a connected
file system subtree. In this respect, a volume is much like a traditional unix
file system partition. Like a partition, a volume can be mounted in the sense
that the root directory of the volume can be named within another volume at an
AFS mount point. The entire file system hierarchy is built up in this manner,
using mount points to glue together the individual subtrees resident within
each volume. The root of this hierarchy is then mounted by each AFS client
machine using a conventional unix mount point within the workstation's local
file system. By convention, this entryway into the AFS domain is mounted on the
/afs local directory. From a user's point of view, there is only a single mount
point to the system; the internal mount points are generally transparent. 

	\subsection sec1-2-2 Section 1.2.2: Volume Naming 

\par
There are two methods by which volumes may be named. The first is via a
human-readable string name, and the second is via a 32-bit numerical
identifier. Volume identifiers, whether string or numerical, must be unique
within any given cell. AFS mount points may use either representation to
specify the volume whose root directory is to be accessed at the given
position. Internally, however, AFS agents use the numerical form of
identification exclusively, having to translate names to the corresponding
32-bit value. 

	\subsection sec1-2-3 Section 1.2.3: Volume Types 

\par
There are three basic volume types: read-write, read-only, and backup volumes. 
\li Read-write: The data in this volume may be both read and written by those
clients authorized to do so. 
\li Read-only: It is possible to create one or more read-only snapshots of
read-write volumes. The read-write volume serving as the source image is
referred to as the parent volume. Each read-only clone, or child, instance must
reside on a different unix disk partition than the other clones. Every clone
instance generated from the same parent read-write volume has the identical
volume name and numerical volume ID. This is the reason why no two clones may
appear on the same disk partition, as there would be no way to differentiate
the two. AFS clients are allowed to read files and directories from read-only
volumes, but cannot overwrite them individually. However, it is possible to
make changes to the read-write parent and then release the contents of the
entire volume to all the read-only replicas. The release operation fails if it
does not reach the appropriate replication sites. 
\li Backup: A backup volume is a special instance of a read-only volume. While
it is also a read-only snapshot of a given read-write volume, only one instance
is allowed to exist at any one time. Also, the backup volume must reside on the
same partition as the parent read-write volume from which it was created. It is
from a backup volume that the AFS backup system writes file system data to
tape. In addition, backup volumes may be mounted into the file tree just like
the other volume types. In fact, by convention, the backup volume for each
user's home directory subtree is typically mounted as OldFiles in that
directory. If a user accidentally deletes a file that resides in the backup
snapshot, the user may simply copy it out of the backup directly without the
assistance of a system administrator, or any kind of tape restore operation. 
Backup volume are implemented in a copy-on-write fashion. Thus, backup volumes
may be envisioned as consisting of a set of pointers to the true data objects
in the base read-write volume when they are first created. When a file is
overwritten in the read-write version for the first time after the backup
volume was created, the original data is physically written to the backup
volume, breaking the copyon-write link. With this mechanism, backup volumes
maintain the image of the read-write volume at the time the snapshot was taken
using the minimum amount of additional disk space. 

	\section sec1-3 Section 1.3: Scope 

\par
This paper is a member of a documentation suite providing specifications of the
operation and interfaces offered by the various AFS servers and agents. The
scope of this work is to provide readers with a sufficiently detailed
description of the Volume Location Server and the Volume Server so that they
may construct client applications which call their RPC interface routines. 

	\section sec1-4 Section 1.4: Document Layout 

\par
After this introductory portion of the document, Chapters 2 and 3 examine the
architecture and RPC interface of the Volume Location Server and its replicated
database. Similarly, Chapters 4 and 5 describe the architecture and RPC
interface of the Volume Server. 

	\page chap2 Chapter 2: Volume Location Server Architecture 

	\section sec2-1 Section 2.1: Introduction 

\par
The Volume Location Server allows AFS agents to query the location and basic
status of volumes resident within the given cell. Volume Location Server
functions may be invoked directly from authorized users via the vos utility. 
\par
This chapter briefly discusses various aspects of the Volume Location Server's
architecture. First, the need for volume location is examined, and the specific
parties that call the Volume Location Server interface routines are identified.
Then, the database maintained to provide volume location service, the Volume
Location Database (VLDB), is examined. Finally, the vlserver process which
implements the Volume Location Server is considered. 
\par
As with all AFS servers, the Volume Location Server uses the Rx remote
procedure call package for communication with its clients. 

	\section sec2-2 Section 2.2: The Need For Volume Location 

\par
The Cache Manager agent is the primary consumer of AFS volume location service,
on which it is critically dependent for its own operation. The Cache Manager
needs to map volume names or numerical identifiers to the set of File Servers
on which its instances reside in order to satisfy the file system requests it
is processing on behalf of it clients. Each time a Cache Manager encounters a
mount point for which it does not have location information cached, it must
acquire this information before the pathname resolution may be successfully
completed. Once the File Server set is known for a particular volume, the Cache
Manager may then select the proper site among them (e.g. choosing the single
home for a read-write volume, or randomly selecting a site from a read-only
volume's replication set) and begin addressing its file manipulation operations
to that specific server. 
\par
While the Cache Manager consults the volume location service, it is not capable
of changing the location of volumes and hence modifying the information
contained therein. This capability to perform acts which change volume location
is concentrated within the Volume Server. The Volume Server process running on
each server machine manages all volume operations affecting that platform,
including creations, deletions, and movements between servers. It must update
the volume location database every time it performs one of these actions. 
\par
None of the other AFS system agents has a need to access the volume location
database for its site. Surprisingly, this also applies to the File Server
process. It is only aware of the specific set of volumes that reside on the set
of physical disks directly attached to the machine on which they execute. It
has no knowlege of the universe of volumes resident on other servers, either
within its own cell or in foreign cells. 

	\section sec2-3 Section 2.3: The VLDB 

\par
The Volume Location Database (VLDB) is used to allow AFS application programs
to discover the location of any volume within its cell, along with select
information about the nature and state of that volume. It is organized in a
very straightforward fashion, and uses the ubik [4] [5] facility to to provide
replication across multiple server sites. 

	\subsection sec2-3-1 Section 2.3.1: Layout 

\par
The VLDB itself is a very simple structure, and synchronized copies may be
maintained at two or more sites. Basically, each copy consists of header
information, followed by a linear (yet unbounded) array of entries. There are
several associated hash tables used to perform lookups into the VLDB. The first
hash table looks up volume location information based on the volume's name.
There are three other hash tables used for lookup, based on volume ID/type
pairs, one for each possible volume type. 
\par
The VLDB for a large site may grow to contain tens of thousands of entries, so
some attempts were made to make each entry as small as possible. For example,
server addresses within VLDB entries are represented as single-byte indicies
into a table containing the full longword IP addresses. 
\par
A free list is kept for deleted VLDB entries. The VLDB will not grow unless all
the entries on the free list have been exhausted, keeping it as compact as
possible. 

	\subsection sec2-3-2 Section 2.3.2: Database Replication 

\par
The VLDB, along with other important AFS databases, may be replicated to
multiple sites to improve its availability. The ubik replication package is
used to implement this functionality for the VLDB. A full description of ubik
and of the quorum completion algorithm it implements may be found in [4] and
[5]. The basic abstraction provided by ubik is that of a disk file replicated
to multiple server locations. One machine is considered to be the
synchronization site, handling all write operations on the database file. Read
operations may be directed to any of the active members of the quorum, namely a
subset of the replication sites large enough to insure integrity across such
failures as individual server crashes and network partitions. All of the quorum
members participate in regular elections to determine the current
synchronization site. The ubik algorithms allow server machines to enter and
exit the quorum in an orderly and consistent fashion. All operations to one of
these replicated "abstract files" are performed as part of a transaction. If
all the related operations performed under a transaction are successful, then
the transaction is committed, and the changes are made permanent. Otherwise,
the transaction is aborted, and all of the operations for that transaction are
undone. 

	\section sec2-4 Section 2.4: The vlserver Process 

\par
The user-space vlserver process is in charge of providing volume location
service for AFS clients. This program maintains the VLDB replica at its
particular server, and cooperates with all other vlserver processes running in
the given cell to propagate updates to the database. It implements the RPC
interface defined in the vldbint.xg definition file for the rxgen RPC stub
generator program. As part of its startup sequence, it must discover the VLDB
version it has on its local disk, move to join the quorum of replication sites
for the VLDB, and get the latest version if the one it came up with was out of
date. Eventually, it will synchronize with the other VLDB replication sites,
and it will begin accepting calls. 
\par
The vlserver program uses at most three Rx worker threads to listen for
incoming Volume Location Server calls. It has a single, optional command line
argument. If the string "-noauth" appears when the program is invoked, then
vlserver will run in an unauthenticated mode where any individual is considered
authorized to perform any VLDB operation. This mode is necessary when first
bootstrapping an AFS installation. 

	\page chap3 Chapter 3: Volume Location Server Interface 

	\section sec3-1 Section 3.1: Introduction 

\par
This chapter documents the API for the Volume Location Server facility, as
defined by the vldbint.xg Rxgen interface file and the vldbint.h include file.
Descriptions of all the constants, structures, macros, and interface functions
available to the application programmer appear here. 
\par
It is expected that Volume Location Server client programs run in user space,
as does the associated vos volume utility. However, the kernel-resident Cache
Manager agent also needs to call a subset of the Volume Location Server's RPC
interface routines. Thus, a second Volume Location Server interface is
available, built exclusively to satisfy the Cache Manager's limited needs. This
subset interface is defined by the afsvlint.xg Rxgen interface file, and is
examined in the final section of this chapter. 

	\section sec3-2 3.2: Constants 

\par
This section covers the basic constant definitions of interest to the Volume
Location Server application programmer. These definitions appear in the
vldbint.h file, automatically generated from the vldbint.xg Rxgen interface
file, and in vlserver.h. 
\par
Each subsection is devoted to describing the constants falling into the
following categories: 
\li Configuration and boundary quantities 
\li Update entry bits 
\li List-by-attribute bits 
\li Volume type indices 
\li States for struct vlentry 
\li States for struct vldbentry 
\li ReleaseType argument values 
\li Miscellaneous items 

	\subsection sec3-2-1 Section 3.2.1: Configuration and Boundary
Quantities 

\par
These constants define some basic system values, including configuration
information. 

\par Name
MAXNAMELEN
\par Value
65
\par Description
Maximum size of various character strings, including volume name fields in
structures and host names.

\par Name
MAXNSERVERS
\par Value
8
\par Description
Maximum number of replications sites for a volume.

\par Name
MAXTYPES
\par Value
3
\par Description
Maximum number of volume types.

\par Name
VLDBVERSION
\par Value
1
\par Description
VLDB database version number

\par Name
HASHSIZE
\par Value
8,191
\par Description
Size of internal Volume Location Server volume name and volume ID hash tables.
This must always be a prime number.

\par Name
NULLO
\par Value
0
\par Description
Specifies a null pointer value.

\par Name
VLDBALLOCCOUNT
\par Value
40
\par Description
Value used when allocating memory internally for VLDB entry records.

\par Name
BADSERVERID
\par Value
255
\par Description
Illegal Volume Location Server host ID.

\par Name
MAXSERVERID
\par Value
30
\par Description
Maximum number of servers appearing in the VLDB.

\par Name
MAXSERVERFLAG
\par Value
0x80
\par Description
First unused flag value in such fields as serverFlags in struct vldbentry and
RepsitesNewFlags in struct VldbUpdateEntry.

\par Name
MAXPARTITIONID
\par Value
126
\par Description
Maximum number of AFS disk partitions for any one server.

\par Name
MAXBUMPCOUNT
\par Value
0x7fffffff
\par Description
Maximum interval that the current high-watermark value for a volume ID can be
increased in one operation.

\par Name
MAXLOCKTIME
\par Value
0x7fffffff
\par Description
Maximum number of seconds that any VLDB entry can remain locked.

\par Name
SIZE
\par Value
1,024
\par Description
Maximum size of the name field within a struct.

	\subsection sec3-2-2 Section 3.2.2: Update Entry Bits 

\par
These constants define bit values for the Mask field in the struct
VldbUpdateEntry. Specifically, setting these bits is equivalent to declaring
that the corresponding field within an object of type struct VldbUpdateEntry
has been set. For example, setting the VLUPDATE VOLUMENAME flag in Mask
indicates that the name field contains a valid value. 

\par Name
VLUPDATE VOLUMENAME
\par Value
0x0001
\par Description
If set, indicates that the name field is valid.

\par Name
VLUPDATE VOLUMETYPE
\par Value
0x0002
\par Description
If set, indicates that the volumeType field is valid.

\par Name
VLUPDATE FLAGS
\par Value
0x0004
\par Description
If set, indicates that the flags field is valid.

\par Name
VLUPDATE READONLYID
\par Value
0x0008
\par Description
If set, indicates that the ReadOnlyId field is valid.

\par Name
VLUPDATE BACKUPID
\par Value
0x0010
\par Description
If set, indicates that the BackupId field is valid.

\par Name
VLUPDATE REPSITES 
\par Value
0x0020
\par Description
If set, indicates that the nModifiedRepsites field is valid.

\par Name
VLUPDATE CLONEID
\par Value
0x0080
\par Description
If set, indicates that the cloneId field is valid.

\par Name
VLUPDATE REPS DELETE
\par Value
0x0100
\par Description
Is the replica being deleted?

\par Name
VLUPDATE REPS ADD
\par Value
0x0200
\par Description
Is the replica being added?

\par Name
VLUPDATE REPS MODSERV
\par Value
0x0400
\par Description
Is the server part of the replica location correct?

\par Name
VLUPDATE REPS MODPART
\par Value
0x0800
\par Description
Is the partition part of the replica location correct?

\par Name
VLUPDATE REPS MODFLAG 
\par Value
0x1000 
\par Description
Various modification flag values.

	\subsection sec3-2-3 Section 3.2.3: List-By-Attribute Bits 

\par
These constants define bit values for the Mask field in the struct
VldbListByAttributes is to be used in a match. Specifically, setting these bits
is equivalent to declaring that the corresponding field within an object of
type struct VldbListByAttributes is set. For example, setting the VLLIST SERVER
flag in Mask indicates that the server field contains a valid value. 

\par Name
VLLIST SERVER
\par Value
0x1
\par Description
If set, indicates that the server field is valid.

\par Name
VLLIST PARTITION
\par Value
0x2
\par Description
If set, indicates that the partition field is valid.

\par Name
VLLIST VOLUMETYPE
\par Value
0x4
\par Description
If set, indicates that the volumetype field is valid.

\par Name
VLLIST VOLUMEID
\par Value
0x8
\par Description
If set, indicates that the volumeid field is valid.

\par Name
VLLIST FLAG
\par Value
0x10
\par Description
If set, indicates that that flag field is valid.

	\subsection sec3-2-4 Section 3.2.4: Volume Type Indices 

\par
These constants specify the order of entries in the volumeid array in an object
of type struct vldbentry. They also identify the three different types of
volumes in AFS. 

\par Name
RWVOL
\par Value
0
\par Description
Read-write volume.

\par Name
ROVOL
\par Value
1
\par Description
Read-only volume.

\par Name
BACKVOL 
\par Value
2
\par Description
Backup volume.

	\subsection sec3-2-5 Section 3.2.5: States for struct vlentry 

\par
The following constants appear in the flags field in objects of type struct
vlentry. The first three values listed specify the state of the entry, while
all the rest stamp the entry with the type of an ongoing volume operation, such
as a move, clone, backup, deletion, and dump. These volume operations are the
legal values to provide to the voloper parameter of the VL SetLock() interface
routine. 
\par
For convenience, the constant VLOP ALLOPERS is defined as the inclusive OR of
the above values from VLOP MOVE through VLOP DUMP. 

\par Name
VLFREE
\par Value
0x1
\par Description
Entry is in the free list.

\par Name
VLDELETED
\par Value
0x2
\par Description
Entry is soft-deleted.

\par Name
VLLOCKED
\par Value
0x4
\par Description
Advisory lock held on the entry.

\par Name
VLOP MOVE
\par Value
0x10
\par Description
The associated volume is being moved between servers.

\par Name
VLOP RELEASE
\par Value
0x20
\par Description
The associated volume is being cloned to its replication sites.

\par Name
VLOP BACKUP
\par Value
0x40
\par Description
A backup volume is being created for the associated volume.

\par Name
VLOP DELETE
\par Value
0x80
\par Description
The associated volume is being deleted.

\par Name
VLOP DUMP
\par Value
0x100 
\par Description
A dump is being taken of the associated volume.

	\subsection sec3-2-6 Section 3.2.6: States for struct vldbentry 

\par
Of the following constants, the first three appear in the flags field within an
object of type struct vldbentry, advising of the existence of the basic volume
types for the given volume, and hence the validity of the entries in the
volumeId array field. The rest of the values provided in this table appear in
the serverFlags array field, and apply to the instances of the volume appearing
in the various replication sites. 
\par
This structure appears in numerous Volume Location Server interface calls,
namely VL CreateEntry(), VL GetEntryByID(), VL GetEntryByName(), VL
ReplaceEntry() and VL ListEntry(). 

\par Name
VLF RWEXISTS
\par Value
0x1000
\par Description
The read-write volume ID is valid.

\par Name
VLF ROEXISTS
\par Value
0x2000
\par Description
The read-only volume ID is valid.

\par Name
VLF BACKEXISTS
\par Value
0x4000
\par Description
The backup volume ID is valid.

\par Name
VLSF NEWREPSITE
\par Value
0x01
\par Description
Not used; originally intended to mark an entry as belonging to a
partially-created volume instance.

\par Name
VLSF ROVOL
\par Value
0x02
\par Description
A read-only version of the volume appears at this server.

\par Name
VLSF RWVOL
\par Value
0x02
\par Description
A read-write version of the volume appears at this server.

\par Name
VLSF BACKVOL
\par Value
0x08
\par Description
A backup version of the volume appears at this server. 

	\subsection sec3-2-7 Section 3.2.7: ReleaseType Argument Values 

\par
The following values are used in the ReleaseType argument to various Volume
Location Server interface routines, namely VL ReplaceEntry(), VL UpdateEntry()
and VL ReleaseLock(). 

\par Name
LOCKREL TIMESTAMP
\par Value
1
\par Description
Is the LockTimestamp field valid?

\par Name
LOCKREL OPCODE
\par Value
2
\par Description
Are any of the bits valid in the flags field?

\par Name
LOCKREL AFSID
\par Value
4
\par Description
Is the LockAfsId field valid?

	\subsection sec3-2-8 Section 3.2.8: Miscellaneous 

\par
Miscellaneous values. 
\par Name
VLREPSITE NEW 	
\par Value
1 	
\par Description
Has a replication site gotten a new release of a volume? 
\par 
A synonym for this constant is VLSF NEWREPSITE. 

	\section sec3-3 Section 3.3: Structures and Typedefs 

\par
This section describes the major exported Volume Location Server data
structures of interest to application programmers, along with the typedefs
based upon those structures. 

	\subsection sec3-3-1 Section 3.3.1: struct vldbentry 

\par
This structure represents an entry in the VLDB as made visible to Volume
Location Server clients. It appears in numerous Volume Location Server
interface calls, namely VL CreateEntry(), VL GetEntryByID(), VL
GetEntryByName(), VL ReplaceEntry() and VL ListEntry(). 
\n \b Fields 
\li char name[] - The string name for the volume, with a maximum length of
MAXNAMELEN (65) characters, including the trailing null. 
\li long volumeType - The volume type, one of RWVOL, ROVOL, or BACKVOL. 
\li long nServers - The number of servers that have an instance of this volume. 
\li long serverNumber[] - An array of indices into the table of servers,
identifying the sites holding an instance of this volume. There are at most
MAXNSERVERS (8) of these server sites allowed by the Volume Location Server. 
\li long serverPartition[] - An array of partition identifiers, corresponding
directly to the serverNumber array, specifying the partition on which each of
those volume instances is located. As with the serverNumber array,
serverPartition has up to MAXNSERVERS (8) entries. 
\li long serverFlags[] - This array holds one flag value for each of the
servers in the previous arrays. Again, there are MAXNSERVERS (8) slots in this
array. 
\li u long volumeId[] - An array of volume IDs, one for each volume type. There
are MAXTYPES slots in this array. 
\li long cloneId - This field is used during a cloning operation. 
\li long flags - Flags concerning the status of the fields within this
structure; see Section 3.2.6 for the bit values that apply. 

	\subsection sec3-3-2 Section 3.3.2: struct vlentry 

\par
This structure is used internally by the Volume Location Server to fully
represent a VLDB entry. The client-visible struct vldbentry represents merely a
subset of the information contained herein. 
\n \b Fields 
\li u long volumeId[] - An array of volume IDs, one for each of the MAXTYPES of
volume types. 
\li long flags - Flags concerning the status of the fields within this
structure; see Section 3.2.6 for the bit values that apply. 
\li long LockAfsId - The individual who locked the entry.  This feature has not
yet been implemented. 
\li long LockTimestamp - Time stamp on the entry lock. 
\li long cloneId - This field is used during a cloning operation. 
\li long AssociatedChain - Pointer to the linked list of associated VLDB
entries. 
\li long nextIdHash[] - Array of MAXTYPES next pointers for the ID hash table
pointer, one for each related volume ID. 
\li long nextNameHash - Next pointer for the volume name hash table. 
\li long spares1[] - Two longword spare fields. 
\li char name[] - The volume's string name, with a maximum of MAXNAMELEN (65)
characters, including the trailing null. 
\li u char volumeType - The volume's type, one of RWVOL, ROVOL, or BACKVOL. 
\li u char serverNumber[] - An array of indices into the table of servers,
identifying the sites holding an instance of this volume. There are at most
MAXNSERVERS (8) of these server sites allowed by the Volume Location Server. 
\li u char serverPartition[] - An array of partition identifiers, corresponding
directly to the serverNumber array, specifying the partition on which each of
those volume instances is located. As with the serverNumber array,
serverPartition has up to MAXNSERVERS (8) entries. 
\li u char serverFlags[] - This array holds one flag value for each of the
servers in the previous arrays. Again, there are MAXNSERVERS (8) slots in this
array. 
\li u char RefCount - Only valid for read-write volumes, this field serves as a
reference count, basically the number of dependent children volumes. 
\li char spares2[] - This field is used for 32-bit alignment. 

	\subsection sec3-3-3 Section 3.3.3: struct vital vlheader 

\par
This structure defines the leading section of the VLDB header, of type struct
vlheader. It contains frequently-used global variables and general statistics
information. 
\n \b Fields 
\li long vldbversion - The VLDB version number. This field must appear first in
the structure. 
\li long headersize - The total number of bytes in the header. 
\li long freePtr - Pointer to the first free enry in the free list, if any. 
\li long eofPtr - Pointer to the first free byte in the header file. 
\li long allocs - The total number of calls to the internal AllocBlock()
function directed at this file. 
\li long frees - The total number of calls to the internal FreeBlock() function
directed at this file. 
\li long MaxVolumeId - The largest volume ID ever granted for this cell. 
\li long totalEntries[] - The total number of VLDB entries by volume type in
the VLDB. This array has MAXTYPES slots, one for each volume type. 

	\subsection sec3-3-4 Section 3.3.4: struct vlheader 

\par
This is the layout of the information stored in the VLDB header. Notice it
includes an object of type struct vital vlheader described above (see Section
3.3.3) as the first field. 
\n \b Fields 
\li struct vital vlheader vital header - Holds critical VLDB header
information. 
\li u long IpMappedAddr[] - Keeps MAXSERVERID+1 mappings of IP addresses to
relative ones. 
\li long VolnameHash[] - The volume name hash table, with HASHSIZE slots. 
\li long VolidHash[][] - The volume ID hash table. The first dimension in this
array selects which of the MAXTYPES volume types is desired, and the second
dimension actually implements the HASHSIZE hash table buckets for the given
volume type. 

	\subsection sec3-3-5 Section 3.3.5: struct VldbUpdateEntry 

\par
This structure is used as an argument to the VL UpdateEntry() routine (see
Section 3.6.7). Please note that multiple entries can be updated at once by
setting the appropriate Mask bits. The bit values for this purpose are defined
in Section 3.2.2. 
\n \b Fields 
\li u long Mask - Bit values determining which fields are to be affected by the
update operation. 
\li char name[] - The volume name, up to MAXNAMELEN (65) characters including
the trailing null. 
\li long volumeType - The volume type. 
\li long flags - This field is used in conjuction with Mask (in fact, one of
the Mask bits determines if this field is valid) to choose the valid fields in
this record. 
\li u long ReadOnlyId - The read-only ID. 
\li u long BackupId - The backup ID. 
\li long cloneId - The clone ID. 
\li long nModifiedRepsites - Number of replication sites whose entry is to be
changed as below. 
\li u long RepsitesMask[] - Array of bit masks applying to the up to
MAXNSERVERS (8) replication sites involved. 
\li long RepsitesTargetServer[] - Array of target servers for the operation, at
most MAXNSERVERS (8) of them. 
\li long RepsitesTargetPart[] - Array of target server partitions for the
operation, at most MAXNSERVERS (8) of them. 
\li long RepsitesNewServer[] - Array of new server sites, at most MAXNSERVERS
(8) of them. 
\li long RepsitesNewPart[] - Array of new server partitions for the operation,
at most MAXNSERVERS (8) of them. 
\li long RepsitesNewFlags[] - Flags applying to each of the new sites, at most
MAXNSERVERS (8) of them. 

	\subsection sec3-3-6 Section 3.3.6: struct VldbListByAttributes 

\par
This structure is used by the VL ListAttributes() routine (see Section 3.6.11). 
\n \b Fields 
\li u long Mask - Bit mask used to select the following attribute fields on
which to match. 
\li long server - The server address to match. 
\li long partition - The partition ID to match. 
\li long volumetype - The volume type to match. 
\li long volumeid - The volume ID to match. 
\li long flag - Flags concerning these values. 

	\subsection sec3-3-7 Section 3.3.7: struct single vldbentry 

\par
This structure is used to construct the vldblist object (See Section 3.3.12),
which basically generates a queueable (singly-linked) version of struct
vldbentry. 
\n \b Fields 
\li vldbentry VldbEntry - The VLDB entry to be queued. 
\li vldblist next vldb - The next pointer in the list. 

	\subsection sec3-3-8 Section 3.3.8: struct vldb list 

\par
This structure defines the item returned in linked list form from the VL
LinkedList() function (see Section 3.6.12). This same object is also returned
in bulk form in calls to the VL ListAttributes() routine (see Section 3.6.11). 
\n \b Fields 
\li vldblist node - The body of the first object in the linked list. 

	\subsection sec3-3-9 Section 3.3.9: struct vldstats 

\par
This structure defines fields to record statistics on opcode hit frequency. The
MAX NUMBER OPCODES constant has been defined as the maximum number of opcodes
supported by this structure, and is set to 30. 
\n \b Fields 
\li unsigned long start time - Clock time when opcode statistics were last
cleared. 
\li long requests[] - Number of requests received for each of the MAX NUMBER
OPCODES opcode types. 
\li long aborts[] - Number of aborts experienced for each of the MAX NUMBER
OPCODES opcode types. 
\li long reserved[] - These five longword fields are reserved for future use. 

	\subsection sec3-3-10 Section 3.3.10: bulk 

\code
typedef opaque bulk<DEFAULTBULK>; 
\endcode
\par
This typedef may be used to transfer an uninterpreted set of bytes across the
Volume Location Server interface. It may carry up to DEFAULTBULK (10,000)
bytes. 
\n \b Fields 
\li bulk len - The number of bytes contained within the data pointed to by the
next field. 
\li bulk val - A pointer to a sequence of bulk len bytes. 

	\subsection sec3-3-11 Section 3.3.11: bulkentries 

\code
t