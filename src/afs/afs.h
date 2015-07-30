/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 *
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */

#ifndef _AFS_H_
#define _AFS_H_
/*
 * AFS system call opcodes
 */
#ifdef KDUMP_KERNEL
#include <afs/afs_args.h>
#include <afs/afs_consts.h>
#else
#include "afs/afs_args.h"
#include "afs/afs_consts.h"
#endif

/*
 * afs_fsfragsize cannot be less than 1023, or some cache-tracking
 * calculations will be incorrect (since we track cache usage in kb).
 * Some filesystems have fundamental blocksizes less than 1k, and
 * normally we would tune afs_fsfragsize to be fragsize-1, but we must
 * make sure to check that afs_fsfragsize does not go below this value.
 */
#define AFS_MIN_FRAGSIZE 1023

/* Upper bound on number of iovecs out uio routines will deal with. */
#define	AFS_MAXIOVCNT	    16


extern int afs_shuttingdown;

/*
 * Macros to uniquely identify the AFS vfs struct
 */
#define	AFS_VFSMAGIC		0x1234
#if defined(AFS_SUN5_ENV) || defined(AFS_HPUX90_ENV) || defined(AFS_LINUX20_ENV)
#define	AFS_VFSFSID		99
#else
#if defined(AFS_SGI_ENV)
#define AFS_VFSFSID		afs_fstype
#else
#define	AFS_VFSFSID		AFS_MOUNT_AFS
#endif
#endif
/* use this value for reporting total space, free space, etc.
 * fake a high number to satisfy programs that use the statfs call to make sure
 * that there's enough space in the device partition before storing something
 * there. keep it just under 2^31, to try and be safe about not breaking
 * stuff that looks at the amount of free space. */
#define AFS_VFS_FAKEFREE (2147483647)

/* Moved from VNOPS/afs_vnop_flocks so can be used in prototypes */
#if     defined(AFS_HPUX102_ENV)
#define AFS_FLOCK       k_flock
#else
#if     defined(AFS_SUN5_ENV) || (defined(AFS_LINUX24_ENV) && !(defined(AFS_LINUX26_ENV) && defined(AFS_LINUX_64BIT_KERNEL)))
#define AFS_FLOCK       flock64
#else
#define AFS_FLOCK       flock
#endif /* AFS_SUN65_ENV */
#endif /* AFS_HPUX102_ENV */

/* The following are various levels of afs debugging */
#define	AFSDEB_GENERAL		1	/* Standard debugging */
#define	AFSDEB_NETWORK		2	/* low level afs networking */
#define	AFSDEB_RX		4	/* RX debugging */
#define	AFSDEB_VNLAYER		8	/* interface layer to AFS (aixops, gfsops, etc) */

/* generic undefined vice id */
#define	UNDEFVID	    (-1)

/* The basic defines for the Andrew file system
    better keep things powers of two so "& (foo-1)" hack works for masking bits */
#define	NBRS		15	/* max number of queued daemon requests */
#define	NUSERS		16	/* hash table size for unixuser table */
#define	NSERVERS	16	/* hash table size for server table */
#define	NVOLS		64	/* hash table size for volume table */
#define	NFENTRIES	256	/* hash table size for disk volume table */
#define	VCSIZE	       1024	/* stat cache hash table size */
#define CBRSIZE		512	/* call back returns hash table size */
#define	PIGGYSIZE	1350	/* max piggyback size */
#define	MAXVOLS		128	/* max vols we can store */
#define	MAXSYSNAME	128	/* max sysname (i.e. @sys) size */
#define MAXNUMSYSNAMES	32	/* max that current constants allow */
#define	NOTOKTIMEOUT	(2*3600)	/* time after which to timeout conns sans tokens */
#define	NOPAG		0xffffffff



#define AFS_MAXCBRSCALL	32	/* max to return in a given call (must be <= AFSCBMAX) */
#define	VCACHE_FREE	5
#define	AFS_NRXPACKETS	80
#define	AFS_RXDEADTIME	50
#define AFS_HARDDEADTIME	120
#define	AFS_IDLEDEADTIME	1200
#define AFS_IDLEDEADTIME_REP    180 /* more than fs's cb dead time */
#define AFS_BLKBITS	12
#define AFS_BLKSIZE	(1 << AFS_BLKBITS)

extern afs_int32 afs_rx_deadtime;
extern afs_int32 afs_rx_harddead;
extern afs_int32 afs_rx_idledead;
extern afs_int32 afs_rx_idledead_rep;

struct sysname_info {
    char *name;
    short offset;
    signed char index, allocked;
};

/* flags to use with AFSOP_CACHEINIT */
#define AFSCALL_INIT_MEMCACHE        0x1	/* use a memory-based cache */

/* below here used only for kernel procedures */
#ifdef KERNEL
/* Store synchrony flags - SYNC means that data should be forced to server's
 * disk immediately upon completion. */
