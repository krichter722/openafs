/*
 * Copyright 2000, International Business Machines Corporation and others.
 * All Rights Reserved.
 * 
 * This software has been released under the terms of the IBM Public
 * License.  For details, see the LICENSE file in the top-level source
 * directory or online at http://www.openafs.org/dl/license10.html
 */

#include <afsconfig.h>
#include "afs/param.h"


#include "afs/sysincludes.h"	/* Standard vendor system headers */
#include "afsincludes.h"	/* Afs-based standard headers */
#include "afs/afs_stats.h"	/* statistics */

/* Linux VM operations
 *
 * The general model for Linux is to treat vm as a cache that's:
 * 1) explicitly updated by AFS when AFS writes the data to the cache file.
 * 2) reads go through the cache. A cache miss is satisfied by the filesystem.
 *
 * This means flushing VM is not required on this OS.
 */

/* Try to discard pages, in order to recycle a vcache entry.
 *
 * We also make some sanity checks:  ref count, open count, held locks.
 *
 * We also do some non-VM-related chores, such as releasing the cred pointer
 * (for AIX and Solaris) and releasing the gnode (for AIX).
 *
 * Locking:  afs_xvcache lock is held.  If it is dropped and re-acquired,
 *   *slept should be set to warn the caller.
 *
 * Formerly, afs_xvcache was dropped and re-acquired for Solaris, but now it
 * is not dropped and re-acquired for any platform.  It may be that *slept is
 * therefore obsolescent.
 */
int
osi_VM_FlushVCache(struct vcache *avc, int *slept)
{
    struct inode *ip = AFSTOV(avc);

    if (VREFCOUNT(avc) > 1)
	return EBUSY;

    if (avc->opens != 0)
	return EBUSY;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
    return vmtruncate(ip, 0);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,15)
    truncate_inode_pages(ip, 0);
#else
    invalidate_inode_pages(ip);
#endif
    return 0;
}

/* Try to invalidate pages, for "fs flush" or "fs flushv"; or
 * try to free pages, when deleting a file.
 *
 * Locking:  the vcache entry's lock is held.  It may be dropped and 
 * re-obtained.
 *
 * Since we drop and re-obtain the lock, we can't guarantee that there won't
 * be some pages around when we return, newly created by concurrent activity.
 */
void
osi_VM_TryToSmush(struct vcache *avc, AFS_UCRED *acred, int sync)
{
    struct inode *ip = AFSTOV(avc);

    invalidate_inode_pages(ip);
}

/* Flush and invalidate pages, for fsync() with INVAL flag
 *
 * Locking:  only the global lock is held.
 */
void
osi_VM_FSyncInval(struct vcache *avc)
{

}

/* Try to store pages to cache, in order to store a file back to the server.
 *
 * Locking:  the vcache entry's lock is held.  It will usually be dropped and
 * re-obtained.
 */
void
osi_VM_StoreAllSegments(struct vcache *avc)
{
    struct inode *ip = AFSTOV(avc);

    if (avc->f.states & CPageWrite)
	return; /* someone already writing */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,5)
    /* filemap_fdatasync() only exported in 2.4.5 and above */
    ReleaseWriteLock(&avc->lock);
    AFS_GUNLOCK();
    filemap_fdatasync(ip->i_mapping);
    filemap_fdatawait(ip->i_mapping);
    AFS_GLOCK();
    ObtainWriteLock(&avc->lock, 121);
#endif
}

/* Purge VM for a file when its callback is revoked.
 *
 * Locking:  No lock is held, not even the global lock.
 */

/* Note that for speed some of our Linux vnodeops do not initialise credp
 * before calling osi_FlushPages(). If credp is ever required on Linux,
 * then these callers should be updated.
 */
void
osi_VM_FlushPages(struct vcache *avc, AFS_UCRED *credp)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
    struct inode *ip = AFSTOV(avc);
    
    truncate_inode_pages(&ip->i_data, 0);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,15)
    struct inode *ip = AFSTOV(avc);

    truncate_inode_pages(ip, 0);
#else
    invalidate_inode_pages(AFSTOV(avc));
#endif
}

/* Purge pages beyond end-of-file, when truncating a file.
 *
 * Locking:  no lock is held, not even the global lock.
 * activeV is raised.  This is supposed to block pageins, but at present
 * it only works on Solaris.
 */
void
osi_VM_Truncate(struct vcache *avc, int alen, AFS_UCRED *acred)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0)
    vmtruncate(AFSTOV(avc), alen);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,2,15)
    struct inode *ip = AFSTOV(avc);

    truncate_inode_pages(ip, alen);
#else
    invalidate_inode_pages(AFSTOV(avc));
#endif
}
