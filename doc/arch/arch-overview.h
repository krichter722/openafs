/*!
  \addtogroup arch-overview Architectural Overview
	\page title AFS-3 Programmer's Reference:  Architectural Overview 

\author Edward R. Zayas 
Transarc Corporation 
\version 1.0
\date 2 September 1991 22:53 .cCopyright 1991 Transarc Corporation All Rights
Reserved FS-00-D160 


	\page chap1 Chapter 1: Introduction 

	\section sec1-1 Section 1.1: Goals and Background 

\par
This paper provides an architectural overview of Transarc's wide-area
distributed file system, AFS. Specifically, it covers the current level of
available software, the third-generation AFS-3 system. This document will
explore the technological climate in which AFS was developed, the nature of
problem(s) it addresses, and how its design attacks these problems in order to
realize the inherent benefits in such a file system. It also examines a set of
additional features for AFS, some of which are actively being considered. 
\par
This document is a member of a reference suite providing programming
specifications as to the operation of and interfaces offered by the various AFS
system components. It is intended to serve as a high-level treatment of
distributed file systems in general and of AFS in particular. This document
should ideally be read before any of the others in the suite, as it provides
the organizational and philosophical framework in which they may best be
interpreted. 

	\section sec1-2 Section 1.2: Document Layout 

\par
Chapter 2 provides a discussion of the technological background and
developments that created the environment in which AFS and related systems were
inspired. Chapter 3 examines the specific set of goals that AFS was designed to
meet, given the possibilities created by personal computing and advances in
communication technology. Chapter 4 presents the core AFS architecture and how
it addresses these goals. Finally, Chapter 5 considers how AFS functionality
may be be improved by certain design changes. 

	\section sec1-3 Section 1.3: Related Documents 

\par
The names of the other documents in the collection, along with brief summaries
of their contents, are listed below. 
\li AFS-3 Programmer?s Reference: File Server/Cache Manager Interface: This
document describes the File Server and Cache Manager agents, which provide the
backbone ?le managment services for AFS. The collection of File Servers for a
cell supplies centralized ?le storage for that site, and allows clients running
the Cache Manager component to access those ?les in a high-performance, secure
fashion. 
\li AFS-3 Programmer?s Reference:Volume Server/Volume Location Server
Interface: This document describes the services through which ?containers? of
related user data are located and managed. 
\li AFS-3 Programmer?s Reference: Protection Server Interface: This paper
describes the server responsible for mapping printable user names to and from
their internal AFS identi?ers. The Protection Server also allows users to
create, destroy, and manipulate ?groups? of users, which are suitable for
placement on Access Control Lists (ACLs). 
\li AFS-3 Programmer?s Reference: BOS Server Interface: This paper covers the
?nanny? service which assists in the administrability of the AFS environment. 
\li AFS-3 Programmer?s Reference: Speci?cation for the Rx Remote Procedure Call
Facility: This document speci?es the design and operation of the remote
procedure call and lightweight process packages used by AFS. 

	\page chap2 Chapter 2: Technological Background 

\par
Certain changes in technology over the past two decades greatly in?uenced the
nature of computational resources, and the manner in which they were used.
These developments created the conditions under which the notion of a
distributed ?le systems (DFS) was born. This chapter describes these
technological changes, and explores how a distributed ?le system attempts to
capitalize on the new computing environment?s strengths and minimize its
disadvantages. 

	\section sec2-1 Section 2.1: Shift in Computational Idioms 

\par
By the beginning of the 1980s, new classes of computing engines and new methods
by which they may be interconnected were becoming firmly established. At this
time, a shift was occurring away from the conventional mainframe-based,
timeshared computing environment to one in which both workstation-class
machines and the smaller personal computers (PCs) were a strong presence. 
\par
The new environment offered many benefits to its users when compared with
timesharing. These smaller, self-sufficient machines moved dedicated computing
power and cycles directly onto people's desks. Personal machines were powerful
enough to support a wide variety of applications, and allowed for a richer,
more intuitive, more graphically-based interface for them. Learning curves were
greatly reduced, cutting training costs and increasing new-employee
productivity. In addition, these machines provided a constant level of service
throughout the day. Since a personal machine was typically only executing
programs for a single human user, it did not suffer from timesharing's
load-based response time degradation. Expanding the computing services for an
organization was often accomplished by simply purchasing more of the relatively
cheap machines. Even small organizations could now afford their own computing
resources, over which they exercised full control. This provided more freedom
to tailor computing services to the specific needs of particular groups. 
\par
However, many of the benefits offered by the timesharing systems were lost when
the computing idiom first shifted to include personal-style machines. One of
the prime casualties of this shift was the loss of the notion of a single name
space for all files. Instead, workstation-and PC-based environments each had
independent and completely disconnected file systems. The standardized
mechanisms through which files could be transferred between machines (e.g.,
FTP) were largely designed at a time when there were relatively few large
machines that were connected over slow links. Although the newer multi-megabit
per second communication pathways allowed for faster transfers, the problem of
resource location in this environment was still not addressed. There was no
longer a system-wide file system, or even a file location service, so
individual users were more isolated from the organization's collective data.
Overall, disk requirements ballooned, since lack of a shared file system was
often resolved by replicating all programs and data to each machine that needed
it. This proliferation of independent copies further complicated the problem of
version control and management in this distributed world. Since computers were
often no longer behind locked doors at a computer center, user authentication
and authorization tasks became more complex. Also, since organizational
managers were now in direct control of their computing facilities, they had to
also actively manage the hardware and software upon which they depended. 
\par
Overall, many of the benefits of the proliferation of independent,
personal-style machines were partially offset by the communication and
organizational penalties they imposed. Collaborative work and dissemination of
information became more difficult now that the previously unified file system
was fragmented among hundreds of autonomous machines. 

	\section sec2-2 Section 2.2: Distributed File Systems 

\par
As a response to the situation outlined above, the notion of a distributed file
system (DFS) was developed. Basically, a DFS provides a framework in which
access to files is permitted regardless of their locations. Specifically, a
distributed file system offers a single, common set of file system operations
through which those accesses are performed. 
\par
There are two major variations on the core DFS concept, classified according to
the way in which file storage is managed. These high-level models are defined
below. 
\li Peer-to-peer: In this symmetrical model, each participating machine
provides storage for specific set of files on its own attached disk(s), and
allows others to access them remotely. Thus, each node in the DFS is capable of
both importing files (making reference to files resident on foreign machines)
and exporting files (allowing other machines to reference files located
locally). 
\li Server-client: In this model, a set of machines designated as servers
provide the storage for all of the files in the DFS. All other machines, known
as clients, must direct their file references to these machines. Thus, servers
are the sole exporters of files in the DFS, and clients are the sole importers. 

\par
The notion of a DFS, whether organized using the peer-to-peer or server-client
discipline, may be used as a conceptual base upon which the advantages of
personal computing resources can be combined with the single-system benefits of
classical timeshared operation. 
\par
Many distributed file systems have been designed and deployed, operating on the
fast local area networks available to connect machines within a single site.
These systems include DOMAIN [9], DS [15], RFS [16], and Sprite [10]. Perhaps
the most widespread of distributed file systems to date is a product from Sun
Microsystems, NFS [13] [14], extending the popular unix file system so that it
operates over local networks. 

	\section sec2-3 Section 2.3: Wide-Area Distributed File Systems 

\par
Improvements in long-haul network technology are allowing for faster
interconnection bandwidths and smaller latencies between distant sites.
Backbone services have been set up across the country, and T1 (1.5
megabit/second) links are increasingly available to a larger number of
locations. Long-distance channels are still at best approximately an order of
magnitude slower than the typical local area network, and often two orders of
magnitude slower. The narrowed difference between local-area and wide-area data
paths opens the window for the notion of a wide-area distributed file system
(WADFS). In a WADFS, the transparency of file access offered by a local-area
DFS is extended to cover machines across much larger distances. Wide-area file
system functionality facilitates collaborative work and dissemination of
information in this larger theater of operation. 

	\page chap3 Chapter 3: AFS-3 Design Goals 

	\section sec3-1 Section 3.1: Introduction 

\par
This chapter describes the goals for the AFS-3 system, the first commercial
WADFS in existence. 
\par
The original AFS goals have been extended over the history of the project. The
initial AFS concept was intended to provide a single distributed file system
facility capable of supporting the computing needs of Carnegie Mellon
University, a community of roughly 10,000 people. It was expected that most CMU
users either had their own workstation-class machine on which to work, or had
access to such machines located in public clusters. After being successfully
implemented, deployed, and tuned in this capacity, it was recognized that the
basic design could be augmented to link autonomous AFS installations located
within the greater CMU campus. As described in Section 2.3, the long-haul
networking environment developed to a point where it was feasible to further
extend AFS so that it provided wide-area file service. The underlying AFS
communication component was adapted to better handle the widely-varying channel
characteristics encountered by intra-site and inter-site operations. 
\par
A more detailed history of AFS evolution may be found in [3] and [18]. 

	\section sec3-2 Section 3.2: System Goals 

\par
At a high level, the AFS designers chose to extend the single-machine unix
computing environment into a WADFS service. The unix system, in all of its
numerous incarnations, is an important computing standard, and is in very wide
use. Since AFS was originally intended to service the heavily unix-oriented CMU
campus, this decision served an important tactical purpose along with its
strategic ramifications. 
\par
In addition, the server-client discipline described in Section 2.2 was chosen
as the organizational base for AFS. This provides the notion of a central file
store serving as the primary residence for files within a given organization.
These centrally-stored files are maintained by server machines and are made
accessible to computers running the AFS client software. 
\par
Listed in the following sections are the primary goals for the AFS system.
Chapter 4 examines how the AFS design decisions, concepts, and implementation
meet this list of goals. 

	\subsection sec3-2-1 Section 3.2.1: Scale 

\par
AFS differs from other existing DFSs in that it has the specific goal of
supporting a very large user community with a small number of server machines.
Unlike the rule-of-thumb ratio of approximately 20 client machines for every
server machine (20:1) used by Sun Microsystem's widespread NFS distributed file
system, the AFS architecture aims at smoothly supporting client/server ratios
more along the lines of 200:1 within a single installation. In addition to
providing a DFS covering a single organization with tens of thousands of users,
AFS also aims at allowing thousands of independent, autonomous organizations to
join in the single, shared name space (see Section 3.2.2 below) without a
centralized control or coordination point. Thus, AFS envisions supporting the
file system needs of tens of millions of users at interconnected yet autonomous
sites. 

	\subsection sec3-2-2 Section 3.2.2: Name Space 

\par
One of the primary strengths of the timesharing computing environment is the
fact that it implements a single name space for all files in the system. Users
can walk up to any terminal connected to a timesharing service and refer to its
files by the identical name. This greatly encourages collaborative work and
dissemination of information, as everyone has a common frame of reference. One
of the major AFS goals is the extension of this concept to a WADFS. Users
should be able to walk up to any machine acting as an AFS client, anywhere in
the world, and use the identical file name to refer to a given object. 
\par
In addition to the common name space, it was also an explicit goal for AFS to
provide complete access transparency and location transparency for its files.
Access transparency is defined as the system's ability to use a single
mechanism to operate on a file, regardless of its location, local or remote.
Location transparency is defined as the inability to determine a file's
location from its name. A system offering location transparency may also
provide transparent file mobility, relocating files between server machines
without visible effect to the naming system. 

	\subsection sec3-2-3 Section 3.2.3: Performance 

\par
Good system performance is a critical AFS goal, especially given the scale,
client-server ratio, and connectivity specifications described above. The AFS
architecture aims at providing file access characteristics which, on average,
are similar to those of local disk performance. 

	\subsection sec3-2-4 Section 3.2.4: Security 

\par
A production WADFS, especially one which allows and encourages transparent file
access between different administrative domains, must be extremely conscious of
security issues. AFS assumes that server machines are "trusted" within their
own administrative domain, being kept behind locked doors and only directly
manipulated by reliable administrative personnel. On the other hand, AFS client
machines are assumed to exist in inherently insecure environments, such as
offices and dorm rooms. These client machines are recognized to be
unsupervisable, and fully accessible to their users. This situation makes AFS
servers open to attacks mounted by possibly modified client hardware, firmware,
operating systems, and application software. In addition, while an organization
may actively enforce the physical security of its own file servers to its
satisfaction, other organizations may be lax in comparison. It is important to
partition the system's security mechanism so that a security breach in one
administrative domain does not allow unauthorized access to the facilities of
other autonomous domains. 
\par
The AFS system is targeted to provide confidence in the ability to protect
system data from unauthorized access in the above environment, where untrusted
client hardware and software may attempt to perform direct remote file
operations from anywhere in the world, and where levels of physical security at
remote sites may not meet the standards of other sites. 

	\subsection sec3-2-5 Section 3.2.5: Access Control 

\par
The standard unix access control mechanism associates mode bits with every file
and directory, applying them based on the user's numerical identifier and the
user's membership in various groups. This mechanism was considered too
coarse-grained by the AFS designers. It was seen as insufficient for specifying
the exact set of individuals and groups which may properly access any given
file, as well as the operations these principals may perform. The unix group
mechanism was also considered too coarse and inflexible. AFS was designed to
provide more flexible and finer-grained control of file access, improving the
ability to define the set of parties which may operate on files, and what their
specific access rights are. 

	\subsection sec3-2-6 Section 3.2.6: Reliability 

\par
The crash of a server machine in any distributed file system causes the
information it hosts to become unavailable to the user community. The same
effect is observed when server and client machines are isolated across a
network partition. Given the potential size of the AFS user community, a single
server crash could potentially deny service to a very large number of people.
The AFS design reflects a desire to minimize the visibility and impact of these
inevitable server crashes. 

	\subsection sec3-2-7 Section 3.2.7: Administrability 

\par
Driven once again by the projected scale of AFS operation, one of the system's
goals is to offer easy administrability. With the large projected user
population, the amount of file data expected to be resident in the shared file
store, and the number of machines in the environment, a WADFS could easily
become impossible to administer unless its design allowed for easy monitoring
and manipulation of system resources. It is also imperative to be able to apply
security and access control mechanisms to the administrative interface. 

	\subsection sec3-2-8 Section 3.2.8: Interoperability/Coexistence 

\par
Many organizations currently employ other distributed file systems, most
notably Sun Microsystem's NFS, which is also an extension of the basic
single-machine unix system. It is unlikely that AFS will receive significant
use if it cannot operate concurrently with other DFSs without mutual
interference. Thus, coexistence with other DFSs is an explicit AFS goal. 
\par
A related goal is to provide a way for other DFSs to interoperate with AFS to
various degrees, allowing AFS file operations to be executed from these
competing systems. This is advantageous, since it may extend the set of
machines which are capable of interacting with the AFS community. Hardware
platforms and/or operating systems to which AFS is not ported may thus be able
to use their native DFS system to perform AFS file references. 
\par
These two goals serve to extend AFS coverage, and to provide a migration path
by which potential clients may sample AFS capabilities, and gain experience
with AFS. This may result in data migration into native AFS systems, or the
impetus to acquire a native AFS implementation. 

	\subsection sec3-2-9 Section 3.2.9: Heterogeneity/Portability 

\par
It is important for AFS to operate on a large number of hardware platforms and
operating systems, since a large community of unrelated organizations will most
likely utilize a wide variety of computing environments. The size of the
potential AFS user community will be unduly restricted if AFS executes on a
small number of platforms. Not only must AFS support a largely heterogeneous
computing base, it must also be designed to be easily portable to new hardware
and software releases in order to maintain this coverage over time. 

	\page chap4 Chapter 4: AFS High-Level Design 

	\section sec4-1 Section 4.1: Introduction 

\par
This chapter presents an overview of the system architecture for the AFS-3
WADFS. Different treatments of the AFS system may be found in several
documents, including [3], [4], [5], and [2]. Certain system features discussed
here are examined in more detail in the set of accompanying AFS programmer
specification documents. 
\par
After the archtectural overview, the system goals enumerated in Chapter 3 are
revisited, and the contribution of the various AFS design decisions and
resulting features is noted. 

	\section sec4-2 Section 4.2: The AFS System Architecture 

	\subsection sec4-2-1 Section 4.2.1: Basic Organization 

\par
As stated in Section 3.2, a server-client organization was chosen for the AFS
system. A group of trusted server machines provides the primary disk space for
the central store managed by the organization controlling the servers. File
system operation requests for specific files and directories arrive at server
machines from machines running the AFS client software. If the client is
authorized to perform the operation, then the server proceeds to execute it. 
\par
In addition to this basic file access functionality, AFS server machines also
provide related system services. These include authentication service, mapping
between printable and numerical user identifiers, file location service, time
service, and such administrative operations as disk management, system
reconfiguration, and tape backup. 

	\subsection sec4-2-2 Section 4.2.2: Volumes 

	\subsubsection sec4-2-2-1 Section 4.2.2.1: Definition 

\par
Disk partitions used for AFS storage do not directly host individual user files
and directories. Rather, connected subtrees of the system's directory structure
are placed into containers called volumes. Volumes vary in size dynamically as
the objects it houses are inserted, overwritten, and deleted. Each volume has
an associated quota, or maximum permissible storage. A single unix disk
partition may thus host one or more volumes, and in fact may host as many
volumes as physically fit in the storage space. However, the practical maximum
is currently 3,500 volumes per disk partition. This limitation is imposed by
the salvager program, which examines and repairs file system metadata
structures. 
\par
There are two ways to identify an AFS volume. The first option is a 32-bit
numerical value called the volume ID. The second is a human-readable character
string called the volume name. 
\par
Internally, a volume is organized as an array of mutable objects, representing
individual files and directories. The file system object associated with each
index in this internal array is assigned a uniquifier and a data version
number. A subset of these values are used to compose an AFS file identifier, or
FID. FIDs are not normally visible to user applications, but rather are used
internally by AFS. They consist of ordered triplets, whose components are the
volume ID, the index within the volume, and the uniquifier for the index. 
\par
To understand AFS FIDs, let us consider the case where index i in volume v
refers to a file named example.txt. This file's uniquifier is currently set to
one (1), and its data version number is currently set to zero (0). The AFS
client software may then refer to this file with the following FID: (v, i, 1).
The next time a client overwrites the object identified with the (v, i, 1) FID,
the data version number for example.txt will be promoted to one (1). Thus, the
data version number serves to distinguish between different versions of the
same file. A higher data version number indicates a newer version of the file. 
\par
Consider the result of deleting file (v, i, 1). This causes the body of
example.txt to be discarded, and marks index i in volume v as unused. Should
another program create a file, say a.out, within this volume, index i may be
reused. If it is, the creation operation will bump the index's uniquifier to
two (2), and the data version number is reset to zero (0). Any client caching a
FID for the deleted example.txt file thus cannot affect the completely
unrelated a.out file, since the uniquifiers differ. 

	\subsubsection sec4-2-2-2 Section 4.2.2.2: Attachment 

\par
The connected subtrees contained within individual volumes are attached to
their proper places in the file space defined by a site, forming a single,
apparently seamless unix tree. These attachment points are called mount points.
These mount points are persistent file system objects, implemented as symbolic
links whose contents obey a stylized format. Thus, AFS mount points differ from
NFS-style mounts. In the NFS environment, the user dynamically mounts entire
remote disk partitions using any desired name. These mounts do not survive
client restarts, and do not insure a uniform namespace between different
machines. 
\par
A single volume is chosen as the root of the AFS file space for a given
organization. By convention, this volume is named root.afs. Each client machine
belonging to this organization peforms a unix mount() of this root volume (not
to be confused with an AFS mount point) on its empty /afs directory, thus
attaching the entire AFS name space at this point. 

	\subsubsection sec4-2-2-3 Section 4.2.2.3: Administrative Uses 

\par
Volumes serve as the administrative unit for AFS ?le system data, providing as
the basis for replication, relocation, and backup operations. 

	\subsubsection sec4-2-2-4 Section 4.2.2.4: Replication 

Read-only snapshots of AFS volumes may be created by administrative personnel.
These clones may be deployed on up to eight disk partitions, on the same server
machine or across di?erent servers. Each clone has the identical volume ID,
which must di?er from its read-write parent. Thus, at most one clone of any
given volume v may reside on a given disk partition. File references to this
read-only clone volume may be serviced by any of the servers which host a copy. 

	\subsubsection sec4-2-2-4 Section 4.2.2.5: Backup 

\par
Volumes serve as the unit of tape backup and restore operations. Backups are
accomplished by first creating an on-line backup volume for each volume to be
archived. This backup volume is organized as a copy-on-write shadow of the
original volume, capturing the volume's state at the instant that the backup
took place. Thus, the backup volume may be envisioned as being composed of a
set of object pointers back to the original image. The first update operation
on the file located in index i of the original volume triggers the
copy-on-write association. This causes the file's contents at the time of the
snapshot to be physically written to the backup volume before the newer version
of the file is stored in the parent volume. 
\par
Thus, AFS on-line backup volumes typically consume little disk space. On
average, they are composed mostly of links and to a lesser extent the bodies of
those few files which have been modified since the last backup took place.
Also, the system does not have to be shut down to insure the integrity of the
backup images. Dumps are generated from the unchanging backup volumes, and are
transferred to tape at any convenient time before the next backup snapshot is
performed. 

	\subsubsection sec4-2-2-6 Section 4.2.2.6: Relocation 

\par
Volumes may be moved transparently between disk partitions on a given file
server, or between different file server machines. The transparency of volume
motion comes from the fact that neither the user-visible names for the files
nor the internal AFS FIDs contain server-specific location information. 
\par
Interruption to file service while a volume move is being executed is typically
on the order of a few seconds, regardless of the amount of data contained
within the volume. This derives from the staged algorithm used to move a volume
to a new server. First, a dump is taken of the volume's contents, and this
image is installed at the new site. The second stage involves actually locking
the original volume, taking an incremental dump to capture file updates since
the first stage. The third stage installs the changes at the new site, and the
fourth stage deletes the original volume. Further references to this volume
will resolve to its new location. 

	\subsection sec4-2-3 Section 4.2.3: Authentication 

\par
AFS uses the Kerberos [22] [23] authentication system developed at MIT's
Project Athena to provide reliable identification of the principals attempting
to operate on the files in its central store. Kerberos provides for mutual
authentication, not only assuring AFS servers that they are interacting with
the stated user, but also assuring AFS clients that they are dealing with the
proper server entities and not imposters. Authentication information is
mediated through the use of tickets. Clients register passwords with the
authentication system, and use those passwords during authentication sessions
to secure these tickets. A ticket is an object which contains an encrypted
version of the user's name and other information. The file server machines may
request a caller to present their ticket in the course of a file system
operation. If the file server can successfully decrypt the ticket, then it
knows that it was created and delivered by the authentication system, and may
trust that the caller is the party identified within the ticket. 
\par
Such subjects as mutual authentication, encryption and decryption, and the use
of session keys are complex ones. Readers are directed to the above references
for a complete treatment of Kerberos-based authentication. 

	\subsection sec4-2-4 Section 4.2.4: Authorization 

	\subsubsection sec4-2-4-1 Section 4.2.4.1: Access Control Lists 

\par
AFS implements per-directory Access Control Lists (ACLs) to improve the ability
to specify which sets of users have access to the ?les within the directory,
and which operations they may perform. ACLs are used in addition to the
standard unix mode bits. ACLs are organized as lists of one or more (principal,
rights) pairs. A principal may be either the name of an individual user or a
group of individual users. There are seven expressible rights, as listed below. 
\li Read (r): The ability to read the contents of the files in a directory. 
\li Lookup (l): The ability to look up names in a directory. 
\li Write (w): The ability to create new files and overwrite the contents of
existing files in a directory. 
\li Insert (i): The ability to insert new files in a directory, but not to
overwrite existing files. 
\li Delete (d): The ability to delete files in a directory. 
\li Lock (k): The ability to acquire and release advisory locks on a given
directory. 
\li Administer (a): The ability to change a directory's ACL. 

	\subsubsection sec4-2-4-2 Section 4.2.4.2: AFS Groups 

\par
AFS users may create a certain number of groups, differing from the standard
unix notion of group. These AFS groups are objects that may be placed on ACLs,
and simply contain a list of AFS user names that are to be treated identically
for authorization purposes. For example, user erz may create a group called
erz:friends consisting of the kazar, vasilis, and mason users. Should erz wish
to grant read, lookup, and insert rights to this group in directory d, he
should create an entry reading (erz:friends, rli) in d's ACL. 
\par
AFS offers three special, built-in groups, as described below. 
\par
1. system:anyuser: Any individual who accesses AFS files is considered by the
system to be a member of this group, whether or not they hold an authentication
ticket. This group is unusual in that it doesn't have a stable membership. In
fact, it doesn't have an explicit list of members. Instead, the system:anyuser
"membership" grows and shrinks as file accesses occur, with users being
(conceptually) added and deleted automatically as they interact with the
system. 
\par
The system:anyuser group is typically put on the ACL of those directories for
which some specific level of completely public access is desired, covering any
user at any AFS site. 
\par
2. system:authuser: Any individual in possession of a valid Kerberos ticket
minted by the organization's authentication service is treated as a member of
this group. Just as with system:anyuser, this special group does not have a
stable membership. If a user acquires a ticket from the authentication service,
they are automatically "added" to the group. 