#define AFS_ASYNC 	0
#define AFS_SYNC  	1
#define AFS_VMSYNC_INVAL 2	/* sync and invalidate pages */
#define AFS_LASTSTORE   4
#define AFS_VMSYNC      8       /* sync pages but do not invalidate */

/* background request structure */
#define	BPARMS		4

#define	BOP_NOOP	0	/* leave 0 unused */
#define	BOP_FETCH	1	/* parm1 is chunk to get */
#define	BOP_STORE	2	/* parm1 is chunk to store */
#define	BOP_PATH	3	/* parm1 is path, parm2 is chunk to fetch */
#define	BOP_FETCH_NOCACHE	4   /* parms are: vnode ptr, offset, segment ptr, addr, cred ptr */
#ifdef AFS_DARWIN_ENV
#define	BOP_MOVE	5	 /* ptr1 afs_uspc_param ptr2 sname ptr3 dname */
#endif
#define BOP_PARTIAL_STORE 6     /* parm1 is chunk to store */

#define	B_DONTWAIT	1	/* On failure return; don't wait */

/* protocol is: refCount is incremented by user to take block out of free pool.
    Next, BSTARTED is set when daemon finds request.  This prevents
    other daemons from picking up the same request.  Finally, when
    request is done, refCount is zeroed.  BDONE and BWAIT are used by
    dudes waiting for operation to proceed to a certain point before returning.
*/
#define	BSTARTED	1	/* request picked up by a daemon */
#define	BUVALID		2	/* code is valid (store) */
#define	BUWAIT		4	/* someone is waiting for BUVALID */
struct brequest {
    struct vcache *vc;		/* vnode to use, with vrefcount bumped */
    afs_ucred_t *cred;	/* credentials to use for operation */
    afs_size_t size_parm[BPARMS];	/* random parameters */
    void *ptr_parm[BPARMS];	/* pointer parameters */
    afs_int32 code_raw;		/* return code from AFS routines */
    afs_int32 code_checkcode;	/* the afs_CheckCode-translated code */
    short refCount;		/* use counter for this structure */
    char opcode;		/* what to do (store, fetch, etc) */
    char flags;			/* free, etc */
    afs_int32 ts;		/* counter "timestamp" */
};

struct SecretToken {
    char data[56];
};

struct ClearToken {
    afs_int32 AuthHandle;
    char HandShakeKey[8];
    afs_int32 ViceId;
    afs_int32 BeginTimestamp;
    afs_int32 EndTimestamp;
};

struct VenusFid {
    afs_int32 Cell;		/* better sun packing if at end of structure */
    struct AFSFid Fid;
};

/* Temporary struct to be passed between afs_fid and afs_vget; in SunOS4.x we can only pass a maximum of 10 bytes for a handle (we ideally need 16!) */
struct SmallFid {
    afs_int32 Volume;
    afs_int32 CellAndUnique;
    u_short Vnode;
};
/* The actual number of bytes in the SmallFid, not the sizeof struct. */
#define SIZEOF_SMALLFID 10

/* Queues
 * ------
 *
 *  Circular queues, implemented with pointers. Structures may contain as many
 *  queues as required, which may be located at any point within the structure,
 *  providing the QEntry macro is used to translate between a queue pointer, and
 *  the address of its containing structure
 */

struct afs_q {
    struct afs_q *next;
    struct afs_q *prev;
};

#define QZero(e)    ((e)->prev = (e)->next = NULL)
#define	QInit(q)    ((q)->prev = (q)->next = (q))
#define	QAdd(q,e)   ((e)->next = (q)->next, (e)->prev = (q), \
			(q)->next->prev = (e), (q)->next = (e))
#define	QRemove(e)  ((e)->next->prev = (e)->prev, (e)->prev->next = (e)->next, (e)->prev = NULL, (e)->next = NULL)
#define	QNext(e)    ((e)->next)
#define QPrev(e)    ((e)->prev)
#define QEmpty(q)   ((q)->prev == (q))
/* this one takes q1 and sticks it on the end of q2 - that is, the other end, not the end
 * that things are added onto.  q1 shouldn't be empty, it's silly */
#define QCat(q1,q2) ((q2)->prev->next = (q1)->next, (q1)->next->prev=(q2)->prev, (q1)->prev->next=(q2), (q2)->prev=(q1)->prev, (q1)->prev=(q1)->next=(q1))

/* Given a pointer to an afs_q within a structure, go back to the address of
 * the parent structure
 */

#define QEntry(queue, structure, member) \
	((structure *)((char *)(queue)-(char *)(&((structure *)NULL)->member)))

/* And implement operations for individual lists in terms of the above macro */

#define QTOV(e)	    QEntry(e, struct vcache, vlruq)
#define QTOC(e)	    QEntry(e, struct cell, lruq)
#define QTOVH(e)    QEntry(e, struct vcache, vhashq)

/*!
 * List of free slot numbers
 */
struct afs_slotlist {
    afs_uint32 slot;
    struct afs_slotlist *next;
};

struct vrequest {
    afs_int32 uid;		/* user id making the request */
    afs_int32 busyCount;	/* how many busies we've seen so far */
    afs_int32 flags;		/* things like O_SYNC, O_NONBLOCK go here */
    char initd;			/* if non-zero, Error fields meaningful */
    char accessError;		/* flags for overriding error return code */
    char volumeError;		/* encountered a missing or busy volume */
    char networkError;		/* encountered network problems */
    char permWriteError;	/* fileserver returns permenent error. */
    char tokenError;            /* a token error other than expired. */
    char idleError;             /* the server idled too long */
    char skipserver[AFS_MAXHOSTS];
    afs_int32 lasterror[AFS_MAXHOSTS];
};
#define VOLMISSING 1
#define VOLBUSY 2

/* structure linked off of a server to keep track of queued returned
 * callbacks.  Sent asynchronously when we run a little low on free dudes.
 */
struct afs_cbr {
    struct afs_cbr **pprev;
    struct afs_cbr *next;

    struct afs_cbr **hash_pprev;
    struct afs_cbr *hash_next;

    struct AFSFid fid;
};

#ifdef AFS_LINUX22_ENV
/* On Linux, we have to be able to allocate the storage for this using
 * kmalloc, as otherwise we may deadlock. So, it needs to be able to fit
 * in a single page
 */
# define AFS_NCBRS	PAGE_SIZE/sizeof(struct afs_cbr)
#else
# define AFS_NCBRS	1024	/* max # of call back return entries */
#endif

/* cellinfo file magic number */
#define AFS_CELLINFO_MAGIC	0xf32817cd

/* cell flags */
#define	CNoSUID			0x02	/* disable suid bit for this cell */
#define CLinkedCell4		0x04	/* reserved for ADDCELL2 pioctl */
#define CNoAFSDB		0x08	/* never bother trying AFSDB */
#define CHasVolRef		0x10	/* volumes were referenced */
#define CLinkedCell		0x20	/* has a linked cell in lcellp */
#define CHush                   0x40    /* don't display until referenced */

struct cell {
    struct afs_q lruq;		/* lru q next and prev */
    char *cellName;		/* char string name of cell */
    afs_int32 cellIndex;	/* sequence number */
    afs_int32 cellNum;		/* semi-permanent cell number */
    struct server *cellHosts[AFS_MAXCELLHOSTS];	/* volume *location* hosts */
    struct cell *lcellp;	/* Associated linked cell */
    u_short fsport;		/* file server port */
    u_short vlport;		/* volume server port */
    short states;		/* state flags */
    time_t timeout;		/* data expire time, if non-zero */
    struct cell_name *cnamep;	/* pointer to our cell_name */
    afs_rwlock_t lock;		/* protects cell data */
    unsigned char cellHandle[16];	/* deterministic handle for this cell */
};

struct cell_name {
    struct cell_name *next;
    afs_int32 cellnum;
    char *cellname;
    char used;
};

struct cell_alias {
    struct cell_alias *next;
    afs_int32 index;
    char *alias;
    char *cell;
};

#define	afs_PutCell(cellp, locktype)

/* the unixuser flag bit definitions */
#define	UHasTokens	1	/* are the st and ct fields valid (ever set)? */
#define	UTokensBad	2	/* are tokens bad? */
#define UPrimary        4	/* on iff primary identity */
#define UNeedsReset	8	/* needs afs_ResetAccessCache call done */
#define UPAGCounted    16	/* entry seen during PAG search (for stats) */
#define UNFSGetCreds   32	/* getting creds for NFS client */
/* A flag used by afs_GCPAGs to keep track of
 * which entries in afs_users need to be deleted.
 * The lifetime of its presence in the table is the
 * lifetime of the afs_GCPAGs function.
 */
#define TMP_UPAGNotReferenced	128

/* unixuser notify events */
#define UTokensObtained 1
#define UTokensDropped  2

/* values for afs_gcpags */
enum { AFS_GCPAGS_NOTCOMPILED = 0, AFS_GCPAGS_OK =
	1, AFS_GCPAGS_USERDISABLED, AFS_GCPAGS_EPROC0, AFS_GCPAGS_EPROCN,
    AFS_GCPAGS_EEQPID, AFS_GCPAGS_EINEXACT, AFS_GCPAGS_EPROCEND,
    AFS_GCPAGS_EPROCWALK, AFS_GCPAGS_ECREDWALK, AFS_GCPAGS_EPIDCHECK,
    AFS_GCPAGS_ENICECHECK
};

extern afs_int32 afs_gcpags;
extern afs_int32 afs_gcpags_procsize;
extern afs_int32 afs_bkvolpref;
extern char afs_cachebasedir[1024];
extern afs_int32 afs_numcachefiles;
extern afs_int32 afs_numfilesperdir;

struct rxkadToken {
    afs_int32 ticketLen;
    char * ticket;
    struct ClearToken clearToken;
};

union tokenUnion {
    struct rxkadToken rxkad;
};

struct tokenJar {
    struct tokenJar *next;
    int type;
    union tokenUnion content;
};

struct unixuser {
    struct unixuser *next;	/* next hash pointer */
    afs_int32 uid;		/* search based on uid and cell */
    afs_int32 cell;
    afs_int32 viceId;		/* Optional viced ID corresponding to current tokens */
    short refCount;		/* reference count for allocation */
    char states;		/* flag info */
    afs_int32 tokenTime;	/* last time tokens were set, used for timing out conn data */
    struct tokenJar *tokens;
    struct afs_exporter *exporter;	/* more info about the exporter for the remote user */
    void *cellinfo;             /* pointer to cell info (PAG manager only) */
    afs_rwlock_t lock;
};

#define CVEC_LEN 3 /* per-user connection pool */

struct sa_conn_vector;
typedef struct sa_conn_vector * p_sa_conn_vector; /* forward decl */

#define CONN_REPLICATED 0x1

struct afs_conn {
    int refCount;
    int activated;
    char forceConnectFS; /* Should we try again with these tokens? */
    struct rx_connection *id; /* RPC connid */
    struct sa_conn_vector *parent; /* the con_vector which contains us */
};

/* An sa_conn_vector replaces the erstwhile list of conn
   structures maintained by the cache manager.  The sa_conn_vector
   contains a C array of connections which, if non-zero, represent
   connections to AFS servers.
*/

struct sa_conn_vector {
    /* linked-list machinery */
    struct sa_conn_vector *next;

    /* AFS conn-identifying info */
    struct unixuser *user;	/* user validated with respect to. */
    struct srvAddr *srvr;	/* server associated with this conn */
    short refCount;		/* reference count for allocation */
    unsigned short port;	/* port associated with this connection */
    int flags;

    /* next connection to return when all in cvec are fully utilized */
    int select_index; 
    
    /* connections vector */
    struct afs_conn cvec[CVEC_LEN];
};

#define SQNULL -1

/* Fid comparison routines */
#define	FidCmp(a,b) ((a)->Fid.Unique != (b)->Fid.Unique \
    || (a)->Fid.Vnode != (b)->Fid.Vnode \
    || (a)->Fid.Volume != (b)->Fid.Volume \
    || (a)->Cell != (b)->Cell)

#define	FidMatches(afid,tvc) ((tvc)->f.fid.Fid.Vnode == (afid)->Fid.Vnode && \
	(tvc)->f.fid.Fid.Volume == (afid)->Fid.Volume && \
	(tvc)->f.fid.Cell == (afid)->Cell && \
	( (tvc)->f.fid.Fid.Unique == (afid)->Fid.Unique || \
	 (!(afid)->Fid.Unique && ((tvc)->f.states & CUnique))))


#define	SRVADDR_MH	1
#define	SRVADDR_ISDOWN	0x20	/* same as SRVR_ISDOWN */
#define  SRVADDR_NOUSE    0x40	/* Don't use this srvAddr */

struct srvAddr {
    struct srvAddr *next_bkt;	/* next item in hash bucket */
    struct srvAddr *next_sa;	/* another interface on same host */
    struct server *server;	/* back to parent */
    struct sa_conn_vector *conns;   /* All user connections to this server */
    struct afs_conn *natping;
    afs_int32 sa_ip;		/* Host addr in network byte order */
    u_short sa_iprank;		/* indiv ip address priority */
    u_short sa_portal;		/* port addr in network byte order */
    u_char sa_flags;
};

/*
 * Values used in the flags field of the server structure below.
 *
 *	AFS_SERVER_FLAG_ACTIVATED Has the server ever had a user connection
 *				  associated with it?
 */
#define AFS_SERVER_FLAG_ACTIVATED	0x01
#define	SNO_LHOSTS			0x04
#define	SYES_LHOSTS			0x08
#define	SVLSRV_UUID			0x10
#define	SRVR_ISDOWN			0x20
#define	SRVR_MULTIHOMED			0x40
#define	SRVR_ISGONE			0x80
#define	SNO_INLINEBULK			0x100
#define SNO_64BIT                       0x200
#define SCAPS_KNOWN			0x400

#define SRV_CAPABILITIES(ts) \
{ if ( !(ts->flags & SCAPS_KNOWN)) afs_GetCapabilities(ts); ts->capabilities; }

#define afs_serverSetNo64Bit(s) (((struct sa_conn_vector*)(s)->parent)->srvr->server->flags |= SNO_64BIT)
#define afs_serverHasNo64Bit(s) (((struct sa_conn_vector*)(s)->parent)->srvr->server->flags & SNO_64BIT)

struct server {
    union {
	struct {
	    afsUUID suuid;
	    afs_int32 addr_uniquifier;
	    afs_int32 spares[2];
	} _srvUuid;
	struct {
	    struct srvAddr haddr;
	} _srvId;
    } _suid;
#define sr_uuid		_suid._srvUuid.suuid
#define sr_addr_uniquifier	_suid._srvUuid.addr_uniquifier
#define sr_host		_suid._srvId.haddr.ip
#define sr_portal	_suid._srvId.haddr.portal
#define sr_rank		_suid._srvId.haddr.ip_rank
#define sr_flags	_suid._srvId.haddr.flags
#define sr_conns	_suid._srvId.haddr.conns
    struct server *next;	/* Ptr to next server in hash chain */
    struct cell *cell;		/* Cell in which this host resides */
    struct afs_cbr *cbrs;	/* Return list of callbacks */
    afs_int32 activationTime;	/* Time when this record was first activated */
    afs_int32 lastDowntimeStart;	/* Time when last downtime incident began */
    afs_int32 numDowntimeIncidents;	/* # (completed) downtime incidents */
    afs_int32 sumOfDowntimes;	/* Total downtime experienced, in seconds */
    struct srvAddr *addr;
    afs_uint32 flags;		/* Misc flags */
    afs_int32 capabilities;
};

#define	afs_PutServer(servp, locktype)

/* structs for some pioctls  - these are (or should be)
 * also in venus.h
 */
struct spref {
    struct in_addr host;
    unsigned short rank;
};

struct sprefrequest_33 {
    unsigned short offset;
    unsigned short num_servers;
};


struct sprefrequest {		/* new struct for 3.4 */
    unsigned short offset;
    unsigned short num_servers;
    unsigned short flags;
};
#define DBservers 1

struct sprefinfo {
    unsigned short next_offset;
    unsigned short num_servers;
    struct spref servers[1];	/* we overrun this array intentionally... */
};

struct setspref {
    unsigned short flags;
    unsigned short num_servers;
    struct spref servers[1];	/* we overrun this array intentionally... */
};
/* struct for GAG pioctl
 */
struct gaginfo {
    afs_uint32 showflags, logflags, logwritethruflag, spare[3];
    unsigned char spare2[128];
};
#define GAGUSER    1
#define GAGCONSOLE 2
#define logwritethruON	1

struct rxparams {
    afs_int32 rx_initReceiveWindow, rx_maxReceiveWindow, rx_initSendWindow,
	rx_maxSendWindow, rxi_nSendFrags, rxi_nRecvFrags, rxi_OrphanFragSize;
    afs_int32 rx_maxReceiveSize, rx_MyMaxSendSize;
    afs_uint32 spare[21];
};

/* struct for checkservers */

struct chservinfo {
    int magic;
    char tbuffer[128];
    int tsize;
    afs_int32 tinterval;
    afs_int32 tflags;
};


/* state bits for volume */
#define VRO			1	/* volume is readonly */
#define VRecheck		2	/* recheck volume info with server */
#define VBackup			4	/* is this a backup volume? */
#define VForeign		8	/* this is a non-afs volume */
#define VPartVisible		16	/* Volume's partition is visible on the client */
#define VHardMount		32	/* we are hard-mount waiting for the vol */

enum repstate { not_busy, end_not_busy = 6, rd_busy, rdwr_busy, offline };

struct volume {
    /* One structure per volume, describing where the volume is located
     * and where its mount points are. */
    struct volume *next;	/* Next volume in hash list. */
    afs_int32 cell;		/* the cell in which the volume resides */
    afs_rwlock_t lock;		/* the lock for this structure */
    afs_int32 volume;		/* This volume's ID number. */
    char *name;			/* This volume's name, or 0 if unknown */
    struct server *serverHost[AFS_MAXHOSTS];	/* servers serving this volume */
    enum repstate status[AFS_MAXHOSTS];	/* busy, offline, etc */
    struct VenusFid dotdot;	/* dir to access as .. */
    struct VenusFid mtpoint;	/* The mount point for this volume. */
    afs_int32 rootVnode, rootUnique;	/* Volume's root fid */
    afs_int32 roVol;
    afs_int32 backVol;
    afs_int32 rwVol;		/* For r/o vols, original read/write volume. */
    afs_int32 accessTime;	/* last time we used it */
    afs_int32 vtix;		/* volume table index */
    afs_int32 copyDate;		/* copyDate field, for tracking vol releases */
    afs_int32 expireTime;	/* for per-volume callbacks... */
    short refCount;		/* reference count for allocation */
    char states;		/* here for alignment reasons */
};

#define afs_PutVolume(av, locktype) ((av)->refCount--)

/* format of an entry in volume info file */
struct fvolume {
    afs_int32 cell;		/* cell for this entry */
    afs_int32 volume;		/* volume */
    afs_int32 next;		/* has index */
    struct VenusFid dotdot;	/* .. value */
    struct VenusFid mtpoint;	/* mt point's fid */
    afs_int32 rootVnode, rootUnique;	/* Volume's root fid */
};

struct SimpleLocks {
    struct SimpleLocks *next;
    int type;
    afs_int32 boff, eoff;
    afs_int32 pid;
#if	defined(AFS_AIX32_ENV) || defined(AFS_SUN5_ENV) || defined(AFS_SGI_ENV)
    afs_int32 sysid;
#endif
};

/* vcache state bits */
#define CStatd		0x00000001	/* has this file ever been stat'd? */
#define CBackup		0x00000002	/* file is on a backup volume */
#define CRO		0x00000004	/* is it on a read-only volume */
#define CMValid		0x00000008	/* is the mount point info valid? */
#define CCore		0x00000010	/* storing a core file, needed since we don't get an open */
#define CDirty		0x00000020	/* file has been modified since first open (... O_RDWR) */
#define CSafeStore	0x00000040	/* close must wait for store to finish (should be in fd) */
#define CMAPPED		0x00000080	/* Mapped files; primarily used by SunOS 4.0.x */
#define CNSHARE		0x00000100	/* support O_NSHARE semantics */
#define CLied		0x00000200
#define CTruth		0x00000400

#if defined(AFS_DARWIN80_ENV)
#define CDeadVnode        0x00000800
#elif defined(AFS_DARWIN_ENV)
#define CUBCinit        0x00000800
#else
#define CWRITE_IGN	0x00000800	/* Next OS hack only */
#endif

#define CUnique		0x00001000	/* vc's uniquifier - latest unifiquier for fid */
#define CForeign	0x00002000	/* this is a non-afs vcache */
#define CReadDir	0x00004000	/* readdir in progress */
#define CUnlinked	0x00010000
#define CBulkStat	0x00020000	/* loaded by a bulk stat, and not ref'd since */
#define CUnlinkedDel	0x00040000
#define CVFlushed	0x00080000
#if defined(AFS_SGI_ENV)
#define CWritingUFS	0x00200000	/* to detect vm deadlock - used by sgi */
#elif defined(AFS_DARWIN80_ENV)
#define CEvent          0x00200000      /* to preclude deadlock when sending events */
#endif
#define CCreating	0x00400000	/* avoid needless store after open truncate */
#define CPageHog	0x00800000	/* AIX - dumping large cores is a page hog. */
#define CCorrupt	0x01000000	/* corrupt dir already reported */
#define CDCLock		0x02000000	/* Vnode lock held over call to GetDownD */
#define CBulkFetching	0x04000000	/* stats are being fetched by bulk stat */
#define CExtendedFile	0x08000000	/* extended file via ftruncate call. */
#define CVInit          0x10000000      /* being initialized */
#define CMetaDirty	0x20000000	/* vnode meta-data needs to be flushed */
#define CPartVisible	0x40000000	/* fileserver partition visible on client */

/* vcache vstate bits */
#define VRevokeWait   0x1
#define VPageCleaning 0x2	/* Solaris - Cache Trunc Daemon sez keep out */

/* Dirty disconnected vcache flags. */
#define VDisconSetTime		0x00000001  	/* set time. */
#define VDisconSetMode		0x00000002	/* set mode. */
/* XXX: to be continued ? */
#define VDisconTrunc		0x00000020	/* truncate file. */
#define VDisconSetAttrMask	0x0000003F	/* Masks for setattr ops. */
#define VDisconWriteClose	0x00000400	/* Write op on file close. */
#define VDisconWriteFlush	0x00000800	/* Write op on normal fsync/flush. */
#define VDisconWriteOsiFlush	0x00001000	/* Write op on osi flush. */

#define VDisconRemove		0x00002000	/* Remove vnop. */
#define VDisconCreate		0x00004000	/* Create vnop. */
#define VDisconCreated		0x00008000	/* A file that was created during
						   this resync operation */
#define VDisconRename		0x00010000	/* Rename vnop. */
#define VDisconRenameSameDir	0x00020000	/* Rename in same dir. */

/*... to be continued ...  */

/* vcache (file) cachingStates bits */
#define FCSDesireBypass   0x1	/* This file should bypass the cache */
#define FCSBypass         0x2	/* This file is currently NOT being cached */
#define FCSManuallySet    0x4	/* The bypass flags were set, or reset, manually (via pioctl)
 				 				   and should not be overridden by the file's name */

/* Flag values used by the Transition routines */
#define TRANSChangeDesiredBit		0x1	/* The Transition routine should set or
										 * reset the FCSDesireBypass bit */
#define TRANSVcacheIsLocked			0x2	/* The Transition routine does not need to
										 * lock vcache (it's already locked) */
#define TRANSSetManualBit		0x4	/* The Transition routine should set FCSManuallySet so that
									 * filename checking does not override pioctl requests */

#define	CPSIZE	    2
#if defined(AFS_XBSD_ENV) || defined(AFS_DARWIN_ENV)
#define vrefCount   v->v_usecount
#else
#define vrefCount   v.v_count
#endif /* AFS_XBSD_ENV */

#if defined(AFS_DARWIN80_ENV)
#define VREFCOUNT_GT(v, y)    vnode_isinuse(AFSTOV(v), (y))
#elif defined(AFS_FBSD_ENV)
#define VREFCOUNT(v)		(vrefcnt(AFSTOV(v)))
#define VREFCOUNT_GT(v, y)	(vrefcnt(AFSTOV(v)) > y)
#elif defined(AFS_XBSD_ENV) || defined(AFS_DARWIN_ENV)
#define VREFCOUNT(v)          ((v)->vrefCount)
#define VREFCOUNT_GT(v, y)    (AFSTOV(v)->v_usecount > (y))
#elif defined(AFS_LINUX24_ENV)
#define VREFCOUNT(v)		atomic_read(&(AFSTOV(v)->v_count))
#define VREFCOUNT_GT(v, y)      (VREFCOUNT(v)>y)
#define VREFCOUNT_SET(v, c)	atomic_set(&(AFSTOV(v)->v_count), c)
#define VREFCOUNT_DEC(v)	atomic_dec(&(AFSTOV(v)->v_count))
#define VREFCOUNT_INC(v)	atomic_inc(&(AFSTOV(v)->v_count))
#else
#define VREFCOUNT(v)		((v)->vrefCount)
#define VREFCOUNT_GT(v,y)     ((v)->vrefCount > (y))
#define VREFCOUNT_SET(v, c)	(v)->vrefCount = c
#define VREFCOUNT_DEC(v)	(v)->vrefCount--
#define VREFCOUNT_INC(v)	(v)->vrefCount++
#define d_unhash(d) list_empty(&(d)->d_hash)
#define dget_locked(d) dget(d)
#endif

#define	AFS_MAXDV   0x7fffffff	/* largest dataversion number */
#ifdef AFS_64BIT_CLIENT
#define AFS_NOTRUNC 0x7fffffffffffffffLL	/* largest positive int64 number */
#else /* AFS_64BIT_CLIENT */
#define	AFS_NOTRUNC 0x7fffffff	/* largest dataversion number */
#endif /* AFS_64BIT_CLIENT */

extern afs_int32 vmPageHog;	/* counter for # of vnodes which are page hogs. */

#if defined(AFS_DARWIN80_ENV)
#define VTOAFS(v) ((struct vcache *)vnode_fsnode((v)))
#define AFSTOV(vc) ((vc)->v)
#elif defined(AFS_NBSD40_ENV)
struct nbvdata {
    struct genfs_node gfsn;
    struct vcache *afsvc;
};
#define VTOAFS(v) ((((struct nbvdata *)((v)->v_data)))->afsvc)
#define AFSTOV(vc) ((vc)->v)
#elif defined(AFS_XBSD_ENV) || defined(AFS_DARWIN_ENV) || (defined(AFS_LINUX22_ENV) && !defined(STRUCT_SUPER_OPERATIONS_HAS_ALLOC_INODE))
#define VTOAFS(v) ((struct vcache *)(v)->v_data)
#define AFSTOV(vc) ((vc)->v)
#else
#define VTOAFS(V) ((struct vcache *)(V))
#define AFSTOV(V) (&(V)->v)
#endif

struct afs_vnuniq {
    afs_uint32 vnode;
    afs_uint32 unique;
};

/* VCache elements which are kept on disk, and in the kernel */
struct fvcache {
    struct VenusFid fid;
    struct mstat {
	afs_size_t Length;
	afs_hyper_t DataVersion;
	afs_uint32 Date;
	afs_uint32 Owner;
	afs_uint32 Group;
	afs_uint16 Mode;	/* XXXX Should be afs_int32 XXXX */
	afs_uint16 LinkCount;
#ifdef AFS_DARWIN80_ENV
        afs_uint16 Type;
#else
	/* vnode type is in v.v_type */
#endif
    } m;
    struct afs_vnuniq parent;

    /*! Truncate file to this position at the next store */
    afs_size_t truncPos;

    /*! System:AnyUser's access to this. */
    afs_int32 anyAccess;

    /*! state bits */
    afs_uint32 states;

    /*! Disconnected flags for this vcache element. */
    afs_uint32 ddirty_flags;
    /*! Shadow vnode + unique keep the shadow dir location. */
    struct afs_vnuniq shadow;
    /*! The old parent FID for renamed vnodes */
    struct afs_vnuniq oldParent;
};

#ifdef AFS_SUN5_ENV
/*
 * This is for the multiPage field in struct vcache. Each one of these
 * represents an outstanding getpage request that is larger than a single page.
 * Recording these is necessary to prevent afs_GetOnePage from trying to evict
 * a dcache entry that an earlier afs_GetOnePage call got in the same getpage
 * request. See osi_VM_MultiPageConflict and afs_getpage.
 */
struct multiPage_range {
    struct afs_q q;
    offset_t off;    /**< offset of getpage request */
    u_int len;       /**< length of getpage request */
};
#endif

/* INVARIANTs: (vlruq.next != NULL) == (vlruq.prev != NULL)
 *             nextfree => !vlruq.next && ! vlruq.prev
 * !(avc->nextfree) && !avc->vlruq.next => (FreeVCList == avc->nextfree)
 */
struct vcache {
#if defined(AFS_XBSD_ENV) || defined(AFS_DARWIN_ENV) || (defined(AFS_LINUX22_ENV) && !defined(STRUCT_SUPER_OPERATIONS_HAS_ALLOC_INODE))
    struct vnode *v;
#else
    struct vnode v;		/* Has reference count in v.v_count */
#endif
    struct afs_q vlruq;		/* lru q next and prev */
#if !defined(AFS_LINUX22_ENV)
    struct vcache *nextfree;	/* next on free list (if free) */
#endif
    struct vcache *hnext;	/* Hash next */
    struct afs_q vhashq;	/* Hashed per-volume list */
    /*! Queue of dirty vcaches. Lock with afs_disconDirtyLock */
    struct afs_q dirtyq;
    /*! Queue of vcaches with shadow entries. Lock with afs_disconDirtyLock */
    struct afs_q shadowq;
    /*! Queue of vcaches with dirty metadata. Locked by afs_xvcdirty */
    struct afs_q metadirty;
    /*! Vcaches slot number in the disk backup. Protected by tvc->lock */
    afs_uint32 diskSlot;
    struct fvcache f;
    afs_rwlock_t lock;		/* The lock on the vcache contents. */
#if	defined(AFS_SUN5_ENV)
    /* Lock used to protect the activeV, multipage, and vstates fields.
     * Do not try to get the vcache lock when the vlock is held */
    afs_rwlock_t vlock;
#endif				/* defined(AFS_SUN5_ENV) */
#if	defined(AFS_SUN5_ENV)
    krwlock_t rwlock;
    struct cred *credp;
#endif
#ifdef	AFS_AIX32_ENV
    afs_lock_t pvmlock;
    vmhandle_t vmh;
#if defined(AFS_AIX51_ENV)
    vmid_t segid;
#else
    int segid;
#endif
    struct ucred *credp;
#endif
#ifdef AFS_AIX_ENV
    int ownslock;		/* pid of owner of excl lock, else 0 - defect 3083 */
#endif
#ifdef AFS_DARWIN80_ENV
    lck_mtx_t *rwlock;
#elif defined(AFS_DARWIN_ENV)
    struct lock__bsd__ rwlock;
#endif
#ifdef AFS_XBSD_ENV
#if !defined(AFS_DFBSD_ENV) && !defined(AFS_NBSD_ENV)
    struct lock rwlock;
#endif
#endif

    struct VenusFid *mvid;	/* Either parent dir (if root) or root (if mt pt) */
    char *linkData;		/* Link data if a symlink. */
    afs_hyper_t flushDV;	/* data version last flushed from text */
    afs_hyper_t mapDV;		/* data version last flushed from map */
    struct server *callback;	/* The callback host, if any */
    afs_uint32 cbExpires;	/* time the callback expires */
    struct afs_q callsort;	/* queue in expiry order, sort of */
    struct axscache *Access;	/* a list of cached access bits */
    afs_int32 last_looker;	/* pag/uid from last lookup here */
#if	defined(AFS_SUN5_ENV)
    afs_int32 activeV;
#endif				/* defined(AFS_SUN5_ENV) */
    struct SimpleLocks *slocks;
    short opens;		/* The numbers of opens, read or write, on this file. */
    short execsOrWriters;	/* The number of execs (if < 0) or writers (if > 0) of
				 * this file. */
    short flockCount;		/* count of flock readers, or -1 if writer */
    char mvstat;		/* 0->normal, 1->mt pt, 2->root. */

    char cachingStates;			/* Caching policies for this file */
    afs_uint32 cachingTransitions;		/* # of times file has flopped between caching and not */

#if defined(AFS_LINUX24_ENV)
    off_t next_seq_offset;	/* Next sequential offset (used by prefetch/readahead) */
#elif defined(AFS_SUN5_ENV) || defined(AFS_SGI65_ENV)
    off_t next_seq_blk_offset; /* accounted in blocks for Solaris & IRIX */
#endif

#if	defined(AFS_SUN5_ENV)
    afs_uint32 vstates;		/* vstate bits */
#endif				/* defined(AFS_SUN5_ENV) */
    struct dcache *dchint;
    struct dcache *dcreaddir;	/* dcache for in-progress readdir */
    unsigned int readdir_pid;   /* pid of the thread in readdir */
#if defined(AFS_SGI_ENV)
    daddr_t lastr;		/* for read-ahead */
#ifdef AFS_SGI64_ENV
    uint64_t vc_rwlockid;	/* kthread owning rwlock */
#else
    short vc_rwlockid;		/* pid of proces